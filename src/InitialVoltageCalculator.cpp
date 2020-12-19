#include "InitialVoltageCalculator.h"

InitialVoltageCalculator::InitialVoltageCalculator(PointManager *pm) {
    this->pointManager = pm;
}

void InitialVoltageCalculator::calculateInitialVoltage() {
    calculateVoltageOverAllPoints();
}

//TODO test making this an IMF
double InitialVoltageCalculator::calculateVoltage(const Coordinates &point) {
    return (pointManager->getNextINeighbor(point)->getVoltage() +
            pointManager->getPrevINeighbor(point)->getVoltage() +
            pointManager->getNextJNeighbor(point)->getVoltage() +
            pointManager->getPrevJNeighbor(point)->getVoltage() +
            pointManager->getNextKNeighbor(point)->getVoltage() +
            pointManager->getPrevKNeighbor(point)->getVoltage()) / 6;
}

//TODO test making this an IMF
double InitialVoltageCalculator::calculateVoltageEdgeCase(const Coordinates &p) {
    Point::Classification ptClassification = pointManager->getClassification(p);  // get the classification of the point

    if (ptClassification == Point::Top) {
        auto twinBottomPt = Coordinates(p.getI(), p.getJ(), pointManager->getStartBound()); // create PointTuple of the bottom twin point coordinates
        double nextKVoltage = pointManager->getVoltage(
                twinBottomPt);  // get the voltage of the corresponding twin point at the bottom of the cube

        return (pointManager->getNextINeighbor(p)->getVoltage() +
                pointManager->getPrevINeighbor(p)->getVoltage() +
                pointManager->getNextJNeighbor(p)->getVoltage() +
                pointManager->getPrevJNeighbor(p)->getVoltage() +
                nextKVoltage +
                pointManager->getPrevKNeighbor(p)->getVoltage()) / 6;

    } else if (ptClassification == Point::Bottom) {
        auto twinTopPt = Coordinates(p.getI(), p.getJ(), pointManager->getEndBound());  // create PointTuple of the top twin point coordinates
        double prevKVoltage = pointManager->getVoltage(twinTopPt);  // get the voltage of the corresponding twin point at the top of the cube

        return (pointManager->getNextINeighbor(p)->getVoltage() +
                pointManager->getPrevINeighbor(p)->getVoltage() +
                pointManager->getNextJNeighbor(p)->getVoltage() +
                pointManager->getPrevJNeighbor(p)->getVoltage() +
                pointManager->getNextKNeighbor(p)->getVoltage() +
                prevKVoltage) / 6;
    } else { // Side face, by default, voltage is 0 if the point does not exist
        return this->calculateVoltage(p);
    }
}

//TODO test making this an IMF
void InitialVoltageCalculator::calculateVoltageOverAllPoints(){
    int nonConvergedPts = 0;
    bool converged = false;

    while(!converged){
        for(const auto &p : *pointManager->getCollectionOfPoints()){
            auto pCoordinates = p.first;

            if(this->pointManager->getConductivity(pCoordinates) > 0)  // point is an electrode, keep voltage the same
                continue;

            double voltage = pointManager->getClassification(pCoordinates) == Point::Normal ? calculateVoltage(pCoordinates) : calculateVoltageEdgeCase(pCoordinates);

            if(!compareTo3DecimalPlaces(voltage, pointManager->getVoltage(pCoordinates))){  // voltages are not equal, set new voltage and increment counter
                pointManager->setVoltage(pCoordinates, voltage);
                nonConvergedPts++;
            }
        }

        std::cout << nonConvergedPts << std::endl;

        if(nonConvergedPts == 0)
            converged = true;
        nonConvergedPts = 0;
    }
}

double InitialVoltageCalculator::truncate(double d) {
    return (d > 0) ? std::floor(d) : std::ceil(d);
}

bool InitialVoltageCalculator::compareTo3DecimalPlaces(double a, double b) {
    // Credit to: https://stackoverflow.com/questions/18686269/compare-floats-to-three-decimal-places
    return (truncate(1000. * a) == truncate(1000. * b));
}

// we dont want to accidentally delete the pointer to PointManager instance
InitialVoltageCalculator::~InitialVoltageCalculator() {}
