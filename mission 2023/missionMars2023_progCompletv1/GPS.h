/*****************************************************************
 * Définition des fonctions utiles à la gestion de la position GPS
******************************************************************/

#include <Arduino.h>
#if !defined DEPLACEMENT_H
  #include "deplacement.h"
  #define DEPLACEMENT_H
#endif 
extern const int nombrePointsMoyenneGPS;
#define TAILLE_BUFFER 10 // le nombre de points GPS sur lesquels on moyenne (moyenne glissante)

boolean GPS_init();
void testGPS();
Point convertSphereToPlanGPS(float lat, float lon, Point origine); // conversion des données latitude/longitude en x/y par rapport au centre du repère sur la carte
Point convertSphereToPlanGPS(float lat, float lon); // conversion des données latitude/longitude en x/y par rapport au centre du repère sur la carte
float convertDegMinSecToDecimalGPS(float deg, float minutes, float sec);  // conversion des données angulaires degrés/minutes/secondes en degrés valeur décimale

class BufferFloat
{
  public:
    BufferFloat();
    void addData(float newData);
    //float getData(int index) const;
    float Mean() const;
  private:
    int _size;
    float _data[TAILLE_BUFFER];
    boolean _full;
};

//Point calculePositionActuelle(float *latitudeBuffer, float *longitudeBuffer, int nombrePointsMoyenneGPS);
Point calculePositionActuelle(BufferFloat latitudeBuffer, BufferFloat longitudeBuffer);
float* positionGPSNouvelle();
//float* actualiserBuffer(float* oldBuffer, float newData);
