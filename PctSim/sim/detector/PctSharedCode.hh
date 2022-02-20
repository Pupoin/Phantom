#pragma once

namespace Encoding
{

    const int CID_TRACKER = 0x1;
    const int CID_TOF = 0x2;
    const int CID_ECAL = 0x4;
    const int CID_TARGET = 0x8;

    int CreateCollectionID(int type, int idx);
    int GetCollectionType(int code);

}