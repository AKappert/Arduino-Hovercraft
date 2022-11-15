// Servo degree 0 is all the way to the right, I.E when it cannot turn more clockwise
// Servo degree 1800 is all the way to the left, I.E when it cannot turn more counter-clockwise


#include "us_manager.h"

// Default Constructor
US_Manager::US_Manager(){
    servo_current_degree = 0;
    servoState = STOPPED;
}

// Function to move servo to next step
void US_Manager::move_servo(int deg){

}

// Scan function used to turn servo in increments of SERVO_STEP_SIZE and measure 1 distance at each step
void US_Manager::scan_and_measure(){

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
US_Manager::~US_Manager (){

}
