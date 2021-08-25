//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef PSIM_DHIT_H
#define PSIM_DHIT_H

#include "TObject.h"
#include "TString.h"
#include <cassert>

enum DetectorType {
    nNone, nTracker, nECAL, nHCAL
};

struct PCTTruth {
    double E = 0.;
    double T = 0.;
    double X = 0.;
    double Y = 0.;
    double Z = 0;
};
typedef std::vector<PCTTruth> PCTTruthVec;

struct PCTWaveform {
    double InitialTime{0.};//in unit of ns
    double SamplingFrequency = 1.E6;//in unit of HZ, defaul 1MHz
    std::vector<double> Data; //time serise
};

struct PCTDigi {
    double fTDC{0.};//front (top) end readout
    double fADC{0.};
    double bTDC{0.};//back (bottom) end readout
    double bADC{0.};
};

struct PCTImage {
    int Width = 256;
    int Height = 256;
    int Depth = 1;//1 slice
    //representation: Depth slices Width x Height pixels 2D image
    std::vector<unsigned char> Data;//default 2M bytes per 3D image
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
        E = rhs.E;
        T = rhs.T;
        Truth = rhs.Truth;
        Image = rhs.Image;
        Waveform = rhs.Waveform;
        Digi = rhs.Digi;

        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const PCTXData &hit) {
        TString str(
                Form("|   %-5d  |  %8.3f, %8.3f, %8.3f  |  %8.3f, %6.3f  |  %5d  |",
                     hit.id, hit.X, hit.Y, hit.Z, hit.E, hit.T, hit.CellID
                )
        );
        os << str;
        return os;
    }

    // Get Methods
    int getId() const { return id; }

    int getCellId() const { return CellID; }

    double getX() const { return X; }

    double getY() const { return Y; }

    double getZ() const { return Z; }

    double getE() const { return E; }

    double getT() const { return T; }

    DetectorType getDetector() const { return Detector; }

    int getCellIdX() const { return CellID_X; }

    int getCellIdY() const { return CellID_Y; }

    int getCellIdZ() const { return CellID_Z; }

    // Set Methods
    void setId(int ID) { PCTXData::id = ID; }

    void setCellId(int cellId) { CellID = cellId; }

    void setX(double x) { X = x; }

    void setY(double y) { Y = y; }

    void setZ(double z) { Z = z; }

    void setE(double e) { E = e; }

    void setT(double t) { T = t; }

    void setDetector(DetectorType detector) { Detector = detector; }

    void setCellIdX(int cellIdX) { CellID_X = cellIdX; }

    void setCellIdY(int cellIdY) { CellID_Y = cellIdY; }

    void setCellIdZ(int cellIdZ) { CellID_Z = cellIdZ; }

    // Getter and Setter for structures
    void addTruth(const PCTTruth &truth) {
        Truth.emplace_back(truth);
    }

    [[nodiscard]] const PCTTruthVec &getTruth() const { return Truth; }

    void setTruth(const PCTTruthVec &truth) { Truth = truth; }

    [[nodiscard]] const PCTWaveform &getWaveform() const { return Waveform; }

    void setWaveform(const PCTWaveform &waveform) { Waveform = waveform; }

    [[nodiscard]] const PCTDigi &getDigi() const { return Digi; }

    void setDigi(const PCTDigi &digi) { Digi = digi; }

    [[nodiscard]] const PCTImage &getImage() const { return Image; }

    void setImage(const PCTImage &image) { Image = image; }

    //the following 4 interfaces for digi
    double getTDC(bool front) {
        return front ? Digi.fTDC : Digi.bTDC;
    }

    double getADC(bool front) {
        return front ? Digi.fADC : Digi.bADC;
    }

    void setADC(double adc, bool front) {
        front ? Digi.fADC = adc : Digi.bADC = adc;
    }

    void setTDC(double tdc, bool front) {
        front ? Digi.fTDC = tdc : Digi.bTDC = tdc;
    }

    //the following interfaces for image
    void setImageSize(int width, int height = 256, int slice = 32) {
        assert((width < 0 || width > 256));
        assert((height < 0 || height > 256));
        assert((slice < 0 || slice > 64));
        Image.Width = width;
        Image.Height = height;
        Image.Depth = slice;
        size_t capacity = Image.Width * Image.Height * Image.Depth;
        if (Image.Data.size() != capacity)
            Image.Data.resize(capacity);
    }

    //d slice //w column //h row, //v: pixel value
    void setImagePixel(int d, int h, int w, unsigned char v) {
        assert(d >= 0 && d < Image.Depth);
        assert(w >= 0 && w < Image.Width);
        assert(h >= 0 && h < Image.Height);
        int idx = (d * Image.Depth + h) * Image.Width + w;
        Image.Data[idx] = v;
    }

    int getImageWidth() const { return Image.Width; }

    int getImageHeight() const { return Image.Height; }

    int getImageDepth() const { return Image.Depth; }

protected:
    int id{0};
    int CellID{0};

    int CellID_X{0};
    int CellID_Y{0};
    int CellID_Z{0};
    double X{0.};
    double Y{0.};
    double Z{0.};
    double E{0.};
    double T{0.};

    DetectorType Detector{nNone};

    PCTTruthVec Truth;
    PCTWaveform Waveform;
    PCTDigi Digi;
    PCTImage Image;

ClassDefOverride(PCTXData, 1);
};

#endif //PSIM_DHIT_H
