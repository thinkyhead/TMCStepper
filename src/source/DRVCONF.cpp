#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) DRVCONF_register.SETTING = B; WRITE_REG(DRVCONF);
#define GET_REG(SETTING) return DRVCONF_register.SETTING;

uint32_t TMC2660Stepper::DRVCONF() { return DRVCONF_register.sr; }
void TMC2660Stepper::DRVCONF(uint32_t data) {
  DRVCONF_register.sr = data;
  WRITE_REG(DRVCONF);
}

void TMC2660Stepper::tst(bool B)      { SET_REG(tst);	}
void TMC2660Stepper::slph(uint8_t B)  { SET_REG(slph);	}
void TMC2660Stepper::slpl(uint8_t B)  { SET_REG(slpl);	}
void TMC2660Stepper::diss2g(bool B)   { SET_REG(diss2g);}
void TMC2660Stepper::ts2g(uint8_t B)  { SET_REG(ts2g);	}
void TMC2660Stepper::sdoff(bool B)    { SET_REG(sdoff);	}
void TMC2660Stepper::vsense(bool B)   { SET_REG(vsense);}
void TMC2660Stepper::rdsel(uint8_t B) { SET_REG(rdsel);	}

bool	 TMC2660Stepper::tst()		{ GET_REG(tst);		}
uint8_t	 TMC2660Stepper::slph()		{ GET_REG(slph);	}
uint8_t	 TMC2660Stepper::slpl()		{ GET_REG(slpl);	}
bool	 TMC2660Stepper::diss2g()	{ GET_REG(diss2g);	}
uint8_t	 TMC2660Stepper::ts2g()		{ GET_REG(ts2g);	}
bool	 TMC2660Stepper::sdoff()	{ GET_REG(sdoff);	}
bool	 TMC2660Stepper::vsense()	{ GET_REG(vsense);	}
uint8_t	 TMC2660Stepper::rdsel()	{ GET_REG(rdsel);	}

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

