/* Project
                            --------
D5(A0)/RESET/ADC0/PB5 -- 1  /      / 8 -------   Vcc   ---------
D3(A3)/XTAL1/ADC3/PB3 -- 2  /      / 7 -- PB2/SCL/SCK/------- D2(A1)
D4(A2)/XTAL2/ADC2/PB4 -- 3  /      / 6 -- PB1/MISO/---------- D1
-----   GND   ---------- 4  /      / 5 -- PB0/MOSI/SDA/AREF/- D0 
                             --------
Pin Pin Name    I/O Type  Description
1   PB5 (OC1A)  Output    PWM output channel A
2   PB3 (OC2A)  Output    PWM output channel B
3   PB4         Input     General-purpose digital I/O pin
4   GND –       Ground
5   PB0 (MOSI)  In/Out    SPI master-out, slave-in or general-purpose digital I/O pin
6   PB1 (MISO)  In/Out    SPI master-in, slave-out or general-purpose digital I/O pin
7   PB2 (SCK)   In/Out    SPI clock or general-purpose digital I/O pin
8   VCC –       Positive supply voltage
upload example 11.ArduinoISP
then select tools->board-> attiny85
programmer Arduino as ISP      
*/

#include <avr/sleep.h> //Needed for sleep_mode
#include <avr/wdt.h> //Needed to enable/disable watch dog timer

// Utility macro
#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)

const int Led1    = 0;        // PB0 (pin 5 of ATtiny85 )
const int Led2    = 1;        // PB1 (pin 6 of ATtiny85 )
const int Led3    = 2;        // PB2 (pin 7 of ATtiny85 ) 
const int Led4    = 4;        // unused PB4 (pin 3 of ATtiny85 )
const int Switch  = 3;        
volatile int s0   = 1;

const byte unused = 4;       //
int watchdog_counter = 0;

int LedArray[] = {Led1, Led2, Led3, Led4};

// Pin change interrupt service routine
ISR (PCINT0_vect) {
 s0++;
 ChangeSwitch();
}

// Called when switch is pressed; On is 0 (down) or 1 (up)
void ChangeSwitch () {
  
switch (s0) {
  case 1 ... 2:
    digitalWrite(LedArray[3], HIGH);
    delay(500);
    digitalWrite(LedArray[3], LOW);
    delay(250);
    
  break;
  case 3 ... 4:
    digitalWrite(LedArray[2], HIGH);
    delay(500);
    digitalWrite(LedArray[2], LOW);
    delay(250);
    
  break;
  case 5 ... 6:
    digitalWrite(LedArray[1], HIGH);
    delay(500);
    digitalWrite(LedArray[1], LOW);
    delay(250);
    
  break;
  case 7 ... 8:
    digitalWrite(LedArray[0], HIGH);
    delay(500);
    digitalWrite(LedArray[0], LOW);
    delay(250);
    
    break; 
  default:
  
  if (s0 == 9) {
    s0 = 0;
  }
  break;
  }
}

void WDDelay(int n) {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  WDTCR = 1<<WDIE | (n & 0x8)<<2 | 1<<WDE | (n & 0x7);
  sleep_enable();
  sleep_cpu();
}

//This runs each time the watch dog wakes us up from sleep
ISR(WDT_vect) {
   watchdog_counter++;
  //Don't do anything. This is just here so that we wake up.
}


void setup() {
  
  pinMode(Switch, INPUT_PULLUP);
                           // Configure pin change interrupts
  PCMSK =  1<<Switch;     // pin change mask register
  GIMSK = 1<<PCIE;       // Enable interrupt
  GIFR = 1<<PCIF;       // Clear interrupt flag
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); //Power down everything, wake up from WDT
  sleep_enable();

  } // end of setup

void loop() 
{
  adc_disable(); // ADC uses ~320uA
  //WDDelay(9);   // 8sec
  //WDDelay(9);  // 8sec
  //WDDelay(9); // 8sec


switch (s0) {
  case 1 ... 2: // ***** random 4 LEDS and 36 seconds delay *****
    for ( int y=0; y<4; y++ ) {            // 4 times
      int x = random(0,4);
          for ( int i=0; i<3; i++){  // 2 flashs
            digitalWrite(LedArray[x], HIGH);
            WDDelay(1);  //0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
            digitalWrite(LedArray[x], LOW);
            WDDelay(3);  // 128ms
            }
    }
    WDDelay(9);
    WDDelay(9);
    WDDelay(9);
    WDDelay(9);
  break;
  
  case 3 ... 4: // ***** 4 leds and 4 seconds sleep *****
    for ( int y=0; y<5; y++ ) {         // 4 times
      for ( int i=0; i<4; i++) {       // 2 flashs
        digitalWrite(LedArray[i], HIGH);
        WDDelay(0);  //0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
        digitalWrite(LedArray[i], LOW);
        WDDelay(2);  // 128ms
       }
    }
    WDDelay(8); // 4 seconds
  break;
 
  case 5 ... 6: // ***** 2 leds led1 and led3 - 4 seconds sleep *****
    for ( int i=0; i<6; i++) {       // 5 flashs
      digitalWrite(LedArray[0], HIGH);
      WDDelay(1);  //0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
      digitalWrite(LedArray[0], LOW);
      WDDelay(2);  // 128ms
      digitalWrite(LedArray[2], HIGH);
      WDDelay(1);  //0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
      digitalWrite(LedArray[2], LOW);
      WDDelay(2);  // 128ms
     }
     WDDelay(7); // 2 seconds
  break;

  case 7 ... 8: // ***** 1 leds (green) - 24 seconds sleep *****
      digitalWrite(LedArray[2], HIGH);
      WDDelay(0);  //0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
      digitalWrite(LedArray[2], LOW);
      WDDelay(9);  //6=1sec, 7=2sec, 8=4sec, 9=8sec
      WDDelay(9);
      WDDelay(9);
      WDDelay(9);
      WDDelay(9);
      WDDelay(9); // 48sec
      WDDelay(9); // 56 sec
      WDDelay(9); // 64 sec   
  break;

  default:
  break;
}

  
} // end of loop


//Sets the watchdog timer to wake us up, but not reset
//0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
//6=1sec, 7=2sec, 8=4sec, 9=8sec
//From: http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/

void setup_watchdog(int timerPrescaler) {
  if (timerPrescaler > 9 ) timerPrescaler = 9; //Limit incoming amount to legal settings
  byte bb = timerPrescaler & 7; 
  if (timerPrescaler > 7) bb |= (1<<5); //Set the special 5th bit if necessary

  //This order of commands is important and cannot be combined
  MCUSR &= ~(1<<WDRF);           //Clear the watch dog reset
  WDTCR |= (1<<WDCE) | (1<<WDE); //Set WD_change enable, set WD enable
  WDTCR = bb;                    //Set new watchdog timeout value
  WDTCR |= _BV(WDIE);            //Set the interrupt enable, this will keep unit from resetting after each int
}
