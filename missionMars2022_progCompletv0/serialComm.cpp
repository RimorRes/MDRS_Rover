/*Serial communication between Arduino and Raspberry Pi. */
#include<Arduino.h>
#define AFFICHAGE // à commenter pour enlever le bavardage

String serialPull(int TIMEOUT = 1000){ // Pulls/Reads incoming data from serial port. It does NOT read the instrution. 
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

void Run(String INSTRUCTION){ // Reads the instruction to call it after. 
    if (INSTRUCTION != ""){
#ifdef AFFICHAGE
      Serial.println("instruction dans Run : " + INSTRUCTION);
      Serial.println("   taille instruction : " + String(INSTRUCTION.length())); 
#endif
      char separator='_'; // caractère de séparation entre fonction et arguments, ou entre arguments
      
      // récupération du code de la fonction appelée
      int posFin = INSTRUCTION.indexOf(separator); // repère le passage de la fonction aux arguments de la fonction
      int numFonction = INSTRUCTION.substring(0, posFin).toInt(); // lit le début de l'instruction comme un entier codant la future fonction à exécuter. Du coup, on aurait pu coder avec parseInt().
      String chaineArguments = INSTRUCTION.substring(posFin);
#ifdef AFFICHAGE
      Serial.println("   fonction n° : " + String(numFonction));
      Serial.println("   chaine d'arguments : " + chaineArguments);
#endif
      
      // récupération des arguments
      int i=0; int numArg=0;
      for (i=0;i<chaineArguments.length();i++){ // on compte les arguments
        if (chaineArguments[i] == separator){numArg++;}
      }
#ifdef AFFICHAGE
      Serial.print("   Il y a "); Serial.print(numArg); Serial.println(" arguments.");
#endif
      String arguments[numArg]; // le tableau des arguments
      for(i=0;i<numArg;i++){
        chaineArguments = chaineArguments.substring(1); // on enlève le séparateur
        posFin = chaineArguments.indexOf(separator);  // on repère la fin de l'argument
        arguments[i] = chaineArguments.substring(0, posFin); // on extrait l'argument
        chaineArguments = chaineArguments.substring(posFin); // on enlève ce qui a été lu
#ifdef AFFICHAGE
        Serial.println("   argument : " + arguments[i]);
#endif
      }
      
      switch(numFonction){
        case 1:
          Serial.println("Youpi !"); break;
        case 2:
          noel(); break;
        case 3:
          if(numArg != 0){serialPush(arguments[0]);} break;
        case 4:
          if(numArg != 0){Serial.println(arguments[0]);} break;
        case 5:
          if(numArg != 0){
            String message = "";
            for (i=0;i<numArg;i++){
              message += arguments[i];
              message += " ";
            }
            Serial.println(message);
          } break;
        default: // Case of unmatched function. 
#ifdef AFFICHAGE
            Serial.println("Ca serait gentil de me donner des instructions que je comprends !");
#endif
            return; // Panic.
      }
    }
}
