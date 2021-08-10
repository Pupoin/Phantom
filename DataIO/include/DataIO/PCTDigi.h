/*
digitized readout for each sensitive detector
*/
#ifndef PCTSIMULATION_PCTDIGI_H
#define PCTSIMULATION_PCTDIGI_H

#include "TObject.h"
#include "TString.h"
#include "PCTXData.h"

class PCTDigi : public TObject {
public:
    // Constructor and Destructor
    PCTDigi() = default;

    PCTDigi(const PCTDigi &rhs) : TObject(rhs) {
        *this = rhs;
    }

    ~PCTDigi() override = default;

    bool operator==(const PCTDigi &rhs) const {
        return id == rhs.id &&
               CellID == rhs.CellID &&
               CellID_X == rhs.CellID_X &&
               CellID_Y == rhs.CellID_Y &&
               CellID_Z == rhs.CellID_Z &&
               Detector == rhs.Detector ;
    }

    bool operator!=(const PCTDigi &rhs) const {
        return !(rhs == *this);
    }

    PCTDigi &operator=(const PCTDigi &rhs) {
        if (&rhs != this)
        {
            id = rhs.id;
            CellID = rhs.CellID;
            CellID_X = rhs.CellID_X;
            CellID_Y = rhs.CellID_Y;
            CellID_Z = rhs.CellID_Z;
            Detector = rhs.Detector;
            lTDC = rhs.lTDC;
            lADC = rhs.lADC;
            rTDC = rhs.rTDC;
            rADC = rhs.rADC;
        }

        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const PCTDigi &digi)
    {
        TString str(
            Form("|   %-5d  |  %8.3f, %8.3f, %8.3f, %8.3f|",
                 digi.id, digi.lADC, digi.lTDC, digi.rADC, digi.rTDC ));
        os << str;
        return os;
    }

    // Get Methods
    int getId() const
    {
        return id;
    }

    int getCellId() const
    {
        return CellID;
    }

    DetectorType getDetector() const
    {
        return Detector;
    }

    int getCellIdX() const
    {
        return CellID_X;
    }

    int getCellIdY() const
    {
        return CellID_Y;
    }

    int getCellIdZ() const
    {
        return CellID_Z;
    }

    // Set Methods
    void setId(int ID)
    {
        PCTDigi::id = ID;
    }

    void setCellId(int cellId)
    {
        CellID = cellId;
    }

    void setTDC(double t, bool front)
    {
        front ? lTDC = t : rTDC = t;
    }

    void setADC(double e, bool front)
    {
        front ? lADC = e : rADC = e;
    }

    void setDetector(DetectorType detector)
    {
        Detector = detector;
    }

    void setCellIdX(int cellIdX)
    {
        CellID_X = cellIdX;
    }

    void setCellIdY(int cellIdY)
    {
        CellID_Y = cellIdY;
    }

    void setCellIdZ(int cellIdZ)
    {
        CellID_Z = cellIdZ;
    }

protected:
    int id{0};
    int CellID{0};

    int CellID_X{0};
    int CellID_Y{0};
    int CellID_Z{0};

    DetectorType Detector{nNone};

    double lTDC{0.};//left (top) end readout
    double lADC{0.};
    double rTDC{0.};//right (bottom) end readout
    double rADC{0.};

    ClassDefOverride(PCTDigi, 1);
};

#endif