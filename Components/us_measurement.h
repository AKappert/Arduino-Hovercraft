// Header file for the US_Measurement

class US_Measurement {
    private:

    public:
    int servo_angle;
    float distance_measured;
    US_Measurement();
    US_Measurement(int servo_angle, float distance_measured);
    void set_servo_angle(int servo_angle);
    void set_distance_measured(float distance_measured);

    ~US_Measurement();
};