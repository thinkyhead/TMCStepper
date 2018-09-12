#include "TMCStepper.h"
#include "SW_SPI.h"

TMC2660Stepper::TMC2660Stepper(uint16_t pinCS, float RS) {
  this->_pinCS = pinCS;
  Rsense = RS;
}

TMC2660Stepper::TMC2660Stepper(uint16_t pinCS, float RS, uint16_t pinMOSI, uint16_t pinMISO, uint16_t pinSCK) {
  this->_pinCS = pinCS;
  Rsense = RS;
  uses_sw_spi = true;
  TMC_SW_SPI.setPins(pinMOSI, pinMISO, pinSCK);
}

void TMC2660Stepper::switchCSpin(bool state) {
  // Allows for overriding in child class to make use of fast io
  digitalWrite(_pinCS, state);
}

uint32_t TMC2660Stepper::read() {
  uint32_t response = 0UL;
  uint32_t dummy = ((uint32_t)DRVCONF_register.address<<17) | DRVCONF_register.sr;
  if (uses_sw_spi) {
    switchCSpin(LOW);
    response |= TMC_SW_SPI.transfer((dummy >> 16) & 0xFF);
    response <<= 8;
    response |= TMC_SW_SPI.transfer((dummy >>  8) & 0xFF);
    response <<= 8;
    response |= TMC_SW_SPI.transfer(dummy & 0xFF);
  } else {
    SPI.beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE3));
    switchCSpin(LOW);
    response |= SPI.transfer((dummy >> 16) & 0xFF);
    response <<= 8;
    response |= SPI.transfer((dummy >>  8) & 0xFF);
    response <<= 8;
    response |= SPI.transfer(dummy & 0xFF);
    SPI.endTransaction();
  }
  switchCSpin(HIGH);
  return response;
}

void TMC2660Stepper::write(uint8_t addressByte, uint32_t config) {
  uint32_t data = (uint32_t)addressByte<<17 | config;
  if (uses_sw_spi) {
    switchCSpin(LOW);
    TMC_SW_SPI.transfer((data >> 16) & 0xFF);
    TMC_SW_SPI.transfer((data >>  8) & 0xFF);
    TMC_SW_SPI.transfer(data & 0xFF);
  } else {
    SPI.beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE3));
    switchCSpin(LOW);
    SPI.transfer((data >> 16) & 0xFF);
    SPI.transfer((data >>  8) & 0xFF);
    SPI.transfer(data & 0xFF);
    SPI.endTransaction();
  }
  switchCSpin(HIGH);
}

void TMC2660Stepper::begin() {
  //set pins
  pinMode(_pinCS, OUTPUT);
  switchCSpin(HIGH);

  //TODO: Push shadow registers

  toff(8); //off_time(8);
  SELF.tbl(1); //blank_time(24);

  _started = true;
}

bool TMC2660Stepper::isEnabled() { return toff() > 0; }

uint8_t TMC2660Stepper::test_connection() {
  uint32_t drv_status = DRVSTATUS();
  switch (drv_status) {
      case 0xFFCFF: return 1;
      case 0: return 2;
      default: return 0;
  }
}

/*
  Requested current = mA = I_rms/1000
  Equation for current:
  I_rms = (CS+1)/32 * V_fs/R_sense * 1/sqrt(2)
  Solve for CS ->
  CS = 32*sqrt(2)*I_rms*R_sense/V_fs - 1

  Example:
  vsense = 0b0 -> V_fs = 0.310V //Typical
  mA = 1650mA = I_rms/1000 = 1.65A
  R_sense = 0.100 Ohm
  ->
  CS = 32*sqrt(2)*1.65*0.100/0.310 - 1 = 24,09
  CS = 24
*/

uint16_t TMC2660Stepper::rms_current() {
  return (float)(cs()+1)/32.0 * (SELF.vsense()?0.165:0.310)/Rsense / 1.41421 * 1000;
}
void TMC2660Stepper::rms_current(uint16_t mA) {
  uint8_t CS = 32.0*1.41421*mA/1000.0*Rsense/0.310 - 1;
  // If Current Scale is too low, turn on high sensitivity R_sense and calculate again
  if (CS < 16) {
    SELF.vsense(true);
    CS = 32.0*1.41421*mA/1000.0*Rsense/0.165 - 1;
  } else { // If CS >= 16, turn off high_sense_r
    SELF.vsense(false);
  }
  cs(CS);
  //val_mA = mA;
}

void TMC2660Stepper::push() {
  DRVCTRL( sdoff() ? DRVCTRL_1_register.sr : DRVCTRL_0_register.sr);
  CHOPCONF(CHOPCONF_register.sr);
  SMARTEN(SMARTEN_register.sr);
  SGCSCONF(SGCSCONF_register.sr);
  DRVCONF(DRVCONF_register.sr);
}

void TMC2660Stepper::hysteresis_end(int8_t value) { SELF.hend(value+3); }
int8_t TMC2660Stepper::hysteresis_end() { return SELF.hend()-3; };

void TMC2660Stepper::hysteresis_start(uint8_t value) { SELF.hstrt(value-1); }
uint8_t TMC2660Stepper::hysteresis_start() { return SELF.hstrt()+1; }

void TMC2660Stepper::microsteps(uint16_t ms) {
  switch(ms) {
    case 256: SELF.mres(0); break;
    case 128: SELF.mres(1); break;
    case  64: SELF.mres(2); break;
    case  32: SELF.mres(3); break;
    case  16: SELF.mres(4); break;
    case   8: SELF.mres(5); break;
    case   4: SELF.mres(6); break;
    case   2: SELF.mres(7); break;
    case   0: SELF.mres(8); break;
    default: break;
  }
}

uint16_t TMC2660Stepper::microsteps() {
  switch(SELF.mres()) {
    case 0: return 256;
    case 1: return 128;
    case 2: return  64;
    case 3: return  32;
    case 4: return  16;
    case 5: return   8;
    case 6: return   4;
    case 7: return   2;
    case 8: return   0;
  }
  return 0;
}

void TMC2660Stepper::blank_time(uint8_t value) {
  switch (value) {
    case 16: SELF.tbl(0b00); break;
    case 24: SELF.tbl(0b01); break;
    case 36: SELF.tbl(0b10); break;
    case 54: SELF.tbl(0b11); break;
  }
}

uint8_t TMC2660Stepper::blank_time() {
  switch (SELF.tbl()) {
    case 0b00: return 16;
    case 0b01: return 24;
    case 0b10: return 36;
    case 0b11: return 54;
  }
  return 0;
}

// Explicit instantiation of all possible templates (linker will remove unused ones)
template class TMCStepper<TMC2130Stepper>;
template class TMCStepper<TMC5130Stepper>;
template class TMCStepper<TMC5160Stepper>;
template class TMCStepper<TMC2208Stepper>;
template class TMCStepper<TMC2224Stepper>;
template class TMC2130StepperBase<TMC2130Stepper>;
template class TMC5130StepperBase<TMC5130Stepper>;
template class TMC5160StepperBase<TMC5160Stepper>;
template class TMC2208StepperBase<TMC2208Stepper>;
template class TMC2208StepperBase<TMC2224Stepper>;

