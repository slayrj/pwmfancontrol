//Tested on Arduino Nano. Ensure the PWMPin you select is appropriate for your Arduino model.
#include <EEPROM.h>

word PWMPin = 3;

int pwmdty = 25;
int addr1 = 0;
int addr2 = 1;

const int buttonPinUp = 4;
const int buttonPinDown = 5;

int buttonState = 0;


void setup() {
  Serial.begin(9600);

  // Lendo EEPROM
  int parte1 = EEPROM.read(0);
  int parte2 = EEPROM.read(1);
  int buttonState = (parte1 * 256) + parte2;


  Serial.println(buttonState);
  
  pinMode(PWMPin, OUTPUT);
  pinMode(buttonPinUp, INPUT_PULLUP);
  pinMode(buttonPinDown, INPUT_PULLUP);
  
  pwm25kHzBegin();
  
}

void loop() {

  if (digitalRead(buttonPinUp) == LOW && buttonState < 5 ) {
    buttonState++;
    EEPROM.write(addr1, buttonState/256);
    EEPROM.write(addr2, buttonState%256);
    Serial.println(buttonState);
    delay(1000);
  }

  if (digitalRead(buttonPinDown) == LOW && buttonState > 0 ) {
    buttonState--;
    delay(1000);
    EEPROM.write(addr1, buttonState/256);
    EEPROM.write(addr2, buttonState%256);
    Serial.println(buttonState);
  }


  if (buttonState == 0) {
    pwmdty = 20; 
  }
  if (buttonState == 1) {
    pwmdty = 25;
  }
  if (buttonState == 2) {
    pwmdty = 30;
  }
  if (buttonState == 3) {
    pwmdty = 35;
  }
  if (buttonState == 4) {
    pwmdty = 45;
  }
  if (buttonState == 5) {
    pwmdty = 55;
  }

  
  pwmDuty(pwmdty); // Change PWM duty cycle to desired value here (For ML350e Gen8 fans, higher PWM = slower fan)
               // 79/79 = slowest, 1/79 = fastest. 65/79, for example, is approx. 18% fan speed
               // The ceiling value (default 79) can be changed with OCR2A below

}

void pwm25kHzBegin() {
  TCCR2A = 0;                               // TC2 Control Register A
  TCCR2B = 0;                               // TC2 Control Register B
  TIMSK2 = 0;                               // TC2 Interrupt Mask Register
  TIFR2 = 0;                                // TC2 Interrupt Flag Register
  TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);  // OC2B cleared/set on match when up/down counting, fast PWM
  TCCR2B |= (1 << WGM22) | (1 << CS21);     // prescaler 8
  OCR2A = 79;                               // TOP overflow value (Hz)
  OCR2B = 0;
}

void pwmDuty(byte ocrb) {
  OCR2B = ocrb;                             // PWM Width (duty)
}
