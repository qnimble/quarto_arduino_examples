IntervalTimer updateTimer;
#define SETPOINT 2300


void setup() {
  // put your setup code here, to run once:
  setTrigger1Direction(PIN_DIRECTION_OUTPUT);
  setTrigger2Direction(PIN_DIRECTION_OUTPUT);
  //configureADC1(4,0,BIPOLAR_1250mV,getADC1);
  //GPIO8_DR_CLEAR = 0x07;
  updateTimer.begin(ddsUpdate, 2); // update every 10 microseconds
  Serial.begin(115200);
  writeDAC3(2.5);
  writeDAC4(-2.5);
  //GPIO7_GDIR = ((0xF)<<20);
  //GPIO6_GDIR = ((0xF)<<12);
  
 /*
  for(uint8_t i=1;i<9;i++){
    pinMode(i,OUTPUT);  
  }
  */
}

void loop() {
  static unsigned long lastrun;    
  if (millis() > lastrun + 500) { //Run once every 500ms
      toggleLEDGreen(); //toggle green LED;            
      lastrun = lastrun + 500;
  }
  while (Serial.available() > 0) {
    Serial.print(Serial.read()); // Echo any serial data back
  }
    
}


void getADC1(void) {
  setTrigger1High();
  static double integral = 0;
  static int16_t prev_adc = 0;
  int16_t newadc = readADC1_from_ISR();
  
  double diff = ( newadc - prev_adc) * .00001; // turn diff down for accuracate BW measurement
  integral += (newadc - SETPOINT) * 0.01; // integral gain
  double prop = (newadc-SETPOINT) * 0.214;
  int16_t newdac = prop + integral + diff;
  writeDACRAW3(-newdac); //invert for negative feedback 
  prev_adc = newadc;
  setTrigger1Low();
}





void ddsUpdate() {  
    setTrigger2High();
  static unsigned int phase = 0;
  phase++; // Increment phase of our repeating function
  if (phase >= 3600) { //set max phase to 3600, so each step is 3600 of a cycle, or 0.1deg
    phase = phase-3600;
    //setTrigger1High();
  } else {
    //setTrigger1Low();
  }
  //writeDAC1(abs(3.4*sin(2*PI/50.0 * phase)));
  //writeDAC2(abs(3.4*cos(2*PI/50.0 * phase)));
  writeDAC1((float) phase);
  writeDAC2((float) phase-5);
  
  
  setTrigger2Low();
}
