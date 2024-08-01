#include "main.h"
#include "camera.h"

using namespace std;

double runTearingDetection()
{
    // todo: add exception handling
    //cout << "tearing detection" << endl;
    string result_string = exec("su latency -c 'python3.10 tearing_detector.py'");
    //cout << result_string << endl;
    return stod(result_string);
}
