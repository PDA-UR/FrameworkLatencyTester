#ifndef GPIO_H
#define GPIO_H

using namespace std;

class GPIO {
    private:
        int click_pin;
        int bright_pin;
        int bright_pin_2;

    public:
        bool state_click;
        bool state_bright;
        bool state_bright_2;
        uint64_t click_time;
        uint64_t bright_time;
        uint64_t bright_time_2;
        bool active; // only used by parport?
        
        GPIO();
        void cleanup();
}

#endif
