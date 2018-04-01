#include "TMCStepper.h"
#include "TMC_MACROS.h"

#define SET_REG(SETTING) SW_MODE_register.cfg.opt.SETTING = B; WRITE_REG(SW_MODE)
#define GET_REG(SETTING) SW_MODE(); return SW_MODE_register.cfg.opt.SETTING

// SW_MODE
uint32_t TMC5130Stepper::SW_MODE() {
	SW_MODE_register.cfg.sr = READ_REG(SW_MODE);
	return SW_MODE_register.cfg.sr;
}
void TMC5130Stepper::SW_MODE(uint32_t input) {
	SW_MODE_register.cfg.sr = input;
	WRITE_REG(SW_MODE);
}

void TMC5130Stepper::stop_l_enable(bool B)		{ SET_REG(stop_l_enable);	}
void TMC5130Stepper::stop_r_enable(bool B)		{ SET_REG(stop_r_enable);	}
void TMC5130Stepper::pol_stop_l(bool B)			{ SET_REG(pol_stop_l);		}
void TMC5130Stepper::pol_stop_r(bool B)			{ SET_REG(pol_stop_r);		}
void TMC5130Stepper::swap_lr(bool B)			{ SET_REG(swap_lr);			}
void TMC5130Stepper::latch_l_active(bool B)		{ SET_REG(latch_l_active);	}
void TMC5130Stepper::latch_l_inactive(bool B)	{ SET_REG(latch_l_inactive);}
void TMC5130Stepper::latch_r_active(bool B)		{ SET_REG(latch_r_active);	}
void TMC5130Stepper::latch_r_inactive(bool B)	{ SET_REG(latch_r_inactive);}
void TMC5130Stepper::en_latch_encoder(bool B)	{ SET_REG(en_latch_encoder);}
void TMC5130Stepper::sg_stop(bool B)			{ SET_REG(sg_stop);			}
void TMC5130Stepper::en_softstop(bool B)		{ SET_REG(en_softstop);		}

bool TMC5130Stepper::stop_r_enable()			{ GET_REG(stop_r_enable);	}
bool TMC5130Stepper::pol_stop_l()				{ GET_REG(pol_stop_l);		}
bool TMC5130Stepper::pol_stop_r()				{ GET_REG(pol_stop_r);		}
bool TMC5130Stepper::swap_lr()					{ GET_REG(swap_lr);			}
bool TMC5130Stepper::latch_l_active()			{ GET_REG(latch_l_active);	}
bool TMC5130Stepper::latch_l_inactive()			{ GET_REG(latch_l_inactive);}
bool TMC5130Stepper::latch_r_active()			{ GET_REG(latch_r_active);	}
bool TMC5130Stepper::latch_r_inactive()			{ GET_REG(latch_r_inactive);}
bool TMC5130Stepper::en_latch_encoder()			{ GET_REG(en_latch_encoder);}
bool TMC5130Stepper::sg_stop()					{ GET_REG(sg_stop);			}
bool TMC5130Stepper::en_softstop()				{ GET_REG(en_softstop);		}
