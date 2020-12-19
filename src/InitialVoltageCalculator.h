#ifndef _INITIALVOLTAGECALCULATOR_H
#define _INITIALVOLTAGECALCULATOR_H

#include "PointManager.h"
#include <iostream>
#include <vector>
#include <algorithm>

/**
 * Class InitialVoltageCalculator responsible for calculating the initial voltage of all points contained
 * in a given PointManager object
 */
class InitialVoltageCalculator {
public:
    /**
     * Constructor responsible for collecting the PointManager object
     *
     * @param pm Pointer to the PointManager object
     */
    explicit InitialVoltageCalculator(PointManager *pm);

    /**
     * InitialVoltageCalculator destructor, responsible for freeing all memory used
     */
    ~InitialVoltageCalculator();

    /**
     * Loop through and calculate the initial voltage at all points until there is a convergence
     */
    void calculateInitialVoltage();

private:
    PointManager *pointManager;

    /**
     * Calculate the voltage at a given point
     *
     * @param p Reference to Coordinates object representing the coordinates of the point to calculate the voltage at
     * @return Calculated voltage at the given point
     */
    inline double calculateVoltage(const Coordinates &p);

    /**
     * Calculate the voltage over all points
     *
     */
    void calculateVoltageOverAllPoints();

    /**
     * Calculate the voltage of a point if it is considered to be an edge case.
     * A point is considered to be an edge case if it's Classification is not equal to Normal.
     * Please see Point::Classification for all possibilities
     *
     * @param p Const reference to the edge case point.
     * @return Voltage of the edge case point
     */
    inline double calculateVoltageEdgeCase(const Coordinates &p);

    /**
     * Truncate a double, if it is less than 0 the ceiling of the double is returned,
     * likewise if the double is greater than 0 the floor is returned
     *
     * @param d Double to truncate
     * @return The truncated double
     */
    inline double truncate(double d);

    /**
     * Compare 2 doubles to 3 decimal places
     *
     * @param a Double to compare with parameter b
     * @param b Double to compare with parameter a
     * @return True if the doubles agree out to 3 decimal places, false otherwise
     */
    inline bool compareTo3DecimalPlaces(double a, double b);
};


#endif //QUANTUM_FOUNDRY_INITIALVOLTAGECALCULATOR_H
