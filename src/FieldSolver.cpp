#include "FieldSolver.h"

FieldSolver::FieldSolver(){
    this->pm = nullptr;
    this->timeStep = 0.0;
    this->currentTime = 0.0;
    this->initEFieldCalculated = false;
    this->drudeScatteringTime = 0.01;
}

FieldSolver::FieldSolver(PointManager *pm, double timeStep, double drudeScatteringTime) {
    this->pm = pm;
    this->timeStep = timeStep;
    this->currentTime = 0.0;
    this->initEFieldCalculated = false;
    this->drudeScatteringTime = drudeScatteringTime;
}

// TODO change to use point reference instead of const coordinate reference
FieldVector FieldSolver::calculateCurl(FieldSolver::Field field, const Coordinates &target, double time) {
    FieldVector nextINeighborField, prevINeighborField, nextJNeighborField, prevJNeighborField, nextKNeighborField, prevKNeighborField;

    switch(field){
        case ElectricField: {
            auto initEField = *pm->getElectricField(target, time);
            prevINeighborField = initEField;
            nextINeighborField = initEField;

            prevJNeighborField = initEField;
            nextJNeighborField = initEField;

            prevKNeighborField = initEField;
            nextKNeighborField = initEField;

            if (!pm->getNextINeighbor(target))
                nextINeighborField = *pm->getNextINeighbor(target)->getElectricField(time);

            if(!pm->getPrevINeighbor(target))
                prevINeighborField = *pm->getPrevINeighbor(target)->getElectricField(time);

            if(!pm->getNextJNeighbor(target))
                nextJNeighborField = *pm->getNextJNeighbor(target)->getElectricField(time);

            if(!pm->getPrevJNeighbor(target))
                prevJNeighborField = *pm->getPrevJNeighbor(target)->getElectricField(time);

            if(!pm->getNextKNeighbor(target))
                nextKNeighborField = *pm->getNextKNeighbor(target)->getElectricField(time);

            if(!pm->getPrevKNeighbor(target))
                prevKNeighborField = *pm->getPrevKNeighbor(target)->getElectricField(time);

            break;
        }
        case MagneticField: {
            auto initBField = *pm->getMagneticField(target, time);

            nextINeighborField = initBField;
            prevINeighborField = initBField;

            nextJNeighborField = initBField;
            prevJNeighborField = initBField;

            nextKNeighborField = initBField;
            prevKNeighborField = initBField;

            if(!pm->getNextINeighbor(target))
                nextINeighborField = *pm->getNextINeighbor(target)->getMagneticField(time);

            if(!pm->getPrevINeighbor(target))
                prevINeighborField = *pm->getPrevINeighbor(target)->getMagneticField(time);

            if(!pm->getNextJNeighbor(target))
                nextJNeighborField = *pm->getNextJNeighbor(target)->getMagneticField(time);

            if(!pm->getPrevJNeighbor(target))
                prevJNeighborField = *pm->getPrevJNeighbor(target)->getMagneticField(time);

            if(!pm->getNextKNeighbor(target))
                nextKNeighborField = *pm->getNextKNeighbor(target)->getMagneticField(time);

            if(!pm->getPrevKNeighbor(target))
                prevKNeighborField = *pm->getPrevJNeighbor(target)->getMagneticField(time);

            break;
        }
        case RK4Y1: {
            auto initField = pm->getPointerToPoint(target)->getRk4Y1();

            nextINeighborField = initField;
            prevINeighborField = initField;

            nextJNeighborField = initField;
            prevJNeighborField = initField;

            nextKNeighborField = initField;
            prevJNeighborField = initField;

            if(!pm->getNextINeighbor(target))
                nextINeighborField = pm->getNextINeighbor(target)->getRk4Y1();

            if(!pm->getPrevINeighbor(target))
                prevINeighborField = pm->getPrevINeighbor(target)->getRk4Y1();

            if(!pm->getNextJNeighbor(target))
                nextJNeighborField = pm->getNextJNeighbor(target)->getRk4Y1();

            if(!pm->getPrevJNeighbor(target))
                prevJNeighborField = pm->getPrevJNeighbor(target)->getRk4Y1();

            if(!pm->getNextKNeighbor(target))
                nextKNeighborField = pm->getNextKNeighbor(target)->getRk4Y1();

            if(!pm->getPrevKNeighbor(target))
                prevKNeighborField = pm->getPrevKNeighbor(target)->getRk4Y1();

            break;
        }
        case RK4Y2: {
            auto initField = pm->getPointerToPoint(target)->getRk4Y2();

            nextINeighborField = initField;
            prevINeighborField = initField;

            nextJNeighborField = initField;
            prevJNeighborField = initField;

            nextKNeighborField = initField;
            prevJNeighborField = initField;

            if(!pm->getNextINeighbor(target))
                nextINeighborField = pm->getNextINeighbor(target)->getRk4Y2();

            if(!pm->getPrevINeighbor(target))
                prevINeighborField = pm->getPrevINeighbor(target)->getRk4Y2();

            if(!pm->getNextJNeighbor(target))
                nextJNeighborField = pm->getNextJNeighbor(target)->getRk4Y2();

            if(!pm->getPrevJNeighbor(target))
                prevJNeighborField = pm->getPrevJNeighbor(target)->getRk4Y2();

            if(!pm->getNextKNeighbor(target))
                nextKNeighborField = pm->getNextKNeighbor(target)->getRk4Y2();

            if(!pm->getPrevKNeighbor(target))
                prevKNeighborField = pm->getPrevKNeighbor(target)->getRk4Y2();

            break;
        }case RK4Y3: {
            auto initField = pm->getPointerToPoint(target)->getRk4Y3();

            nextINeighborField = initField;
            prevINeighborField = initField;

            nextJNeighborField = initField;
            prevJNeighborField = initField;

            nextKNeighborField = initField;
            prevJNeighborField = initField;

            if (!pm->getNextINeighbor(target))
                nextINeighborField = pm->getNextINeighbor(target)->getRk4Y3();

            if (!pm->getPrevINeighbor(target))
                prevINeighborField = pm->getPrevINeighbor(target)->getRk4Y3();

            if (!pm->getNextJNeighbor(target))
                nextJNeighborField = pm->getNextJNeighbor(target)->getRk4Y3();

            if (!pm->getPrevJNeighbor(target))
                prevJNeighborField = pm->getPrevJNeighbor(target)->getRk4Y3();

            if (!pm->getNextKNeighbor(target))
                nextKNeighborField = pm->getNextKNeighbor(target)->getRk4Y3();

            if (!pm->getPrevKNeighbor(target))
                prevKNeighborField = pm->getPrevKNeighbor(target)->getRk4Y3();

            break;
        }
        default:
            return FieldVector(INT_MIN, INT_MIN, INT_MIN);
    }

    double i = nextJNeighborField.getKComp() -
               prevJNeighborField.getKComp() -
               nextKNeighborField.getJComp() +
               prevKNeighborField.getJComp();

    double j = nextKNeighborField.getIComp() -
               prevKNeighborField.getIComp() -
               nextINeighborField.getKComp() +
               prevINeighborField.getKComp();

    double k = nextINeighborField.getJComp() -
               prevINeighborField.getJComp() -
               nextJNeighborField.getIComp() +
               prevJNeighborField.getIComp();

    double denom = 2 * pm->getSpacingDelta();

    return FieldVector(i / denom, j / denom, k / denom);
}

void FieldSolver::calculateAndSetInitialElectricField() {
    if(initEFieldCalculated)
        throw std::invalid_argument("The initial electric field was already calculated");

    double spacingDelta = this->pm->getSpacingDelta();
    double gradientDenom = 2 * spacingDelta;

    for (const auto &p : *(pm->getCollectionOfPoints())) {
        double iComp = pm->getPrevINeighbor(p.first)->getVoltage() - pm->getNextINeighbor(p.first)->getVoltage();
        double jComp = pm->getPrevJNeighbor(p.first)->getVoltage() - pm->getNextJNeighbor(p.first)->getVoltage();
        double kComp = pm->getPrevKNeighbor(p.first)->getVoltage() - pm->getNextKNeighbor(p.first)->getVoltage();

        // Set the pointer reference's electric field instead of p.second which is only a reference
        FieldVector result = (1 / gradientDenom) * FieldVector(iComp, jComp, kComp);
        pm->getPointerToPoint(p.first)->setElectricField(result, 0);
    }
    this->initEFieldCalculated = true;
}

void FieldSolver::calculateNextFields() {
    double newTime = getNextTime();

    // calculate and set next magnetic field for all points
    calculateNextMagneticField(currentTime);
    calculateNextElectricField(currentTime);
    calculateNextCurrentField(currentTime);

    this->currentTime = newTime;
}

inline double FieldSolver::getNextTime() const { return currentTime + timeStep;}

void FieldSolver::calculateNextMagneticField(double time) {
    FieldVector zeroVector = FieldVector(0, 0 ,0);

    for(auto &p : *pm->getCollectionOfPoints()){
        FieldVector k1 = calculateCurl(Field::ElectricField, p.first, time);
        if(k1 != zeroVector)
            k1 = -1.0 * k1;

        FieldVector y1 = *p.second->getElectricField(time) + (k1 * (timeStep / 2));

        // set k1 and y1
        p.second->setRk4K1(k1);
        p.second->setRk4Y1(y1);
    }

    for(auto &p: *pm->getCollectionOfPoints()){
        FieldVector k2 = calculateCurl(Field::RK4Y1, p.first);
        if(k2 != zeroVector)
            k2 = -1.0 * k2;

        FieldVector y2 = *p.second->getElectricField(time) + (k2 * (timeStep / 2));

        // set k2 and y2
        p.second->setRk4K2(k2);
        p.second->setRk4Y2(y2);
    }

    for(auto &p : *pm->getCollectionOfPoints()){
        FieldVector k3 = calculateCurl(Field::RK4Y2, p.first);
        if(k3 != zeroVector)
            k3 = -1.0 * k3;

        FieldVector y3 = *p.second->getElectricField(time) + (k3 * timeStep);

        // set k3 and y3
        p.second->setRk4K3(k3);
        p.second->setRk4Y3(y3);
    }

    for(auto &p: *pm->getCollectionOfPoints()){
        FieldVector k4 = calculateCurl(Field::RK4Y3, p.first);
        if(k4 != zeroVector)
            k4 = -1.0 * k4;

        // set k4
        p.second->setRk4K4(k4);
    }

    double nextTime = getNextTime();

    for(auto &p : *pm->getCollectionOfPoints()){
        FieldVector weightedAvg = (timeStep / 6) * (p.second->getRk4K1() +
                (2 * p.second->getRk4K2()) + (2 * p.second->getRk4K3()) + p.second->getRk4K4());

        FieldVector result = *p.second->getMagneticField(time) + weightedAvg;
        p.second->setMagneticField(result, nextTime);
    }
}

void FieldSolver::calculateNextElectricField(double time) {
    double intermediateTimeStep = timeStep / 2;
    double nextTime = getNextTime();
    FieldVector zeroVector = FieldVector(0,0,0);

    for(auto &p : *pm->getCollectionOfPoints()){
//        if(p.second->getConductivity() > 0) // ignore points on the device
//            p.second->setElectricField(zeroVector, nextTime);

        double scalar = SPEED_OF_LIGHT_SQUARED / p.second->getPermittivity();
        FieldVector initEField = *p.second->getElectricField(time);

        FieldVector k1 = scalar * (calculateCurl(Field::MagneticField, p.first, time)
                - VACUUM_PERMEABILITY * *p.second->getCurrentField(time));

        FieldVector y1 = initEField + (k1 * (intermediateTimeStep));

        FieldVector k2 = scalar * y1;
        FieldVector y2 = initEField + (k2 * intermediateTimeStep);

        FieldVector k3 = scalar * y2;
        FieldVector y3 = initEField + (k3 * timeStep);

        FieldVector k4 = scalar * y3;

        FieldVector result = initEField + ((timeStep / 6) * (k1 + (2 * k2) + (2 * k3) + k4));
        p.second->setElectricField( result, nextTime);
    }
}

void FieldSolver::calculateNextCurrentField(double time){
    double scalar = 1 / DRUDE_SCATTERING_TIME;
    double intermediateTimeStep = timeStep / 2;
    double nextTime = getNextTime();
    FieldVector zeroVector= FieldVector(0, 0, 0);

    for(auto &p : *pm->getCollectionOfPoints()){ // loop through all points and calculate k1 and y1
        if(p.second->getConductivity() == 0) // ignore points not on the device
            p.second->setCurrentField(zeroVector, nextTime);

        FieldVector initJField = *p.second->getCurrentField(time);

        FieldVector k1 = scalar * (*p.second->getElectricField(time) * (p.second->getConductivity()) - *p.second->getCurrentField(time));
        FieldVector y1 = initJField + (k1 * intermediateTimeStep);

        FieldVector k2 = scalar * y1;
        FieldVector y2 = initJField + (k1 * (intermediateTimeStep));

        FieldVector k3 = scalar * y2;
        FieldVector y3 = initJField + (k3 * timeStep);

        FieldVector k4 = scalar * y3;

        FieldVector result = initJField + ((timeStep / 6) * (k1 + (2 * k2) + (2 * k3) + k4));
        p.second->setCurrentField(result, nextTime);
    }
}
