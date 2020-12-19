#define IMPORT_INITIAL_VOLTAGES true
#define IMPORT_POINTS false
#define CALC_INIT_VOLTAGE false
#define LOG_INIT_VOLTAGE false
#define CALC_INIT_ELEC_FIELD true
#define LOG_INIT_ELEC_FIELD true
#define CALC_NEXT_FIELDS true
#define LOG_MAG_FIELD true

#define TIME_STEP 0.00125

#include "PointManager.h"
#include "InitialVoltageCalculator.h"
#include "DevicePointImporter.h"
#include "FieldSolver.h"

#include <iostream>
#include <chrono>

using namespace std;

int main() {
    cout << "Starting simulation" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

#if IMPORT_INITIAL_VOLTAGES
    cout << "Creating points and setting initial voltages" << endl;
    auto initialVoltagePath = new std::string("../ref/saved-initial-voltages/rod-initial-voltages");
    auto pointManager = new PointManager(101, 0, 100.0, initialVoltagePath);
    delete initialVoltagePath;
#else
    cout << "Creating points" << endl;
    auto pointManager = new PointManager(101, 0, 100.0, nullptr);

    for(int k = 0; k <= 100; k++){
        auto pt = Coordinates(50, 50, k);

        pointManager->setConductivity(pt, 1);
        pointManager->setVoltage(pt, 1);
    }
#endif

#if IMPORT_POINTS
    cout << "Importing points" << endl;
    DevicePointImporter dvi(pointManager, "../DataVisualization/pts.txt", 1.0);
    dvi.importPoints();
    cout << "Finished importing points" << endl;
#endif

#if CALC_INIT_VOLTAGE
    InitialVoltageCalculator ivc(pointManager);
    std::cout << "Calculating initial voltage." << std::endl;
    ivc.calculateInitialVoltage();
#endif

#if LOG_INIT_VOLTAGE
    std::cout << "Saving initial voltage." << std::endl;
    pointManager->logVoltageToFile("../out/initial-voltages"); //TODO fix file path due to CLion
#endif

    auto fs = new FieldSolver(pointManager, TIME_STEP, DRUDE_SCATTERING_TIME);

#if CALC_INIT_ELEC_FIELD
    cout << "Calculating initial electric field" << endl;
    fs->calculateAndSetInitialElectricField();
#endif

#if LOG_INIT_ELEC_FIELD
    cout << "Saving calculated electric fields." << endl;
    pointManager->logElectricFieldToFile("../out/e-field", 0); // TODO fix file path due to CLion
#endif

#if CALC_NEXT_FIELDS
    cout << "Calculating all fields at time: " << fs->getNextTime() << endl;
    fs->calculateNextFields();
#endif

#if LOG_MAG_FIELD
    cout << "Saving magnetic field" << endl;
    pointManager->logMagneticFieldToFile("../out/b-field", TIME_STEP); // TODO fix file path due to CLion
#endif

    cout << "Simulation complete" << endl;

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << endl;

    delete fs;
    delete pointManager;

    return 0;
}
