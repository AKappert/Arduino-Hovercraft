// Header file for the US-Manager
// The US_MANAGER class is responsible for  - controlling the servo at the front
//                                          - collecting distance values from the US-sensor
//                                          - ...

#include "us_measurement.h"


// Servo steps and range
#define SERVO_STEP_SIZE 5
#define SERVO_DEGREE_RANGE 180

//ENUM to track which way direction the servo is turning
enum SERVO_STATE {STOPPED, CCW, CW};

class  US_Manager {
    private:

    // Array of measurements obtained from the US_sensor
    US_Measurement us_measurements[ int(SERVO_DEGREE_RANGE / SERVO_STEP_SIZE) ];
    US_Measurement us_measurements_old[ int(SERVO_DEGREE_RANGE / SERVO_STEP_SIZE) ];
    int servo_current_degree;
    int servo_next_degree;
    SERVO_STATE servoState;


    public:

    // Constructor
    US_Manager();

    // Function to move servo to degree "deg"
    void move_servo(int deg);

    // Scan function used to turn servo in increments of SERVO_STEP_SIZE and measure 1 distance at each step
    void scan_and_measure();

    // Scan function used to turn servo in increments of SERVO_STEP_SIZE and measure "number_of_scans" distances at each step then avg. them
    void scan_and_measure(int number_of_scans);

    // Manually set a measurement in the array
    void set_measurement(int index, US_Measurement m);

    // Getters for a measurement
    US_Measurement get_measurement_from_angle(int angle);
    US_Measurement get_measurement_from_index(int index);

    // Destructor
    ~US_Manager ();

 };
