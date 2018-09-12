#pragma once

#define SELF this->self()
#define DEBUG_PRINT(CFG, VAL) Serial.print(CFG); Serial.print('('); Serial.print(VAL, HEX); Serial.println(')')
#define WRITE_REG(R) SELF.write(SELF.R##_register.address, SELF.R##_register.sr)
#define READ_REG(R) SELF.read(SELF.R##_register.address)
