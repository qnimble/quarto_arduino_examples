void setup() {
  // put your setup code here, to run once:
  setTrigger1Direction(PIN_DIRECTION_OUTPUT);
  setTrigger2Direction(PIN_DIRECTION_OUTPUT);
  Serial.begin(115200);  
}

void loop() {
  static unsigned long lastrun;
  
  if (millis() > lastrun + 500) { //Run once every 500ms            
      static int state = 0;
      state++;
      if (state >=8) {
        state=0;        
      }
      switch(state) {
        case 0:
          setLED(1,0,0); //red on, blue and green off          
          break;
        case 2:
          setLED(0,1,0); //green on, red and blue off
          break;
        case 4:
          setLED(0,0,1); //blue on, red and green off
          break;
        case 6:
          setLED(1,1,1); //white: RGB on
          break;
        default:
          setLED(0,0,0); //LED off
          break;          
      }

      lastrun = lastrun + 500;
  }

  char dat;
  while (Serial.available() > 0) {
   dat = Serial.read();
   dat++;
   Serial.print(dat);
  }
  
  
}
