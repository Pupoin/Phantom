
#include "PctSharedCode.hh"
namespace Encoding
{

    int CreateCollectionID(int type, int idx)
    {
        return type*1000 + idx +1;
    }
    int GetCollectionType(int code)
    {
        return code/1000;
    }

}