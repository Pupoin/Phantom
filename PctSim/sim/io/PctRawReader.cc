#include "PctRawReader.hh"
#include <iomanip>


PctRawReader* PctRawReader::pReaderHandle = NULL;


PctRawReader* PctRawReader::Instance(){
    if(NULL == PctRawReader::pReaderHandle){
        pReaderHandle = new PctRawReader();
    }
    return pReaderHandle;
}

