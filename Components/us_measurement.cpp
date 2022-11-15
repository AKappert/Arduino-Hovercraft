    #include "us_measurement.h"

   
   // Default constructor
    US_Measurement::US_Measurement(){
        servo_angle = 0;
        distance_measured = 0.0;
    }

    // Constructor with initialization
    US_Measurement::US_Measurement(int servo_angle, float distance_measured){
        this->servo_angle = servo_angle;
        this->distance_measured = distance_measured;
    }

    // Setter for angle
    void US_Measurement::set_servo_angle(int servo_angle){
        this->servo_angle = servo_angle;
    }

    // Setter for distance
    void US_Measurement::set_distance_measured(float distance_measured){
        this->distance_measured = distance_measured;
    }

    // Default destructor
    US_Measurement::~US_Measurement(){

    }