/*****************************************************************
 * Définition des fonctions utiles à la gestion de la position GPS
******************************************************************/

#include <Arduino.h>
#if !defined DEPLACEMENT_H
  #include "deplacement.h"
  #define DEPLACEMENT_H
#endif

boolean GPS_init();
void testGPS();
Point convertSphereToPlan(float lat, float lon, Point origine); // conversion des données latitude/longitude en x/y par rapport au centre du repère sur la carte
Point convertSphereToPlan(float lat, float lon); // conversion des données latitude/longitude en x/y par rapport au centre du repère sur la carte
float convertDegMinSecToDecimal(float deg, float minutes, float sec);  // conversion des données angulaires degrés/minutes/secondes en degrés valeur décimale

class BufferFloat
{
  public:
    void addData(float newData);
    //float getData(int index) const;
    float Mean() const;
  private:
    int _dataPtr = 0;
    int _size = 0;
    float _data[10];
    const int _nombrePointsMoyenneGPS = 10; // le nombre de points GPS sur lesquels on moyenne (moyenne glissante)
};

Point calculePositionActuelle(BufferFloat latitudeBuffer, BufferFloat longitudeBuffer);
float* positionGPSNouvelle();
