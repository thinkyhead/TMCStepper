#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG0(SETTING) DRVCTRL_0_register.SETTING = B; DRVCTRL(DRVCTRL_0_register.sr);
#define SET_REG1(SETTING) DRVCTRL_1_register.SETTING = B; DRVCTRL(DRVCTRL_1_register.sr);
#define GET_REG0(SETTING) return DRVCTRL_0_register.SETTING;
#define GET_REG1(SETTING) return DRVCTRL_1_register.SETTING;

uint32_t TMC2660Stepper::DRVCTRL() {
	if(sdoff() == 1) return DRVCTRL_1_register.sr;
	else return DRVCTRL_0_register.sr;
}
void TMC2660Stepper::DRVCTRL(uint32_t data) {
	if(sdoff() == 1) {
		DRVCTRL_1_register.sr = data;
		WRITE_REG(DRVCTRL_1);
	} else {
		DRVCTRL_0_register.sr = data;
		WRITE_REG(DRVCTRL_0);
	}
}

// DRVCTRL (SPI)
void TMC2660Stepper::pha(bool B)    { if(sdoff() == 0) return; SET_REG1(pha); }
void TMC2660Stepper::ca(uint8_t B)  { if(sdoff() == 0) return; SET_REG1(ca);  }
void TMC2660Stepper::phb(bool B)    { if(sdoff() == 0) return; SET_REG1(phb); }
void TMC2660Stepper::cb(uint8_t B)  { if(sdoff() == 0) return; SET_REG1(cb);  }

bool TMC2660Stepper::pha()    { if(sdoff() == 0) sdoff(1); GET_REG1(pha); }
uint8_t TMC2660Stepper::ca()  { if(sdoff() == 0) sdoff(1); GET_REG1(ca);  }
bool TMC2660Stepper::phb()    { if(sdoff() == 0) sdoff(1); GET_REG1(phb); }
uint8_t TMC2660Stepper::cb()  { if(sdoff() == 0) sdoff(1); GET_REG1(cb);  }

// DRVCTRL (STEP/DIR)
void TMC2660Stepper::intpol(bool B) { if(sdoff()) return; SET_REG0(intpol); }
void TMC2660Stepper::dedge(bool B)  { if(sdoff()) return; SET_REG0(dedge);  }
void TMC2660Stepper::mres(uint8_t B){ if(sdoff()) return; SET_REG0(mres);   }

bool TMC2660Stepper::intpol() { if(sdoff()) sdoff(0); GET_REG0(intpol); }
bool TMC2660Stepper::dedge()  { if(sdoff()) sdoff(0); GET_REG0(dedge);  }
uint8_t TMC2660Stepper::mres(){ if(sdoff()) sdoff(0); GET_REG0(mres);   }

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

