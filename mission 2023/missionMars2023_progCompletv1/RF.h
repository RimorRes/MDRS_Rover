#include <RF24.h>
void Run(String INSTRUCTION);
extern RF24 radio;
extern String messageRF;

boolean emettreMessage(String message);
String RFpull();
void RFpush(String phrase);
String recupererMessageRF();
String ecouterAntenneRF();
