#include "Arduino.h"

#ifndef UTIL
#define UTIL

#define pmo(pin) pinMode(pin, OUTPUT)
#define pmi(pin) pinMode(pin, INPUT)
#define pmipup(pin) pinMode(pin, INPUT_PULLUP)

#define dwh(pin) digitalWrite(pin, HIGH)
#define dwl(pin) digitalWrite(pin, LOW)

#define dr digitalRead

#define ar analogRead
#define aw analogWrite

#endif
