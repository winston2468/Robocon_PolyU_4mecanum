#include "library/robot.h"

class Motor {
    public:
        void init();
        void update(int motor1, int motor2, int motor3, int motor4);
        void manual();
        int getMaxPVelocity();
        Motor();
    private:
        static const int maxPVelocity = 8700;
};





