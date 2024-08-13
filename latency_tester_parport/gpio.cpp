#include "main.h"
#include "gpio.h"

#include <stdio.h>
#include <iostream>

using namespace std;

GPIOHandler::GPIOHandler()
{

}

//GPIOHandler::read()
//{
//
//}

void GPIOHandler::reset()
{
    bright_time = 0;
    bright_time_2 = 0;
    state_click = 0;
    state_bright = 0;
    state_bright_2 = 0;
}

void GPIOHandler::cleanup()
{

}

void GPIOHandler::trigger_click()
{
	//cout << "click" << endl;
	state_click = 1;
	click_time = get_micros();
}

void GPIOHandler::trigger_bright()
{
	//cout << "bright" << endl;
	state_bright = 1;
	bright_time = get_micros();
}

void GPIOHandler::trigger_bright_2()
{
	//cout << "bright2" << endl;
	state_bright_2 = 1;
	bright_time_2 = get_micros();
}
