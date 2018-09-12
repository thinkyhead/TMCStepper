#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) SHORT_CONF_register.SETTING = B; WRITE_REG(SHORT_CONF);
#define GET_REG(SETTING) return SHORT_CONF_register.SETTING;

template<typename T>
uint32_t TMC5160StepperBase<T>::SHORT_CONF() { return SHORT_CONF_register.sr; }
template<typename T>
void TMC5160StepperBase<T>::SHORT_CONF(uint32_t input) {
	SHORT_CONF_register.sr = input;
	WRITE_REG(SHORT_CONF);
}

template<typename T>
void TMC5160StepperBase<T>::s2vs_level(uint8_t B)	{ SET_REG(s2vs_level);	}
template<typename T>
void TMC5160StepperBase<T>::s2g_level(uint8_t B)	{ SET_REG(s2g_level);	}
template<typename T>
void TMC5160StepperBase<T>::shortfilter(uint8_t B)	{ SET_REG(shortfilter);	}
template<typename T>
void TMC5160StepperBase<T>::shortdelay(bool B)		{ SET_REG(shortdelay);	}
template<typename T>
uint8_t TMC5160StepperBase<T>::s2vs_level()		{ GET_REG(s2vs_level);	}
template<typename T>
uint8_t TMC5160StepperBase<T>::s2g_level()			{ GET_REG(s2g_level);	}
template<typename T>
uint8_t TMC5160StepperBase<T>::shortfilter()		{ GET_REG(shortfilter);	}
template<typename T>
bool TMC5160StepperBase<T>::shortdelay()			{ GET_REG(shortdelay);	}