#ifndef PCTSIMULATION_GEOMETRYHELPER_H
#define PCTSIMULATION_GEOMETRYHELPER_H

#include "TGeoManager.h"
#include "TVector3.h"
#include "TString.h"

#include "TGeoNode.h"
#include "TGeoBBox.h"
#include "TGeoVolume.h"

#include "DataIO/PCTXData.h"

#include <map>

#ifdef RM_UNIT
#define CUNIT 1
#else
#define CUNIT 10
#endif


using std::map, std::pair, std::make_pair;

struct DetCell {
    TGeoNode *node;
    TGeoBBox *shape;
    TGeoVolume *volume;

    TVector3 position;
    TVector3 size;
};

enum class surface {
    x_plus, x_minus, y_plus, y_minus, z_plus, z_minus
};

typedef std::map<TString, std::map<int, DetCell> > DetectorInfo;

class EventReader;

class GeometryHelper {
public:
    static GeometryHelper *CreateInstance();

    void setVerbose(int verbose) { Verbose = verbose; }

private:
    friend class EventReader;

    GeometryHelper();

    void setGeoManager();

    bool checkGeoManager();

    int Verbose{0};

    TGeoNode *world_node{};

    map<TString, TGeoNode *> main_detectors;

    DetectorInfo det_info;

public:
    [[nodiscard]] const DetectorInfo &getDetInfo() const { return det_info; }

    /* Calculation */

    // Calculate distance of two points
    double GetDisOfTwo(const TVector3 &a, const TVector3 &b);

    // Get Center Point of the targeted geometry
    TVector3 GetPosOfVolume(const TString &det_name, int copyNo);

//    TVector3 GetPosOfVolume(PCTXData *hit);

    // Get Size of the targeted geometry (Not Half!)
    TVector3 GetSizeOfVolume(const TString &det_name, int copyNo);

//    TVector3 GetSizeOfVolume(PCTXData *hit);

    // Get Distance between the given point and the surface of targeted geometry
    /* surface is defined as:
     *  x_plus: point to the +x surface (left)
     *  x_minus: point to the -x surface (right)
     *  y_plus: point to the +y surface (top)
     *  y_minus: point to the -y surface (bottom)
     *  z_plus: point to the +z surface (front)
     *  z_minus: point to the -z surface (back)
     */
    double GetDisToSuf(const TVector3 &point, const TString &det_name, int copyNo, surface surf);

//    TVector3 GetDisToSuf( PCTXData *hit, surface surf);

public:
    // Delete Copy constructor
    GeometryHelper(const GeometryHelper &) = delete;

    GeometryHelper &operator=(GeometryHelper const &) = delete;
};

extern GeometryHelper *pGeo;

#endif //PCTSIMULATION_GEOMETRYHELPER_H
