#include "inputhandler.h"
#include <vector>
#include <thread>
#include <functional>
#include <stdio.h>
#include <iostream>

using namespace std;

InputHandler::InputHandler()
{
    running = 1;
    input_handler_thread = thread(&InputHandler::handle_input, this);
}

void InputHandler::notify_callbacks()
{
    for (auto& f : callbacks)
    {
        f();
    }
}

void InputHandler::register_callback(function<void()>f)
{
    callbacks.push_back(f);
}

void InputHandler::reset()
{
    input_time = 0;
}

void InputHandler::cleanup()
{
    running = 0;
}
