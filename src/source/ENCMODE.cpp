#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) ENCMODE_register.SETTING = B; WRITE_REG(ENCMODE);
#define GET_REG(SETTING) ENCMODE(); return ENCMODE_register.SETTING;

// ENCMODE
template<typename T>
uint32_t TMC5130StepperBase<T>::ENCMODE() {
	ENCMODE_register.sr = READ_REG(ENCMODE);
	return ENCMODE_register.sr;
}
template<typename T>
void TMC5130StepperBase<T>::ENCMODE(uint32_t input) {
	ENCMODE_register.sr = input;
	WRITE_REG(ENCMODE);
}

template<typename T>
void TMC5130StepperBase<T>::pol_a(bool B)			{ SET_REG(pol_a);			}
template<typename T>
void TMC5130StepperBase<T>::pol_b(bool B)			{ SET_REG(pol_b);			}
template<typename T>
void TMC5130StepperBase<T>::pol_n(bool B)			{ SET_REG(pol_n);			}
template<typename T>
void TMC5130StepperBase<T>::ignore_ab(bool B)		{ SET_REG(ignore_ab);		}
template<typename T>
void TMC5130StepperBase<T>::clr_cont(bool B)		{ SET_REG(clr_cont);		}
template<typename T>
void TMC5130StepperBase<T>::clr_once(bool B)		{ SET_REG(clr_once);		}
template<typename T>
void TMC5130StepperBase<T>::pos_edge(bool B)		{ SET_REG(pos_edge);		}
template<typename T>
void TMC5130StepperBase<T>::neg_edge(bool B)		{ SET_REG(neg_edge);		}
template<typename T>
void TMC5130StepperBase<T>::clr_enc_x(bool B)		{ SET_REG(clr_enc_x);		}
template<typename T>
void TMC5130StepperBase<T>::latch_x_act(bool B)	{ SET_REG(latch_x_act);		}
template<typename T>
void TMC5130StepperBase<T>::enc_sel_decimal(bool B){ SET_REG(enc_sel_decimal);	}

template<typename T>
bool TMC5130StepperBase<T>::pol_a()			{ GET_REG(pol_a);			}
template<typename T>
bool TMC5130StepperBase<T>::pol_b()			{ GET_REG(pol_b);			}
template<typename T>
bool TMC5130StepperBase<T>::pol_n()			{ GET_REG(pol_n);			}
template<typename T>
bool TMC5130StepperBase<T>::ignore_ab()		{ GET_REG(ignore_ab);		}
template<typename T>
bool TMC5130StepperBase<T>::clr_cont()			{ GET_REG(clr_cont);		}
template<typename T>
bool TMC5130StepperBase<T>::clr_once()			{ GET_REG(clr_once);		}
template<typename T>
bool TMC5130StepperBase<T>::pos_edge()			{ GET_REG(pos_edge);		}
template<typename T>
bool TMC5130StepperBase<T>::neg_edge()			{ GET_REG(neg_edge);		}
template<typename T>
bool TMC5130StepperBase<T>::clr_enc_x()		{ GET_REG(clr_enc_x);		}
template<typename T>
bool TMC5130StepperBase<T>::latch_x_act()		{ GET_REG(latch_x_act);		}
template<typename T>
bool TMC5130StepperBase<T>::enc_sel_decimal()	{ GET_REG(enc_sel_decimal);	}
