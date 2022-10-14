/* routines de gestion du capteur TMP102 pour surveiller la temp�rature interne du rover */
#include<Arduino.h>
#include <SparkFunTMP102.h>
//#define AFFICHAGE // commenter pour rendre moins bavard !

boolean init_tmp102(TMP102 sensor0){

  //pinMode(ALERT_PIN,INPUT);  // Declare alertPin as an input
  if(!sensor0.begin())
  {
#ifdef AFFICHAGE
    Serial.println("Cannot connect to TMP102.");
    Serial.println("Is the board connected? Is the device ID correct?");
#endif
    return false; // détection d'un problème
  }
#ifdef AFFICHAGE
  Serial.println("Connected to TMP102!");
#endif
  delay(100);

  // Initialize sensor0 settings
  // These settings are saved in the sensor, even if it loses power

  // set the number of consecutive faults before triggering alarm.
  // 0-3: 0:1 fault, 1:2 faults, 2:4 faults, 3:6 faults.
  sensor0.setFault(0);  // Trigger alarm immediately

  // set the polarity of the Alarm. (0:Active LOW, 1:Active HIGH).
  sensor0.setAlertPolarity(1); // Active HIGH

  // set the sensor in Comparator Mode (0) or Interrupt Mode (1).
  sensor0.setAlertMode(0); // Comparator Mode.

  // set the Conversion Rate (how quickly the sensor gets a new reading)
  //0-3: 0:0.25Hz, 1:1Hz, 2:4Hz, 3:8Hz
  sensor0.setConversionRate(2);

  //set Extended Mode.
  //0:12-bit Temperature(-55C to +128C) 1:13-bit Temperature(-55C to +150C)
  sensor0.setExtendedMode(0);

  //set T_HIGH, the upper limit to trigger the alert on
  sensor0.setHighTempF(85.0);  // set T_HIGH in F
  //sensor0.setHighTempC(29.4); // set T_HIGH in C

  //set T_LOW, the lower limit to shut turn off the alert
  sensor0.setLowTempF(84.0);  // set T_LOW in F
  //sensor0.setLowTempC(26.67); // set T_LOW in C

  return true;
}

float read_temperature(TMP102 sensor0){
  // lit la température interne avec le tmp102
  float temp;

  // Turn sensor on to start temperature measurement.
  // Current consumtion typically ~10uA.
  sensor0.wakeup();

  // read temperature data
  //temp = sensor0.readTempF();
  temp = sensor0.readTempC();

  /*
    // Check for Alert
  alertPinState = digitalRead(ALERT_PIN); // read the Alert from pin
  alertRegisterState = sensor0.alert();   // read the Alert from register
  */

  // Place sensor in sleep mode to save power.
  // Current consumtion typically <0.5uA.
  sensor0.sleep();

  return temp;
}

String test_temp_int(TMP102 sensor0, float Tint_min, float Tint_max){
  String phrase_alerte ="";
  float temperature_int = read_temperature(sensor0); // température interne
  
  return "coucou";//phrase_alerte;
}

/*
String test_temp_int(TMP102 sensor0, float Tint_min, float Tint_max){
  String phrase_alerte ="";
  float temperature_int = read_temperature(sensor0); // température interne
  // boolean alertPinState, alertRegisterState; // si on veut utiliser la sortie alert du TMP102

  // Print temperature and alarm state
#ifdef AFFICHAGE
  Serial.print("Temperature interne : ");
  Serial.print(temperature_int);
  Serial.println(" °C");
#endif
  if (temperature_int < Tint_min){
    phrase_alerte.concat("température interne trop basse\n");
  } else if (temperature_int > Tint_max){
    phrase_alerte.concat("température interne trop haute\n");
  }
  
  return "coucou";//phrase_alerte;
}*/
