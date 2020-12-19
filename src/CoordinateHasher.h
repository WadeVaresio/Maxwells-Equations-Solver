#ifndef _COORDINATEHASHER_H
#define _COORDINATEHASHER_H

#include <sstream>
#include <ostream>
#include "Point.h"

/**
 * Class responsible for hashing Coordinate objects.
 */
class CoordinateHasher {
public:
    /**
     * Overload the () operator to support the hashing of a Coordinate object
     *
     * @param Const reference to the Coordinate object to hash
     * @return Non-negative integer representing the hash of the Coordinate object
     */
    size_t operator() (const Coordinates &c) const;

};

#endif //QUANTUMFOUNDRY_COORDINATEHASHER_H
