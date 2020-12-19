#ifndef _COORDINATES_H
#define _COORDINATES_H

#include <iostream>

/**
 * Class responsible for representing Coordinates in 3D space
 * All member functions are inlined for execution efficiency
 */
class Coordinates {
public:
    /**
     * Default Coordinates constructor, sets all coordinates to 0.0
     */
    Coordinates() : i(0.0), j(0.0), k(0.0) {}

    /**
     * Constructor for given Coordinate components
     *
     * @param iCoor I coordinate
     * @param jCoor J coordinate
     * @param kCoor K coordinate
     */
    Coordinates(double iCoor, double jCoor, double kCoor) : i(iCoor), j(jCoor), k(kCoor){}

    /**
     * Overload the == operator to test for Coordinate object equality
     *
     * @param c const reference to a Coordinate object to compare to
     * @return True if all components in the two Coordinate objects agree
     */
    bool operator==(const Coordinates &c) const{
        return (this->i == c.i) && (this->j == c.j) && (this->k == c.k);
    }

    /**
     * Get the I component of the Coordinate
     *
     * @return I component of the Coordinate
     */
    double getI() const{ return i;}

    /**
     * Get the J component of the Coordinate
     *
     * @return J component of the Coordinate
     */
    double getJ() const { return j;}

    /**
     * Get the K component of the Coordinate
     *
     * @return K component of the Coordinate
     */
    double getK() const { return k;}

    friend std::ostream& operator <<(std::ostream &outs, const Coordinates &coordinates){
      outs << coordinates.i << " " << coordinates.j<< " " << coordinates.k;

      return outs;
    }

private:
    double i, j, k;

    void setI(double iCoor){ this->i = iCoor;}

    void setJ(double jCoor){ this->j = jCoor;}

    void setK(double kCoor){ this->k = kCoor;}
};


#endif //QUANTUMFOUNDRY_COORDINATES_H
