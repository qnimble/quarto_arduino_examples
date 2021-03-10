#define SETPOINT 2300

void setup(void) {
  Serial.begin(115200);
  setTrigger1Direction(OUTPUT);  
  setTrigger2Direction(OUTPUT);  
  
  configureADC1(2,0,BIPOLAR_1250mV,getADC1);  //Set servo on ADC/DAC channel 1
  configureADC4(2,1,BIPOLAR_1250mV,getADC4); //Set servo on ADC/DAC channel 4
}

void getADC1(void) {
  setTrigger1High(); // Measure execution time
  
  static double integral = 0;
  static int16_t prev_adc = 0;
  
  int16_t newadc = readADC1_from_ISR(); // read ADC data
  double diff = ( newadc - prev_adc) * .00001; // turn diff down for accuracate BW measurement
  integral += (newadc - SETPOINT) * 0.000001; // set integral gain  
  double prop = (newadc-SETPOINT) * 0.0000814; // proportional gain
  double newdac = prop + integral + diff; // P+I+D
  writeDAC1(-newdac); //invert for negative feedback 
  prev_adc = newadc;
  setTrigger1Low();
}

void getADC4(void) {  
  setTrigger2High();
  
  static double integral = 0;
  static int16_t prev_adc = 0;
  
  int16_t newadc = readADC4_from_ISR(); // read ADC data
  double diff = ( newadc - prev_adc) * .00001; // turn diff down for accuracate BW measurement
  integral += (newadc - SETPOINT) * 0.0000001; // set integral gain  
  double prop = (newadc-SETPOINT) * 0.0000814; // prportional gain
  double newdac = prop + integral + diff;  // P+I+D
  writeDAC4(-newdac); //invert for negative feedback  
  prev_adc = newadc; 
  setTrigger2Low(); 
}

void loop() {
  static uint lastrun1;  
  
  if (millis() > lastrun1 + 500) {      
      toggleLEDBlue(); //toggle blue LED every 500ms
      lastrun1 = millis();
  }
}
