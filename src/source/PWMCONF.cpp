#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) PWMCONF_register.SETTING = B; WRITE_REG(PWMCONF);
#define GET_REG(SETTING) return PWMCONF_register.SETTING;

// PWMCONF
template<typename T>
uint32_t TMC2130StepperBase<T>::PWMCONF() { return PWMCONF_register.sr; }
template<typename T>
void TMC2130StepperBase<T>::PWMCONF(uint32_t input) {
	PWMCONF_register.sr = input;
	WRITE_REG(PWMCONF);
}

template<typename T>
void TMC2130StepperBase<T>::pwm_ampl(		uint8_t B )	{ SET_REG(pwm_ampl);		}
template<typename T>
void TMC2130StepperBase<T>::pwm_grad(		uint8_t B )	{ SET_REG(pwm_grad);		}
template<typename T>
void TMC2130StepperBase<T>::pwm_freq(		uint8_t B )	{ SET_REG(pwm_freq);		}
template<typename T>
void TMC2130StepperBase<T>::pwm_autoscale(	bool 	B )	{ SET_REG(pwm_autoscale);	}
template<typename T>
void TMC2130StepperBase<T>::pwm_symmetric(	bool 	B )	{ SET_REG(pwm_symmetric);	}
template<typename T>
void TMC2130StepperBase<T>::freewheel(		uint8_t B )	{ SET_REG(freewheel);		}

template<typename T>
uint8_t TMC2130StepperBase<T>::pwm_ampl()		{ GET_REG(pwm_ampl);		}
template<typename T>
uint8_t TMC2130StepperBase<T>::pwm_grad()		{ GET_REG(pwm_grad);		}
template<typename T>
uint8_t TMC2130StepperBase<T>::pwm_freq()		{ GET_REG(pwm_freq);		}
template<typename T>
bool 	TMC2130StepperBase<T>::pwm_autoscale()	{ GET_REG(pwm_autoscale);	}
template<typename T>
bool 	TMC2130StepperBase<T>::pwm_symmetric()	{ GET_REG(pwm_symmetric);	}
template<typename T>
uint8_t TMC2130StepperBase<T>::freewheel()		{ GET_REG(freewheel);		}

template<typename T>
uint32_t TMC2208StepperBase<T>::PWMCONF() {
	if (write_only) return PWMCONF_register.sr;
	PWMCONF_register.sr = READ_REG(PWMCONF);
	return PWMCONF_register.sr;
}
template<typename T>
void TMC2208StepperBase<T>::PWMCONF(uint32_t input) {
	PWMCONF_register.sr = input;
	WRITE_REG(PWMCONF);
}

template<typename T>
void TMC2208StepperBase<T>::pwm_ofs		( uint8_t B ) { PWMCONF_register.pwm_ofs = B; WRITE_REG(PWMCONF); }
template<typename T>
void TMC2208StepperBase<T>::pwm_grad		( uint8_t B ) { PWMCONF_register.pwm_grad = B; WRITE_REG(PWMCONF); }
template<typename T>
void TMC2208StepperBase<T>::pwm_freq		( uint8_t B ) { PWMCONF_register.pwm_freq = B; WRITE_REG(PWMCONF); }
template<typename T>
void TMC2208StepperBase<T>::pwm_autoscale	( bool 	  B ) { PWMCONF_register.pwm_autoscale = B; WRITE_REG(PWMCONF); }
template<typename T>
void TMC2208StepperBase<T>::pwm_autograd	( bool    B ) { PWMCONF_register.pwm_autograd = B; WRITE_REG(PWMCONF); }
template<typename T>
void TMC2208StepperBase<T>::freewheel		( uint8_t B ) { PWMCONF_register.freewheel = B; WRITE_REG(PWMCONF); }
template<typename T>
void TMC2208StepperBase<T>::pwm_reg		( uint8_t B ) { PWMCONF_register.pwm_reg = B; WRITE_REG(PWMCONF); }
template<typename T>
void TMC2208StepperBase<T>::pwm_lim		( uint8_t B ) { PWMCONF_register.pwm_lim = B; WRITE_REG(PWMCONF); }

template<typename T>
uint8_t TMC2208StepperBase<T>::pwm_ofs()		{ PWMCONF(); return PWMCONF_register.pwm_ofs;		}
template<typename T>
uint8_t TMC2208StepperBase<T>::pwm_grad()		{ PWMCONF(); return PWMCONF_register.pwm_grad;		}
template<typename T>
uint8_t TMC2208StepperBase<T>::pwm_freq()		{ PWMCONF(); return PWMCONF_register.pwm_freq;		}
template<typename T>
bool 	TMC2208StepperBase<T>::pwm_autoscale()	{ PWMCONF(); return PWMCONF_register.pwm_autoscale;	}
template<typename T>
bool 	TMC2208StepperBase<T>::pwm_autograd()	{ PWMCONF(); return PWMCONF_register.pwm_autograd;	}
template<typename T>
uint8_t TMC2208StepperBase<T>::freewheel()		{ PWMCONF(); return PWMCONF_register.freewheel;		}
template<typename T>
uint8_t TMC2208StepperBase<T>::pwm_reg()		{ PWMCONF(); return PWMCONF_register.pwm_reg;		}
template<typename T>
uint8_t TMC2208StepperBase<T>::pwm_lim()		{ PWMCONF(); return PWMCONF_register.pwm_lim;		}

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

