#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

// constants required for WiFi connection
#define ssid "Tech_D3786836"
#define password "PUUUUSPF"
#define server "http://192.168.0.13:8000/esp_handler"

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

// set the LCD number of columns and rows
#define lcdColumns 16
#define lcdRows 2
// To get display address, run an I2C scanner sketch, default is 0x27
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// Set voltage divider pin
#define voltage_sensor_pin 26

// Set temperature sensor pin
#define temperature_sensor_pin 25
// value of R1_temperature_sensor on temperature sensor board
#define R1_temperature_sensor 10000 
//steinhart-hart coeficients for thermistor
#define c1 0.001129148
#define c2 0.000234125
#define c3 0.0000000876741

// Set amperemeter pin
#define amperemeter_pin 35
// amperemeter ACS712 (version up to 5A) raises output voltage with 185mV per 1A
// points_per_amp = 0.185 / 3.34 * 4096, where 3.34 is the work voltage of esp32 and 4096 is the resolution ot the ADC
#define points_per_ampere 270
// ACS712 output range starts from 2.5V, that means 2.5V (read as 3065 points by the ADC) is 0A
// 3065 is adjusted to 2820 to level inaccuracies of the system
#define offset_in_points 2820

// Set pin numbers for LDR voltage dividers
#define ldr_pin_1 33
#define ldr_pin_2 32
#define ldr_pin_3 34
#define ldr_pin_4 35
// Since LDRs aren't precisely equal, different multipliers will be used to level their accuracy 
#define ldr_multiplier_1 1
#define ldr_multiplier_2 1
#define ldr_multiplier_3 1.1
#define ldr_multiplier_4 0.9

//constants required for measure_resistance()
#define R1 51000 // the resistance of the known resistor
#define Vin 3.34 // the voltage across both resistors

// Set servo pins
#define servo_pin_1 18
#define servo_pin_2 19
// Servo objects dervived from ESP32Servo.h to control the servos
Servo servo_1;
Servo servo_2;
// Variables to store the servo positions
int servo_position_1 = 90; 
int servo_position_2 = 90; 

// function that prints "Connecting to WiFi.." on the LCD
void lcd_print_connecting(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print("WiFi..");  
}
// function that prints "Connection to WiFi successful" on the LCD
void lcd_print_connected(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connection to");
  lcd.setCursor(0, 1);
  lcd.print("WiFi successful");
  Serial.println("Connected to the WiFi network");
}
// function that prints "Connecting to WiFi is lost" on the LCD
void lcd_print_lost_connection(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connection to");
  lcd.setCursor(0, 1);
  lcd.print("WiFi is lost");  
}
// function that prints "Connecting to WiFi is lost" on the LCD
void lcd_print_server_down(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Server is down");
}

// function that creates and sends json with the measured data and sends it to the specified url in the "server" constatnt
int send_data(){
  if(WiFi.status()== WL_CONNECTED){ //Check WiFi connection status

    // Get measurement from ammperemeter
    float current = measure_current();
    // Get measurement from voltage divider
    float voltage = measure_voltage();
    // Get measurement from temperature sensor
    float temperature = measure_temperature();

    // Take multiple measurements
    for(int i = 0; i<4; i++){
      delay(50);
      current += measure_current();
      voltage += measure_voltage();
      temperature += measure_temperature();
    }
    // Average the measurements and for greater accuracy
    current /= 5;
    voltage /= 5;
    temperature /= 5;

    // Create HTTP client and point it to the address of the server
    HTTPClient http;   
    http.begin(server);
    
    // Create and post a JSON object using the HTTP client
    // First it's specified that the request body contains JSON
    http.addHeader("Content-Type", "application/json");
    // Then the JSON is created and immediately the request is sent
    int httpResponseCode = http.POST("{\"current\":" + String(current) + 
                                     ",\"voltage\":"  + String(voltage) + 
                                     ",\"temperature\":" + String(voltage) + "}");
    // Check server response
    if(httpResponseCode>0){
      String response = http.getString(); //Get the response to the request
      Serial.println(httpResponseCode); //Print return code
      //Serial.println(response); //Print request answer
    }else{
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();  //Free resources
    return 0;
  
  }else{ // If WiFi status is compromised print message on the LCD
    lcd_print_lost_connection();
    Serial.println("Error in WiFi connection");
    return -1;
  }  
}
 
void setup() {
  Serial.begin(115200);
  
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

  delay(4000);   //Delay needed before calling the WiFi.begin
  // Try connection to lockal WiFi network using the specified credentials in "ssid" and "password"
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { // Continiously check for connection
    delay(1000); // Delay is added to make sure the network doesn't mistake the ESP32 for DDOS bot
    lcd_print_connecting();
  }
  // Show the connection was sucessful on the LCD
  lcd_print_connected();

  // Allow allocation of all timers. This is required to controll the servos
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  // Standard 50 hz servo (20ms wavelenght), this is the required frequency for the MG996R servo
  servo_1.setPeriodHertz(50);
  servo_2.setPeriodHertz(50);
  // Attaches the servo on pin 18 to the servo object
  // 500 and 4000 specify the range (reach) of the servo which in reality is 120 degrees
  servo_1.attach(servo_pin_1, 500, 4000);
  servo_2.attach(servo_pin_2, 500, 4000); 
}

// function reseiving number of pin connected to Vout of voltage divider using known resistor of "R1" ohms 
int measure_resistance(int input_pin){
    int raw = analogRead(input_pin); // Get input from ADC pin (ranging from 0-4096 equal to 0-3.3V)
    if(raw){
      float buffer = raw * Vin;
      float Vout = buffer/ 4095; // Calculate voltage drop on second resistor, 4095 is the resolution of the ESP32 ADCs
      buffer = (Vin/Vout) -1;
      int R2 = R1 / buffer; // Calculate resistance knowing that R1 is 51K ohms
      //Serial.println(String(R2));
      return R2;
    }
    return -1; // If analogRead() didn't yield proper value return -1
}

// functin that measures the input of ACS712 amperemeter connected to amperemeter_pin and returns amperes measured
float measure_current(){
    float input = analogRead(amperemeter_pin);
    float current = (input - offset_in_points) / points_per_ampere;
    return current > 0 ? current : 0;
}

// functin that measures the input of temperature sensor TU K13 an returns degrees Celsius
float measure_voltage(){
    float input = analogRead(voltage_sensor_pin);
    Serial.print( "Voltage:  " );
    Serial.println( input / 33.5 ); //33.5 because the voltage divider is made using 51K and 10K resistors
    return input / 33.5;
}

// functin that measures the input of temperature sensor TU K13 an returns degrees Celsius
float measure_temperature(){
    int input = analogRead(temperature_sensor_pin);
    float R2 = R1_temperature_sensor * (1023.0 / (float)input - 1); //calculate resistance on thermistor
    float logR2 = log(R2);
    float T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // temperature in Kelvin
    T = T - 273.15; //convert Kelvin to Celcius
    //Serial.println("Temperature: " + String(T) + " C"); 
    return T;

}

void controll_servo(){
  // Get measurement for LDR1
  int ldr1 = measure_resistance(ldr_pin_1) * ldr_multiplier_1; 
  //Serial.println("LDR 1: " + String(ldr1));
  // Get measurement for LDR2
  int ldr2 = measure_resistance(ldr_pin_2) * ldr_multiplier_2;
  //Serial.println("LDR 2: " + String(ldr2));

  // Compare LDR1 and LDR2 and move the servo towards the brighter side
  if( (ldr1 - ldr2) > (ldr1 + ldr2)/2 * 0.2 ){
    if(servo_position_1 < 180) servo_position_1 ++;
    }
    if( (ldr2 - ldr1) > (ldr1 + ldr2)/2 * 0.2 ){
      if(servo_position_1 > 0) servo_position_1 --;
    }
  // Move servo one
  servo_1.write(servo_position_1);

  // Get measurement for LDR1
  int ldr3 = measure_resistance(ldr_pin_3) * ldr_multiplier_3; 
  //Serial.println("LDR 3: " + String(ldr3));
  // Get measurement for LDR2
  int ldr4 = measure_resistance(ldr_pin_4) * ldr_multiplier_4;
  //Serial.println("LDR 4: " + String(ldr4));

  // Compare LDR1 and LDR2 and move the servo towards the brighter side
  if( (ldr3 - ldr4) > (ldr3 + ldr4)/2 * 0.2 ){
    if(servo_position_2 < 180) servo_position_2 ++;
    }
    if( (ldr3 - ldr4) > (ldr3 + ldr4)/2 * 0.2 ){
      if(servo_position_2 > 0) servo_position_2 --;
    }
  // Move servo one
  servo_2.write(servo_position_2);
}

void loop() {

  // Compare LDRs and move the servo if necessary
  controll_servo();

  // Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay){
    //send_data();
    lastTime = millis();
  }
  delay(100);
  
}
