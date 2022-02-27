
#define ldr_pin_1 13
#define ldr_pin_2 12

// Since LDRs aren't precisely equal, different multipliers will be used to level their accuracy 
#define ldr_multiplier_1 1
#define ldr_multiplier_2 1

#define R1 51000
#define Vin 3.34
 
void setup() {

  Serial.begin(9600);
}

int measure_resistance(int input_pin){
    int raw = analogRead(input_pin);
    
    Serial.print("Raw: ");
    Serial.println( raw );
    
    if(raw){
      float buffer = raw * Vin;
      float Vout = buffer/4095;
      
      Serial.print("Vout: ");
      Serial.println( Vout );
      
      buffer = (Vin/Vout) -1;
      int R2 = R1 / buffer;
      return R2;
    }
    return -1;
}

void loop() {
  int ldr1 = measure_resistance(ldr_pin_1) * ldr_multiplier_1;
  Serial.print("LDR 1: ");
  Serial.println( ldr1 );

  int ldr2 = measure_resistance(ldr_pin_2) * ldr_multiplier_2;
  Serial.print("LDR 2: ");
  Serial.println( ldr2 );

  

  delay(800);
}
