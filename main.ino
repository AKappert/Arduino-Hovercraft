//#include "Components/infrared.cpp"
//#include "Components/servo.cpp"
//#include "Components/setup.cpp"
//#include "Components/ultrasonic.cpp"
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 11 //attach pin D3 Arduino to pin Trig of HC-SR04
#define servoPin 9
#define scanner_divider 5
#define scanner_max 180
#define s_array_size (scanner_max/scanner_divider)
#include <Servo.h>
//#include "scanner.h"
//  ======================================== VARIABLES ========================================
   
  // Enums
  enum MACHINE_STATE {SHUTDOWN, CALCULATING, SCANNING, MOVING, PAUSING, STARTING};
  enum LED_TYPE {PWM, DISCRETE};
  enum SERVO_STATE {STATIC, MOVING_CW, MOVING_CCW};

class Scanner
{
  public:
  int distance;
  int degree;
  Scanner()
  {
    this->distance = 0;
    this->degree = 0;
  }
  Scanner(int distance, int degree)
  {
    this->distance = distance;
    this->degree = degree;
  }
};



//  ======================================== FUNCTIONS ========================================


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
Servo myservo;
long duration, scan_duration; // variable for the duration of sound wave travel
int distance, scan_distance; // variable for the distance measurement
int counter;
Scanner scanner_array[scanner_max];
MACHINE_STATE status = STARTING;
//int[180] scan_heap;

// int largestValueInScanner(Scanner[] arr, int size) {
//     int max = arr[0].distance;
//     for(int i = 1 to i < size) {
//         if(max < arr[i].distance) {
//             max = arr[i].distance;
//         }
//     }
//     return max;
// }


void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  myservo.attach(servoPin);
  myservo.write(90);
  status = MOVING;
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
void loop() {
  if(status == MOVING)
  {
    // set thrust fan pin to high
    delay(3000);
    status = PAUSING;
  }
  
  if(status == SCANNING)
  {
    for(int i = 0; i <= 180; i+=5)
    {
      myservo.write(i);
      delay(400);

      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      // Reads the echoPin, returns the sound wave travel time in microseconds
      scan_duration = pulseIn(echoPin, HIGH);
      // Calculating the distance
      scan_distance = scan_duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
      Scanner temp = Scanner(scan_distance, i);
      scanner_array[i] = temp;
    }
  } 

  if(status = CALCULATING)
  {
    // int largest = largestValueInScanner(scanner_array, s_array_size);
  // calculate largest value in scanner
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

  if(distance < 10)
    status = SCANNING;
  else
    status = PAUSING;
}
