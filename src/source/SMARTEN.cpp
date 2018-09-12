#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) SMARTEN_register.SETTING = B; WRITE_REG(SMARTEN);
#define GET_REG(SETTING) return SMARTEN_register.SETTING;

uint32_t TMC2660Stepper::SMARTEN() { return SMARTEN_register.sr; }
void TMC2660Stepper::SMARTEN(uint32_t data) {
  SMARTEN_register.sr = data;
  WRITE_REG(SMARTEN);
}

void TMC2660Stepper::seimin(bool B) 	{ SET_REG(seimin);	}
void TMC2660Stepper::sedn(uint8_t B) 	{ SET_REG(sedn); 	}
void TMC2660Stepper::semax(uint8_t B) 	{ SET_REG(semax);	}
void TMC2660Stepper::seup(uint8_t B) 	{ SET_REG(seup);	}
void TMC2660Stepper::semin(uint8_t B) 	{ SET_REG(semin);	}

bool TMC2660Stepper::seimin() 	{ GET_REG(seimin);	}
uint8_t TMC2660Stepper::sedn() 	{ GET_REG(sedn);	}
uint8_t TMC2660Stepper::semax() { GET_REG(semax);	}
uint8_t TMC2660Stepper::seup() 	{ GET_REG(seup);	}
uint8_t TMC2660Stepper::semin() { GET_REG(semin);	}

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

