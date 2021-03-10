#define SETPOINT 0.25

void setup(void) {  
  configureADC1(1,0,BIPOLAR_1250mV,getADC1); // Have ADC take measurement every 1us, ±1.25V range
  Serial.begin(115200); //initize COM port
}

void getADC1(void) {
  static double integral = 0;
  static double prev_adc = 0;
  
  double newadc = readADC1_from_ISR(); //read ADC voltage
  
  double prop = (newadc-SETPOINT) * 1.975; //proportional
  integral += (newadc - SETPOINT) * 0.01; // integral gain
  double diff = ( newadc - prev_adc) * .00001; // turn diff down for accuracate BW measurement
  double newdac = prop + integral + diff;
  
  writeDAC1(-newdac); //invert for negative feedback  
  prev_adc = newadc; //store new adc value for differential calculation
}

void loop() {
  static unsigned long lastrun = millis();    
  if (millis() > lastrun ) { 
      lastrun += 500; // Run loop every 500ms
      toggleLEDGreen(); //toggle green LED;                  
  }
  while (Serial.available() > 0) {
    Serial.print(Serial.read()); // Echo any serial data back
  }    
}
