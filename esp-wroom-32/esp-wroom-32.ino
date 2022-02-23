int servo_position = 90;
#define servo_pin 6

#define current_sensor_pin 14
#define voltage_sensor_pin 27
#define temperature_sensor_pin 26

#define ldr_pin_1 13
#define ldr_pin_2 12

#define ldr_multiplier_1 1
#define ldr_multiplier_2 1

#define R1 1000
#define Vin 3.3
 
void setup() {

  Serial.begin(9600);
}

int measure_light(int input_pin){
    int raw = analogRead(input_pin);
    Serial.println(raw);
    if(raw){
      float buffer = raw * Vin;
      float Vout = buffer/4096;
      buffer = (Vin/Vout) -1;
      int R2 = R1 * buffer;
      return R2;
    }
    return -1;
}

void loop() {
  int ldr1 = measure_light(ldr_pin_1) * ldr_multiplier_1;
  Serial.print("LDR 13: ");
  Serial.println( ldr1 );

  int ldr2 = measure_light(ldr_pin_2) * ldr_multiplier_2;
  Serial.print("LDR 12: ");
  Serial.println( ldr2 );

  ldr1 = measure_light(27) * ldr_multiplier_1;
  Serial.print("LDR 27: ");
  Serial.println( ldr1 );

  ldr2 = measure_light(26) * ldr_multiplier_2;
  Serial.print("LDR 26: ");
  Serial.println( ldr2 );

  ldr1 = measure_light(33) * ldr_multiplier_1;
  Serial.print("LDR 33: ");
  Serial.println( ldr1 );

  ldr2 = measure_light(32) * ldr_multiplier_2;
  Serial.print("LDR 32: ");
  Serial.println( ldr2 );

  ldr1 = measure_light(4) * ldr_multiplier_1;
  Serial.print("LDR 4: ");
  Serial.println( ldr1 );

  ldr2 = measure_light(2) * ldr_multiplier_2;
  Serial.print("LDR 2: ");
  Serial.println( ldr2 );

  delay(2000);
}
