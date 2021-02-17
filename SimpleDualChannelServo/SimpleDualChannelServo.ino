#define SETPOINT 2300

void setup(void) {
  Serial.begin(115200);
  setTrigger1Direction(OUTPUT);  
  setTrigger2Direction(OUTPUT);  
  
  configureADC1(2,0,BIPOLAR_1250mV,getADC1);  
  configureADC4(2,1,BIPOLAR_1250mV,getADC4);
}

void getADC1(void) {
  static double integral = 0;
  static int16_t prev_adc = 0;
  int16_t newadc = readADC1_from_ISR();
  
  double diff = ( newadc - prev_adc) * .00001; // turn diff down for accuracate BW measurement
  integral += (newadc - SETPOINT) * 0.01; // integral gain
  double prop = (newadc-SETPOINT) * 0.214;
  int16_t newdac = prop + integral + diff;
  writeDAC1(-newdac); //invert for negative feedback 
  prev_adc = newadc;
}

void getADC4(void) {  
  static double integral = 0;
  static int16_t prev_adc = 0;
  int16_t newadc = readADC4_from_ISR();
  
  double diff = ( newadc - prev_adc) * .00001; // turn diff down for accuracate BW measurement
  integral += (newadc - SETPOINT) * 0.01; // integral gain
  double prop = (newadc-SETPOINT) * 0.2125;
  int16_t newdac = prop + integral + diff;  
  writeDAC4(-newdac); //invert for negative feedback  
  prev_adc = newadc;  
}

void loop() {
  static uint lastrun1;  
  
  if (millis() > lastrun1 + 500) {
      setTrigger1High();      
      toggleLED(false,true,false); //toggle green LED;
      setTrigger2High();
      Serial.print("_");
      setTrigger2Low();
      delay(5);
      lastrun1 = millis();      
      setTrigger1Low();
  }
  
}
