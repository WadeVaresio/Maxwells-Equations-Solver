#include "CoordinateHasher.h"

std::size_t CoordinateHasher::operator ()(const Coordinates &p) const {
    return ((std::hash<double>()(p.getI())
            ^ (std::hash<double>()(p.getJ()) << 1)) >> 1)
            ^ (std::hash<double>()(p.getK()) << 1);


//    std::ostringstream strStream;
//    strStream << p.getI() << p.getJ() << p.getK();
//
//    std::hash<std::string> strHash;
//    return strHash(strStream.str());
}
