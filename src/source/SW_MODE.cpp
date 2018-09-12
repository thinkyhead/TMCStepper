#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) SW_MODE_register.SETTING = B; WRITE_REG(SW_MODE)
#define GET_REG(SETTING) SW_MODE(); return SW_MODE_register.SETTING

// SW_MODE
template<typename T>
uint32_t TMC5130StepperBase<T>::SW_MODE() {
	SW_MODE_register.sr = READ_REG(SW_MODE);
	return SW_MODE_register.sr;
}
template<typename T>
void TMC5130StepperBase<T>::SW_MODE(uint32_t input) {
	SW_MODE_register.sr = input;
	WRITE_REG(SW_MODE);
}

template<typename T>
void TMC5130StepperBase<T>::stop_l_enable(bool B)		{ SET_REG(stop_l_enable);	}
template<typename T>
void TMC5130StepperBase<T>::stop_r_enable(bool B)		{ SET_REG(stop_r_enable);	}
template<typename T>
void TMC5130StepperBase<T>::pol_stop_l(bool B)			{ SET_REG(pol_stop_l);		}
template<typename T>
void TMC5130StepperBase<T>::pol_stop_r(bool B)			{ SET_REG(pol_stop_r);		}
template<typename T>
void TMC5130StepperBase<T>::swap_lr(bool B)			{ SET_REG(swap_lr);			}
template<typename T>
void TMC5130StepperBase<T>::latch_l_active(bool B)		{ SET_REG(latch_l_active);	}
template<typename T>
void TMC5130StepperBase<T>::latch_l_inactive(bool B)	{ SET_REG(latch_l_inactive);}
template<typename T>
void TMC5130StepperBase<T>::latch_r_active(bool B)		{ SET_REG(latch_r_active);	}
template<typename T>
void TMC5130StepperBase<T>::latch_r_inactive(bool B)	{ SET_REG(latch_r_inactive);}
template<typename T>
void TMC5130StepperBase<T>::en_latch_encoder(bool B)	{ SET_REG(en_latch_encoder);}
template<typename T>
void TMC5130StepperBase<T>::sg_stop(bool B)			{ SET_REG(sg_stop);			}
template<typename T>
void TMC5130StepperBase<T>::en_softstop(bool B)		{ SET_REG(en_softstop);		}

template<typename T>
bool TMC5130StepperBase<T>::stop_r_enable()			{ GET_REG(stop_r_enable);	}
template<typename T>
bool TMC5130StepperBase<T>::pol_stop_l()				{ GET_REG(pol_stop_l);		}
template<typename T>
bool TMC5130StepperBase<T>::pol_stop_r()				{ GET_REG(pol_stop_r);		}
template<typename T>
bool TMC5130StepperBase<T>::swap_lr()					{ GET_REG(swap_lr);			}
template<typename T>
bool TMC5130StepperBase<T>::latch_l_active()			{ GET_REG(latch_l_active);	}
template<typename T>
bool TMC5130StepperBase<T>::latch_l_inactive()			{ GET_REG(latch_l_inactive);}
template<typename T>
bool TMC5130StepperBase<T>::latch_r_active()			{ GET_REG(latch_r_active);	}
template<typename T>
bool TMC5130StepperBase<T>::latch_r_inactive()			{ GET_REG(latch_r_inactive);}
template<typename T>
bool TMC5130StepperBase<T>::en_latch_encoder()			{ GET_REG(en_latch_encoder);}
template<typename T>
bool TMC5130StepperBase<T>::sg_stop()					{ GET_REG(sg_stop);			}
template<typename T>
bool TMC5130StepperBase<T>::en_softstop()				{ GET_REG(en_softstop);		}

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

