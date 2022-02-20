#include "PctDataType.hh"
#include <iomanip>
#include <iostream>

namespace PCT
{
    std::ostream &operator<<(std::ostream &os, const PctPoint &val)
    {
        os << std::setw(10) << val.x
           << std::setw(10) << val.y
           << std::setw(10) << val.z
           << std::setw(10) << val.eDep
           << std::setw(10) << val.eTot
           << std::setw(10) << val.materialIndex
           << std::setw(10) << val.processType
           << std::setw(10) << val.status;
        return os;
    }
    std::ostream &operator<<(std::ostream &os, const PctDigi& val)
    {
        os << std::setw(10) << val.detID
           << std::setw(10) << val.adcL
           << std::setw(10) << val.tdcL
           << std::setw(10) << val.adcR
           << std::setw(10) << val.tdcR;
        return os;
    }
    std::ostream &operator<<(std::ostream &os, const PctTrack &val)
    {
        os << std::setw(15) << val.pdg_code
           << std::setw(8) << val.track_id
           << std::setw(8) << val.parent_id
           << std::setw(10) << val.points.size()
           << std::endl;
        for (size_t p = 0; p < val.points.size(); ++p)
        {
            os << val.points[p] << std::endl;
        }
        os << std::endl;
        return os;
    }
    std::ostream &operator<<(std::ostream &os, const PctHit& val)
    {
        os << std::setw(15) << val.detID
           << std::setw(8) << val.dx
           << std::setw(8) << val.dy
           << std::setw(8) << val.dz
           << std::setw(8) << val.dt
           << std::setw(8) << val.dE
           //<< std::setw(10) << val.points.size()
           << std::endl;
        //for (size_t p = 0; p < val.points.size(); ++p)
        //{
        //    os << val.points[p] << std::endl;
        //}
        os << std::endl;
        return os;
    }
    namespace READER
    {
        std::ostream &operator<<(std::ostream &os, const PctEvent &val)
        {
            os << std::setw(10) << val.run_id
               << std::setw(10) << val.evt_id
               << std::setw(10) << val.tracks.size()
               << std::setw(10) << val.hits.size()
               << std::setw(10) << val.digis.size()
               << std::endl;
            for (size_t p = 0; p < val.tracks.size(); ++p)
            {
                os << val.tracks[p] << std::endl;
            }
            for (size_t p = 0; p < val.hits.size(); ++p)
            {
                os << val.hits[p] << std::endl;
            }
            for (size_t p = 0; p < val.digis.size(); ++p)
            {
                os << val.digis[p] << std::endl;
            }
            os << std::endl;
            return os;
        }
    }
}
