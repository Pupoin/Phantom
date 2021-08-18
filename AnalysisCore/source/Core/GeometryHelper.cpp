#include "Core/GeometryHelper.h"

#include <iostream>

using std::cerr, std::cout, std::endl;

// Required by Singleton
GeometryHelper *pGeo = nullptr;

// Get Instance Class
GeometryHelper *GeometryHelper::CreateInstance() {
    if (pGeo == nullptr)
        pGeo = new GeometryHelper();

    return pGeo;
}

bool GeometryHelper::checkGeoManager() {
    if (world_node) return true;
    else {
        cerr << "[Error] -- No Geometry loaded..." << endl;
        return false;
    }
}

void GeometryHelper::setGeoManager() {
    world_node = gGeoManager->GetTopNode();

    // Print daughter nodes information
    if (Verbose >= 0) {
        Printf("============================================================================================");
        Printf("[Reading Geometry] : (Verbosity 0) ");
        Printf("--------------------------------------------------------------------------------------------");
        Printf("Col. Name            copy No.            center pos. [mm]                size [mm]");
        Printf("--------------------------------------------------------------------------------------------");
    }
    for (int i = 0; i < world_node->GetNdaughters(); ++i) {
        auto *cur_node = dynamic_cast<TGeoNode *>(world_node->GetDaughter(i));
        auto *cur_volume = dynamic_cast<TGeoVolume *>(cur_node->GetVolume());
        auto *cur_shape = dynamic_cast<TGeoBBox *>(cur_volume->GetShape());
        auto cur_Name = TString(cur_node->GetVolume()->GetName());
        cur_Name = cur_Name(0, cur_Name.Index("_Region_LV"));

        main_detectors.emplace(std::make_pair(cur_Name, cur_node));

        // Unit: millimeter
        auto cur_pos_x = cur_node->GetMatrix()->GetTranslation()[0] * CUNIT;
        auto cur_pos_y = cur_node->GetMatrix()->GetTranslation()[1] * CUNIT;
        auto cur_pos_z = cur_node->GetMatrix()->GetTranslation()[2] * CUNIT;
        auto cur_size_x = 2 * cur_shape->GetDX() * CUNIT;
        auto cur_size_y = 2 * cur_shape->GetDY() * CUNIT;
        auto cur_size_z = 2 * cur_shape->GetDZ() * CUNIT;

        if (Verbose >= 0) {
            Printf("%-24s%4d%7s| (%2.1f, %2.1f, % 8.3f) | (%8.2f, %8.2f, % 8.3f) ", cur_Name.Data(),
                   cur_node->GetNdaughters(), "", cur_pos_x,
                   cur_pos_y, cur_pos_z, "", cur_size_x,
                   cur_size_y, cur_size_z);
        }

        std::map<int, DetCell> cur_det_dict;
        for (int j = 0; j < cur_node->GetNdaughters(); ++j) {
            DetCell cur;
            cur.node = dynamic_cast<TGeoNode *>(cur_node->GetDaughter(j));
            cur.volume = dynamic_cast<TGeoVolume *>(cur.node->GetVolume());
            cur.shape = dynamic_cast<TGeoBBox *>(cur.volume->GetShape());
            cur.position = TVector3({cur_pos_x + cur.node->GetMatrix()->GetTranslation()[0] * CUNIT,
                                     cur_pos_y + cur.node->GetMatrix()->GetTranslation()[1] * CUNIT,
                                     cur_pos_z + cur.node->GetMatrix()->GetTranslation()[2] * CUNIT});
            cur.size = TVector3({2 * cur.shape->GetDX() * CUNIT,
                                 2 * cur.shape->GetDY() * CUNIT,
                                 2 * cur.shape->GetDZ() * CUNIT});

            cur_det_dict.insert(std::make_pair(j, cur));
        }
        det_info.emplace(std::make_pair(cur_Name, cur_det_dict));
    }
    if (Verbose >= 0) {
        Printf("--------------------------------------------------------------------------------------------");
    }
}

double GeometryHelper::GetDisOfTwo(const TVector3 &a, const TVector3 &b) {
    if (!checkGeoManager()) exit(EXIT_FAILURE);

    return (a - b).Mag();
}

TVector3 GeometryHelper::GetPosOfVolume(const TString &det_name, int copyNo) {
    if (!checkGeoManager()) exit(EXIT_FAILURE);

    return det_info.at(det_name).at(copyNo).position;
}

TVector3 GeometryHelper::GetSizeOfVolume(const TString &det_name, int copyNo) {
    if (!checkGeoManager()) exit(EXIT_FAILURE);

    return det_info.at(det_name).at(copyNo).size;
}

double GeometryHelper::GetDisToSuf(const TVector3 &point, const TString &det_name, int copyNo, surface surf) {
    if (!checkGeoManager()) exit(EXIT_FAILURE);

    auto cur = det_info.at(det_name).at(copyNo);
    double master[3];
    master[0] = point.x();
    master[1] = point.y();
    master[2] = point.z();

    double local[3];

    cur.node->MasterToLocal(master, local);

    if (surf == surface::x_plus)
        return fabs(local[0] - cur.size.x() / 2.);
    else if (surf == surface::x_minus)
        return fabs(local[0] + cur.size.x() / 2.);
    else if (surf == surface::y_plus)
        return fabs(local[1] - cur.size.y() / 2.);
    else if (surf == surface::y_minus)
        return fabs(local[1] + cur.size.y() / 2.);
    else if (surf == surface::z_plus)
        return fabs(local[2] - cur.size.z() / 2.);
    else if (surf == surface::z_minus)
        return fabs(local[2] + cur.size.z() / 2.);

    return -1;
}


GeometryHelper::GeometryHelper() = default;

