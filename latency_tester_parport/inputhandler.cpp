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
	//cout << "notify callbacks" << endl;
	int i = 0;
    for (auto& f : callbacks)
    {
	    //cout << "cb " << i++ << endl;
        f();
    }
}

void InputHandler::register_callback(function<void()>f)
{
	//cout << "register callback" << endl;
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
