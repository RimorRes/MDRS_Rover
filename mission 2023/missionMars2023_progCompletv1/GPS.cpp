/*****************************************************************
 * Définition des fonctions utiles à la gestion de la position GPS
******************************************************************/

#include <Arduino.h>
#include <Math.h>
#if !defined SPECIFICATIONS_H
  #include "specifications.h"
  #define SPECIFICATIONS_H
#endif
//extern Rover_config rover_config;
#if !defined DEPLACEMENT_H
  #include "deplacement.h"
  #define DEPLACEMENT_H
#endif 
#if !defined GPS_H
  #include "GPS.h"
  #define GPS_H
#endif
#if !defined ADAFRUIT_GPS_H
#include <Adafruit_GPS.h> // module Adafruit
//#include <Adafruit_PMTK.h>  // module associé à Adafruit_GPS, mais inutile ici
//#include <NMEA_data.h>  // module associé à Adafruit_GPS, mais inutile ici
#define ADAFRUIT_GPS_H
#endif
extern Adafruit_GPS GPS;
extern HardwareSerial GPSSerial;
extern uint32_t timer;
#define GPSECHO true // pour que tout soit renvoyé vers la comm série

/*#if !defined SOFTWARESERIAL_H
#include <SoftwareSerial.h> // pour simuler la communication série sur des ports non-dédiés
#define SOFTWARESERIAL_H
#endif
SoftwareSerial mySerial(myPINs.PIN_GPS_TX, myPINs.PIN_GPS_RX);
*/

/* Dans Adafruit_GPS :
 *  latitude et longitude sont au format DDDMM.MMMM 
 *  latitudeDegrees et longitudeDegrees sont au format décimal, en degrés, signé (pos = N et E)
 *  latitude_fixed et longitude_fixed sont la même chose mais sont des entiers (* 1E7)
 *  lat et lon sont la direction : un char parmi N, S, E, W ou false */
extern float latitudeBuffer;
extern float longitudeBuffer;

/*float moyenneGlissante(float* tab){
  int nbr = length(tab);
  if (nbr>0){
    float moyenne = 0;
    for (int i=0; i++; i<nbr){
      moyenne += tab[i];
    }
  }
  return moyenne/nbr;
}

float* miseAJourTampon(float* tab, float valeur){
  
  return tab;
}*/

boolean GPS_init(){
  GPS.begin(9600);  // 4800 serait aussi possible
  Serial.println("begin");
    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
    //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // uncomment this line to turn on only the "minimum recommended" data
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  Serial.println("RMC only");
    //GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  //Serial.println("1Hz");
    // For the parsing code to work nicely and have time to sort thru the data, and
    // print it out we don't suggest using anything higher than 1 Hz
  //
    // Request updates on antenna status, comment out to keep quiet
  //  GPS.sendCommand(PGCMD_ANTENNA);
  //Serial.println("antenna status");
  //
  delay(1000);
  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
  return true;
}

void testGPS(){
  // gérer la varaible timer, pour l'instant elle est juste après l'appel du module, dans le fichier principal
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
//  if ((c) && (GPSECHO))
//    Serial.write(c);
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer

    Serial.print("\nTime: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);

      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }
  return;
}

// conversion des données latitude/longitude en x/y par rapport au centre du repère sur la carte
Point convertSphereToPlan(float lat, float lon, Point origine){
  // angles en degrés, valeurs décimales
  // latitude : positive pour le nord, négative pour le sud
  // longitude : positive pour l'est, négative pour l'ouest
  const long RayonTerreEnMetres = 6378137;
  float y = ( lat - origine.getX() ) / 180 * PI * RayonTerreEnMetres;
  float x = ( lon - origine.getY() ) / 180 * PI * RayonTerreEnMetres * cos(lat);
  return Point(x, y);
}

Point convertSphereToPlan(float lat, float lon){
  return convertSphereToPlan(lat, lon, rover_config.getCentreRepere());
}

// conversion des données angulaires degrés/minutes/secondes en degrés valeur décimale
float convertDegMinSecToDecimal(float deg, float minutes, float sec){
  minutes += sec / 60;
  deg += minutes / 60;
  return deg;
}

/*// retourne la position GPS en mètre, position absolue (par rapport à l'intersection Greenwich/équateur
Point pointGPS_absolu(){
  return convertSphereToPlan(gps.latitude, gps.longitude, Point(O,O,O));
}

// retourne la position GPS en mètre, par rapport au centre de la carte
Point pointGPS_carte(){
  return convertSphereToPlan(gps.latitude, gps.longitude, getCentreRepere());
}*/

Point calculePositionActuelle(BufferFloat latitudeBuffer, BufferFloat longitudeBuffer){
  return convertSphereToPlan(latitudeBuffer.Mean(), longitudeBuffer.Mean());
}

float* positionGPSNouvelle(){
  static float positionGPSNouvelle[2] = {0,0};  // si on le retourne tel quel, ce sera interprété comme "pas de nouveau point GPS"
  char c = GPS.read();
  //if (GPSECHO) // inutile, je crois
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())){   // this also sets the newNMEAreceived() flag to false
      return positionGPSNouvelle;  // we can fail to parse a sentence in which case we should just wait for another
    }
  } // ajouter "else {return positionGPSNouvelle;}" ?

  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer

    if (GPS.fix) {
      positionGPSNouvelle[0] = GPS.latitude;
      positionGPSNouvelle[1] = GPS.longitude;
    }
  }
  return positionGPSNouvelle; // retourne donc {0,0} s'il n'a pas pu localiser
}

/* ------------------
 * classe BufferFloat 
 * ------------------*/

void BufferFloat::addData(float newData){
  if(_size < _nombrePointsMoyenneGPS){
    _data[_size++] = newData;
  }
  else{
    _data[_dataPtr++] = newData;
    if(_dataPtr == _nombrePointsMoyenneGPS){_dataPtr = 0;}
  }
}

/*float BufferFloat::getData(int index) const{
  if (index<_size) {return _data[index];}
  return 0;
}*/

float BufferFloat::Mean() const{
  float mean = 0;
  for (int i=0;i<_size;i++){
    mean += _data[i];
  }
  mean /= _size;
  return mean;
}
