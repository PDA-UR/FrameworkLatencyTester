#include "main.h"
#include "camera.h"

using namespace std;

// source: Meritozh on GitHub: https://gist.github.com/meritozh/f0351894a2a4aa92871746bf45879157
string exec(const char* cmd) 
{
    shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while (!feof(pipe.get())) 
    {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

double runTearingDetection()
{
    // todo: add exception handling
    //cout << "tearing detection" << endl;
    string result_string = exec("su latency -c 'python3.10 tearing_detector.py'");
    //cout << result_string << endl;
    return stod(result_string);
}
