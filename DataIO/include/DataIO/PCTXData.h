//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef PSIM_DHIT_H
#define PSIM_DHIT_H

#include "TObject.h"
#include "TString.h"

enum DetectorType {
    nNone, nTracker, nECAL, nHCAL
};

struct PCTTruth {
    double E = 0.;
    double T = 0.;
};

struct PCTWaveform {

};

struct PCTDigi {

};

struct PCTImage {

};

class PCTXData : public TObject {
public:
    // Constructor and Destructor
    PCTXData() = default;

    PCTXData(const PCTXData &rhs) : TObject(rhs) {
        *this = rhs;
    }

    ~PCTXData() override = default;

    PCTXData &operator=(const PCTXData &rhs) {
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
        Truth = rhs.Truth;
        Image = rhs.Image;
        Waveform = rhs.Waveform;
        Digi = rhs.Digi;

        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const PCTXData &hit) {
        TString str(
                Form("|   %-5d  |  %8.3f, %8.3f, %8.3f  |  %8.3f, %6.3f  |  %5d  |",
                     hit.id, hit.X, hit.Y, hit.Z, hit.Truth.E, hit.Truth.T, hit.CellID
                )
        );
        os << str;
        return os;
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
        PCTXData::id = ID;
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

    // Getter and Setter for structures
    [[nodiscard]] const PCTTruth &getTruth() const {
        return Truth;
    }

    void setTruth(const PCTTruth &truth) {
        Truth = truth;
    }

    [[nodiscard]] const PCTWaveform &getWaveform() const {
        return Waveform;
    }

    void setWaveform(const PCTWaveform &waveform) {
        Waveform = waveform;
    }

    [[nodiscard]] const PCTDigi &getDigi() const {
        return Digi;
    }

    void setDigi(const PCTDigi &digi) {
        Digi = digi;
    }

    [[nodiscard]] const PCTImage &getImage() const {
        return Image;
    }

    void setImage(const PCTImage &image) {
        Image = image;
    }


protected:
    int id{0};
    int CellID{0};

    int CellID_X{0};
    int CellID_Y{0};
    int CellID_Z{0};
    double X{0.};
    double Y{0.};
    double Z{0.};

    DetectorType Detector{nNone};

    PCTTruth Truth;
    PCTWaveform Waveform;
    PCTDigi Digi;
    PCTImage Image;

ClassDefOverride(PCTXData, 1);
};

#endif //PSIM_DHIT_H
