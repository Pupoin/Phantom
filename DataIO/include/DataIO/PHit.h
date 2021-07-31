//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef PSIM_DHIT_H
#define PSIM_DHIT_H

#include "TObject.h"

enum DetectorType {
    nNone, nTracker, nECAL, nHCAL
};

class PHit : public TObject {
public:
    // Constructor and Destructor
    PHit() = default;

    PHit(const PHit &rhs)  : TObject(rhs) {
        *this = rhs;
    }

    ~PHit() override = default;

    bool operator==(const PHit &rhs) const {
        return id == rhs.id &&
               CellID == rhs.CellID &&
               CellID_X == rhs.CellID_X &&
               CellID_Y == rhs.CellID_Y &&
               CellID_Z == rhs.CellID_Z &&
               Detector == rhs.Detector &&
               X == rhs.X &&
               Y == rhs.Y &&
               Z == rhs.Z &&
               T == rhs.T &&
               E == rhs.E;
    }

    bool operator!=(const PHit &rhs) const {
        return !(rhs == *this);
    }

    PHit &operator=(const PHit &rhs) {
        if (&rhs == this) { return *this; }
        id = rhs.id;
        CellID = rhs.CellID;
        CellID_X = rhs.CellID_X;
        CellID_Y = rhs.CellID_Y;
        CellID_Z = rhs.CellID_Z;
        Detector = rhs.Detector;
        X = rhs.X;
        Y = rhs.Y;
        Z = rhs.Z;
        T = rhs.T;
        E = rhs.E;

        return *this;
    }

    // Get Methods
    int getId() const {
        return id;
    }

    int getCellId() const {
        return CellID;
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

    double getT() const {
        return T;
    }

    double getE() const {
        return E;
    }

    DetectorType getDetector() const {
        return Detector;
    }

    int getCellIdX() const {
        return CellID_X;
    }

    int getCellIdY() const {
        return CellID_Y;
    }

    int getCellIdZ() const {
        return CellID_Z;
    }

    // Set Methods
    void setId(int ID) {
        PHit::id = ID;
    }

    void setCellId(int cellId) {
        CellID = cellId;
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

    void setT(double t) {
        T = t;
    }

    void setE(double e) {
        E = e;
    }

    void setDetector(DetectorType detector) {
        Detector = detector;
    }

    void setCellIdX(int cellIdX) {
        CellID_X = cellIdX;
    }

    void setCellIdY(int cellIdY) {
        CellID_Y = cellIdY;
    }

    void setCellIdZ(int cellIdZ) {
        CellID_Z = cellIdZ;
    }


protected:
    int id{0};
    int CellID{0};

    int CellID_X{0};
    int CellID_Y{0};
    int CellID_Z{0};

    DetectorType Detector{nNone};

    double X{0.};
    double Y{0.};
    double Z{0.};
    double T{0.};
    double E{0.};

ClassDefOverride(PHit, 1);
};

#endif //PSIM_DHIT_H
