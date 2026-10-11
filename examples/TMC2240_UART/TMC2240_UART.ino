/**
 * TMC2240 UART Example
 *
 * ************************************************************************
 * UNTESTED / NOT YET IMPLEMENTED
 *
 * TMCStepper's TMC2240Stepper class currently implements SPI only.
 * TMC2240 modules (BTT TMC2240 V1.0, Mellow FLY TMC2240, FYSETC S2240) ship
 * in SPI mode, so no UART hardware has been available for testing. This
 * sketch shows the intended UART usage. It will not compile until UART
 * transport is added to TMC2240Stepper (which will define TMC2240_HAS_UART).
 * ************************************************************************
 *
 * The TMC2240 interface is selected by the UART_EN pin:
 *   UART_EN low  = SPI
 *   UART_EN high = single-wire UART on the DIAG1/SW pin, with CRC checking
 *
 * In UART mode the SPI pins become address inputs:
 *   SDI/AD0 (+1), SCK/AD1 (+2), CSN/AD2 (+4)
 * and the node address is NODEADDR (register) + the pin selection.
 * The UART baud rate is detected automatically (minimum 9000 baud).
 *
 * Motion comes from the STEP/DIR inputs. The TMC2240 has no internal
 * ramp generator, so there is no VACTUAL register.
 *
 * Serial commands (end each with a newline):
 *   speed <hz>   Set step rate in Hz (0 = stop)
 *   forward      Set DIR high
 *   backward     Set DIR low
 *   status       Print full status
 *   temp         Print chip temperature
 *   voltage      Print supply voltage
 *   stealth      Toggle StealthChop2 / SpreadCycle (motor must be stopped)
 *   sg4 <value>  Set StallGuard4 threshold (0-255)
 *   stop         Stop the motor
 *
 * Safety Warning:
 *   Always ensure your belts are properly secured before testing!
 *   Disconnect the motor when not in use to prevent unexpected movement.
 *   Verify your power supply voltage is within the driver's specifications.
 */

#include <TMCStepper.h>

#ifndef TMC2240_HAS_UART
  #error "TMC2240 UART is not yet implemented in TMCStepper. Use the TMC2240_SPI example."
#endif

// Pin definitions
#define EN_PIN           38 // Enable pin (LOW = enabled)
#define STEP_PIN         54 // Step pin
#define DIR_PIN          55 // Direction pin

// Software UART pins (alternative to hardware UART)
#define SW_RX            63 // TMC2240 SoftwareSerial receive pin
#define SW_TX            40 // TMC2240 SoftwareSerial transmit pin

// Hardware UART port
#define SERIAL_PORT Serial1 // TMC2240 HardwareSerial port

// Resistor on the IREF pin (Ω, 12k-60k) - match to your driver board
#define R_REF 12000

// Full-scale current range (DRV_CONF.current_range), the peak current at RREF = 12kΩ:
// 0 = 1A, 1 = 2A, 2 = 3A, 3 = 3A. Select the lowest range that fits your motor.
#define CURRENT_RANGE 1

// Serial baud rate for PC monitoring
#define PC_SERIAL_BAUD 250000

// UART baud rate for TMC2240 communication (auto-detected by the driver, min. 9000)
#define TMC_UART_BAUD 115200

// UART node configuration
#define NODE_ADDRESS    0     // NODEADDR register (0-254), added to the AD0-AD2 pin address
#define SEND_DELAY      2     // SENDDELAY (0-15). Use 2 or more with multiple nodes.

// Motor parameters
#define MICROSTEPS     16     // 256, 128, 64, 32, 16, 8, 4, 2, or 0 (fullstep)
#define DEFAULT_SPEED  1000   // Initial step rate (Hz)
#define MAX_SPEED      10000  // Maximum step rate (Hz)

// StallGuard4 threshold SG4_THRS (0-255). A stall is signaled when SG4_RESULT
// falls below this value, so higher values are more sensitive.
#define SG4_THRESHOLD  50

// Current settings
#define RUN_CURRENT_mA  1200  // Run current in mA RMS
#define HOLD_MULTIPLIER 0.5   // Hold current as fraction of run current

// Over-temperature pre-warn threshold (Celsius)
#define OTPW_THRESHOLD 105.0

// Overvoltage threshold for the OV output pin (Volts)
#define OV_THRESHOLD 30.0

// Timing for monitoring updates (milliseconds)
#define MONITOR_INTERVAL 500

// Forward declarations
void configure_driver();
void print_adc_readings();
void print_sg4_status();
void print_uart_status();
void print_drv_status();
void print_stealthchop_status();

// ============================================
// Select your TMC2240 configuration
// (Proposed UART constructors, matching TMC2209Stepper without a sense resistor)
// ============================================

// Option 1: Hardware UART (default, uncommented)
TMC2240Stepper driver(&SERIAL_PORT, NODE_ADDRESS);

// Option 2: Software UART (uncomment if needed)
// TMC2240Stepper driver(SW_RX, SW_TX, NODE_ADDRESS);

bool stealthchop_enabled = true;
bool direction_forward = true;
uint32_t current_speed = DEFAULT_SPEED;  // Step rate in Hz

void setup() {
  // Initialize PC serial communication
  Serial.begin(PC_SERIAL_BAUD);
  while (!Serial) {
    delay(10);  // Wait for serial port to connect (needed for native USB)
  }

  Serial.println(F("\n========================================"));
  Serial.println(F("  TMC2240 UART Example"));
  Serial.println(F("========================================"));
  Serial.println(F("\nInitializing..."));

  // Configure GPIO pins
  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  // Enable driver in hardware (LOW = enabled)
  digitalWrite(EN_PIN, LOW);
  digitalWrite(DIR_PIN, direction_forward ? HIGH : LOW);

  // ============================================
  // Initialize UART Communication
  // ============================================

  Serial.println(F("\n--- UART Initialization ---"));

  // Initialize hardware UART to TMC2240
  SERIAL_PORT.begin(TMC_UART_BAUD);
  // For software UART use driver.beginSerial(TMC_UART_BAUD) instead
  Serial.print(F("  UART baud: "));
  Serial.println(TMC_UART_BAUD);

  // Initialize the TMC2240 driver
  driver.begin();

  // Test connection by reading DRV_STATUS
  // 0 = OK, 1 = read all ones (no response), 2 = read all zeros
  Serial.print(F("  Connection test: "));
  uint8_t connection_status = driver.test_connection();
  if (connection_status == 0) {
    Serial.println(F("OK"));
  } else {
    Serial.print(F("FAILED (error code: "));
    Serial.print(connection_status);
    Serial.println(F(")"));
    Serial.println(F("  WARNING: Check UART connections, UART_EN, and motor power!"));
  }

  configure_driver();

  Serial.print(F("  Node Address (NODEADDR): "));
  Serial.println(driver.nodeaddr());
  Serial.print(F("  Send Delay (SENDDELAY): "));
  Serial.println(driver.senddelay());

  Serial.println(F("\n--- Basic Configuration ---"));
  Serial.print(F("  Run Current: "));
  Serial.print(driver.rms_current());
  Serial.println(F(" mA"));
  Serial.print(F("  Microsteps: "));
  Serial.println(driver.microsteps());

  Serial.println(F("\n--- StealthChop2 Configuration ---"));
  Serial.print(F("  Mode: "));
  Serial.println(driver.en_pwm_mode() ? F("Enabled") : F("Disabled"));

  Serial.println(F("\n--- StallGuard4 Configuration ---"));
  print_sg4_status();

  Serial.println(F("\n--- Protection Settings ---"));
  Serial.print(F("  OTPreWarn Threshold: "));
  Serial.print(driver.get_overtemp_prewarn_celsius(), 1);
  Serial.println(F(" °C"));
  Serial.print(F("  Overvoltage Threshold: "));
  Serial.print(driver.get_overvoltage_threshold_voltage(), 1);
  Serial.println(F(" V"));

  Serial.println(F("\n--- STEP/DIR Speed Control ---"));
  Serial.print(F("  Speed (Hz): "));
  Serial.println(current_speed);

  Serial.println(F("\nConfiguration complete!"));
  Serial.println(F("Available serial commands:"));
  Serial.println(F("  speed <hz>      - Set step rate in Hz (0 = stop)"));
  Serial.println(F("  forward         - Set DIR high"));
  Serial.println(F("  backward        - Set DIR low"));
  Serial.println(F("  status          - Print full status"));
  Serial.println(F("  temp            - Print temperature"));
  Serial.println(F("  voltage         - Print supply voltage"));
  Serial.println(F("  stealth         - Toggle StealthChop2 / SpreadCycle (motor stopped)"));
  Serial.println(F("  sg4 <value>     - Set StallGuard4 threshold (0-255)"));
  Serial.println(F("  stop            - Stop motor"));
  Serial.println();
}

/**
 * Apply the complete driver configuration
 */
void configure_driver() {
  // UART node configuration (NODECONF: NODEADDR[7:0], SENDDELAY[11:8])
  driver.nodeaddr(NODE_ADDRESS);
  driver.senddelay(SEND_DELAY);

  // Full-scale current is set by RREF and DRV_CONF.current_range (no sense resistors)
  driver.Rref = R_REF;
  driver.current_range(CURRENT_RANGE);

  // Enable driver in software (TOFF > 0 enables the power stage)
  driver.toff(4);

  // Set RMS run current and hold current (sets GLOBAL_SCALER, IRUN, IHOLD)
  driver.rms_current(RUN_CURRENT_mA, HOLD_MULTIPLIER);
  driver.iholddelay(1);    // Delay per current reduction step at standstill (x 2^18 clocks)

  // Set microstep resolution (CHOPCONF.MRES)
  driver.microsteps(MICROSTEPS);

  // Chopper configuration
  driver.TBL(1);           // Comparator blank time: 0=16, 1=24, 2=36, 3=54 clocks
  driver.chm(false);       // Chopper mode: false = SpreadCycle, true = constant off time
  driver.intpol(true);     // Interpolate to 256 microsteps
  driver.dedge(false);     // Step on rising edge only (this example sends full pulses)
  driver.hysteresis_end(-3);  // Hysteresis end value (can be negative)
  driver.hysteresis_start(3); // Hysteresis start value

  // StealthChop2 (switch it on only at standstill)
  driver.en_pwm_mode(stealthchop_enabled);
  driver.pwm_autoscale(true);   // Automatic current control
  driver.pwm_autograd(true);    // Automatic PWM_GRAD tuning (requires pwm_autoscale)
  driver.pwm_ofs(30);           // Initial PWM amplitude offset (0-255)
  driver.pwm_grad(0);           // Initial PWM amplitude gradient (0-255)
  driver.pwm_freq(0);           // PWM frequency: 0 = 2/1024 fCLK (lowest that works well)

  // StallGuard4 works with StealthChop2 (StallGuard2 works with SpreadCycle)
  driver.sg4_filt_en(true);     // Average over 4 fullsteps, enables SG4_IND_0..3
  driver.sg4_thrs(SG4_THRESHOLD);

  // The stall output (DIAG and DRV_STATUS.stallguard) is active while TCOOLTHRS >= TSTEP.
  // The maximum value enables it at all velocities.
  driver.TCOOLTHRS(0xFFFFF);

  // Over-temperature pre-warning (DRV_STATUS.otpw) and OV pin threshold
  driver.set_overtemp_prewarn_celsius(OTPW_THRESHOLD);
  driver.set_overvoltage_threshold_voltage(OV_THRESHOLD);

  // Write the remaining shadow registers (incl. NODECONF and DRV_CONF) to the driver
  driver.push();
}

void loop() {
  // Handle serial commands
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.startsWith("speed ")) {
      long speed = cmd.substring(6).toInt();
      current_speed = constrain(speed, 0, MAX_SPEED);
      Serial.print(F("  Speed set to: "));
      Serial.println(current_speed);
    }
    else if (cmd == "forward") {
      direction_forward = true;
      digitalWrite(DIR_PIN, HIGH);
      Serial.println(F("  Direction: Forward"));
    }
    else if (cmd == "backward") {
      direction_forward = false;
      digitalWrite(DIR_PIN, LOW);
      Serial.println(F("  Direction: Backward"));
    }
    else if (cmd == "status") {
      Serial.println(F("\n--- Full Status ---"));
      print_adc_readings();
      print_drv_status();
      if (stealthchop_enabled) {
        print_stealthchop_status();
      }
      print_sg4_status();
      print_uart_status();
      Serial.println();
    }
    else if (cmd == "temp") {
      Serial.print(F("  Temperature: "));
      Serial.print(driver.get_chip_temperature(), 1);
      Serial.println(F(" °C"));
    }
    else if (cmd == "voltage") {
      Serial.print(F("  Supply Voltage: "));
      Serial.print(driver.get_vsupply_voltage(), 1);
      Serial.println(F(" V"));
    }
    else if (cmd == "stealth") {
      // The switch into StealthChop2 is only allowed at standstill
      if (current_speed > 0) {
        Serial.println(F("  Stop the motor before changing the chopper mode."));
      }
      else {
        stealthchop_enabled = !stealthchop_enabled;
        driver.en_pwm_mode(stealthchop_enabled);  // en_pwm_mode = 0 selects SpreadCycle
        Serial.print(F("  StealthChop2: "));
        Serial.println(stealthchop_enabled ? F("Enabled") : F("Disabled (SpreadCycle)"));
      }
    }
    else if (cmd.startsWith("sg4 ")) {
      uint8_t sg4_val = constrain(cmd.substring(4).toInt(), 0, 255);
      driver.sg4_thrs(sg4_val);
      Serial.print(F("  SG4 threshold set to: "));
      Serial.println(sg4_val);
    }
    else if (cmd == "stop") {
      current_speed = 0;
      Serial.println(F("  Motor stopped"));
    }
    else if (cmd.length() > 0) {
      Serial.print(F("  Unknown command: "));
      Serial.println(cmd);
    }
  }

  // Run motor - simple step generation
  if (current_speed > 0) {
    static uint32_t last_step = 0;
    const uint32_t period = 1000000UL / current_speed;  // Step period in microseconds
    const uint32_t now_us = micros();
    if (now_us - last_step >= period) {
      last_step = now_us;
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(1);
      digitalWrite(STEP_PIN, LOW);
    }
  }

  // Periodic monitoring
  static uint32_t last_monitor = 0;
  uint32_t now = millis();

  if (now - last_monitor >= MONITOR_INTERVAL) {
    last_monitor = now;

    print_adc_readings();
    print_drv_status();

    if (stealthchop_enabled) {
      print_stealthchop_status();
    }

    print_sg4_status();

    Serial.print(F("  Speed (Hz): "));
    Serial.print(current_speed);
    Serial.println(direction_forward ? F(" Forward") : F(" Backward"));

    Serial.println();
  }
}

/**
 * Print ADC-based readings (temperature, voltage)
 */
void print_adc_readings() {
  // Chip temperature: (ADC_TEMP - 2038) / 7.7
  float temp = driver.get_chip_temperature();
  Serial.print(F("  Temperature: "));
  Serial.print(temp, 1);
  Serial.println(F(" °C"));

  // Supply voltage: ADC_VSUPPLY x 9.732mV
  float vsup = driver.get_vsupply_voltage();
  Serial.print(F("  Supply Voltage: "));
  Serial.print(vsup, 1);
  Serial.println(F(" V"));

  // AIN pin voltage: ADC_AIN x 305.2µV
  float ain = driver.get_ain_voltage();
  Serial.print(F("  AIN Voltage: "));
  Serial.print(ain, 3);
  Serial.println(F(" V"));
}

/**
 * Print DRV_STATUS register flags
 */
void print_drv_status() {
  Serial.print(F("  Standstill: "));
  Serial.print(driver.stst() ? F("Yes") : F("No"));
  Serial.print(F(" | Stall: "));
  Serial.print(driver.stallguard() ? F("Yes") : F("No"));
  Serial.print(F(" | Over-temp: "));
  Serial.print(driver.ot() ? F("Yes") : F("No"));
  Serial.print(F(" | OTPreWarn: "));
  Serial.print(driver.otpw() ? F("Yes") : F("No"));
  Serial.print(F(" | Open Load A: "));
  Serial.print(driver.ola() ? F("Yes") : F("No"));
  Serial.print(F(" | Open Load B: "));
  Serial.print(driver.olb() ? F("Yes") : F("No"));
  Serial.print(F(" | Short to Gnd A: "));
  Serial.print(driver.s2ga() ? F("Yes") : F("No"));
  Serial.print(F(" | Short to Gnd B: "));
  Serial.println(driver.s2gb() ? F("Yes") : F("No"));

  // Actual current scale
  Serial.print(F("  Current Scale (CS_ACTUAL): "));
  Serial.println(driver.CS_ACTUAL());
}

/**
 * Print StealthChop2/PWM status
 */
void print_stealthchop_status() {
  Serial.print(F("  PWM Scale Sum: "));
  Serial.print(driver.pwm_scale_sum());
  Serial.print(F(" | PWM Scale Auto: "));
  Serial.print(driver.pwm_scale_auto());
  Serial.print(F(" | PWM Ofs Auto: "));
  Serial.print(driver.pwm_ofs_auto());
  Serial.print(F(" | PWM Grad Auto: "));
  Serial.println(driver.pwm_grad_auto());
}

/**
 * Print StallGuard4 result and threshold
 */
void print_sg4_status() {
  // SG4_RESULT (0-510): a lower value means a higher motor load
  Serial.print(F("  StallGuard4 Result: "));
  Serial.print(driver.sg4_result());
  Serial.print(F(" | StallGuard4 Threshold: "));
  Serial.println(driver.sg4_thrs());
}

/**
 * Print UART-specific status information
 */
void print_uart_status() {
  // IOIN.UART_EN reflects the interface selection pin
  Serial.print(F("  Interface: "));
  Serial.println(driver.uart_en() ? F("UART") : F("SPI"));

  // IFCNT counts successful UART write accesses (8-bit, wraps around)
  Serial.print(F("  Interface Write Count (IFCNT): "));
  Serial.println(driver.IFCNT());

  // CRC error flag
  Serial.print(F("  CRC Errors: "));
  Serial.println(driver.CRCerror ? F("Yes") : F("No"));
}
