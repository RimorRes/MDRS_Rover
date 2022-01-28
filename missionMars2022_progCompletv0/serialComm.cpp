/*Serial communication between Arduino and Raspberry Pi. */
#include<Arduino.h>
#define AFFICHAGE // à commenter pour enlever le bavardage

String serialPull(int TIMEOUT = 1000){ // Pulls/Reads incoming data from serial port. It reads one instrution but does not interpret it. 
  String phrase;
  char terminator =';';
  Serial.setTimeout(TIMEOUT); // le temps au bout duquel on arrête de surveiller le bus
  phrase = Serial.readStringUntil(terminator);  // on lit le bus jusqu'à trouver le caractère de fin
  int posFin = phrase.indexOf(char(10));  // on repère le retour à la ligne qui vient lorsqu'on utilise le terminal série
  if (posFin != -1){phrase.remove(posFin);} // et on l'enlève !
 
#ifdef AFFICHAGE
    // say what you got:
    //Serial.println("I received: " + phrase);
#endif
  return phrase;
}

void serialPush(String TXT){ // Send bytes from serial port. Simplification of Serial.Write().
#ifdef AFFICHAGE
    Serial.println("Coucou, nous sommes dans serialPush().");
#endif
    unsigned int longueur = TXT.length();
    if (longueur != 0){
      char message[longueur+1]; // +1 car il faut stocker le caractère de fin de chaîne, je crois
      TXT.toCharArray(message, longueur+1); //  conversion de type
#ifdef AFFICHAGE
      Serial.println("   Texte à transmettre : " + TXT + " de longueur " + String(longueur) + ".");
      Serial.println("   Texte envoyé : " + String(message));
#endif
      Serial.write(message, longueur);  // Pas +1, donc sans le caractère de fin.
#ifdef AFFICHAGE
      Serial.println(" <-- C'est fait.");
#endif
    }
    return;
}

void noel(/*const char* ARG*/){
    digitalWrite(8, HIGH);
    delay(250);
    digitalWrite(8, LOW);
    delay(250);
    digitalWrite(9, HIGH);
    delay(250);
    digitalWrite(9, LOW);
    delay(250);
    //Serial.println(ARG);
}
