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

int read_Photoresistor(int pin ){
  int value=analogRead(pin);
  return value;
}

int initialize_Photoresistor(int pin) {
  pinMode(pin, INPUT);
  int calibrationValue = calibrate_Photoresistor(pin);
  return calibrationValue;
}

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
