#ifndef _DEVICEPOINTIMPORTER_H
#define _DEVICEPOINTIMPORTER_H

#include <string>
#include <fstream>
#include <stdexcept>

#include "PointManager.h"


class DevicePointImporter {
public:

    DevicePointImporter(PointManager *pointManager, std::string inputFilePath, double voltage);

    void importPoints();

private:
    PointManager *pointManager;
    std::string inputFilePath;
    double voltage;
};


#endif //QUANTUMFOUNDRY_DEVICEPOINTIMPORTER_H
