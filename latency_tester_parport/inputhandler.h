#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <vector>
#include <thread>
#include <functional>

using namespace std;

class InputHandler
{
    protected:
        bool running;
        thread input_handler_thread;
        vector<function<void()>> callbacks;
        void notify_callbacks();

    public:
        uint64_t input_time;

        InputHandler();
        void handle_input();
        void register_callback(function<void()>f);
        void cleanup();
        void reset();
};

#endif
