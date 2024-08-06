#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <vector>

class InputHandler
{
    private:
        thread input_handler_thread;
        vector<void*> callbacks;
        void notify_callbacks();
        bool running;
        uint64_t click_time;

    public:
        InputHandler();
        void handle_input();
        void register_callback(void *function);
        void cleanup();
        void reset();
}

#endif
