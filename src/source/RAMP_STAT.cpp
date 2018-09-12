#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define GET_REG(SETTING) RAMP_STAT(); return RAMP_STAT_register.SETTING;

template<typename T>
uint32_t TMC5130StepperBase<T>::RAMP_STAT() {
	RAMP_STAT_register.sr = READ_REG(RAMP_STAT);
	return RAMP_STAT_register.sr;
}

template<typename T>
bool TMC5130StepperBase<T>::status_stop_l()		{ GET_REG(status_stop_l);		}
template<typename T>
bool TMC5130StepperBase<T>::status_stop_r()		{ GET_REG(status_stop_r);		}
template<typename T>
bool TMC5130StepperBase<T>::status_latch_l()		{ GET_REG(status_latch_l);		}
template<typename T>
bool TMC5130StepperBase<T>::status_latch_r()		{ GET_REG(status_latch_r);		}
template<typename T>
bool TMC5130StepperBase<T>::event_stop_l()			{ GET_REG(event_stop_l);		}
template<typename T>
bool TMC5130StepperBase<T>::event_stop_r()			{ GET_REG(event_stop_r);		}
template<typename T>
bool TMC5130StepperBase<T>::event_stop_sg()		{ GET_REG(event_stop_sg);		}
template<typename T>
bool TMC5130StepperBase<T>::event_pos_reached()	{ GET_REG(event_pos_reached);	}
template<typename T>
bool TMC5130StepperBase<T>::velocity_reached()		{ GET_REG(velocity_reached);	}
template<typename T>
bool TMC5130StepperBase<T>::position_reached()		{ GET_REG(position_reached);	}
template<typename T>
bool TMC5130StepperBase<T>::vzero()				{ GET_REG(vzero);	 			}
template<typename T>
bool TMC5130StepperBase<T>::t_zerowait_active()	{ GET_REG(t_zerowait_active);	}
template<typename T>
bool TMC5130StepperBase<T>::second_move()			{ GET_REG(second_move);			}
template<typename T>
bool TMC5130StepperBase<T>::status_sg()			{ GET_REG(status_sg);	 		}

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

