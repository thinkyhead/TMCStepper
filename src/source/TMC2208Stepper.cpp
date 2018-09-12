#include "TMCStepper.h"
#include "TMC_MACROS.h"

template<typename T>
TMC2208StepperBase<T>::TMC2208StepperBase(Stream * SerialPort, float RS, bool has_rx) :
	TMCStepper<T>(RS),
	write_only(!has_rx),
	uses_sw_serial(false)
	{ HWSerial = SerialPort; }

#if SW_CAPABLE_PLATFORM
	template<typename T>
	TMC2208StepperBase<T>::TMC2208StepperBase(uint16_t SW_RX_pin, uint16_t SW_TX_pin, float RS, bool has_rx) :
		TMCStepper<T>(RS),
		write_only(!has_rx),
		uses_sw_serial(true)
		{
			SoftwareSerial *SWSerialObj = new SoftwareSerial(SW_RX_pin, SW_TX_pin);
			SWSerial = SWSerialObj;
		}

	template<typename T>
	void TMC2208StepperBase<T>::beginSerial(uint32_t baudrate) {
		if (uses_sw_serial) SWSerial->begin(baudrate);
	}
#endif

template<typename T>
void TMC2208StepperBase<T>::push() {
	this->GCONF(this->GCONF_register.sr);
	this->IHOLD_IRUN(this->IHOLD_IRUN_register.sr);
	this->SLAVECONF(this->SLAVECONF_register.sr);
	this->FACTORY_CONF(this->FACTORY_CONF_register.sr);
	this->TPOWERDOWN(this->TPOWERDOWN_register.sr);
	this->TPWMTHRS(this->TPWMTHRS_register.sr);
	this->VACTUAL(this->VACTUAL_register.sr);
	this->CHOPCONF(this->CHOPCONF_register.sr);
	this->PWMCONF(this->PWMCONF_register.sr);
}

template<typename T>
bool TMC2208StepperBase<T>::isEnabled() { return !enn() && toff(); }

template<typename T>
uint8_t TMC2208StepperBase<T>::calcCRC(uint8_t datagram[], uint8_t len) {
	uint8_t crc = 0;
	for (uint8_t i = 0; i < len; i++) {
		uint8_t currentByte = datagram[i];
		for (uint8_t j = 0; j < 8; j++) {
			if ((crc >> 7) ^ (currentByte & 0x01)) {
				crc = (crc << 1) ^ 0x07;
			} else {
				crc = (crc << 1);
			}
			crc &= 0xff;
			currentByte = currentByte >> 1;
		} 
	}
	return crc;
}

template<typename T>
void TMC2208StepperBase<T>::write(uint8_t addr, uint32_t regVal) {
	uint8_t len = 7;
	addr |= this->TMC_WRITE;
	uint8_t datagram[] = {TMC2208_SYNC, TMC2208_SLAVE_ADDR, addr, (uint8_t)(regVal>>24), (uint8_t)(regVal>>16), (uint8_t)(regVal>>8), (uint8_t)(regVal>>0), 0x00};

	datagram[len] = calcCRC(datagram, len);

	if (uses_sw_serial) {
		#if SW_CAPABLE_PLATFORM
			for(int i=0; i<=len; i++){
				bytesWritten += SWSerial->write(datagram[i]);
			}
		#endif
	} else {
		for(int i=0; i<=len; i++){
			bytesWritten += HWSerial->write(datagram[i]);
		}
	}
	delay(replyDelay);
}

template<typename SERIAL_TYPE>
uint64_t _sendDatagram(SERIAL_TYPE &serPtr, uint8_t datagram[], uint8_t len, uint16_t replyDelay) {
	uint64_t out = 0x00000000UL;

	while (serPtr.available() > 0) serPtr.read(); // Flush

	for(int i=0; i<=len; i++) serPtr.write(datagram[i]);
	for(int byte=0; byte<4; byte++) serPtr.read(); // Flush bytes written
	delay(replyDelay);

	while(serPtr.available() > 0) {
		uint8_t res = serPtr.read();
		out <<= 8;
		out |= res&0xFF;
	}
	return out;
}

template<typename T>
uint32_t TMC2208StepperBase<T>::read(uint8_t addr) {
	uint8_t len = 3;
	addr |= this->TMC_READ;
	uint8_t datagram[] = {TMC2208_SYNC, TMC2208_SLAVE_ADDR, addr, 0x00};
	datagram[len] = calcCRC(datagram, len);
	uint64_t out = 0x00000000UL;

	if (uses_sw_serial) {
		#if SW_CAPABLE_PLATFORM
			SWSerial->listen();
			out = _sendDatagram(*SWSerial, datagram, len, replyDelay);
		#endif
	} else {
		out = _sendDatagram(*HWSerial, datagram, len, replyDelay);
	}

	uint8_t out_datagram[] = {(uint8_t)(out>>56), (uint8_t)(out>>48), (uint8_t)(out>>40), (uint8_t)(out>>32), (uint8_t)(out>>24), (uint8_t)(out>>16), (uint8_t)(out>>8), (uint8_t)(out>>0)};
	if (calcCRC(out_datagram, 7) == (uint8_t)(out&0xFF)) {
		CRCerror = false;
	} else {
		CRCerror = true;
	}
	return out>>8;
}

template<typename T>
uint8_t TMC2208StepperBase<T>::IFCNT() {
	return READ_REG(IFCNT);
}

template<typename T>
void TMC2208StepperBase<T>::SLAVECONF(uint16_t input) {
	SLAVECONF_register.sr = input&0xF00;
	WRITE_REG(SLAVECONF);
}
template<typename T>
uint16_t TMC2208StepperBase<T>::SLAVECONF() {
	return SLAVECONF_register.sr;
}
template<typename T>
void TMC2208StepperBase<T>::senddelay(uint8_t B) 	{ SLAVECONF_register.senddelay = B; WRITE_REG(SLAVECONF); }
template<typename T>
uint8_t TMC2208StepperBase<T>::senddelay() 		{ return SLAVECONF_register.senddelay; }

template<typename T>
void TMC2208StepperBase<T>::OTP_PROG(uint16_t input) {
	SELF.write(OTP_PROG_register.address, input);
}

template<typename T>
uint32_t TMC2208StepperBase<T>::OTP_READ() {
	return SELF.read(OTP_READ_register.address);
}

template<typename T>
uint32_t TMC2208StepperBase<T>::IOIN() {
	IOIN_register.sr = READ_REG(IOIN);
	return IOIN_register.sr;
}
template<typename T>
bool TMC2208StepperBase<T>::enn()			{ IOIN(); return IOIN_register.enn;		}
template<typename T>
bool TMC2208StepperBase<T>::ms1()			{ IOIN(); return IOIN_register.ms1;		}
template<typename T>
bool TMC2208StepperBase<T>::ms2()			{ IOIN(); return IOIN_register.ms2;		}
template<typename T>
bool TMC2208StepperBase<T>::diag()			{ IOIN(); return IOIN_register.diag;	}
template<typename T>
bool TMC2208StepperBase<T>::pdn_uart()		{ IOIN(); return IOIN_register.pdn_uart;}
template<typename T>
bool TMC2208StepperBase<T>::step()			{ IOIN(); return IOIN_register.step;	}
template<typename T>
bool TMC2208StepperBase<T>::sel_a()			{ IOIN(); return IOIN_register.sel_a;	}
template<typename T>
bool TMC2208StepperBase<T>::dir()			{ IOIN(); return IOIN_register.dir;		}
template<typename T>
uint8_t TMC2208StepperBase<T>::version() 	{ IOIN(); return IOIN_register.version;	}

uint32_t TMC2224Stepper::IOIN() {
	IOIN_register.sr = READ_REG(IOIN);
	return IOIN_register.sr;
}
bool TMC2224Stepper::enn()			{ IOIN(); return IOIN_register.enn;		}
bool TMC2224Stepper::ms1()			{ IOIN(); return IOIN_register.ms1;		}
bool TMC2224Stepper::ms2()			{ IOIN(); return IOIN_register.ms2;		}
bool TMC2224Stepper::pdn_uart()		{ IOIN(); return IOIN_register.pdn_uart;}
bool TMC2224Stepper::spread()		{ IOIN(); return IOIN_register.spread;	}
bool TMC2224Stepper::step()			{ IOIN(); return IOIN_register.step;	}
bool TMC2224Stepper::sel_a()		{ IOIN(); return IOIN_register.sel_a;	}
bool TMC2224Stepper::dir()			{ IOIN(); return IOIN_register.dir;		}
uint8_t TMC2224Stepper::version() 	{ IOIN(); return IOIN_register.version;	}

template<typename T>
uint16_t TMC2208StepperBase<T>::FACTORY_CONF() {
	if (write_only) return FACTORY_CONF_register.sr;
	FACTORY_CONF_register.sr = READ_REG(FACTORY_CONF);
	return FACTORY_CONF_register.sr;
}
template<typename T>
void TMC2208StepperBase<T>::FACTORY_CONF(uint16_t input) {
	FACTORY_CONF_register.sr = input;
	WRITE_REG(FACTORY_CONF);
}
template<typename T>
void TMC2208StepperBase<T>::fclktrim(uint8_t B)	{ FACTORY_CONF_register.fclktrim = B; WRITE_REG(FACTORY_CONF); }
template<typename T>
void TMC2208StepperBase<T>::ottrim(uint8_t B)	{ FACTORY_CONF_register.ottrim = B; WRITE_REG(FACTORY_CONF); }
template<typename T>
uint8_t TMC2208StepperBase<T>::fclktrim()		{ READ_REG(FACTORY_CONF); return FACTORY_CONF_register.fclktrim; }
template<typename T>
uint8_t TMC2208StepperBase<T>::ottrim()			{ READ_REG(FACTORY_CONF); return FACTORY_CONF_register.ottrim; }

template<typename T>
void TMC2208StepperBase<T>::VACTUAL(uint32_t input) {
	VACTUAL_register.sr = input;
	this->write(VACTUAL_register.address, VACTUAL_register.sr);
}
template<typename T>
uint32_t TMC2208StepperBase<T>::VACTUAL() {
	return VACTUAL_register.sr;
}

template<typename T>
uint32_t TMC2208StepperBase<T>::PWM_SCALE() {
	PWM_SCALE_register.sr = READ_REG(PWM_SCALE);
	return PWM_SCALE_register.sr;
}
template<typename T>
uint8_t TMC2208StepperBase<T>::pwm_scale_sum() { PWM_SCALE(); return PWM_SCALE_register.pwm_scale_sum; }
template<typename T>
int16_t TMC2208StepperBase<T>::pwm_scale_auto() {
	PWM_SCALE();
	return PWM_SCALE_register.pwm_scale_auto;
	// Not two's complement? 9nth bit determines sign
	/*
	uint32_t d = PWM_SCALE();
	int16_t response = (d>>PWM_SCALE_AUTO_bp)&0xFF;
	if (((d&PWM_SCALE_AUTO_bm) >> 24) & 0x1) return -response;
	else return response;
	*/
}

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

