#include "PointManager.h"

PointManager::PointManager(int pointsPerDim, double startBound = -5.0, double endBound = 5.0,
                           std::string *initialVoltagePath = nullptr) {
    this->numPointsPerDim = pointsPerDim;
    this->startBound = startBound;
    this->endBound = endBound;
    this->spacingDelta = calculateSpacingDelta(); // Calculate the spacing between points

    this->vacuumPermittivity = 1; // set relative permittivity in a vacuum
    this->gaasPermittivity = 12; // set relative permittivity in gallium arsenide

    this->pointMap = new std::unordered_map<Coordinates, Point *, CoordinateHasher>();

    this->nullPoint = new Point(-1000.0, -1000.0, -1000.0, Point::Null,
                                -1); // Point representing a null value, i.e a point that does not exist
    this->nullPoint->setVoltage(0.0);

    if (initialVoltagePath != nullptr)
        importInitialVoltages(initialVoltagePath); // create necessary points and set voltages
    else
        generatePoints();
}

//TODO deprecate?
PointManager::PointManager(double spacingDelta, double startBound = -5.0, double endBound = 5.0) {
    this->spacingDelta = spacingDelta;
    this->startBound = startBound;
    this->endBound = endBound;
    this->numPointsPerDim = pow(calculateTotalNumPts(), 1.0 / 3.0);

    this->vacuumPermittivity = 1; // set relative permittivity in a vacuum
    this->gaasPermittivity = 12; // set relative permittivity in gallium arsenide

    this->nullPoint = new Point(-1000.0, -1000.0, -1000.0, Point::Null,
                                -1); // Point representing a null value, i.e a point that does not exist
    this->nullPoint->setVoltage(0.0);

    generatePoints(); // create all points to be used in the simulation
}

PointManager::~PointManager() {
    delete nullPoint;

    for(auto p : *pointMap)
        delete p.second;

    delete pointMap;
}

double PointManager::getBoundsDiff() const {
    return std::abs(endBound - startBound);
}

/**
 * Get the number of points being held by the PointManager object
 *
 * @return Number of points being held
 */
int PointManager::getTotalNumberPoints() const {
    return this->pointMap->size();
}

/**
 * Calculate the spacing delta between points based on bounds and number of points
 *
 * @param numPoints Number of total points
 * @return Spacing delta between points
 */
double PointManager::calculateSpacingDelta() {
    // Add 1 to bounds diff to account for the origin (0, 0, 0)
    return (getBoundsDiff() + 1) / (double) numPointsPerDim;
}

/**
 * Calculate the total number of points based on bounds and spacing delta
 *
 * @return Integer of the calculated number of points based on bounds and spacing delta
 */
int PointManager::calculateTotalNumPts() const {
    return (int) (getBoundsDiff() / spacingDelta);
}

/**
 * Loop through all directions and generate points with the appropriate spacing
 */
void PointManager::generatePoints() {
    //TODO is there a more efficient way of initializing the 3D space? This is O(n^3)
    double midLevel = getMidPointBetweenBounds();

    for (double i = startBound; i <= endBound; i += spacingDelta) {
        for (double j = startBound; j <= endBound; j += spacingDelta) {
            for (double k = startBound; k <= endBound; k += spacingDelta) {
                auto ptCoor = Coordinates(i, j, k);
                Point::Classification classification = classifyPoint(ptCoor);

                if (k > midLevel) { // Point is in a vacuum
                    auto entry = new Point(i, j, k, classification, vacuumPermittivity);
                    pointMap->insert(pair<Coordinates, Point *>(ptCoor, entry));
                } else if (k < midLevel) { // point is in gallium arsenide
                    auto entry = new Point(i, j, k, classification, gaasPermittivity);
                    pointMap->insert(pair<Coordinates, Point *>(ptCoor, entry));
                } else { // point is in substrate
                    auto entry = new Point(i, j, k, classification, 1);
                    pointMap->insert(pair<Coordinates, Point *>(ptCoor, entry));
                }
            }
        }
    }
}

void PointManager::importInitialVoltages(std::string *initialVoltagePath) {
    std::ifstream inputFile(*initialVoltagePath);

    if (!inputFile.is_open())
        throw std::invalid_argument("Initial voltage input file does not exist");

    double i, j, k, conductivity, voltage;
    double midLevel = getMidPointBetweenBounds();

    while (!inputFile.eof()) {
        inputFile >> i >> j >> k >> conductivity >> voltage;
        auto ptCoor = Coordinates(i, j, k);
        Point::Classification classification = classifyPoint(ptCoor);

        if (k == midLevel){
            auto entry = new Point(i, j, k, classification, 1);
            entry->setConductivity(conductivity);
            entry->setVoltage(voltage);
            pointMap->insert(pair<Coordinates, Point*>(ptCoor, entry));
        }else if (k > midLevel){
            auto entry = new Point(i, j, k, classification, vacuumPermittivity);
            entry->setConductivity(conductivity);
            entry->setVoltage(voltage);
            pointMap->insert(pair<Coordinates, Point*>(ptCoor, entry));
        }else{
            auto entry = new Point(i, j, k, classification, gaasPermittivity);
            entry->setConductivity(conductivity);
            entry->setVoltage(voltage);
            pointMap->insert(pair<Coordinates, Point*>(ptCoor, entry));
        }
    }
}

Point* PointManager::getPoint(Coordinates target) const {
    if (pointMap->find(target) == pointMap->end()) // coordinates not contained in unordered_map
        return nullPoint;
    else
        return pointMap->at(target);
}

Point* PointManager::getPointerToPoint(const Coordinates &target) {
    auto x = pointMap->find(target);

    if (x == pointMap->end())
        return nullPoint;

    return pointMap->at(target);
}

double PointManager::getSpacingDelta() const { return this->spacingDelta; }

Point* PointManager::getNextINeighbor(const Coordinates &pt) {
    return getPointerToPoint(Coordinates(pt.getI() + spacingDelta, pt.getJ(), pt.getK()));
}

Point* PointManager::getPrevINeighbor(const Coordinates &pt) {
    return this->getPointerToPoint(Coordinates(pt.getI() - spacingDelta, pt.getJ(), pt.getK()));
}

Point* PointManager::getNextJNeighbor(const Coordinates &pt) {
    return this->getPointerToPoint(Coordinates(pt.getI(), pt.getJ() + spacingDelta, pt.getK()));
}

Point* PointManager::getPrevJNeighbor(const Coordinates &pt) {
    return this->getPointerToPoint(Coordinates(pt.getI(), pt.getJ() - spacingDelta, pt.getK()));
}

Point* PointManager::getNextKNeighbor(const Coordinates &pt) {
    return this->getPointerToPoint(Coordinates(pt.getI(), pt.getJ(), pt.getK() + spacingDelta));
}

Point* PointManager::getPrevKNeighbor(const Coordinates &pt) {
    return this->getPointerToPoint(Coordinates(pt.getI(), pt.getJ(), pt.getK() - spacingDelta));
}

void PointManager::setVoltage(Coordinates target, double voltage) {
    if (this->checkPointExists(target))
        pointMap->at(target)->setVoltage(voltage);
}

void PointManager::setConductivity(Coordinates target, double conductivity) {
    if (checkPointExists(target))
        pointMap->at(target)->setConductivity(conductivity);
}

std::unordered_map<Coordinates, Point*, CoordinateHasher> *PointManager::getCollectionOfPoints() { return pointMap; }

bool PointManager::checkPointExists(const Coordinates &target) const {
    return pointMap->find(target) != pointMap->end();
}

double PointManager::getVoltage(Coordinates target) {
    return checkPointExists(target) ? getPoint(target)->getVoltage() : 0;
}

void PointManager::logConductivityAndVoltageToFile(const std::string &filePath) {
    std::fstream logFile(filePath, std::ios::out);

    if (!logFile.is_open())
        throw std::invalid_argument("File path to log voltage does not exist");

    for (auto p : *pointMap) {
        logFile << *p.second << " " << p.second->getConductivity() << " " << p.second->getVoltage() << std::endl;
    }

    logFile.close();
}

void PointManager::logElectricFieldToFile(const std::string &filePath, double time) {
    std::fstream logFile(filePath, std::ios::out);

    if (!logFile.is_open())
        throw std::invalid_argument("File path to log electric field does not exist");

    for (auto p : *pointMap) {
        auto eField = p.second->getElectricField(time);

        logFile << p.first << " " << *eField << std::endl;
    }

    logFile.close();
}

void PointManager::logMagneticFieldToFile(const std::string &filePath, double time) {
    std::fstream logFile(filePath, std::ios::out);

    if (!logFile.is_open())
        throw std::invalid_argument("File path to log magnetic field does not exist");

    for (auto p : *pointMap) {
        auto magneticField = p.second->getMagneticField(time);

        logFile << p.first << " " << *magneticField << std::endl;
    }

    logFile.close();
}

void PointManager::logCurrentFieldToFile(const std::string &filePath, double time) {
    std::fstream logFile(filePath, std::ios::out);

    if (!logFile)
        throw std::invalid_argument("File path log current field does not exist");

    for (auto p : *pointMap) {
        auto currentField = p.second->getCurrentField(time);

        logFile << p.first << " " << *currentField << std::endl;
    }

    logFile.close();
}

FieldVector *PointManager::getMagneticField(Coordinates target, double time) {
    if (!checkPointExists(target))
        return nullptr;

    return getPointerToPoint(target)->getMagneticField(time);
}

FieldVector *PointManager::getElectricField(const Coordinates &target, double time) {
    if (!checkPointExists(target))
        return nullptr;

    return getPointerToPoint(target)->getElectricField(time);
}

FieldVector *PointManager::getCurrentField(const Coordinates &target, double time) {
    if (!checkPointExists(target))
        return nullptr;

    return getPointerToPoint(target)->getCurrentField(time);
}

Point::Classification PointManager::classifyPoint(const Coordinates &ptCoor) {
    int pos = 0;

    double iCoor = ptCoor.getI();
    double jCoor = ptCoor.getJ();
    double kCoor = ptCoor.getK();

    pos += (iCoor == startBound || iCoor == endBound); // if point lies on an x axis bound
    pos += (jCoor == startBound || jCoor == endBound); // if point lies on a y axis bound
    pos += (kCoor == startBound || kCoor == endBound); // if point lies on a z axis bound

    switch (pos) {
        case 0: // point is not in a special position within the cube
            return Point::Normal;
        case 2: // point is on an edge
            return Point::Edge;
        case 3: // point is on a corner
            return Point::Corner;
        default: // need to do further work to determine the position
            break;
    }

    if (kCoor == startBound) // point is on the top face
        return Point::Bottom;
    else if (kCoor == endBound) // point is on the bottom face
        return Point::Top;
    else // point must be a side face
        return Point::Side_Face;
}

Point::Classification PointManager::getClassification(const Coordinates &target) {
    if (!checkPointExists(target)) // target point does not exist
        return Point::Unclassified;

    return this->pointMap->at(target)->getClassification();
}

double PointManager::getStartBound() { return this->startBound; }

double PointManager::getEndBound() { return this->endBound; }

double PointManager::getMidPointBetweenBounds() { return startBound + (double) (endBound - startBound) / 2; }

double PointManager::getConductivity(Coordinates target) {
    return checkPointExists(target) ? pointMap->at(target)->getConductivity() : -1;
}

bool PointManager::checkIfNullPoint(const Point &target) {
    return target == *nullPoint;
}

void PointManager::setCurrentField(const Coordinates &target, FieldVector field, double time) {
    if (!checkPointExists(target)) // point doesn't exist
        return;

    getPointerToPoint(target)->setCurrentField(field, time);
}

void PointManager::setElectricField(const Coordinates &target, FieldVector field, double time) {
    if (!checkPointExists(target)) // point doesn't exist
        return;

    getPointerToPoint(target)->setElectricField(field, time);
}

void PointManager::setMagneticField(const Coordinates &target, FieldVector magneticField, double time) {
    if (!checkPointExists(target)) // point doesn't exist
        return;

    this->getPointerToPoint(target)->setMagneticField(magneticField, time);
}
