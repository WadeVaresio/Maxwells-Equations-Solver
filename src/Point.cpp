#include "Point.h"

Point::Point(){
    this->i = 0.0;
    this->j = 0.0;
    this->k = 0.0;
}

Point::Point(double i, double j, double k, Classification c, int permittivity) {
    this->i = i;
    this->j = j;
    this->k = k;

    this->permittivity = permittivity;
    this->voltage = this->conductivity = 0.0;
    this->classification = c;

    this->eField = new std::map<double, FieldVector>();
    this->bField = new std::map<double, FieldVector>();
    this->currentField = new std::map<double, FieldVector>();

    //initialize to be the zero vector
    FieldVector zeroVector(0.0, 0.0, 0.0);

    FieldMapEntry initFieldEntry(0, zeroVector);
    this->bField->insert(initFieldEntry);
    this->currentField->insert(initFieldEntry);
}

Point::Point(const Point &p){
    this->i = p.i;
    this->j = p.j;
    this->k = p.k;
    this->permittivity = p.permittivity;
    this->voltage = p.voltage;
    this->conductivity = p.conductivity;
    this->classification = p.classification;

    // Allocate space for all field vectors
    this->eField = new std::map<double, FieldVector>();
    this->bField = new std::map<double, FieldVector>();
    this->currentField = new std::map<double, FieldVector>();

    // Deep copy of p's eField
    for(const auto &e : *(p.eField)){
        this->eField->insert(FieldMapEntry(e.first, e.second));
    }

    // Deep copy of p's bField
    for(const auto &b : *(p.bField)){
        this->bField->insert(FieldMapEntry(b.first, b.second));
    }

    // Deep copy of p's currentField
    for(const auto &jField : *(p.currentField)){
        this->currentField->insert(FieldMapEntry(jField.first, jField.second));
    }
}

Point& Point::operator=(const Point &rhs){
    if(this == &rhs) // check for self assignment
        return *this;

    // Set all member variables to rhs member variables
    this->i = rhs.i;
    this->j = rhs.j;
    this->k = rhs.k;
    this->permittivity = rhs.permittivity;
    this->voltage = rhs.voltage;
    this->conductivity = rhs.conductivity;
    this->classification = rhs.classification;

    delete this->eField;
    delete this->eField;
    delete this->currentField;

    // Clear eField, bField, currentField for new entries
    this->eField = new std::map<double, FieldVector>();
    this->bField = new std::map<double, FieldVector>();
    this->currentField = new std::map<double, FieldVector>();

    this->eField->clear();
    this->bField->clear();
    this->currentField->clear();

    // Deep copy of rhs eField
    for(const auto &e : *(rhs.eField)){
        this->eField->insert(FieldMapEntry(e.first, e.second));
    }

    // Deep copy of rhs bField
    for(const auto &b : *(rhs.bField)){
        this->bField->insert(FieldMapEntry(b.first, b.second));
    }

    // Deep copy of rhs currentField
    for(const auto &jField : *(rhs.currentField)){
        this->currentField->insert(FieldMapEntry(jField.first, jField.second));
    }

    return *this;
}

Point::~Point(){
    delete this->eField;
    delete this->bField;
    delete this->currentField;
}

double Point::getVoltage() { return this->voltage; }

double Point::getConductivity() { return this->conductivity; }

void Point::setVoltage(double voltage) { this->voltage = voltage; }

void Point::setConductivity(double conductivity) { this->conductivity = conductivity; }

void Point::setElectricField(FieldVector eField, double time) {
    this->eField->insert(FieldMapEntry(time, eField));
}

FieldVector *Point::getElectricField(double time) {
    return this->eField->find(time) == eField->end() ? nullptr : &this->eField->at(time);
}

FieldVector* Point::getCurrentField(double time){
    auto a = currentField->find(time) == currentField->end() ? nullptr : &currentField->at(time);

    return currentField->find(time) == currentField->end() ? nullptr : &currentField->at(time);
}

void Point::setMagneticField(FieldVector bField, double time) {
    this->bField->insert(FieldMapEntry(time, bField));
}

void Point::setCurrentField(FieldVector jField, double time) {
    this->currentField->insert(FieldMapEntry(time, jField));
}

FieldVector* Point::getMagneticField(double time) {
    return this->bField->find(time) == this->bField->end() ? nullptr : &this->bField->at(time);
}

Point::Classification Point::getClassification() { return this->classification; }

bool Point::operator==(const Point &p1) const {
    return this->i == p1.i && this->j == p1.j && this->k == p1.k;
}

Coordinates Point::getCoordinates() const{
    return Coordinates(i, j, k);
}

double Point::getPermittivity() const {
    return this->permittivity;
}

int operator==(const Point &p1, const Point &p2){
    auto p1Coor = p1.getCoordinates();
    auto p2Coor = p2.getCoordinates();

    return (p1Coor.getI() == p2Coor.getI()) && (p1Coor.getJ() == p2Coor.getJ()) && (p1Coor.getK() == p2Coor.getK());
}