#include "TMCStepper.h"
#include "TMC_MACROS.h"

template<typename T>
TMC5130StepperBase<T>::TMC5130StepperBase(uint16_t pinCS, float RS) : TMC2130StepperBase<T>(pinCS, RS) {}
template<typename T>
TMC5130StepperBase<T>::TMC5130StepperBase(uint16_t pinCS) : TMC2130StepperBase<T>(pinCS, 0.15) {}

template<typename T>
void TMC5130StepperBase<T>::begin() {
  //set pins
  pinMode(this->_pinCS, OUTPUT);
  this->switchCSpin(HIGH);

  if (this->uses_sw_spi) TMC_SW_SPI.init();

  GCONF(this->GCONF_register.sr);
  CHOPCONF(this->CHOPCONF_register.sr);
  COOLCONF(this->COOLCONF_register.sr);
  PWMCONF(this->PWMCONF_register.sr);
  IHOLD_IRUN(this->IHOLD_IRUN_register.sr);

  this->toff(8); //off_time(8);
  this->tbl(1); //blank_time(24);

  XTARGET(0);
  XACTUAL(0);
  //while (( RAMP_STAT() & cfg.VZERO_bm) != cfg.VZERO_bm) {}

  this->_started = true;
}

///////////////////////////////////////////////////////////////////////////////////////
// R: IFCNT
template<typename T>
uint8_t TMC5130StepperBase<T>::IFCNT() { return READ_REG(IFCNT); }
///////////////////////////////////////////////////////////////////////////////////////
// W: SLAVECONF
template<typename T>
uint16_t TMC5130StepperBase<T>::SLAVECONF() { return SLAVECONF_register.sr; }
template<typename T>
void TMC5130StepperBase<T>::SLAVECONF(uint16_t input) {
  SLAVECONF_register.sr = input;
  WRITE_REG(SLAVECONF);
}
///////////////////////////////////////////////////////////////////////////////////////
// R: IOIN
template<typename T>
uint32_t  TMC5130StepperBase<T>::IOIN() {
  IOIN_register.sr = READ_REG(IOIN);
  return IOIN_register.sr;
}
template<typename T>
bool    TMC5130StepperBase<T>::refl_step()      { IOIN(); return IOIN_register.refl_step; }
template<typename T>
bool    TMC5130StepperBase<T>::refr_dir()       { IOIN(); return IOIN_register.refr_dir; }
template<typename T>
bool    TMC5130StepperBase<T>::encb_dcen_cfg4() { IOIN(); return IOIN_register.encb_dcen_cfg4; }
template<typename T>
bool    TMC5130StepperBase<T>::enca_dcin_cfg5() { IOIN(); return IOIN_register.enca_dcin_cfg5; }
template<typename T>
bool    TMC5130StepperBase<T>::drv_enn_cfg6()   { IOIN(); return IOIN_register.drv_enn_cfg6; }
template<typename T>
bool    TMC5130StepperBase<T>::enc_n_dco()      { IOIN(); return IOIN_register.enc_n_dco; }
template<typename T>
bool    TMC5130StepperBase<T>::sd_mode()        { IOIN(); return IOIN_register.sd_mode; }
template<typename T>
bool    TMC5130StepperBase<T>::swcomp_in()      { IOIN(); return IOIN_register.swcomp_in; }
template<typename T>
uint8_t   TMC5130StepperBase<T>::version()      { IOIN(); return IOIN_register.version; }
///////////////////////////////////////////////////////////////////////////////////////
// W: OUTPUT
template<typename T>
bool TMC5130StepperBase<T>::TMC_OUTPUT() { return OUTPUT_register.sr; }
template<typename T>
void TMC5130StepperBase<T>::TMC_OUTPUT(bool input) {
  OUTPUT_register.sr = input;
  this->write(OUTPUT_register.address, OUTPUT_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: X_COMPARE
template<typename T>
uint32_t TMC5130StepperBase<T>::X_COMPARE() { return X_COMPARE_register.sr; }
template<typename T>
void TMC5130StepperBase<T>::X_COMPARE(uint32_t input) {
  X_COMPARE_register.sr = input;
  this->write(X_COMPARE_register.address, X_COMPARE_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// RW: RAMPMODE
template<typename T>
uint8_t TMC5130StepperBase<T>::RAMPMODE() { return READ_REG(RAMPMODE); }
template<typename T>
void TMC5130StepperBase<T>::RAMPMODE(uint8_t input) {
  RAMPMODE_register.sr = input;
  this->write(this->THIGH_register.address, this->THIGH_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// RW: XACTUAL
template<typename T>
int32_t TMC5130StepperBase<T>::XACTUAL() { return READ_REG(XACTUAL); }
template<typename T>
void TMC5130StepperBase<T>::XACTUAL(int32_t input) {
  this->write(XACTUAL_register.address, input);
}
///////////////////////////////////////////////////////////////////////////////////////
// R: VACTUAL
template<typename T>
int32_t TMC5130StepperBase<T>::VACTUAL() { return READ_REG(VACTUAL); }
///////////////////////////////////////////////////////////////////////////////////////
// W: VSTART
template<typename T>
uint32_t TMC5130StepperBase<T>::VSTART() { return VSTART_register.sr; }
template<typename T>
void TMC5130StepperBase<T>::VSTART(uint32_t input) {
  VSTART_register.sr = input;
  this->write(VSTART_register.address, VSTART_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: A1
template<typename T>
uint16_t TMC5130StepperBase<T>::A1() { return A1_register.sr; }
template<typename T>
void TMC5130StepperBase<T>::A1(uint16_t input) {
  A1_register.sr = input;
  this->write(A1_register.address, A1_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: V1
template<typename T>
uint32_t TMC5130StepperBase<T>::V1() { return V1_register.sr; }
template<typename T>
void TMC5130StepperBase<T>::V1(uint32_t input) {
  V1_register.sr = input;
  this->write(V1_register.address, V1_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: AMAX
template<typename T>
uint16_t TMC5130StepperBase<T>::AMAX() { return AMAX_register.sr; }
template<typename T>
void TMC5130StepperBase<T>::AMAX(uint16_t input) {
  AMAX_register.sr = input;
  this->write(AMAX_register.address, AMAX_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: VMAX
template<typename T>
uint32_t TMC5130StepperBase<T>::VMAX() { return VMAX_register.sr; }
template<typename T>
void TMC5130StepperBase<T>::VMAX(uint32_t input) {
  VMAX_register.sr = input;
  this->write(VMAX_register.address, VMAX_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: DMAX
template<typename T>
uint16_t TMC5130StepperBase<T>::DMAX() { return DMAX_register.sr; }
template<typename T>
void TMC5130StepperBase<T>::DMAX(uint16_t input) {
  DMAX_register.sr = input;
  this->write(DMAX_register.address, DMAX_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: D1
template<typename T>
uint16_t TMC5130StepperBase<T>::D1() { return D1_register.sr; }
template<typename T>
void TMC5130StepperBase<T>::D1(uint16_t input) {
  D1_register.sr = input;
  this->write(D1_register.address, D1_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: VSTOP
template<typename T>
uint32_t TMC5130StepperBase<T>::VSTOP() { return VSTOP_register.sr; }
template<typename T>
void TMC5130StepperBase<T>::VSTOP(uint32_t input) {
  if (input == 0 && RAMPMODE() == 0) return;
  VSTOP_register.sr = input;
  this->write(VSTOP_register.address, VSTOP_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// W: TZEROWAIT
template<typename T>
uint16_t TMC5130StepperBase<T>::TZEROWAIT() { return TZEROWAIT_register.sr; }
template<typename T>
void TMC5130StepperBase<T>::TZEROWAIT(uint16_t input) {
  TZEROWAIT_register.sr = input;
  this->write(TZEROWAIT_register.address, TZEROWAIT_register.sr);
}
///////////////////////////////////////////////////////////////////////////////////////
// RW: XTARGET
template<typename T>
int32_t TMC5130StepperBase<T>::XTARGET() { return READ_REG(XTARGET); }
template<typename T>
void TMC5130StepperBase<T>::XTARGET(int32_t input) {
  this->write(XTARGET_register.address, input);
}
