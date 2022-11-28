// Servo degree 0 is all the way to the right, I.E when it cannot turn more clockwise
// Servo degree 180 is all the way to the left, I.E when it cannot turn more counter-clockwise

#include "us_manager.h"

// Default Constructor
US_Manager::US_Manager(){
    
    // Initializing this manager class will set servo to 90 deg
    frontServo.attach(SERVO_PIN);
    frontServo.write(90);
    
    // Start with current degree = 90 and state = stopped
    servo_current_degree = 90;
        
}

// Function to move servo to next step
void US_Manager::move_servo(int deg){
    frontServo.write(deg);
}

// Scan function used to turn servo in increments of SERVO_STEP_SIZE and measure 1 distance at each step
void US_Manager::scan_and_measure(){
    for (int deg = 0; deg <= SERVO_DEGREE_RANGE; deg += SERVO_STEP_SIZE){
        // Move servo and wait 4 millis
        frontServo.write(deg);
        delay(400);

        // Start with trig pin low
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);

        // Set the trigPin HIGH (ACTIVE) for 10 microseconds
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);

        // Reads the echoPin, returns the sound wave travel time in microseconds
        unsigned long scan_duration = pulseIn(ECHO_PIN, HIGH);

        // Calculating the distance.
        unsigned long scan_distance = scan_duration * 0.034 / 2; 

        // Storing the distance with the degree
        us_measurements[deg] = US_Measurement(deg, scan_distance);
    }
}

// Scan function used to turn servo in increments of SERVO_STEP_SIZE and measure "number_of_scans" distances at each step then avg. them
void US_Manager::scan_and_measure(int number_of_scans){

}

// Manually set a measurement in the array
void US_Manager::set_measurement(int index, US_Measurement m){

}

// Getters for a measurement
US_Measurement US_Manager::get_measurement_from_angle(int angle){

}

US_Measurement US_Manager::get_measurement_from_index(int index){

}

// Destructor
US_Manager::~ US_Manager (){

}
