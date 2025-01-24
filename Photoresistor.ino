/**
 * @file Photoresistor.ino
 * @author Hákon Hlynsson  (s225765@dtu.dk)
 * @brief file for monitoring light levels using a photoresistor and triggering an alarm if light levels deviate significantly.
 * @version 1.0
 * @date 2025-01-17
 * 
 * @copyright open source
 * 
 */

// Photoresistor 
int photopin = A1;
int calibrated_Photoresistor = 0;
int value_Photoresistor=0;
int threeshold_Photoresistor=100;
bool alarm_sound=false; 


void setup() {
  Serial.begin(9600); // Enable Serial monitor 
  calibrated_Photoresistor = initialize_Photoresistor(photopin); // Initialize and calibrate the photoresistor
}

void loop() {
    value_Photoresistor = read_Photoresistor(photopin);
    alarm_sound = check_for_change_Photoresistor(value_Photoresistor, calibrated_Photoresistor, threeshold_Photoresistor);
    
    Serial.println("alarm sound: " + String(alarm_sound) + 
                   " value_for_Photoresistor: " + String(value_Photoresistor) + 
                   " calibrated_Photoresistor: " + String(calibrated_Photoresistor));
}

/**
 * @brief read the value of the photoresistor
 * 
 * @param pin the pin to which the photoresistor is connected
 * @return int the value of the photoresistor
 */
int read_Photoresistor(int pin ){
  int value=analogRead(pin);
  return value;
}

/**
 * @brief function to initialize the photoresistor
 * 
 * @param pin the pin to which the photoresistor is connected
 * @return int the calibration value
 */
int initialize_Photoresistor(int pin) {
  pinMode(pin, INPUT);
  int calibrationValue = calibrate_Photoresistor(pin);
  return calibrationValue;
}

/**
 * @brief function to calibrate the photoresistor
 * 
 * @param pin the pin to which the photoresistor is connected
 * @return int returns the calibration value
 */

int calibrate_Photoresistor(int pin) {
  int total = 0;
  // Read the photoresistor 100 times and average the values
  for (int i = 0; i < 100; i++) {
    total += analogRead(pin);
    if (i>=1){
      total/=2;
    }
  }
  return total;
}
/**
 * @brief function to check if the photoresistor value has changed significantly
 * 
 * @param Photoresistor_value  the current value of the photoresistor
 * @param calibration calibration value for the photoresistor
 * @param threeshold the threshold for detecting significant light changes
 * @return true or false depending on if the light level change exceeds the threshold
 */
 
bool check_for_change_Photoresistor(int Photoresistor_value,int calibration,int threeshold){
  bool alarm;
  if (Photoresistor_value > ( calibration+threeshold)||Photoresistor_value < ( calibration-threeshold)){
    alarm=true;
  }
  else{
    alarm=false;
  }

  return alarm;
}
