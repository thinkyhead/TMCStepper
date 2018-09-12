#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define GET_REG(SETTING) SELF.DRV_STATUS(); return DRV_STATUS_register.SETTING;

template<typename T>
uint32_t TMC2130StepperBase<T>::DRV_STATUS() {
	DRV_STATUS_register.sr = READ_REG(DRV_STATUS);
	return DRV_STATUS_register.sr;
}

template<typename T>
uint16_t TMC2130StepperBase<T>::sg_result(){ GET_REG(sg_result); 	}
template<typename T>
bool TMC2130StepperBase<T>::fsactive()		{ GET_REG(fsactive); 	}
template<typename T>
uint8_t TMC2130StepperBase<T>::cs_actual()	{ GET_REG(cs_actual); 	}
template<typename T>
bool TMC2130StepperBase<T>::stallguard()	{ GET_REG(stallGuard); 	}
template<typename T>
bool TMC2130StepperBase<T>::ot()			{ GET_REG(ot); 			}
template<typename T>
bool TMC2130StepperBase<T>::otpw()			{ GET_REG(otpw); 		}
template<typename T>
bool TMC2130StepperBase<T>::s2ga()			{ GET_REG(s2ga); 		}
template<typename T>
bool TMC2130StepperBase<T>::s2gb()			{ GET_REG(s2gb); 		}
template<typename T>
bool TMC2130StepperBase<T>::ola()			{ GET_REG(ola); 		}
template<typename T>
bool TMC2130StepperBase<T>::olb()			{ GET_REG(olb); 		}
template<typename T>
bool TMC2130StepperBase<T>::stst()			{ GET_REG(stst); 		}

template<typename T>
uint32_t TMC2208StepperBase<T>::DRV_STATUS() {
	if (write_only) return 0;
	DRV_STATUS_register.sr = READ_REG(DRV_STATUS);
	return DRV_STATUS_register.sr;
}

template<typename T>
bool TMC2208StepperBase<T>::otpw()		{ GET_REG(otpw); 		}
template<typename T>
bool TMC2208StepperBase<T>::ot() 		{ GET_REG(ot); 	 		}
template<typename T>
bool TMC2208StepperBase<T>::s2ga() 		{ GET_REG(s2ga); 		}
template<typename T>
bool TMC2208StepperBase<T>::s2gb() 		{ GET_REG(s2gb); 		}
template<typename T>
bool TMC2208StepperBase<T>::s2vsa() 	{ GET_REG(s2vsa);		}
template<typename T>
bool TMC2208StepperBase<T>::s2vsb() 	{ GET_REG(s2vsb);		}
template<typename T>
bool TMC2208StepperBase<T>::ola() 		{ GET_REG(ola);  		}
template<typename T>
bool TMC2208StepperBase<T>::olb() 		{ GET_REG(olb);  		}
template<typename T>
bool TMC2208StepperBase<T>::t120() 		{ GET_REG(t120); 		}
template<typename T>
bool TMC2208StepperBase<T>::t143() 		{ GET_REG(t143); 		}
template<typename T>
bool TMC2208StepperBase<T>::t150() 		{ GET_REG(t150); 		}
template<typename T>
bool TMC2208StepperBase<T>::t157() 		{ GET_REG(t157); 		}
template<typename T>
uint16_t TMC2208StepperBase<T>::cs_actual()	{ GET_REG(cs_actual);	}
template<typename T>
bool TMC2208StepperBase<T>::stealth() 	{ GET_REG(stealth);		}
template<typename T>
bool TMC2208StepperBase<T>::stst() 		{ GET_REG(stst); 		}

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

