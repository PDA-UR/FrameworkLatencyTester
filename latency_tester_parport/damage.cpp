#include "main.h"
#include "damage.h"
#include <stdio.h>
#include <iostream>

using namespace std;

DamageHandler::DamageHandler(int window_id)
{
    damage_count = 0;
    measure = 0;
    use_xdamage = 0;
    win = (Window)window_id;
    running = true;

    if (window_id != 0)
    {
    	measure_xdamage_thread = thread(&DamageHandler::get_xdamage, this);
    }
    //measure_xdamage_thread.run();
}

void DamageHandler::get_xdamage()
{
	if (!use_xdamage)
	{
		return;
	}
	//win = 0x3400007;
	//cout << hex << win << endl;

	//XSetErrorHandler(&DamageHandler::handle_xdamage_error, this);
	Display* dsp = XOpenDisplay(NULL);
	XWindowAttributes attributes = {0};
	int damage_event, damage_error, ret;
	Damage damage;

	try {
		XGetWindowAttributes(dsp, win, &attributes);
		ret = XDamageQueryExtension(dsp, &damage_event, &damage_error);

		if (ret < 1)
		{
			use_xdamage = 0;
			damage_count = 0;
			return;
		}

		damage = XDamageCreate(dsp, win, XDamageReportNonEmpty);
	}
	catch (int e) {
		use_xdamage = 0;
		return;
	}
	XEvent ev;
	//XDamageNotifyEvent* ev_dmg;

	while(running)
	{
		if (measure)
		{
			XNextEvent(dsp, &ev);
			if (ev.type == damage_event + XDamageNotify)
			{
				//cout << "damage" << endl;
				//ev_dmg = (XDamageNotifyEvent*) &ev;
				uint64_t new_damage_time = get_micros();
				damage_time[damage_count] = new_damage_time;			
				damage_count++;
				if (damage_count > 1000000)
				{
					cout << "obacht! damage_count too big" << endl;
				}
				XDamageSubtract(dsp, damage, None, None);
			}
		}
	}
	XCloseDisplay(dsp);
}

int DamageHandler::handle_xdamage_error(Display *d, XErrorEvent *e)
{
	use_xdamage = 0;
	return 0;
}

void DamageHandler::cleanup()
{
    running = false;
}
