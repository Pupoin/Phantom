#include "PctRawWriter.hh"


PctRawWriter* PctRawWriter::pWriterHandle = NULL;


PctRawWriter* PctRawWriter::Instance(){
    if(NULL == PctRawWriter::pWriterHandle){
        pWriterHandle = new PctRawWriter();
    }
    return pWriterHandle;
}