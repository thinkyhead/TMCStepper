#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) this->IHOLD_IRUN_register.SETTING = B; WRITE_REG(IHOLD_IRUN);
#define GET_REG(SETTING) return this->IHOLD_IRUN_register.SETTING;

// IHOLD_IRUN
template<typename T>
uint32_t TMCStepper<T>::IHOLD_IRUN() { return this->IHOLD_IRUN_register.sr; }
template<typename T>
void TMCStepper<T>::IHOLD_IRUN(uint32_t input) {
	this->IHOLD_IRUN_register.sr = input;
	WRITE_REG(IHOLD_IRUN);
}

template<typename T>
void 	TMCStepper<T>::ihold(uint8_t B)			{ SET_REG(ihold);		}
template<typename T>
void 	TMCStepper<T>::irun(uint8_t B)			{ SET_REG(irun); 		}
template<typename T>
void 	TMCStepper<T>::iholddelay(uint8_t B)	{ SET_REG(iholddelay); 	}

template<typename T>
uint8_t TMCStepper<T>::ihold()					{ GET_REG(ihold);		}
template<typename T>
uint8_t TMCStepper<T>::irun()					{ GET_REG(irun); 		}
template<typename T>
uint8_t TMCStepper<T>::iholddelay()				{ GET_REG(iholddelay);	}
