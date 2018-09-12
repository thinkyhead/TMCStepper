#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) SGCSCONF_register.SETTING = B; WRITE_REG(SGCSCONF);
#define GET_REG(SETTING) return SGCSCONF_register.SETTING;

uint32_t TMC2660Stepper::SGCSCONF() { return SGCSCONF_register.sr; }
void TMC2660Stepper::SGCSCONF(uint32_t data) {
  SGCSCONF_register.sr = data;
  WRITE_REG(SGCSCONF);
}

void TMC2660Stepper::sfilt(bool B) 	{ SET_REG(sfilt); }
void TMC2660Stepper::sgt(uint8_t B) { SET_REG(sgt); }
void TMC2660Stepper::cs(uint8_t B) 	{ SET_REG(cs); }

bool TMC2660Stepper::sfilt() { GET_REG(sfilt); }
uint8_t TMC2660Stepper::sgt(){ GET_REG(sgt); }
uint8_t TMC2660Stepper::cs() { GET_REG(cs); }

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

