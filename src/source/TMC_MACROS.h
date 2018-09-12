#pragma once

#define DEBUG_PRINT(CFG, VAL) Serial.print(CFG); Serial.print('('); Serial.print(VAL, HEX); Serial.println(')')
#define WRITE_REG(R) this->write(this->R##_register.address, this->R##_register.sr)
#define READ_REG(R) this->read(this->R##_register.address)
