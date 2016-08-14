
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

const int pin_led = 13;
int v = 'R';

boolean is_standby = true;

ISR(WDT_vect) {
  Serial.print('I');
  //wdt_reset();
  //是否要reboot, 关键决策在这里...

  if (is_standby) {
    wdt_reset();
  } else {
    // eg. must be rebooted
    // configure
    MCUSR = 0;                          // reset flags
       
                                        // Put timer in reset-only mode:
    WDTCSR |= 0b00011000;               // Enter config mode.
    WDTCSR =  0b00001000 | 0b000000;    // clr WDIE (interrupt enable...7th from left)
                                        // set WDE (reset enable...4th from left), and set delay interval
                                        // reset system in 16 ms...
                                        // unless wdt_disable() in loop() is reached first                             
    // reboot
    while(1);
  }
}

void configure_wdt() {
  // disable interrupt
  cli();

  // reset status register flags
  MCUSR = 0;
  
  // enter into configure mode
  WDTCSR |= 0b00011000;

  // enable interrupt, reset disabled, set timeout to 8 seconds
  WDTCSR = 0b1000000 | 0b00100001;

  // re-enable interrupts
  sei();
}

void setup() {
  // put your setup code here, to run once:
  wdt_disable();
  
  Serial.begin(9600);
  Serial.print('S');
  wdt_enable(WDTO_4S);
  WDTCSR |= _BV(WDIE); // first time interrupt, second time reset the cpu
    // wdt application
    // 1. disabled
    // 2. wdt_reset periodically
    // 3. interrupt
    // 4. interrupt + reset
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    int t = Serial.read();
    if (t == 'I') {
      wdt_disable();
      configure_wdt();
      v = 'I';
    } else if (t == 'R') {
      wdt_disable();
      wdt_enable(WDTO_4S);
      v = 'R';
    }
  }

  if (v == 'I') {
    Serial.print('i');
    delay(1000);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode(); // how to wake up??
                  //  only when interrrupt event happens, the execution will leave from here
    sleep_disable();
    power_all_enable();
  } else if (v == 'R') {
    Serial.print('r');
    //wdt_reset();
    delay(1000);
  }
}
