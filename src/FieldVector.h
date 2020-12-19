#ifndef QUANTUMFOUNDRY_FIELDVECTOR_H
#define QUANTUMFOUNDRY_FIELDVECTOR_H

#include <ostream>

//TODO extract all member functions to FieldVector.cpp but mark as inline for efficiency
/**
 * Class FieldVector is responsible for holding the components of a vector for a given field (i.e electric, magnetic, current)
 * All of the member functions have been inlined to support faster execution time when accessing/setting components of the FieldVector
 */
class FieldVector {
public:
    /**
     * Construct a default FieldVector, all vector components are set to 0
     */
    FieldVector() : i(0.0), j(0.00), k(0.0) {}

    /**
     * Construct a FieldVector with given components
     *
     * @param iComp I component of the FieldVector
     * @param jComp J component of the FieldVector
     * @param kComp K component of the FieldVector
     */
    FieldVector(double iComp, double jComp, double kComp) : i(iComp), j(jComp), k(kComp) {}

    /**
     * Get the I component of the FieldVector
     *
     * @return I component of the FieldVector
     */
    inline double getIComp() const { return this->i; }

    /**
     * Get the J component of the FieldVector
     *
     * @return J component of the FieldVector
     */
    inline double getJComp() const { return this->j; }

    /**
     * Get the K component of the FieldVector
     *
     * @return K component of the FieldVector
     */
    inline double getKComp() const { return this->k; }

    /**
     * Overload the << operator to easily log the contents of the FieldVector.
     * The FieldVector is output to the outputstream in the following format: "iComponent jComponent kComponent"
     *
     * @param outs Reference to the output stream to log the FieldVector to
     * @param target FieldVector to log contents of into the outputstream
     * @return Reference to the output stream in order to support outputstream chaining
     */
    friend std::ostream &operator<<(std::ostream &outs, const FieldVector &target) {
        outs << target.i << " " << target.j << " " << target.k;

        return outs;
    }

    /**
     * Overload the + operator, to easily support addition of Field Vectors
     *
     * @param lhs const reference to the FieldVector on the left hand side of the + operator
     * @param rhs const reference to the FieldVector on the right hand side of the + operator
     * @return FieldVector containing the results of the addition operation
     */
    friend FieldVector operator+(const FieldVector &lhs, const FieldVector &rhs) {
        double iComp = lhs.i + rhs.i;
        double jComp = lhs.j + rhs.j;
        double kComp = lhs.k + rhs.k;

        return FieldVector(iComp, jComp, kComp);
    }

    inline friend FieldVector operator*(double scalar, const FieldVector &rhs) {
        return FieldVector(scalar * rhs.i, scalar * rhs.j, scalar * rhs.k);
    }

    inline friend FieldVector operator*(const FieldVector &lhs, double scalar){
        return scalar * lhs;
    }

    friend FieldVector operator-(const FieldVector &lhs, const FieldVector &rhs){
        return lhs + (-1.0 * rhs);
    }

    FieldVector& operator=(const FieldVector& rhs){
        if(this == &rhs)
            return *this;

        this->i = rhs.i;
        this->j = rhs.j;
        this->k = rhs.k;

        return *this;
    }

    inline friend bool operator==(const FieldVector &lhs, const FieldVector &rhs){
        return (lhs.i == rhs.i) && (lhs.j == rhs.j) && (lhs.k == rhs.k);
    }

    inline friend bool operator!=(const FieldVector &lhs, const FieldVector &rhs){
        return !(lhs == rhs);
    }

private:
    double i, j, k;
};

#endif //QUANTUMFOUNDRY_FIELDVECTOR_H
