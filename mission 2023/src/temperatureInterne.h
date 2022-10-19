/* routines de gestion du capteur TMP102 pour surveiller la temp�rature interne du rover */
#include<Arduino.h>
#include <SparkFunTMP102.h>

boolean init_tmp102(TMP102 sensor0);

float read_temperature(TMP102 sensor0);
  // lit la température interne avec le tmp102

String test_temp_int(TMP102 sensor0, float Tint_min, float Tint_max);
