#define IMPORT_INITIAL_VOLTAGES true
#define CALC_INIT_VOLTAGE false
#define LOG_INIT_VOLTAGE false
#define CALC_INIT_ELEC_FIELD true
#define LOG_INIT_ELEC_FIELD false
#define LOG_CURRENT_FIELD true
#define LOG_ELECTRIC_FIELD true
#define LOG_MAGNETIC_FIELD true
#define CALC_NEXT_FIELDS true
#define CLOSE_GAP true

#define TIME_STEP 0.00125
#define END_TIME 0.00375
#define POINT_MANAGER_PARAMETERS 101, 0, 100
#define INITIAL_CONDUCTIVITY 1.0
#define INITIAL_VOLTAGE 1.0

#define IMPORT_INIT_VOLTAGE_PATH "../ref/saved-initial-voltages/half-rod-initial-voltages"
#define INIT_ELEC_FIELD_LOG_PATH "../out/e-field"
#define INIT_VOLTAGE_LOG_PATH "../out/initial-voltages"
#define CURRENT_FIELD_LOG_PATH "../out/currentField/j-field"
#define ELECTRIC_FIELD_LOG_PATH "../out/electricField/e-field"
#define MAGNETIC_FIELD_LOG_PATH "../out/magneticField/b-field"

#include <iostream>
#include <string>

#include "../src/PointManager.h"
#include "../src/Coordinates.h"
#include "../src/FieldSolver.h"
#include "../src/InitialVoltageCalculator.h"

using namespace std;


int main(){
    cout << "Test rod current flow" << endl;

#if IMPORT_INITIAL_VOLTAGES
    cout << "Creating points and setting initial voltages from: " << IMPORT_INIT_VOLTAGE_PATH << endl;
    auto initialVoltagePath = new string(IMPORT_INIT_VOLTAGE_PATH);
    auto pointManager = new PointManager(POINT_MANAGER_PARAMETERS, initialVoltagePath);
    delete initialVoltagePath;
#else
    cout << "Creating points" << endl;
    auto pointManager = new PointManager(POINT_MANAGER_PARAMETERS, nullptr);

    for(int k = 0; k <= 100; k++){
        if(k == 50) // skip over k = 50
            continue;
        auto pt = Coordinates(50, 50, k);

        pointManager->setConductivity(pt, INITIAL_CONDUCTIVITY);

        if(k < 50)
            pointManager->setVoltage(pt, 0); // i.e a wire with no current flow
        else
            pointManager->setVoltage(pt, INITIAL_VOLTAGE); // i.e a wire held at a potential
    }
#endif

#if CALC_INIT_VOLTAGE
    cout << "Calculating initial voltage" << endl;

    InitialVoltageCalculator ivc(pointManager);
    ivc.calculateInitialVoltage();
#endif

#if LOG_INIT_VOLTAGE
    cout << "Saving initial voltage to" << INIT_VOLTAGE_LOG_PATH << endl;
    pointManager->logConductivityAndVoltageToFile(INIT_VOLTAGE_LOG_PATH);
#endif

    auto fs = new FieldSolver(pointManager, TIME_STEP, DRUDE_SCATTERING_TIME);

#if CALC_INIT_ELEC_FIELD
    cout << "Calculating initial electric field" << endl;
    fs->calculateAndSetInitialElectricField();
#endif

#if LOG_INIT_ELEC_FIELD
    cout << "Saving calculated initial electric field to: " << INIT_ELEC_FIELD_LOG_PATH<< endl;
    pointManager->logElectricFieldToFile(INIT_ELEC_FIELD_LOG_PATH, 0);
#endif

#if CLOSE_GAP
    cout << "Closing gap between ends of the wire" << endl;
    auto pt = Coordinates(50, 50, 50);
    pointManager->setConductivity(pt, INITIAL_CONDUCTIVITY);
#endif

#if CALC_NEXT_FIELDS
    for(int i = 0; i <= END_TIME / TIME_STEP; i++){
        cout << "Calculating all fields at time " << fs->getNextTime() << endl;
        fs->calculateNextFields();
    }
#endif

#if LOG_CURRENT_FIELD
    cout << "Saving curent field" << endl;

    for(int i = 0; i <= END_TIME / TIME_STEP; i++){
        std::string JFieldLogPath = CURRENT_FIELD_LOG_PATH + std::to_string(TIME_STEP * i);

        pointManager->logCurrentFieldToFile(JFieldLogPath, TIME_STEP * i);
    }
#endif

#if LOG_ELECTRIC_FIELD
    cout << "Saving electric field" << endl;

    for(int i = 0; i <= END_TIME / TIME_STEP; i++){
        double time = TIME_STEP * i;
        std::string eFieldLogPath = ELECTRIC_FIELD_LOG_PATH + std::to_string(time);

        pointManager->logElectricFieldToFile(eFieldLogPath, time);
    }
#endif

#if LOG_MAGNETIC_FIELD
    cout << "Saving magnetic field" << endl;

    for(int i = 0; i <= END_TIME / TIME_STEP; i++){
        double time = TIME_STEP * i;
        std::string bFieldLogPath = MAGNETIC_FIELD_LOG_PATH + std::to_string(time);

        pointManager->logMagneticFieldToFile(bFieldLogPath, time);
    }
#endif

    delete fs;
    delete pointManager;

    return 0;
}