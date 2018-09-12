#include "TMCStepper.h"
#include "TMC_MACROS.h"

template<typename T>
TMC2130StepperBase<T>::TMC2130StepperBase(uint16_t pinCS, float RS) :
  TMCStepper<T>(RS),
  _pinCS(pinCS)
  {}

template<typename T>
TMC2130StepperBase<T>::TMC2130StepperBase(uint16_t pinCS, float RS, uint16_t pinMOSI, uint16_t pinMISO, uint16_t pinSCK) :
  TMCStepper<T>(RS),
  _pinCS(pinCS),
  uses_sw_spi(true)
  { TMC_SW_SPI.setPins(pinMOSI, pinMISO, pinSCK); }

template<typename T>
void TMC2130StepperBase<T>::setSPISpeed(uint32_t speed) {
  spi_speed = speed;
}

template<typename T>
void TMC2130StepperBase<T>::switchCSpin(bool state) {
  // Allows for overriding in child class to make use of fast io
  digitalWrite(_pinCS, state);
}

template<typename T>
uint32_t TMC2130StepperBase<T>::read(uint8_t addressByte) {
  uint32_t out = 0UL;
  if (this->uses_sw_spi) {
    switchCSpin(LOW);
    TMC_SW_SPI.transfer(addressByte & 0xFF);
    TMC_SW_SPI.transfer16(0x0000); // Clear SPI
    TMC_SW_SPI.transfer16(0x0000);

    switchCSpin(HIGH);
    switchCSpin(LOW);

    status_response = TMC_SW_SPI.transfer(addressByte & 0xFF); // Send the address byte again
    out  = TMC_SW_SPI.transfer(0x00);
    out <<= 8;
    out |= TMC_SW_SPI.transfer(0x00);
    out <<= 8;
    out |= TMC_SW_SPI.transfer(0x00);
    out <<= 8;
    out |= TMC_SW_SPI.transfer(0x00);

  } else {
    SPI.beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE3));
    switchCSpin(LOW);
    SPI.transfer(addressByte & 0xFF);
    SPI.transfer16(0x0000); // Clear SPI
    SPI.transfer16(0x0000);

    switchCSpin(HIGH);
    switchCSpin(LOW);

    status_response = SPI.transfer(addressByte & 0xFF); // Send the address byte again
    out  = SPI.transfer(0x00);
    out <<= 8;
    out |= SPI.transfer(0x00);
    out <<= 8;
    out |= SPI.transfer(0x00);
    out <<= 8;
    out |= SPI.transfer(0x00);

    SPI.endTransaction();
  }
  switchCSpin(HIGH);
  return out;
}

template<typename T>
void TMC2130StepperBase<T>::write(uint8_t addressByte, uint32_t config) {
  addressByte |= this->TMC_WRITE;
  if (this->uses_sw_spi) {
    switchCSpin(LOW);
    status_response = TMC_SW_SPI.transfer(addressByte & 0xFF);
    TMC_SW_SPI.transfer16((config>>16) & 0xFFFF);
    TMC_SW_SPI.transfer16(config & 0xFFFF);
  } else {
    SPI.beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE3));
    switchCSpin(LOW);
    status_response = SPI.transfer(addressByte & 0xFF);
    SPI.transfer16((config>>16) & 0xFFFF);
    SPI.transfer16(config & 0xFFFF);
    SPI.endTransaction();
  }
  switchCSpin(HIGH);
}

template<typename T>
void TMC2130StepperBase<T>::begin() {
  //set pins
  pinMode(_pinCS, OUTPUT);
  switchCSpin(HIGH);

  if (this->uses_sw_spi) TMC_SW_SPI.init();

  GCONF(GCONF_register.sr);
  CHOPCONF(CHOPCONF_register.sr);
  COOLCONF(COOLCONF_register.sr);
  PWMCONF(PWMCONF_register.sr);
  this->IHOLD_IRUN(this->IHOLD_IRUN_register.sr);

  toff(8); //off_time(8);
  SELF.tbl(1); //blank_time(24);

  this->_started = true;
}

/**
 *  Helper functions
 */

template<typename T>
bool TMC2130StepperBase<T>::isEnabled() { return !drv_enn_cfg6() && toff(); }

template<typename T>
void TMC2130StepperBase<T>::push() {
  this->GCONF(GCONF_register.sr);
  this->IHOLD_IRUN(this->IHOLD_IRUN_register.sr);
  this->TPOWERDOWN(this->TPOWERDOWN_register.sr);
  this->TPWMTHRS(this->TPWMTHRS_register.sr);
  this->TCOOLTHRS(this->TCOOLTHRS_register.sr);
  this->THIGH(this->THIGH_register.sr);
  this->XDIRECT(this->XDIRECT_register.sr);
  this->VDCMIN(this->VDCMIN_register.sr);
  this->CHOPCONF(this->CHOPCONF_register.sr);
  this->COOLCONF(this->COOLCONF_register.sr);
  this->PWMCONF(this->PWMCONF_register.sr);
  this->ENCM_CTRL(this->ENCM_CTRL_register.sr);
}

///////////////////////////////////////////////////////////////////////////////////////
// R: IOIN
template<typename T>
uint32_t  TMC2130StepperBase<T>::IOIN() {
  this->IOIN_register.sr = READ_REG(IOIN);
  return this->IOIN_register.sr;
}
template<typename T>
bool TMC2130StepperBase<T>::step()         { IOIN(); return IOIN_register.step; }
template<typename T>
bool TMC2130StepperBase<T>::dir()          { IOIN(); return IOIN_register.dir; }
template<typename T>
bool TMC2130StepperBase<T>::dcen_cfg4()    { IOIN(); return IOIN_register.dcen_cfg4; }
template<typename T>
bool TMC2130StepperBase<T>::dcin_cfg5()    { IOIN(); return IOIN_register.dcin_cfg5; }
template<typename T>
bool TMC2130StepperBase<T>::drv_enn_cfg6() { IOIN(); return IOIN_register.drv_enn_cfg6; }
template<typename T>
bool TMC2130StepperBase<T>::dco()          { IOIN(); return IOIN_register.dco; }
template<typename T>
uint8_t TMC2130StepperBase<T>::version()   { IOIN(); return IOIN_register.version; }
///////////////////////////////////////////////////////////////////////////////////////
// W: TCOOLTHRS
template<typename T>
uint32_t TMC2130StepperBase<T>::TCOOLTHRS() { return TCOOLTHRS_register.sr; }
template<typename T>
void TMC2130StepperBase<T>::TCOOLTHRS(uint32_t input) {
  TCOOLTHRS_register.sr = input;
  SELF.write(TCOOLTHRS_register.address, TCOOLTHRS_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: THIGH
template<typename T>
uint32_t TMC2130StepperBase<T>::THIGH() { return THIGH_register.sr; }
template<typename T>
void TMC2130StepperBase<T>::THIGH(uint32_t input) {
  THIGH_register.sr = input;
  SELF.write(THIGH_register.address, THIGH_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// RW: XDIRECT
template<typename T>
uint32_t TMC2130StepperBase<T>::XDIRECT() {
  XDIRECT_register.sr = READ_REG(XDIRECT);
  return XDIRECT_register.sr;
}
template<typename T>
void TMC2130StepperBase<T>::XDIRECT(uint32_t input) {
  XDIRECT_register.sr = input;
  WRITE_REG(XDIRECT);
}
template<typename T>
void TMC2130StepperBase<T>::coil_A(int16_t B)  { XDIRECT_register.coil_A = B; WRITE_REG(XDIRECT); }
template<typename T>
void TMC2130StepperBase<T>::coil_B(int16_t B)  { XDIRECT_register.coil_B = B; WRITE_REG(XDIRECT); }
template<typename T>
int16_t TMC2130StepperBase<T>::coil_A()        { XDIRECT(); return XDIRECT_register.coil_A; }
template<typename T>
int16_t TMC2130StepperBase<T>::coil_B()        { XDIRECT(); return XDIRECT_register.coil_B; }
///////////////////////////////////////////////////////////////////////////////////////
// W: VDCMIN
template<typename T>
uint32_t TMC2130StepperBase<T>::VDCMIN() { return VDCMIN_register.sr; }
template<typename T>
void TMC2130StepperBase<T>::VDCMIN(uint32_t input) {
  VDCMIN_register.sr = input;
  SELF.write(VDCMIN_register.address, VDCMIN_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// R: PWM_SCALE
template<typename T>
uint8_t TMC2130StepperBase<T>::PWM_SCALE() { return READ_REG(PWM_SCALE); }
///////////////////////////////////////////////////////////////////////////////////////
// W: ENCM_CTRL
template<typename T>
uint8_t TMC2130StepperBase<T>::ENCM_CTRL() { return ENCM_CTRL_register.sr; }
template<typename T>
void TMC2130StepperBase<T>::ENCM_CTRL(uint8_t input) {
  ENCM_CTRL_register.sr = input;
  WRITE_REG(ENCM_CTRL);
}
template<typename T>
void TMC2130StepperBase<T>::inv(bool B)      { ENCM_CTRL_register.inv = B; WRITE_REG(ENCM_CTRL); }
template<typename T>
void TMC2130StepperBase<T>::maxspeed(bool B) { ENCM_CTRL_register.maxspeed  = B; WRITE_REG(ENCM_CTRL); }
template<typename T>
bool TMC2130StepperBase<T>::inv()            { return ENCM_CTRL_register.inv; }
template<typename T>
bool TMC2130StepperBase<T>::maxspeed()       { return ENCM_CTRL_register.maxspeed; }
///////////////////////////////////////////////////////////////////////////////////////
// R: LOST_STEPS
template<typename T>
uint32_t TMC2130StepperBase<T>::LOST_STEPS() { return READ_REG(LOST_STEPS); }

template<typename T>
void TMC2130StepperBase<T>::sg_current_decrease(uint8_t value) {
  switch(value) {
    case 32: sedn(0b00); break;
    case  8: sedn(0b01); break;
    case  2: sedn(0b10); break;
    case  1: sedn(0b11); break;
  }
}
template<typename T>
uint8_t TMC2130StepperBase<T>::sg_current_decrease() {
  switch(sedn()) {
    case 0b00: return 32;
    case 0b01: return  8;
    case 0b10: return  2;
    case 0b11: return  1;
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

