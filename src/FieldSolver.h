#ifndef _FIELDSOLVER_H
#define _FIELDSOLVER_H

#define SPEED_OF_LIGHT 3 *(int)1.0e8
#define SPEED_OF_LIGHT_SQUARED std::pow(SPEED_OF_LIGHT, 2)
#define VACUUM_PERMEABILITY (4 * M_PI) * 1.0e-7
#define DRUDE_SCATTERING_TIME 0.25

#include "PointManager.h"
#include "Coordinates.h"
#include "FieldVector.h"

#include <climits>
#include <stdexcept>
#include <cmath>


class FieldSolver {
public:
    typedef enum{ElectricField, MagneticField, CurrentField, RK4Y1, RK4Y2, RK4Y3} Field;

    /**
     * Default constructor, sets all member variables to default values
     */
    FieldSolver();

    /**
     * Construct a new FieldSolver object
     *
     * @param pm PointManager that contains all points for the simulation
     * @param timeStep Time step to take when calculating each field
     * @param drudeScatteringTime Drude scattering time constant
     */
    FieldSolver(PointManager *pm, double timeStep, double drudeScatteringTime);

    /**
     * Calculate the curl of a given field at a given point at a given time
     *
     * @param field The field to calculate the curl with ( del x field )
     * @param target Point to calculate the curl at
     * @param time Time to calculate the curl at
     * @return FieldVector representing the curl of the given field at the given point and at the given time
     */
    FieldVector calculateCurl(FieldSolver::Field field, const Coordinates &target, double time=0.0);

    /**
     * Calculate the initial electric field based on the calculated initial voltages for all points
     */
    void calculateAndSetInitialElectricField();

    void calculateNextFields();

    /**
     * Get the next time step that will be calculated upon the next call of calculateNextFields
     *
     * @return double representing the next time for which all fields will be calculated at
     */
    double getNextTime() const;

private:
    PointManager *pm;
    double timeStep, currentTime, drudeScatteringTime;
    bool initEFieldCalculated;

    void calculateNextMagneticField(double time);

    void calculateNextElectricField(double time);

    void calculateNextCurrentField(double time);
};

#endif //QUANTUM_FOUNDRY_FIELDSOLVER_H
