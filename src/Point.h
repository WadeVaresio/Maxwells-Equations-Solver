#ifndef _POINT_H
#define _POINT_H

#include <cmath>
#include <map>
#include <iostream>

#include "Coordinates.h"
#include "FieldVector.h"

/**
 * Point class is a point representation in 3 dimensional space with
 * a corresponding vector for the electric/magnetic field and current density
 */
class Point {
public:
    typedef enum {
        Normal, Top, Bottom, Corner, Side_Face, Edge, Unclassified, Null
    } Classification;

    Point();

    /**
     * Point Constructor
     *
     * @param i I coordinate of the point
     * @param j J coordinate of the point
     * @param k K coordinate of the point
     * @param edgeCase Boolean whether the point is an edge case
     * @param permittivity permittivity of the points location, defaults to 0
     */
    Point(double i, double j, double k, Classification c = Unclassified, int permittivity = 0);

    /**
     * Copy constructor, create a Point based on a pre existing Point
     *
     * @param p const reference to the Point to make a copy of
     */
    Point(const Point &p);

    /**
     * Destructor, responsible for freeing all memory used by the Point class
     */
    ~Point();

    /**
     * Overloaded assignment operator. A deep copy of all fields will be performed
     *
     * @param rhs Point object to set this object to
     * @return Reference to the object that was just set
     */
    Point& operator=(const Point &rhs);

    FieldVector *getElectricField(double time);

    double getVoltage();

    /**
     * Get the conductivity at this point
     *
     * @return Corresponding conductivity at this point
     */
    double getConductivity();

    /**
     * Set the voltage at this point
     *
     * @param voltage The voltage to set this point at
     */
    void setVoltage(double voltage);

    /**
     * Set the conductivity of this point
     */
    void setConductivity(double);

    /**
     * Set the electric field at this point a given time
     *
     * @param eField The electric field to set this point to
     * @param time Time associated with the electric field
     */
    void setElectricField(FieldVector eField, double time);

    /**
     * Set the magnetic field at a given point in time
     *
     * @param bField FieldVector representing the components of the magnetic field
     * @param time Time of the corresponding magnetic field
     */
    void setMagneticField(FieldVector bField, double time);

    /**
     * Set the current field at a given point in time
     *
     * @param jField FieldVector representing the current field at this point
     * @param time Time associated with the current field
     */
    void setCurrentField(FieldVector jField, double time);

    /**
     * Get the magnetic field at a given time
     *
     * @param time Time to get the corresponding magnetic field
     * @return Pointer to the Point::FieldVector representing the magnetic field at the given time
     */
    FieldVector* getMagneticField(double time);

    /**
     * Get the current field at a given time
     *
     * @param time Tune to get the corresponding current field
     * @return Pointer to the FieldVector representing the current field at the given time
     */
    FieldVector* getCurrentField(double time);

    /**
     * Get the points classification, i.e. whether it is is on the top/bottom face, edge, corner, etc.
     *
     * @return the points classification within the simulated cube
     */
    Point::Classification getClassification();

    /**
     * Compare whether two points are equal to one another
     *
     * @param p1 comparison point
     * @return True if all coordinates are equal, false otherwise
     */
    bool operator==(const Point &p1) const;

    /**
     * Get the coordinates of this point
     *
     * @return Point object representing the coordinates of this object
     */
    Coordinates getCoordinates() const;

    /**
     * Get the relative permittivity of this point
     *
     * @return Double representing the permittivity of the point
     */
    double getPermittivity() const;

    /**
     * Overload the >> operator to support easy output of Point coordinates
     *
     * @param outs Reference to output stream
     * @param p Point to output into the output stream
     * @return Output stream reference, to support chaining of outputs
     */
    inline friend std::ostream& operator <<(std::ostream &outs, const Point &p){
        outs << p.i << " " << p.j << " " << p.k;

        return outs;
    }

    inline const FieldVector &getRk4Y1() const {return this->rk4Y1;}

    inline void setRk4Y1(const FieldVector &rk4Y1) { this->rk4Y1 = rk4Y1;}

    inline const FieldVector &getRk4Y2() const { return this->rk4Y2;}

    inline void setRk4Y2(const FieldVector &rk4Y2) { this->rk4Y2 = rk4Y2;}

    inline const FieldVector &getRk4Y3() const {return this->rk4Y3;}

    inline void setRk4Y3(const FieldVector &rk4Y3) {this->rk4Y3 = rk4Y3;}

    inline const FieldVector &getRk4K1() const {return this->rk4K1;}

    inline void setRk4K1(const FieldVector &rk4K1) {this->rk4K1 = rk4K1;}

    inline const FieldVector &getRk4K2() const{ return this->rk4K2;}

    inline void setRk4K2(const FieldVector &rk4K2) { this->rk4K2 = rk4K2;}

    inline const FieldVector &getRk4K3() const {return this->rk4K3;}

    inline void setRk4K3(const FieldVector &rk4K3) { this->rk4K3 = rk4K3;}

    inline void setRk4K4(const FieldVector &rk4K4) { this->rk4K4 = rk4K4;}

    inline FieldVector getRk4K4() { return this->rk4K4;}
private:
    typedef std::pair<double, FieldVector> FieldMapEntry;

    double i, j, k, permittivity, voltage, conductivity;
    FieldVector rk4Y1, rk4Y2, rk4Y3;
    FieldVector rk4K1, rk4K2, rk4K3, rk4K4;
    Classification classification;
    std::map<double, FieldVector>* eField;
    std::map<double, FieldVector>* bField;
    std::map<double, FieldVector>* currentField;
};

#endif //QUANTUM_FOUNDRY_POINT_H
