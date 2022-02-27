#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// set the LCD number of columns and rows
#define lcdColumns 16
#define lcdRows 2
// To get display address, run an I2C scanner sketch, default is 0x27
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

// Set pin numbers for LDR voltage dividers
#define ldr_pin_1 13
#define ldr_pin_2 12
// Since LDRs aren't precisely equal, different multipliers will be used to level their accuracy 
#define ldr_multiplier_1 1
#define ldr_multiplier_2 1

//constants required for measure_resistance()
#define R1 51000
#define Vin 3.34

Servo myservo;  // Create servo object to control a servo. 16 servo objects can be created on the ESP32
int servo_position = 0;    // Variable to store the servo 
#define servo_pin_1 18
 
void setup() {
  Serial.begin(9600);

  // This line is required to bypass a bug with the LCD where random characters are displaied
  // It lowers the clock from it's default value of 100 000 to lesser 10 000
  Wire.setClock(10000);
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
//  lcd.backlight();
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Power On :D");
  
  // Allow allocation of all timers. This is required to controll the servos
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // Standard 50 hz servo (20ms)
  myservo.attach(servo_pin_1, 1000, 2000); // Attaches the servo on pin 18 to the servo object
}

int measure_resistance(int input_pin){
    int raw = analogRead(input_pin); // Get input from ADC pin (ranging from 0-4096 equal to 0-3.3V)
    if(raw){
      float buffer = raw * Vin;
      float Vout = buffer/4095; // Calculate voltage drop on second resistor
      buffer = (Vin/Vout) -1;
      int R2 = R1 / buffer; // Calculate resistance knowing that R1 is 51K ohms
      return R2;
    }
    return -1; // If analogRead() didn't yield proper value return -1
}

void loop() {
  // Get measurement for LDR1
  int ldr1 = measure_resistance(ldr_pin_1) * ldr_multiplier_1; 
  Serial.print("LDR 1: ");
  Serial.println( ldr1 );

  // Get measurement for LDR2
  int ldr2 = measure_resistance(ldr_pin_2) * ldr_multiplier_2;
  Serial.print("LDR 2: ");
  Serial.println( ldr2 );

  // Compare LDR1 and LDR2 and move the servo towards the brighter side
  if( (ldr1 - ldr2) > (ldr1+ldr2)/2 * 0.2 ){
    if(servo_position < 180) servo_position ++;
    }
    if( (ldr2 - ldr1) > (ldr1+ldr2)/2 * 0.2 ){
      if(servo_position > 0) servo_position --;
    }
    
  myservo.write(servo_position);
  delay(50);
}
