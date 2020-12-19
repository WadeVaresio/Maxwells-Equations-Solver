#ifndef _POINTMANAGER_H
#define _POINTMANAGER_H

#include <unordered_map>
#include <cmath>
#include <string>
#include <fstream>
#include <utility>

#include "Point.h"
#include "CoordinateHasher.h"
#include "Coordinates.h"

using std::pair;

class PointManager {
public:
    typedef std::unordered_map<Coordinates, Point*, CoordinateHasher> PointCollection;

    /**
     * Constructor based on the number of points per dimension and axis bounds
     *
     * @param pointsPerDim The number of points to generate per dimension. For example 5 corresponds to a 5x5x5 cube of points
     * @param startBound Starting point to generate points for each axis (inclusively)
     * @param endBound Ending point to generate points for each axis (inclusively)
     */
    PointManager(int pointsPerDim, double startBound, double endBound, std::string* initialVoltagePath);

    /**
     * Constructor based on point spacing and starting/ending bounds
     *
     * @param spacingDelta Spacing between each generated point
     * @param startBound Starting point to generate points for each axis (inclusively)
     * @param endBound Ending point to generate points for each axis (inclusively)
     */
    PointManager(double spacingDelta, double startBound, double endBound);

    /**
     * PointManager Destructor.
     * Responsible for releasing the collection of points held on the heap
     */
    ~PointManager();

    /**
     * Get the difference between the bounds on each axis
     *
     * @return Difference between the bounds for generated points on each axis
     */
    double getBoundsDiff() const;

    /**
     * Calculate the spacing delta based on number of total points and the difference between the bounds
     *
     * @return Spacing delta between generated points
     */
    double calculateSpacingDelta();

    /**
     * Get the total number of points managed
     *
     * @return Total number of points managed
     */
    int getTotalNumberPoints() const;

    /**
     * Get the spacing delta between generated points
     *
     * @return Spacing delta between points
     */
    double getSpacingDelta() const;

    /**
     * Get point based on coordinates
     *
     * @param target Coordinates class representing the point's coordinates
     * @return Point object if coordinates are valid, otherwise a point with all coordinates of -1000 is returned
     */
    Point* getPoint(Coordinates target) const;

    /**
     * Get a reference to a point at the given coordinates
     *
     * @param target Coordinates of the Point to get
     * @return Reference to the point at the given coordinates
     */
    Point* getPointerToPoint(const Coordinates &target);

    /**
     * Get the next neighbor from a given point in the i direction
     *
     * @param pt Coordinates representing the point coordinates to get the next neighbor in the i direction
     * @return Reference to next neighbor from this point in the i direction
     */
    Point* getNextINeighbor(const Coordinates &pt);

    /**
     * Get the previous neighbor from a given point in the i direction
     *
     * @param pt Coordinates representing the point coordinates to get the previous neighbor in the i direction
     * @return Reference to previous neighbor from this point in the i direction
     */
    Point* getPrevINeighbor(const Coordinates &pt);

    /**
     * Get the next neighbor from a given point in the j direction
     *
     * @param pt Coordinates representing the point coordinates to get the next neighbor in the j direction
     * @return Reference to next neighbor from this point in the j direction
     */
    Point* getNextJNeighbor(const Coordinates &pt);

    /**
     * Get the previous neighbor from a given point in the j direction
     *
     * @param pt Coordinates representing the point coordinates to get the previous neighbor in the j direction
     * @return Reference to previous neighbor from this point in the j direction
     */
    Point* getPrevJNeighbor(const Coordinates &pt);

    /**
     * Get the next neighbor from a given point in the k direction
     *
     * @param pt Coordinates representing the point coordinates to get the next neighbor in the k direction
     * @return Reference to next neighbor from this point in the k direction
     */
    Point* getNextKNeighbor(const Coordinates &pt);

    /**
     * Get the previous neighbor from a given point in the k direction
     *
     * @param pt PointTuple representing the point coordinates to get the previous neighbor in the k direction
     * @return Previous neighbor from this point in the k direction
     */
    Point* getPrevKNeighbor(const Coordinates &pt);

    /**
     * Set the voltage at a specific point if the point exists
     *
     * @param target Tuple containing the coordinates of the point to set the voltage at
     * @param voltage Voltage to set the point at
     */
    void setVoltage(Coordinates target, double voltage);

    /**
     * Set the conductivity of a given point
     *
     * @param target Tuple containing the coordinates of the point to set the conductivity at
     * @param conductivity Conductivity value to set the point's conductivity to
     */
    void setConductivity(Coordinates target, double conductivity);

    /**
     * Get the conductivity value at a given point
     *
     * @param target Point to obtain the corresponding conductivity
     * @return Conductivity at the point, if the point does not exist a -1 is returned.
     */
    double getConductivity(Coordinates target);

    /**
     * Get a pointer to the hashmap containing all generated points
     *
     * @return Pointer to the hashmap containing all generated points
     */
    PointCollection* getCollectionOfPoints();

    /**
     * Check if a point exists
     *
     * @param target Reference to the Point to check for
     * @return Boolean representing if the point exists in the set
     */
    bool checkPointExists(const Coordinates &target) const;

    /**
     * Get the voltage of a given point
     *
     * @param target Tuple representing the coordinates of the point to obtain the voltage of
     * @return Voltage at the target point
     */
    double getVoltage(Coordinates target);

    /**
     * Log each points corresponding conductivity and voltage to a file.
     * Each line represents a point with data in the form of: i j k conductivity voltage
     *
     * @param filePath File path to write the data to
     */
    void logConductivityAndVoltageToFile(const std::string &filePath);

    /**
     * Log each point and corresponding electric field components to a file.
     * Each line has the form (please note the curly braces are absent from each line and are present for documentation purposes:
     * {Point i comp.} {Point j comp.} {Point k comp.} {E. Field i comp.} {E. Field j comp.} {E. Field k comp.}
     *
     * @param filePath Path to the file where to save the logged electric fields
     * @param time Associated time of the electric field to log
     */
    void logElectricFieldToFile(const std::string &filePath, double time);

    /**
     *
     * @param target Point to get the magnetic field at
     * @param time Associated time of the magnetic field
     * @return Pointer to the magnetic field vector, nullptr is returned if the magnetic field does not exist at the given time/point
     */
    FieldVector *getMagneticField(Coordinates target, double time);

    /**
     * Get the electric at a point at a given time
     *
     * @param target Const qualified reference to the coordinates of the point
     * @param time Time to get the electric field at
     * @return Electric field at the point at a given time, nullptr if the electric field does not exist
     */
    FieldVector* getElectricField(const Coordinates &target, double time);

    /**
     * Get the current field at a given time
     *
     * @param target Const qualified reference to the coordinates of the point
     * @param time Time to get the current field at
     * @return Current field at the point at a given time, nullptr if the current field does not exist
     */
    FieldVector* getCurrentField(const Coordinates &target, double time);

    /**
     * Log each point and corresponding magnetic field components to a file.
     * Each line has the form (please note the curly braces are absent from each line and are present for documentation purposes:
     * {Point i comp.} {Point j comp.} {Point k comp.} {M. Field i comp.} {M. Field j comp.} {M. Field k comp.}
     *
     * @param filePath Path to the file where to save the logged magnetic fields
     * @param time Associated time of the magnetic field to log
     */
    void logMagneticFieldToFile(const std::string &filePath, double time);

    /**
     * Log each point and corresponding current field components to a file.
     * Each line has the form (please note the curly braces are absent from each line and are present for documentation purposes:
     * {Point i comp.} {Point j comp.} {Point k comp.} {M. Field i comp.} {M. Field j comp.} {M. Field k comp.}
     *
     * @param filePath Path to the file where to save the logged current fields
     * @param time Associated time of the current field to log
     */
    void logCurrentFieldToFile(const std::string &filePath, double time);


    /**
     * Get the corresponding classification of a point
     *
     * @param target Reference to the Cooridinates object containing the points coordinates
     * @return Point's classification, see Point Classification enum for all possibilities
     */
    Point::Classification getClassification(const Coordinates &target);

    /**
     * Get the starting bound used when generating points for each axis
     *
     * @return Starting bound on each axis
     */
    double getStartBound();

    /**
     * Get the ending bound used when generating points for each axis
     *
     * @return End bound on each axis
     */
    double getEndBound();

    /**
     * Get the midpoint between the starting and ending bounds used to generate points for each axis
     *
     * @return Midpoint between starting and ending points on each axis
     */
    double getMidPointBetweenBounds();

    /**
     * Check if a given point is considered to be null
     *
     * @param target Const qualified point target to check
     * @return true if target is equal to the null Point, false otherwise
     */
    bool checkIfNullPoint(const Point &target);

    /**
    * Set the current field at a given point
    *
    * @param target Coordinates of the point to set the current field at
    * @param field FieldVector containing the current field
    * @param time Time to set the current field at
    */
    void setCurrentField(const Coordinates& target, FieldVector field, double time);

    /**
     * Set the electric field at a given point
     *
     * @param target Coordinates of the point to set the electric field at
     * @param field FieldVector containing the electric field
     * @param time Time to set the electric field at
     */
    void setElectricField(const Coordinates& target, FieldVector field, double time);

    /**
     * Set the magnetic field at a given point
     *
     * @param target Point to set the magnetic field of
     * @param magneticField FieldVector representing the the magnetic field
     * @parm time Time to set the magnetic field at
     */
    void setMagneticField(const Coordinates& target, FieldVector magneticField, double time);

private:
    int numPointsPerDim, vacuumPermittivity, gaasPermittivity; //num points per dimension
    double spacingDelta, startBound, endBound;
    std::unordered_map<Coordinates, Point*, CoordinateHasher>* pointMap;
    Point *nullPoint;

    /**
     * Calculate the total number of points based on given bounds and spacing delta
     *
     * @return Calculated number of generated points based on given bounds and spacing delta
     */
    int calculateTotalNumPts() const;

    /**
     * Classify a given point based on it's position in the simulated cube.
     *
     * @param ptCoor Tuple containing the coordinates of the point
     * @return Enum entry representing the points classification.
     */
    Point::Classification classifyPoint(const Coordinates& ptCoor);

    /**
    * Generate all points within the given parameters, i.e. bounds, spacing delta, and total number of points
    */
    void generatePoints();

    /**
     * Import initial voltages from a file.
     * File format needs to be of the form: iCoordinate jCoordinate kCoordinate voltage
     *
     * @param initialVoltagePath Path to the file containing the intial voltages
     */
    void importInitialVoltages(std::string* initialVoltagePath);
};

#endif //QUANTUM_FOUNDRY_POINTMANAGER_H
