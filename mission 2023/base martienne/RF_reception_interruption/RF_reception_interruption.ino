#include <nRF24L01.h>
#include <RF24.h>

const int PIN_CE = 7;//9;
const int PIN_CSN = 8;//10;
char gotByte[32];
volatile int count = 0;
int pCount = 0;
RF24 wirelessSPI(PIN_CE, PIN_CSN);
//const uint64_t pAdress = 0xB00B1E5000LL; // LL indicates "long long type"
const byte pAdress[6] = "PIPE2";

void setup() {
  Serial.begin(9600);  wirelessSPI.begin();
  
  wirelessSPI.setAutoAck(1);
//  wirelessSPI.enableAckPayload();
  wirelessSPI.maskIRQ(1, 1, 0);
  wirelessSPI.setPALevel(RF24_PA_MIN);//LOW);
  wirelessSPI.openReadingPipe(1, pAdress);
  wirelessSPI.startListening();
  attachInterrupt(1, interruptFunction, FALLING);
  Serial.println("coucou setup");
}

void loop() {
  Serial.println("coucou loop, count = " + String(count));
  if(pCount>count){
//    Serial.begin(9600); // 57600
    Serial.println("coucou loop");
    Serial.print("Received pacet number ");
    Serial.println(count);
//    Serial.end();
    pCount = count;
  }
}

void interruptFunction(){
  count++;
  while(wirelessSPI.available()){
    wirelessSPI.read(&gotByte, sizeof(gotByte));
  }
}
