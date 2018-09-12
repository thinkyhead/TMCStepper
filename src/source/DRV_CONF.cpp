#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) DRV_CONF_register.SETTING = B; WRITE_REG(DRV_CONF);
#define GET_REG(SETTING) return DRV_CONF_register.SETTING;

// W: DRV_CONF
template<typename T>
uint32_t TMC5160StepperBase<T>::DRV_CONF() { return DRV_CONF_register.sr; }
template<typename T>
void TMC5160StepperBase<T>::DRV_CONF(uint32_t input) {
	DRV_CONF_register.sr = input;
	WRITE_REG(DRV_CONF);
}

template<typename T>
void TMC5160StepperBase<T>::bbmtime(uint8_t B)		{ SET_REG(bbmtime); 	}
template<typename T>
void TMC5160StepperBase<T>::bbmclks(uint8_t B)		{ SET_REG(bbmclks); 	}
template<typename T>
void TMC5160StepperBase<T>::otselect(uint8_t B)	{ SET_REG(otselect); 	}
template<typename T>
void TMC5160StepperBase<T>::drvstrength(uint8_t B)	{ SET_REG(drvstrength); }
template<typename T>
void TMC5160StepperBase<T>::filt_isense(uint8_t B)	{ SET_REG(filt_isense); }
template<typename T>
uint8_t TMC5160StepperBase<T>::bbmtime()			{ GET_REG(bbmtime);		}
template<typename T>
uint8_t TMC5160StepperBase<T>::bbmclks()			{ GET_REG(bbmclks);		}
template<typename T>
uint8_t TMC5160StepperBase<T>::otselect()			{ GET_REG(otselect);	}
template<typename T>
uint8_t TMC5160StepperBase<T>::drvstrength()		{ GET_REG(drvstrength);	}
template<typename T>
uint8_t TMC5160StepperBase<T>::filt_isense()		{ GET_REG(filt_isense);	}

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

