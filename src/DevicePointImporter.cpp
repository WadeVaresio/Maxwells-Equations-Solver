#include "DevicePointImporter.h"

#include <utility>


DevicePointImporter::DevicePointImporter(PointManager *pointManager, std::string inputFilePath, double voltage) {
    this->pointManager = pointManager;
    this->inputFilePath = std::move(inputFilePath);
    this->voltage = voltage;
}

void DevicePointImporter::importPoints() {
    std::ifstream inputFile(this->inputFilePath);

    if(!inputFile.is_open())
        throw std::invalid_argument("Input file path does not exist.");

    while(!inputFile.eof()){
        int i, j;
        inputFile >> i;
        inputFile >> j;

        auto x  = pointManager->getSpacingDelta();
        auto y = pointManager->getMidPointBetweenBounds();

        for(int k = 45; k <= 55; k++){
            auto pt = Coordinates {(double) i, (double) j, (double) k};
            pointManager->setConductivity(pt, 1.0);
            pointManager->setVoltage(pt, this->voltage);
        }


    }

    inputFile.close();
}