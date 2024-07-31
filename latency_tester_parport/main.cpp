#include "main.h"
#include "parport.h"
#include "camera.h"
#include "serial.h"
#include "read_pixel.h"
#include "vblank.h"
#include "damage.h"

using namespace std;

int ITERATIONS = 100;

bool state_click = 0;
bool state_bright = 0;
bool state_bright_2 = 0;

// get current microseconds
uint64_t get_micros()
{
    using namespace chrono;
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}

// do all the cleanup before terminating
void cleanup()
{
    measuring = 0;
    close(serial_port);
    closeXShm();
    //GPIO::cleanup();
    cleanup_parport();
    fw_test_thread.join();
    //cleanup_xshm();
    measure_vblank_thread.join();
}

// make sure we clean up and print current logs when the program is killed
// log is only printed when terminated, not when interrupted
void signalHandlerInt(int sig)
{
    cleanup();
    exit(sig);
}

void signalHandlerTerm(int sig)
{
    cleanup();
    exit(sig);
}

void trigger_click(void)
{
	//cout << "click" << endl;
	state_click = 1;
	click_time = get_micros();
}

void trigger_bright(void)
{
	//cout << "bright" << endl;
	state_bright = 1;
	bright_time = get_micros();
}

void trigger_bright_2(void)
{
	//cout << "bright2" << endl;
	state_bright_2 = 1;
	bright_time_2 = get_micros();
}

int main(int argc, char** argv)
{
    signal(SIGINT, signalHandlerInt);
    signal(SIGTERM, signalHandlerTerm);

    srand(time(nullptr));

    // variables used by evdev to get input events
    int input_fd = -1;
    char* event_handle; // path to /dev/input/eventXY for the used input device

    // check command line parameters
    if(argc < 4)
    {
        cerr << "Too few arguments!" << endl
             << "Usage: latency_tester INPUT_DEVICE PROGRAM_NAME ITERATIONS" << endl
             << "INPUT_DEVICE: path to /dev/input/eventXY" << endl
             << "PROGRAM_NAME: name of program under test to appear in log files" << endl
             << "ITERATIONS: number of iterations" << endl;
        exit(SIGABRT);
    }
    else
    {
        event_handle = argv[1];
        testProgramName = argv[2];
	    ITERATIONS = atoi(argv[3]);
    }

    if(argc == 5)
    {
        xdamage_win = atoi(argv[4]);
	    use_xdamage = 1;
    }

    int iteration = 0;
    timeout.tv_sec = 0;
    timeout.tv_usec = 200000;

    // should this be after opening the serail port?
    FD_ZERO(&read_fds);
    FD_SET(serial_port, &read_fds);

    initXShm();

    // open input device
    input_fd = open(event_handle, O_RDONLY | O_NONBLOCK);

    if(input_fd == -1)
    {
        cerr << "Could not open input device " << event_handle << endl;
        exit(SIGABRT);
    }

    // Pin Setup.
    //wiringPiSetupGpio();
    //wiringPiSetup();

    // set pin as an output pin with optional initial state of HIGH
    //pinMode(click_pin, INPUT);
    //pinMode(bright_pin, INPUT);
    //pinMode(bright_pin_2, INPUT);

    //wiringPiISR(click_pin, INT_EDGE_RISING, &trigger_click); // , 10)
    //wiringPiISR(bright_pin, INT_EDGE_RISING, &trigger_bright);
    //wiringPiISR(bright_pin_2, INT_EDGE_RISING, &trigger_bright_2);

    init_parport();

    serial_port = init_serial_port();

    if (serial_port < 0)
    {
	    cleanup();
	    exit(SIGABRT);
    }

    char serial_read_buffer[256];
    int serial_read_num_bytes = 0;

    usleep(2 * 1000 * 1000);
    write(serial_port, msg_toggle, 1); 
    usleep(500 * 1000);
    write(serial_port, msg_toggle, 1);
    usleep(500 * 1000);

    write(serial_port, msg_calibrate, 1);
    //memset(&serial_read_buffer, '\0', sizeof(serial_read_buffer));

    // TODO: fix this using poll() or select()
    memset(&serial_read_buffer, '\0', sizeof(serial_read_buffer));
    serial_read_num_bytes = 0;

    usleep(2 * 1000 * 1000);

    serial_read_num_bytes = read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer));
    //cout << "read " << serial_read_num_bytes << " bytes from buffer" << endl;
    //cout << "calib: " << serial_read_buffer << endl;

    //usleep(10000);
    //while(read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer)))
    //{
    //        usleep(1000);
    //}
    //while(read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer)) > 0)
    //{
    //        cout << "still reading from port..." << serial_read_buffer << endl;
    //        usleep(100000);
    //}

    usleep(1000000);
    //ioctl(serial_port, TCFLSH, 2);
    //usleep(10000);
    int tcflush_result = tcflush(serial_port, TCIOFLUSH);
    //cout << "tcflush says " << tcflush_result << endl;
    usleep(10000);

    //while(read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer)) > 0)
    //{
    //        cout << "still reading from port..." << serial_read_buffer << endl;
    //        usleep(100000);
    //}
    
    measuring = 1;

    fw_test_thread = thread(measure_fw_latency, input_fd);

    usleep(100 * 1000);

    measure_vblank = 0;
    measure_vblank_thread = thread(get_vblanks);

    if (use_xdamage)
    {
	    measure_xdamage = 0;
	    measure_xdamage_thread = thread(get_xdamage, xdamage_win);
    }

    read_parport_thread = thread(read_partport);

    usleep(100 * 1000);

    cout << "iteration,click_time,start_time,end_time,bright_time,bright_time_2,xshm_start_time,xshm_end_time,yalmd_latency,tearing_offset,vblanks,damage" << endl;

    while(measuring)
    {
	    start_time = 0;
	    click_time = 0;
	    end_time = 0;
	    bright_time = 0;
	    bright_time_2 = 0;
	    state_click = 0;
	    state_bright = 0;
	    state_bright_2 = 0;

	    measure_vblank = 1;
	    measure_xdamage = 1;
	    
	    usleep(20000);

	    parport_active = 1;
	    write(serial_port, msg_measure, 1);

	    //cout << "measure" << endl;

	    while (start_time == 0 || click_time == 0 || end_time == 0 || bright_time == 0 || bright_time_2 == 0)
	    {
		usleep(10);
		// click
		// bright
		// bright_2
		//cout << start_time << " - " << click_time << " - " << end_time << " - " << bright_time << " - " << bright_time_2 << endl;
	    }

	    //cout << "after big while" << endl;

	    parport_active = 0;
	    usleep(20000);

	    measure_vblank = 0;
	    measure_xdamage = 0;

	    serial_read_num_bytes = 0;
	    memset(&serial_read_buffer, '\0', sizeof(serial_read_buffer));

	    usleep(300000);

	    serial_read_num_bytes = read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer));

	    int input_latency = start_time - click_time;
	    int framework_latency = end_time - start_time;
	    int display_latency = bright_time - end_time;
	    int ete_latency = bright_time - click_time;
	    int sum_latency = input_latency + framework_latency + display_latency;
	    int yalmd_latency = atoi(serial_read_buffer);

        double tearing_offset = 0.;
	//cout << bright_time - bright_time_2 << endl;
        if (bright_time_2 < bright_time)
        {
            tearing_offset = runTearingDetection();
        }

	    //cout << "return from yalmd " << serial_read_buffer << endl;
	    //cout << "click to bright1: " << (int)(bright_time - click_time) << endl;
	    //cout << "click to bright2: " << (int)(bright_time_2 - click_time) << endl;
	    //cout << "bright1 to bright2: " << (int)(bright_time_2 - bright_time) << endl;
	    //cout << "yalmd:" << yalmd_latency << endl;

	    cout << iteration << ","
		 << click_time << ","
		 << start_time << ","
		 << end_time << ","
		 << bright_time << ","
		 << bright_time_2 << ","
		 << xshm_start_time << ","
		 << xshm_end_time << ","
		 << yalmd_latency << ","
        	 << tearing_offset << ",";

		for (int i = 0; i < vsync_count; i++)
		{
			cout << vsync_time[i] << ";";
			vsync_time[i] = 0;
		}

		cout << ",";

		if (use_xdamage)
		{
			for (int i = 0; i < damage_count; i++)
			{
				cout << damage_time[i] << ";";
				damage_time[i] = 0;
			}
		}

		cout << endl;

		vsync_count = 0;
		damage_count = 0;

	    iteration++;

	    if (iteration > ITERATIONS) break;

	    usleep(200 * 1000);
    }

    cleanup();

    return 0;
}
