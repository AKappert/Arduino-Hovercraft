//#include "Components/infrared.cpp"
//#include "Components/servo.cpp"
//#include "Components/setup.cpp"
//#include "Components/ultrasonic.cpp"
#include <Wire.h>
#include <Servo.h>
#define echoPin 3 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 13 //attach pin D3 Arduino to pin Trig of HC-SR04
#define servoPin 9
#define THRUST_FAN 6
#define LIFT_FAN 5
//#define scanner_divider 5
#define scanner_max 180
#define SERVO_INITIAL 86
#define SERVO_STRAIGHT 88

#define THRUST_FAN_MAX_SPEED 230
#define LIFT_FAN_MAX_SPEED 240
#define LIFT_FAN_MIN_SPEED 230

// --------------SERVO_STUFF--------------
#define SERVO_STEP_SIZE 5
#define SERVO_DEGREE_RANGE 180
Servo myservo;

// --------------US_STUFF--------------
#define NUM_SCANS_PER_STEP 10


// --------------IMU STUFF--------------
#define IMU_ERROR_NUM 20
#define IMU_TIMEOUT 2000

//  ======================================== VARIABLES ========================================
  
  
// Enums
enum MACHINE_STATE {SHUTDOWN, SCANNING, STARTING, CALCULATING, MOVING, PAUSING};
enum TURN_RADIUS{PETITE, SMALL, MEDIUM, LARGE, HUGE};
enum LED_TYPE {PWM, DISCRETE};
enum SERVO_STATE {STATIC, MOVING_CW, MOVING_CCW};

//  (  1 ) Divide servo's 180deg range of motion into 180 / SERVO_STEP
//  (  2 ) Create array or datatype (ie an object class) to store each angle.
//          -> min/max heaps could be a good idea
//  (  3 ) Setup servo pins
//  (  4 ) Setup servo motion
//  (  5 ) Setup timers
//  (  6 ) Setup US sensor pins
//  (  7 ) 
//  (  8 ) 
//  (  9 ) 
//  ( 10 ) 
//  ( 11 ) 
//  ======================================== SETUP ========================================
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int counter;
int largest, degree;
MACHINE_STATE status = STARTING;
const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;
//int[180] scan_heap;

struct US_measurement{
  unsigned long distance = 0;
  int degree = 0;
}; 

const int arraySize = (SERVO_DEGREE_RANGE/SERVO_STEP_SIZE) + 1;
struct US_measurement us_measurements[arraySize];
double scanFixer;

long scan_duration;
long scan_distance[NUM_SCANS_PER_STEP];

void measure_distance(int num){
    // Start with trig pin low
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Set the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    scan_duration = pulseIn(echoPin, HIGH);

    // Calculating the distance and storing into distance array.
    scan_distance[num] = scan_duration * 0.034 / 2; 
}

long return_avg_distance(){
  long d = 0;
  for(int n = 0; n < NUM_SCANS_PER_STEP; n++){
      d += scan_distance[n];
  }
  return (d / NUM_SCANS_PER_STEP);
}

void scan_and_measure(){
  int index = 0;
  for (int deg = 0; deg <= SERVO_DEGREE_RANGE; deg += SERVO_STEP_SIZE){
    myservo.write(deg);
    if (deg == 0) 
      delay(50);
    delay(40);

    // Measure distance NUM_SCANS_PER_STEP number of times
    for(int n = 0; n < NUM_SCANS_PER_STEP; n++){
      measure_distance(n);
    }
    // Average the distance values
    long avg_distance = return_avg_distance();
    // Storing the distance with the degree

    if(avg_distance > 250)      //go back to 110 if everything breaks
      us_measurements[index].distance = 0;
    else
      us_measurements[index].distance = avg_distance;
    us_measurements[index].degree = deg;
    Serial.print(us_measurements[index].distance);
    Serial.print(" ");
    Serial.print(us_measurements[index].degree);
    Serial.print (" ");
    Serial.println();
    // Increase the index
    index++;
    }
}

US_measurement findMax(US_measurement arr[], int size){
    int max = arr[0].distance;
    int deg = arr[0].degree;    
    //Loop through the array    
    for (int i = 0; i < size; i++) {     
        //Compare elements of array with max    
       if(arr[i].distance > max)
       {
           max = arr[i].distance;
           deg = arr[i].degree;    
       }    
    }
    US_measurement temp;
    temp.degree = deg;
    temp.distance = max;
    return temp;
}     

void turnServo(int deg)
{
  delay(200);
  analogWrite(LIFT_FAN, LIFT_FAN_MAX_SPEED);
  analogWrite(THRUST_FAN, THRUST_FAN_MAX_SPEED);
  if(deg > 80 && deg < 100)
  {
    delay(300); // these delays essentially stack depending on how fat the degree is
  }
  if(deg > 60 && deg < 120)
  {
    delay(300);
  }
  if(deg > 40 && deg < 140)
  {
    delay(300);
  }
  if(deg > 20 && deg < 160)
  {
    delay(300);
  }
  delay(1000);
  analogWrite(THRUST_FAN, LIFT_FAN_MIN_SPEED);
  myservo.write(SERVO_STRAIGHT);
  analogWrite(THRUST_FAN, THRUST_FAN_MAX_SPEED);
  delay(1000);
}


void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(THRUST_FAN, OUTPUT);
  pinMode(LIFT_FAN, OUTPUT);
  pinMode(servoPin, OUTPUT);
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  myservo.attach(servoPin);
  myservo.write(SERVO_INITIAL);
  status = MOVING;
  delay(1000);
  // Call this function if you need to get the IMU error values for your module
  // Serial.println("Calculating IMU error");
  // calculate_IMU_error();
  // delay(50);
  // Serial.print("After calculate_IMU_error()");
  analogWrite(THRUST_FAN, THRUST_FAN_MAX_SPEED);
  delay(500);
  analogWrite(LIFT_FAN, LIFT_FAN_MAX_SPEED);
  delay(200);
}


//  ======================================== MAIN ========================================
//  (  1 ) Check if IS_RUNNING = true.
//  (  2 ) For each step in servo's range of motion, measure distance
//  (  3 ) Store distance and servo angle into array/datatype
//  (  4 ) Start a loop that cycles through each value in the array/datatype
//  (  5 ) Average values next to each other in sizes of SERVO_CLUSTER_SIZE
//  (  6 ) Calculate what moves need to be done to move the hover craft
//  (  7 ) 
//  (  8 ) 
//  (  9 ) 
//  ( 10 ) 
//  ( 11 ) 
void loop() {;
  if(status == MOVING)
  {
    analogWrite(THRUST_FAN, THRUST_FAN_MAX_SPEED);
    myservo.write(90 + yaw);
    status = PAUSING;
  }
  if(status == SCANNING)
  {
    analogWrite(THRUST_FAN, LOW);
    analogWrite(LIFT_FAN, LIFT_FAN_MIN_SPEED);
    delay(1000);
    scan_and_measure();
    Serial.println();
    delay(1000);
    int max = us_measurements[0].distance;
    int deg = us_measurements[0].degree;    
    //Loop through the array    
    for (int i = 0; i < arraySize; i++) {     
        //Compare elements of array with max    
       if(us_measurements[i].distance > max)
       {
           max = us_measurements[i].distance;
           deg = us_measurements[i].degree;    
       }    
    }
    Serial.println(max);
    Serial.println(deg);
    //myservo.write(deg);
    turnServo(deg);

  } 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if(distance < 20)
    status = SCANNING;
  else
  {
    status = MOVING;
    scanFixer++;
  }
  if(((int)scanFixer % 300) == 0)
    status = SCANNING;
}


// void calculate_IMU_error() {
  
//   Serial.print("calculate_IMU_error():\t\t\tTOP");
//   while (c < IMU_ERROR_NUM) {
//     Serial.print("calculate_IMU_error():\t\t\tPRE beginTransmission. 1");
//     Wire.beginTransmission(MPU);
//     Wire.write(0x3B);
//     delay(5);
//     Wire.endTransmission(false);
//     delay(5);
//     Serial.print("calculate_IMU_error():\t\t\tpost endTransmission(false). 1");
//     Wire.requestFrom(MPU, 6, true);
//     AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
//     AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
//     AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
//     // Sum all readings
//     AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
//     AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
//     c++;
//   }

  
//   c = 0;
//   // Read gyro values 200 times
//   while (c < IMU_ERROR_NUM) {
    
//     Serial.print("calculate_IMU_error():\t\t\tPRE beginTransmission. 2");
//     Wire.beginTransmission(MPU);
//     Wire.write(0x43);
//     delay(5);
//     Wire.endTransmission(false);
//     delay(5);
//     Serial.print("calculate_IMU_error():\t\t\tpost endTransmission(false). 2");
//     Wire.requestFrom(MPU, 6, true);
//     GyroX = Wire.read() << 8 | Wire.read();
//     GyroY = Wire.read() << 8 | Wire.read();
//     GyroZ = Wire.read() << 8 | Wire.read();
//     // Sum all readings
//     GyroErrorX = GyroErrorX + (GyroX / 131.0);
//     GyroErrorY = GyroErrorY + (GyroY / 131.0);
//     GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
//     c++;
//   }
  
//   //Divide the sum by 200 to get the error value
//   AccErrorX = AccErrorX / IMU_ERROR_NUM;
//   AccErrorY = AccErrorY / IMU_ERROR_NUM;

  
//   //Divide the sum by 200 to get the error value
//   GyroErrorX = GyroErrorX / IMU_ERROR_NUM;
//   GyroErrorY = GyroErrorY / IMU_ERROR_NUM;
//   GyroErrorZ = GyroErrorZ / IMU_ERROR_NUM;
//   // Print the error values on the Serial Monitor
//   Serial.print("AccErrorX: ");
//   Serial.println(AccErrorX);
//   Serial.print("AccErrorY: ");
//   Serial.println(AccErrorY);
//   Serial.print("GyroErrorX: ");
//   Serial.println(GyroErrorX);
//   Serial.print("GyroErrorY: ");
//   Serial.println(GyroErrorY);
//   Serial.print("GyroErrorZ: ");
//   Serial.println(GyroErrorZ);
// }


//setup functionality

  // Wire.begin();                      // Initialize comunication
  // #if defined(IMU_TIMEOUT)
  //   Wire.setWireTimeout(IMU_TIMEOUT, true );
  // #endif
  // Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  // Wire.write(0x6B);                  // Talk to the register 6B
  // Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  // Wire.endTransmission(true);        //end the transmission
  
  // // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  // Wire.beginTransmission(MPU);
  // Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  // Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  // Wire.endTransmission(true);
  // // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
  // Wire.beginTransmission(MPU);
  // Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
  // Wire.write(0x10);                   // Set the register bits as 00010000 (1000deg/s full scale)
  // Wire.endTransmission(true);
  // delay(20);


// loop functionality
  // Wire.beginTransmission(MPU);
  // Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  // Wire.endTransmission(false);
  // Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  // //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  // AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  // AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  // AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
  // // Calculating Roll and Pitch from the accelerometer data
  // //accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - AccErrorX; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
  // //accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + AccErrorY; // AccErrorY ~(-1.58)
  // // === Read gyroscope data === //
  // previousTime = currentTime;        // Previous time is stored before the actual time read
  // currentTime = millis();            // Current time actual time read
  // elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  // Wire.beginTransmission(MPU);
  // Wire.write(0x43); // Gyro data first register address 0x43
  // Wire.endTransmission(false);
  // Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  // GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  // GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  // GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  // // Correct the outputs with the calculated error values
  // //GyroX = GyroX + GyroErrorX; // GyroErrorX ~(-0.56)
  // //GyroY = GyroY - GyroErrorY; // GyroErrorY ~(2)
  // //GyroZ = GyroZ + GyroErrorZ; // GyroErrorZ ~ (-0.8)
  // // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  // gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
  // gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  // yaw =  GyroZ;//yaw + GyroZ * elapsedTime;
  // // Complementary filter - combine acceleromter and gyro angle values
  // roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  // pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
  
  // // Print the values on the serial monitor
  // Serial.print(roll);
  // Serial.print("/");
  // Serial.print(pitch);
  // Serial.print("/");
  // Serial.println(yaw);
