

#include "PctROOTWriter.hh"

PctROOTWriter *PctROOTWriter::pWriterHandle = NULL;

PctROOTWriter *PctROOTWriter::Instance()
{
    if (pWriterHandle == NULL)
        pWriterHandle = new PctROOTWriter();
    return pWriterHandle;
}
