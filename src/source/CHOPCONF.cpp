#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) this->CHOPCONF_register.SETTING = B; WRITE_REG(CHOPCONF);
#define GET_REG(SETTING) this->CHOPCONF(); return this->CHOPCONF_register.SETTING;

// CHOPCONF
template<typename T>
uint32_t TMC2130StepperBase<T>::CHOPCONF() {
	CHOPCONF_register.sr = READ_REG(CHOPCONF);
	return CHOPCONF_register.sr;
}
template<typename T>
void TMC2130StepperBase<T>::CHOPCONF(uint32_t input) {
	CHOPCONF_register.sr = input;
	WRITE_REG(CHOPCONF);
}

template<typename T>
void TMC2130StepperBase<T>::toff(		uint8_t B )	{ SET_REG(toff);	}
template<typename T>
void TMC2130StepperBase<T>::hstrt(		uint8_t B )	{ SET_REG(hstrt);	}
template<typename T>
void TMC2130StepperBase<T>::hend(		uint8_t B )	{ SET_REG(hend);	}
//template<typename T>
//void TMC2130StepperBase<T>::fd(		uint8_t B )	{ SET_REG(fd);		}
template<typename T>
void TMC2130StepperBase<T>::disfdcc(	bool 	B )	{ SET_REG(disfdcc);	}
template<typename T>
void TMC2130StepperBase<T>::rndtf(		bool 	B )	{ SET_REG(rndtf);	}
template<typename T>
void TMC2130StepperBase<T>::chm(		bool 	B )	{ SET_REG(chm);		}
template<typename T>
void TMC2130StepperBase<T>::tbl(		uint8_t B )	{ SET_REG(tbl);		}
template<typename T>
void TMC2130StepperBase<T>::vsense(		bool 	B )	{ SET_REG(vsense);	}
template<typename T>
void TMC2130StepperBase<T>::vhighfs(	bool 	B )	{ SET_REG(vhighfs);	}
template<typename T>
void TMC2130StepperBase<T>::vhighchm(	bool 	B )	{ SET_REG(vhighchm);}
template<typename T>
void TMC2130StepperBase<T>::sync(		uint8_t B )	{ SET_REG(sync);	}
template<typename T>
void TMC2130StepperBase<T>::mres(		uint8_t B )	{ SET_REG(mres);	}
template<typename T>
void TMC2130StepperBase<T>::intpol(		bool 	B )	{ SET_REG(intpol);	}
template<typename T>
void TMC2130StepperBase<T>::dedge(		bool 	B )	{ SET_REG(dedge);	}
template<typename T>
void TMC2130StepperBase<T>::diss2g(		bool 	B )	{ SET_REG(diss2g);	}

template<typename T>
uint8_t TMC2130StepperBase<T>::toff()		{ GET_REG(toff);	}
template<typename T>
uint8_t TMC2130StepperBase<T>::hstrt()		{ GET_REG(hstrt);	}
template<typename T>
uint8_t TMC2130StepperBase<T>::hend()		{ GET_REG(hend);	}
//template<typename T>
//uint8_t TMC2130StepperBase<T>::fd()		{ GET_REG(fd);		}
template<typename T>
bool 	TMC2130StepperBase<T>::disfdcc()	{ GET_REG(disfdcc);	}
template<typename T>
bool 	TMC2130StepperBase<T>::rndtf()		{ GET_REG(rndtf);	}
template<typename T>
bool 	TMC2130StepperBase<T>::chm()		{ GET_REG(chm);		}
template<typename T>
uint8_t TMC2130StepperBase<T>::tbl()		{ GET_REG(tbl);		}
template<typename T>
bool 	TMC2130StepperBase<T>::vsense()		{ GET_REG(vsense);	}
template<typename T>
bool 	TMC2130StepperBase<T>::vhighfs()	{ GET_REG(vhighfs);	}
template<typename T>
bool 	TMC2130StepperBase<T>::vhighchm()	{ GET_REG(vhighchm);}
template<typename T>
uint8_t TMC2130StepperBase<T>::sync()		{ GET_REG(sync);	}
template<typename T>
uint8_t TMC2130StepperBase<T>::mres()		{ GET_REG(mres);	}
template<typename T>
bool 	TMC2130StepperBase<T>::intpol()		{ GET_REG(intpol);	}
template<typename T>
bool 	TMC2130StepperBase<T>::dedge()		{ GET_REG(dedge);	}
template<typename T>
bool 	TMC2130StepperBase<T>::diss2g()		{ GET_REG(diss2g);	}

template<typename T>
void TMC2208StepperBase<T>::CHOPCONF(uint32_t input) {
	CHOPCONF_register.sr = input;
	WRITE_REG(CHOPCONF);
}
template<typename T>
uint32_t TMC2208StepperBase<T>::CHOPCONF() {
	if (write_only) return CHOPCONF_register.sr;
	CHOPCONF_register.sr = READ_REG(CHOPCONF);
	return CHOPCONF_register.sr;
}
template<typename T>
void TMC2208StepperBase<T>::toff	( uint8_t  B )	{ SET_REG(toff); 	}
template<typename T>
void TMC2208StepperBase<T>::hstrt	( uint8_t  B )	{ SET_REG(hstrt); 	}
template<typename T>
void TMC2208StepperBase<T>::hend	( uint8_t  B )	{ SET_REG(hend); 	}
template<typename T>
void TMC2208StepperBase<T>::tbl		( uint8_t  B )	{ SET_REG(tbl); 	}
template<typename T>
void TMC2208StepperBase<T>::vsense	( bool     B )	{ SET_REG(vsense); 	}
template<typename T>
void TMC2208StepperBase<T>::mres	( uint8_t  B )	{ SET_REG(mres); 	}
template<typename T>
void TMC2208StepperBase<T>::intpol	( bool     B )	{ SET_REG(intpol); 	}
template<typename T>
void TMC2208StepperBase<T>::dedge	( bool     B )	{ SET_REG(dedge); 	}
template<typename T>
void TMC2208StepperBase<T>::diss2g	( bool     B )	{ SET_REG(diss2g); 	}
template<typename T>
void TMC2208StepperBase<T>::diss2vs	( bool     B )	{ SET_REG(diss2vs); }

template<typename T>
uint8_t TMC2208StepperBase<T>::toff()		{ GET_REG(toff); 	}
template<typename T>
uint8_t TMC2208StepperBase<T>::hstrt()		{ GET_REG(hstrt); 	}
template<typename T>
uint8_t TMC2208StepperBase<T>::hend()		{ GET_REG(hend); 	}
template<typename T>
uint8_t TMC2208StepperBase<T>::tbl()		{ GET_REG(tbl); 	}
template<typename T>
bool 	TMC2208StepperBase<T>::vsense()		{ GET_REG(vsense); 	}
template<typename T>
uint8_t TMC2208StepperBase<T>::mres()		{ GET_REG(mres); 	}
template<typename T>
bool 	TMC2208StepperBase<T>::intpol()		{ GET_REG(intpol); 	}
template<typename T>
bool 	TMC2208StepperBase<T>::dedge()		{ GET_REG(dedge); 	}
template<typename T>
bool 	TMC2208StepperBase<T>::diss2g()		{ GET_REG(diss2g); 	}
template<typename T>
bool 	TMC2208StepperBase<T>::diss2vs()	{ GET_REG(diss2vs); }

#define GET_REG_2660(SETTING) return CHOPCONF_register.SETTING;

uint32_t TMC2660Stepper::CHOPCONF() { return CHOPCONF_register.sr; }
void TMC2660Stepper::CHOPCONF(uint32_t data) {
  SMARTEN_register.sr = data;
  WRITE_REG(SMARTEN);
}

void TMC2660Stepper::toff(uint8_t B) 	{
	SET_REG(toff);
	if (B>0) _savedToff = B;
}
void TMC2660Stepper::hstrt(uint8_t B)	{ SET_REG(hstrt);	}
void TMC2660Stepper::hend(uint8_t B)	{ SET_REG(hend);	}
void TMC2660Stepper::hdec(uint8_t B)	{ SET_REG(hdec);	}
void TMC2660Stepper::rndtf(bool B)		{ SET_REG(rndtf);	}
void TMC2660Stepper::chm(bool B)		{ SET_REG(chm);		}
void TMC2660Stepper::tbl(uint8_t B)		{ SET_REG(tbl);		}

uint8_t TMC2660Stepper::toff()	{ GET_REG_2660(toff);	}
uint8_t TMC2660Stepper::hstrt()	{ GET_REG_2660(hstrt);	}
uint8_t TMC2660Stepper::hend()	{ GET_REG_2660(hend);	}
uint8_t TMC2660Stepper::hdec() 	{ GET_REG_2660(hdec);	}
bool TMC2660Stepper::rndtf()	{ GET_REG_2660(rndtf);	}
bool TMC2660Stepper::chm()		{ GET_REG_2660(chm);	}
uint8_t TMC2660Stepper::tbl()	{ GET_REG_2660(tbl);	}
