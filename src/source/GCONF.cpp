#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) this->GCONF_register.SETTING = B; WRITE_REG(GCONF)
#define GET_REG(SETTING) this->GCONF(); return this->GCONF_register.SETTING

// GCONF
template<typename T>
uint32_t TMC2130StepperBase<T>::GCONF() {
	GCONF_register.sr = READ_REG(GCONF);
	return GCONF_register.sr;
}
template<typename T>
void TMC2130StepperBase<T>::GCONF(uint32_t input) {
	GCONF_register.sr = input;
	WRITE_REG(GCONF);
}

template<typename T>
void TMC2130StepperBase<T>::I_scale_analog(bool B)			{ SET_REG(i_scale_analog);			}
template<typename T>
void TMC2130StepperBase<T>::internal_Rsense(bool B)		{ SET_REG(internal_rsense);			}
template<typename T>
void TMC2130StepperBase<T>::en_pwm_mode(bool B)			{ SET_REG(en_pwm_mode);				}
template<typename T>
void TMC2130StepperBase<T>::enc_commutation(bool B)		{ SET_REG(enc_commutation);			}
template<typename T>
void TMC2130StepperBase<T>::shaft(bool B) 					{ SET_REG(shaft);					}
template<typename T>
void TMC2130StepperBase<T>::diag0_error(bool B) 			{ SET_REG(diag0_error);				}
template<typename T>
void TMC2130StepperBase<T>::diag0_otpw(bool B) 			{ SET_REG(diag0_otpw);				}
template<typename T>
void TMC2130StepperBase<T>::diag0_stall(bool B) 			{ SET_REG(diag0_stall);				}
template<typename T>
void TMC2130StepperBase<T>::diag1_stall(bool B) 			{ SET_REG(diag1_stall);				}
template<typename T>
void TMC2130StepperBase<T>::diag1_index(bool B) 			{ SET_REG(diag1_index);				}
template<typename T>
void TMC2130StepperBase<T>::diag1_onstate(bool B) 			{ SET_REG(diag1_onstate);			}
template<typename T>
void TMC2130StepperBase<T>::diag1_steps_skipped(bool B) 	{ SET_REG(diag1_steps_skipped);		}
template<typename T>
void TMC2130StepperBase<T>::diag0_int_pushpull(bool B) 	{ SET_REG(diag0_int_pushpull);		}
template<typename T>
void TMC2130StepperBase<T>::diag1_pushpull(bool B) 		{ SET_REG(diag1_poscomp_pushpull);	}
template<typename T>
void TMC2130StepperBase<T>::small_hysteresis(bool B) 		{ SET_REG(small_hysteresis);		}
template<typename T>
void TMC2130StepperBase<T>::stop_enable(bool B) 			{ SET_REG(stop_enable);				}
template<typename T>
void TMC2130StepperBase<T>::direct_mode(bool B) 			{ SET_REG(direct_mode);				}

template<typename T>
bool TMC2130StepperBase<T>::I_scale_analog()				{ GET_REG(i_scale_analog);			}
template<typename T>
bool TMC2130StepperBase<T>::internal_Rsense()				{ GET_REG(internal_rsense);			}
template<typename T>
bool TMC2130StepperBase<T>::en_pwm_mode()					{ GET_REG(en_pwm_mode);				}
template<typename T>
bool TMC2130StepperBase<T>::enc_commutation()				{ GET_REG(enc_commutation);			}
template<typename T>
bool TMC2130StepperBase<T>::shaft() 						{ GET_REG(shaft);					}
template<typename T>
bool TMC2130StepperBase<T>::diag0_error() 					{ GET_REG(diag0_error);				}
template<typename T>
bool TMC2130StepperBase<T>::diag0_otpw() 					{ GET_REG(diag0_otpw);				}
template<typename T>
bool TMC2130StepperBase<T>::diag0_stall() 					{ GET_REG(diag0_stall);				}
template<typename T>
bool TMC2130StepperBase<T>::diag1_stall() 					{ GET_REG(diag1_stall);				}
template<typename T>
bool TMC2130StepperBase<T>::diag1_index() 					{ GET_REG(diag1_index);				}
template<typename T>
bool TMC2130StepperBase<T>::diag1_onstate() 				{ GET_REG(diag1_onstate);			}
template<typename T>
bool TMC2130StepperBase<T>::diag1_steps_skipped() 			{ GET_REG(diag1_steps_skipped);		}
template<typename T>
bool TMC2130StepperBase<T>::diag0_int_pushpull() 			{ GET_REG(diag0_int_pushpull);		}
template<typename T>
bool TMC2130StepperBase<T>::diag1_pushpull()		 		{ GET_REG(diag1_poscomp_pushpull);	}
template<typename T>
bool TMC2130StepperBase<T>::small_hysteresis() 			{ GET_REG(small_hysteresis);		}
template<typename T>
bool TMC2130StepperBase<T>::stop_enable() 					{ GET_REG(stop_enable);				}
template<typename T>
bool TMC2130StepperBase<T>::direct_mode() 					{ GET_REG(direct_mode);				}

/*
bit 18 not implemented:
test_mode 0: 
Normal operation 1:
Enable analog test output on pin DCO. IHOLD[1..0] selects the function of DCO: 
0…2: T120, DAC, VDDH Attention:
Not for user, set to 0 for normal operation! 
*/

template<typename T>
void TMC5160StepperBase<T>::recalibrate(bool B)			{ SET_REG(recalibrate); 			}
template<typename T>
void TMC5160StepperBase<T>::faststandstill(bool B)			{ SET_REG(faststandstill); 			}
template<typename T>
void TMC5160StepperBase<T>::multistep_filt(bool B)			{ SET_REG(multistep_filt); 			}
template<typename T>
bool TMC5160StepperBase<T>::recalibrate()					{ GET_REG(recalibrate);				}
template<typename T>
bool TMC5160StepperBase<T>::faststandstill()				{ GET_REG(faststandstill);			}
template<typename T>
bool TMC5160StepperBase<T>::multistep_filt()				{ GET_REG(multistep_filt);			}

template<typename T>
uint32_t TMC2208StepperBase<T>::GCONF() {
	if (write_only) return GCONF_register.sr;
	GCONF_register.sr = READ_REG(GCONF);
	return GCONF_register.sr;
}
template<typename T>
void TMC2208StepperBase<T>::GCONF(uint32_t input) {
	GCONF_register.sr = input;
	WRITE_REG(GCONF);
}

template<typename T>
void TMC2208StepperBase<T>::I_scale_analog(bool B)		{ SET_REG(i_scale_analog);	}
template<typename T>
void TMC2208StepperBase<T>::internal_Rsense(bool B)	{ SET_REG(internal_rsense);	}
template<typename T>
void TMC2208StepperBase<T>::en_spreadCycle(bool B)		{ SET_REG(en_spreadcycle);	}
template<typename T>
void TMC2208StepperBase<T>::shaft(bool B) 				{ SET_REG(shaft);			}
template<typename T>
void TMC2208StepperBase<T>::index_otpw(bool B)			{ SET_REG(index_otpw);		}
template<typename T>
void TMC2208StepperBase<T>::index_step(bool B)			{ SET_REG(index_step);		}
template<typename T>
void TMC2208StepperBase<T>::pdn_disable(bool B)		{ SET_REG(pdn_disable);		}
template<typename T>
void TMC2208StepperBase<T>::mstep_reg_select(bool B)	{ SET_REG(mstep_reg_select);}
template<typename T>
void TMC2208StepperBase<T>::multistep_filt(bool B)		{ SET_REG(multistep_filt);	}

template<typename T>
bool TMC2208StepperBase<T>::I_scale_analog()	{ GET_REG(i_scale_analog);	}
template<typename T>
bool TMC2208StepperBase<T>::internal_Rsense()	{ GET_REG(internal_rsense);	}
template<typename T>
bool TMC2208StepperBase<T>::en_spreadCycle()	{ GET_REG(en_spreadcycle);	}
template<typename T>
bool TMC2208StepperBase<T>::shaft()			{ GET_REG(shaft);			}
template<typename T>
bool TMC2208StepperBase<T>::index_otpw()		{ GET_REG(index_otpw);		}
template<typename T>
bool TMC2208StepperBase<T>::index_step()		{ GET_REG(index_step);		}
template<typename T>
bool TMC2208StepperBase<T>::pdn_disable()		{ GET_REG(pdn_disable);		}
template<typename T>
bool TMC2208StepperBase<T>::mstep_reg_select()	{ GET_REG(mstep_reg_select);}
template<typename T>
bool TMC2208StepperBase<T>::multistep_filt()	{ GET_REG(multistep_filt);	}

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

