#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) COOLCONF_register.SETTING = B; WRITE_REG(COOLCONF);
#define GET_REG(SETTING) COOLCONF(); return COOLCONF_register.SETTING;

// COOLCONF
template<typename T>
uint32_t TMC2130StepperBase<T>::COOLCONF() { return COOLCONF_register.sr; }
template<typename T>
void TMC2130StepperBase<T>::COOLCONF(uint32_t input) {
	COOLCONF_register.sr = input;
	WRITE_REG(COOLCONF);
}

template<typename T>
void TMC2130StepperBase<T>::semin(	uint8_t B )	{ SET_REG(semin);	}
template<typename T>
void TMC2130StepperBase<T>::seup(	uint8_t B )	{ SET_REG(seup);	}
template<typename T>
void TMC2130StepperBase<T>::semax(	uint8_t B )	{ SET_REG(semax);	}
template<typename T>
void TMC2130StepperBase<T>::sedn(	uint8_t B )	{ SET_REG(sedn);	}
template<typename T>
void TMC2130StepperBase<T>::seimin(bool 	B )	{ SET_REG(seimin);	}
template<typename T>
void TMC2130StepperBase<T>::sgt(	int8_t  B )	{ SET_REG(sgt);		}
template<typename T>
void TMC2130StepperBase<T>::sfilt(	bool 	B )	{ SET_REG(sfilt);	}

template<typename T>
uint8_t TMC2130StepperBase<T>::semin()	{ GET_REG(semin);	}
template<typename T>
uint8_t TMC2130StepperBase<T>::seup()	{ GET_REG(seup);	}
template<typename T>
uint8_t TMC2130StepperBase<T>::semax()	{ GET_REG(semax);	}
template<typename T>
uint8_t TMC2130StepperBase<T>::sedn()	{ GET_REG(sedn);	}
template<typename T>
bool 	TMC2130StepperBase<T>::seimin(){ GET_REG(seimin);	}
template<typename T>
bool 	TMC2130StepperBase<T>::sfilt()	{ GET_REG(sfilt);	}

template<typename T>
int8_t TMC2130StepperBase<T>::sgt() {
	// Two's complement in a 7bit value
	int8_t val = (COOLCONF_register.sgt &  0x40) << 1; // Isolate sign bit
	val |= COOLCONF_register.sgt & 0x7F;
	return val;
}
