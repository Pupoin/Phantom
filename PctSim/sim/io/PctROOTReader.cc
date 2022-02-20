

#include "PctROOTReader.hh"

PctROOTReader *PctROOTReader::pReaderHandle = NULL;

PctROOTReader *PctROOTReader::Instance()
{
    if (pReaderHandle == NULL)
        pReaderHandle = new PctROOTReader();
    return pReaderHandle;
}
