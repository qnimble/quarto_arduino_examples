IntervalTimer myTimer;



void setup() {
  // put your setup code here, to run once:
  setTrigger1Direction(PIN_DIRECTION_OUTPUT);
  setTrigger2Direction(PIN_DIRECTION_OUTPUT);
  //GPIO8_DR_CLEAR = 0x07;
  myTimer.begin(test, 2000);
  Serial.begin(115200);
  //GPIO7_GDIR = ((0xF)<<20);
  //GPIO6_GDIR = ((0xF)<<12);
  
 
  for(uint8_t i=1;i<9;i++){
    pinMode(i,OUTPUT);  
  }
  
}

void loop() {
  static unsigned long lastrun;    
  if (millis() > lastrun + 400) { //Run once every 500ms            
      toggleLEDBlue(); //toggle green LED;            
      lastrun = lastrun + 400;
  }
}
void test() {  
  setTrigger1High();
  static uint16_t counts=0;
  counts++;

  if (counts % 2 == 0) {
    setTrigger1High();
  } else {
    setTrigger1Low();
  }

  for(uint8_t i=0;i<8;i++){
    if ( counts % (1<<(3+i)) == 0 ) {      
      digitalWriteFast(i+1,HIGH);
    } else if (counts % (1<<(2+i)) == 0 ) {
      digitalWriteFast(i+1,LOW);
    }
  }
  setTrigger1Low();
}
