/*Serial communication between Arduino and Raspberry Pi. */
#include<Arduino.h>
void Run(String INSTRUCTION);
extern String messageBus;
//define AFFICHAGE // à commenter pour enlever le bavardage

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

void lectureBus(){
  int dureeAttente = 1000;  // en millisecondes
  int TIMEOUT = 1000; // durée max de surveillance du bus
  String reception = serialPull();  // lecture bus
  if (reception =="A"){ // vérifier que le message commence par "A" permet d'éviter de transmettre de mauvaise instructions si le début du message s'est perdu
    // la fin du message n'est pas vérifiée, ce qui est un défaut.
    Serial.print("R");  // on répond qu'on est prêt
    delay(dureeAttente);
    Serial.setTimeout(TIMEOUT); // le temps au bout duquel on arrête de surveiller le bus
    while (Serial.available()){ // tant que le bus n'est pas vide...
      reception = serialPull(); // on lit une instruction
#ifdef AFFICHAGE
      Serial.println("Je vais exécuter : " + reception);
#endif
      Run(reception); // et on exécute l'instruction
    } // le bus est vide
  } // si ça ne commençait pas par "A" on n'a rien fait
  return;
}

void parlerBus(){
  int dureeAttente = 1000;  // en millisecondes
  int nombreEssais = 2;
  for (int i = 0; i<nombreEssais; i++){ // essais de prise de parole
    Serial.print("A");  // demande la parole
    delay(dureeAttente);
    String reception = serialPull();  // lit le bus
    if (reception =="R"){ // si ça répond...
      Serial.print(messageBus); // on parle
      messageBus="";  // plus rien à dire
      break;  // et on arrête
    } // ...sinon on retente
  }
  return;
}
