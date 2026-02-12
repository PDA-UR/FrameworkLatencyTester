#ifndef CAMERA_H
#define CAMERA_H

class CameraHandler {
    private:

    public:
        CameraHandler();
        double runTearingDetection();
	void storeTearingData(int id);
};

#endif
