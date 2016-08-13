
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#define PINLED 13

void setup() {
  // put your setup code here, to run once:
  wdt_disable();
  Serial.begin(9600);
  Serial.print('S');
  wdt_enable(WDTO_4S);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.read();
  Serial.print('r');
  delay(1000);
}
