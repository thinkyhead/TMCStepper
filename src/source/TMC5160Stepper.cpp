#include "TMCStepper.h"
#include "TMC_MACROS.h"

template<typename T>
TMC5160StepperBase<T>::TMC5160StepperBase(uint16_t pinCS, float RS) : TMC5130StepperBase<T>(pinCS, RS) {}

/*
  Requested current = mA = I_rms/1000
  Equation for current:
  I_rms = GLOBALSCALER/256 * (CS+1)/32 * V_fs/R_sense * 1/sqrt(2)
  Solve for CS ->
       32 * 256 * sqrt(2) * I_rms * R_sense      |
  CS = ------------------------------------ -1   |
                GLOBALSCALER * V_fs              | V_fs = 0.325
*/
template<typename T>
void TMC5160StepperBase<T>::rms_current(uint16_t mA) {
  uint32_t V_fs = 325; // x1000
  uint16_t scaler = GLOBAL_SCALER();
  if (!scaler) scaler = 256;

  uint32_t numerator = 1414UL * mA;
  numerator *= this->Rsense * 1000UL;
  uint32_t CS = numerator / scaler;
  CS /= V_fs;
  CS <<= (5+8); // Multiply by 32 and 256
  CS /= 1000UL;
  CS /= 1000UL;
  CS -= 1.0;
  this->irun(CS);
  this->ihold(CS * this->holdMultiplier);
  //val_mA = mA;
}
template<typename T>
void TMC5160StepperBase<T>::rms_current(uint16_t mA, float mult) {
  this->holdMultiplier = mult;
  rms_current(mA);
}
template<typename T>
uint16_t TMC5160StepperBase<T>::rms_current() {
    uint16_t scaler = GLOBAL_SCALER();
    if (!scaler) scaler = 256;
    uint32_t numerator = scaler * (this->irun()+1);
    numerator *= 325;
    numerator >>= (8+5); // Divide by 256 and 32
    numerator *= 1000000;
    uint32_t denominator = this->Rsense * 1000;
    denominator *= 1414;

    return numerator / denominator;
}

// W: GLOBAL_SCALER
template<typename T>
uint8_t TMC5160StepperBase<T>::GLOBAL_SCALER() { return GLOBAL_SCALER_register.sr; }
template<typename T>
void TMC5160StepperBase<T>::GLOBAL_SCALER(uint8_t input) {
  GLOBAL_SCALER_register.sr = input;
  this->write(GLOBAL_SCALER_register.address, GLOBAL_SCALER_register.sr);
}

// R: OFFSET_READ
template<typename T>
uint16_t TMC5160StepperBase<T>::OFFSET_READ() { return READ_REG(OFFSET_READ); }

// R+WC: ENC_STATUS
template<typename T>
uint8_t TMC5160StepperBase<T>::ENC_STATUS() { return READ_REG(ENC_STATUS); }
template<typename T>
void TMC5160StepperBase<T>::ENC_STATUS(uint8_t input) {
	this->write(this->ENC_STATUS_register.address, input & 0x3);
}

// W: ENC_DEVIATION
template<typename T>
uint32_t TMC5160StepperBase<T>::ENC_DEVIATION() { return this->ENC_DEVIATION_register.sr; }
template<typename T>
void TMC5160StepperBase<T>::ENC_DEVIATION(uint32_t input) {
	ENC_DEVIATION_register.sr = input;
	this->write(this->ENC_DEVIATION_register.address, this->ENC_DEVIATION_register.sr);
}

// R: PWM_SCALE
template<typename T>
uint32_t TMC5160StepperBase<T>::PWM_SCALE() {
	PWM_SCALE_register.sr = READ_REG(PWM_SCALE);
	return PWM_SCALE_register.sr;
}
template<typename T>
uint8_t TMC5160StepperBase<T>::pwm_scale_sum() { PWM_SCALE(); return PWM_SCALE_register.pwm_scale_sum; }
template<typename T>
uint16_t TMC5160StepperBase<T>::pwm_scale_auto() { PWM_SCALE(); return PWM_SCALE_register.pwm_scale_auto; }

// R: PWM_AUTO
template<typename T>
uint32_t TMC5160StepperBase<T>::PWM_AUTO() {
	PWM_AUTO_register.sr = READ_REG(PWM_AUTO);
	return PWM_AUTO_register.sr;
}
template<typename T>
uint8_t TMC5160StepperBase<T>::pwm_ofs_auto() { PWM_AUTO(); return PWM_AUTO_register.pwm_ofs_auto; }
template<typename T>
uint8_t TMC5160StepperBase<T>::pwm_grad_auto() { PWM_AUTO(); return PWM_AUTO_register.pwm_grad_auto; }
