/**
 * TMC2240 SPI Example
 *
 * Demonstrates the TMC2240 stepper driver using SPI communication.
 * The TMC2240 is a versatile driver with unique features including:
 * - Integrated current sensing (no sense resistors; full scale set by RREF on IREF)
 * - StallGuard4 (SG4) for load sensing in StealthChop2
 * - ADC-based temperature and voltage monitoring
 * - Over-temperature pre-warning and overvoltage (OV pin) thresholds
 * - StealthChop2 for quiet operation
 *
 * Connect your TMC2240 driver via SPI:
 *   CS_PIN  -> Chip Select
 *   MOSI    -> SDI
 *   MISO    -> SDO
 *   SCK     -> SCK
 * Motion comes from the STEP/DIR inputs.
 *
 * Serial commands:
 *   ' '  Start / stop the motor
 *   's'  Toggle StealthChop2 / SpreadCycle (motor must be stopped)
 *   'd'  Toggle direction (GCONF.shaft)
 *   '+'  Increase speed
 *   '-'  Decrease speed
 *   'r'  Re-apply the configuration
 *
 * Safety Warning:
 *   Always ensure your belts are properly secured before testing!
 *   Disconnect the motor when not in use to prevent unexpected movement.
 *   Verify your power supply voltage is within the driver's specifications.
 */

#include <TMCStepper.h>

// Pin definitions
#define EN_PIN           38 // Enable pin (LOW = enabled)
#define DIR_PIN          55 // Direction pin
#define STEP_PIN         54 // Step pin
#define CS_PIN           42 // Chip select pin

// Software SPI pins (alternative to hardware SPI)
#define SW_MOSI          66 // Software Master Out Slave In (MOSI)
#define SW_MISO          44 // Software Master In Slave Out (MISO)
#define SW_SCK           64 // Software Slave Clock (SCK)

// Resistor on the IREF pin (Ω, 12k-60k) - match to your driver board
#define R_REF 12000

// Full-scale current range (DRV_CONF.current_range), the peak current at RREF = 12kΩ:
// 0 = 1A, 1 = 2A, 2 = 3A, 3 = 3A. Select the lowest range that fits your motor.
#define CURRENT_RANGE 1

// Serial baud rate for monitoring
#define SERIAL_BAUD 250000

// Motor parameters
#define MICROSTEPS     16     // 256, 128, 64, 32, 16, 8, 4, 2, or 0 (fullstep)
#define DEFAULT_SPEED  1000   // Initial step rate (Hz)
#define MIN_SPEED      100    // Minimum step rate (Hz)
#define MAX_SPEED      10000  // Maximum step rate (Hz)

// Select your TMC2240 configuration
// Option 1: Hardware SPI (default, uncommented)
TMC2240Stepper driver(CS_PIN);

// Option 2: Software SPI (uncomment if needed)
// TMC2240Stepper driver(CS_PIN, SW_MOSI, SW_MISO, SW_SCK);

// StallGuard4 threshold SG4_THRS (0-255). A stall is signaled when SG4_RESULT
// falls below this value, so higher values are more sensitive.
// Tune it by watching SG4_RESULT under load (start at half the lowest value seen before a stall).
#define SG4_THRESHOLD 50

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
void print_stealthchop_status();
void print_sg4_status();
void print_adc_readings();
void print_drv_status();

bool motor_running = true;
bool stealthchop_enabled = true;
uint32_t step_rate = DEFAULT_SPEED;  // Steps per second

void setup() {
  // Initialize serial communication
  Serial.begin(SERIAL_BAUD);
  while (!Serial) {
    delay(10);  // Wait for serial port to connect (needed for native USB)
  }

  Serial.println(F("\n========================================"));
  Serial.println(F("  TMC2240 SPI Example"));
  Serial.println(F("========================================"));
  Serial.println(F("\nInitializing..."));

  // Configure GPIO pins
  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);

  // Enable driver in hardware (LOW = enabled)
  digitalWrite(EN_PIN, LOW);
  digitalWrite(DIR_PIN, HIGH);

  // Initialize SPI
  SPI.begin();

  // Initialize the TMC2240 driver
  driver.begin();

  // Test connection by reading DRV_STATUS
  // 0 = OK, 1 = read all ones (no response), 2 = read all zeros
  Serial.print(F("Connection test: "));
  uint8_t connection_status = driver.test_connection();
  if (connection_status == 0) {
    Serial.println(F("OK"));
  } else {
    Serial.print(F("FAILED (error code: "));
    Serial.print(connection_status);
    Serial.println(F(")"));
    Serial.println(F("WARNING: Check SPI connections and motor power!"));
  }

  configure_driver();

  Serial.println(F("\n--- Basic Configuration ---"));
  Serial.print(F("  Run Current: "));
  Serial.print(driver.rms_current());
  Serial.println(F(" mA"));
  Serial.print(F("  Microsteps: "));
  Serial.println(driver.microsteps());

  Serial.println(F("\n--- CHOPCONF Configuration ---"));
  Serial.print(F("  Blank Time (TBL): "));
  Serial.println(driver.TBL());
  Serial.print(F("  Interpolation: "));
  Serial.println(driver.intpol() ? F("Enabled") : F("Disabled"));

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

  Serial.println(F("\nConfiguration complete!"));
  Serial.println(F("Commands: ' ' start/stop, 's' StealthChop2, 'd' direction, '+'/'-' speed, 'r' reconfigure\n"));
}

/**
 * Apply the complete driver configuration
 */
void configure_driver() {
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

  // Write the remaining shadow registers (incl. DRV_CONF) to the driver
  driver.push();
}

void loop() {
  static uint32_t last_monitor = 0;
  static bool direction = false;

  // Handle serial commands
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    switch (cmd) {
      case ' ':
        motor_running = !motor_running;
        Serial.println(motor_running ? F("\nMotor started") : F("\nMotor stopped"));
        break;

      case 's':
      case 'S':
        // The switch into StealthChop2 is only allowed at standstill
        if (motor_running) {
          Serial.println(F("\nStop the motor (' ') before changing the chopper mode."));
          break;
        }
        stealthchop_enabled = !stealthchop_enabled;
        driver.en_pwm_mode(stealthchop_enabled);
        Serial.print(F("\nStealthChop2: "));
        Serial.println(stealthchop_enabled ? F("Enabled") : F("Disabled (SpreadCycle)"));
        break;

      case 'r':
      case 'R':
        Serial.println(F("\nRe-applying configuration..."));
        configure_driver();
        Serial.println(F("Reconfiguration complete.\n"));
        break;

      case '+':
        step_rate = min((uint32_t)MAX_SPEED, step_rate * 2);
        Serial.print(F("\nStep rate: "));
        Serial.println(step_rate);
        break;

      case '-':
        step_rate = max((uint32_t)MIN_SPEED, step_rate / 2);
        Serial.print(F("\nStep rate: "));
        Serial.println(step_rate);
        break;

      case 'd':
      case 'D':
        // Invert the motor direction in the driver (GCONF.shaft)
        direction = !direction;
        driver.shaft(direction);
        Serial.print(F("\nDirection: "));
        Serial.println(direction ? F("Inverted") : F("Normal"));
        break;
    }
  }

  // Run motor - simple step generation
  if (motor_running) {
    static uint32_t last_step = 0;
    const uint32_t period = 1000000UL / step_rate;  // Step period in microseconds
    const uint32_t now_us = micros();
    if (now_us - last_step >= period) {
      last_step = now_us;

      // Generate step pulse
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(1);
      digitalWrite(STEP_PIN, LOW);
    }
  }

  // Periodic monitoring
  uint32_t now = millis();
  if (now - last_monitor >= MONITOR_INTERVAL) {
    last_monitor = now;

    print_adc_readings();
    print_drv_status();

    if (stealthchop_enabled) {
      print_stealthchop_status();
    }

    print_sg4_status();

    Serial.println();
  }
}

/**
 * Print ADC-based readings (temperature, voltage)
 */
void print_adc_readings() {
  // Chip temperature: (ADC_TEMP - 2038) / 7.7
  float temp = driver.get_chip_temperature();
  Serial.print(F("Temp: "));
  Serial.print(temp, 1);
  Serial.print(F("°C | VSUP: "));

  // Supply voltage: ADC_VSUPPLY x 9.732mV
  float vsup = driver.get_vsupply_voltage();
  Serial.print(vsup, 1);
  Serial.print(F("V | AIN: "));

  // AIN pin voltage: ADC_AIN x 305.2µV
  float ain = driver.get_ain_voltage();
  Serial.print(ain, 3);
  Serial.println(F("V"));
}

/**
 * Print StealthChop2/PWM status
 */
void print_stealthchop_status() {
  Serial.print(F("PWM: "));
  Serial.print(driver.en_pwm_mode() ? F("Enabled") : F("Disabled"));

  // PWM scale values (read from PWM_SCALE and PWM_AUTO registers)
  uint8_t scale_sum = driver.pwm_scale_sum();
  int16_t scale_auto = driver.pwm_scale_auto();
  uint8_t ofs_auto = driver.pwm_ofs_auto();
  uint8_t grad_auto = driver.pwm_grad_auto();

  Serial.print(F(" | SSum: "));
  Serial.print(scale_sum);
  Serial.print(F(" | SAuto: "));
  Serial.print(scale_auto);
  Serial.print(F(" | OAuto: "));
  Serial.print(ofs_auto);
  Serial.print(F(" | GAuto: "));
  Serial.println(grad_auto);
}

/**
 * Print StallGuard4 result and threshold
 */
void print_sg4_status() {
  // SG4_RESULT (0-510): a lower value means a higher motor load
  uint16_t sg4_result = driver.sg4_result();
  Serial.print(F("SG4 Result: "));
  Serial.print(sg4_result);

  uint8_t sg4_thrs = driver.sg4_thrs();
  Serial.print(F(" | SG4 Thrs: "));
  Serial.println(sg4_thrs);
}

/**
 * Print DRV_STATUS register flags
 */
void print_drv_status() {
  Serial.print(F("Standstill: "));
  Serial.print(driver.stst() ? F("Yes") : F("No"));
  Serial.print(F(" | Stall: "));
  Serial.print(driver.stallguard() ? F("Yes") : F("No"));
  Serial.print(F(" | OT: "));
  Serial.print(driver.ot() ? F("Yes") : F("No"));
  Serial.print(F(" | OTPW: "));
  Serial.print(driver.otpw() ? F("Yes") : F("No"));
  Serial.print(F(" | OL-A: "));
  Serial.print(driver.ola() ? F("Yes") : F("No"));
  Serial.print(F(" | OL-B: "));
  Serial.print(driver.olb() ? F("Yes") : F("No"));
  Serial.print(F(" | S2G-A: "));
  Serial.print(driver.s2ga() ? F("Yes") : F("No"));
  Serial.print(F(" | S2G-B: "));
  Serial.println(driver.s2gb() ? F("Yes") : F("No"));

  // Actual current scale and microstep counter
  uint16_t cs_actual = driver.CS_ACTUAL();
  Serial.print(F("CS: "));
  Serial.print(cs_actual);
  Serial.print(F(" | MSCNT: "));
  Serial.println(driver.MSCNT());
}
