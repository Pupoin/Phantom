//
// Created by Zhang Yulei on 9/20/20.
//

#ifndef PSIM_DSTEP_H
#define PSIM_DSTEP_H

#include <string>
#include <ostream>

#include "TObject.h"
#include "TString.h"

class PCTStep : public TObject {
public:
    PCTStep() = default;

    PCTStep(const PCTStep &rhs) : TObject(rhs) {
        *this = rhs;
    }

    ~PCTStep() override = default;

    // Operators
    bool operator==(const PCTStep &rhs) const {
        return id == rhs.id &&
               X == rhs.X &&
               Y == rhs.Y &&
               Z == rhs.Z &&
               Px == rhs.Px &&
               Py == rhs.Py &&
               Pz == rhs.Pz &&
               E == rhs.E &&
               deltaE == rhs.deltaE &&
               PVName == rhs.PVName &&
               ProcessName == rhs.ProcessName;
    }

    bool operator!=(const PCTStep &rhs) const {
        return !(rhs == *this);
    }

    PCTStep &operator=(const PCTStep &rhs) {
        if (&rhs == this) { return *this; }
        id = rhs.id;
        X = rhs.X;
        Y = rhs.Y;
        Z = rhs.Z;
        Px = rhs.Px;
        Py = rhs.Py;
        Pz = rhs.Pz;
        E = rhs.E;
        deltaE = rhs.deltaE;
        PVName = rhs.PVName;
        ProcessName = rhs.ProcessName;

        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const PCTStep &step) {
        TString str(
                Form("|  %-5d  |  %8.3f, %8.3f, %8.3f  |  %10.5f, %10.5f, %10.5f, %10.5f, %10.5f  |   %-21s   %-15s  |",
                     step.id, step.X, step.Y, step.Z, step.Px, step.Py,
                     step.Pz, step.E, step.deltaE, step.PVName.data(), step.ProcessName.data())
        );
        os << str;
        return os;
    }

    // Get Methods
    int getId() const {
        return id;
    }

    double getX() const {
        return X;
    }

    double getY() const {
        return Y;
    }

    double getZ() const {
        return Z;
    }

    double getPx() const {
        return Px;
    }

    double getPy() const {
        return Py;
    }

    double getPz() const {
        return Pz;
    }

    double getE() const {
        return E;
    }

    double getDeltaE() const {
        return deltaE;
    }

    const std::string &getPVName() const {
        return PVName;
    }

    const std::string &getProcessName() const {
        return ProcessName;
    }

    // Set Methods
    void setId(int ID) {
        id = ID;
    }

    void setX(double x) {
        X = x;
    }

    void setY(double y) {
        Y = y;
    }

    void setZ(double z) {
        Z = z;
    }

    void setPx(double px) {
        Px = px;
    }

    void setPy(double py) {
        Py = py;
    }

    void setPz(double pz) {
        Pz = pz;
    }

    void setE(double e) {
        E = e;
    }

    void setDeltaE(double deltae) {
        PCTStep::deltaE = deltae;
    }

    void setPVName(const std::string &pvName) {
        PVName = pvName;
    }

    void setProcessName(const std::string &processName) {
        ProcessName = processName;
    }

private:
    int id{0};
    double X{0.};
    double Y{0.};
    double Z{0.};
    double Px{0.};
    double Py{0.};
    double Pz{0.};
    double E{0.};
    double deltaE{0.};
    std::string PVName;
    std::string ProcessName;

ClassDefOverride(PCTStep, 1);
};

#endif //PSIM_DSTEP_H
