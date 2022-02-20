#include "PctRawReader2ROOT.hh"
#include <iomanip>


PctRawReader2ROOT* PctRawReader2ROOT::pReaderHandle = NULL;


PctRawReader2ROOT* PctRawReader2ROOT::Instance(){
    if(NULL == PctRawReader2ROOT::pReaderHandle){
        pReaderHandle = new PctRawReader2ROOT();
    }
    return pReaderHandle;
}

