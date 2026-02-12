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
    //cerr << "before tearing detector" << endl;
    string result_string = exec("su latency -c 'python3.10 tearing_detector.py'");
    //cerr << "after tearing detector" << endl;
    //cerr << "tearing:" << result_string << endl;
    return stod(result_string);
}

void CameraHandler::storeTearingData(int id)
{
	//cerr << "before mv command" << endl;
	string command = "su latency -c 'mv tearing_capture_line.png tearing_img_tmp/" + to_string(id) + ".png'";
	//cerr << "after mv command" << endl;
	exec(command.c_str());
}
