#include "main.h"
#include "camera.h"
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

CameraHandler::CameraHandler()
{

}

double CameraHandler::runTearingDetection()
{
    // todo: add exception handling
    //cout << "tearing detection" << endl;
    string result_string = exec("su latency -c 'python3.10 tearing_detector.py'");
    //cerr << "tearing:" << result_string << endl;
    return stod(result_string);
}
