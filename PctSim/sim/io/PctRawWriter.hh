
#pragma once

#include <vector>
#include <stdio.h>
#include "G4Types.hh"
#include "PctDataType.hh"
#include <assert.h>

class PctRawWriter
{
private:
    static PctRawWriter *pWriterHandle;
    FILE* pFileHandle;
    bool fFileIsOK;
public:
    static PctRawWriter *Instance();
    ~PctRawWriter(){};

    bool open(const char *fileName, const char *mode)
    {
        if (!pFileHandle)
            pFileHandle = ::fopen(fileName, mode);
        fFileIsOK = ::ferror(pFileHandle) == 0 ? true : false;
        return fFileIsOK;
    }
    bool isFileOK() const { return fFileIsOK; }
    // bool write(PCT::READER::PctEvent* pctevt)
    //{
    //     std::vector<int> databuff;
    //     pctevt->writeTo(databuff);
    //     size_t nSize = databuff.size();
    //     ::fwrite(&nSize, sizeof(G4int), 1, pFileHandle);
    //     ::fwrite(&(databuff[0]), sizeof(G4int), nSize, pFileHandle);
    //     return ::ferror(pFileHandle) == 0 ? true : false;
    // }
    bool write(std::vector<G4int> &data)
    {
        G4int nSize = data.size();
        ::fwrite(&nSize, sizeof(G4int), 1, pFileHandle);
        ::fwrite(&(data[0]), sizeof(G4int), nSize, pFileHandle);
        return ::ferror(pFileHandle) == 0 ? true : false;
    }
    bool write(std::vector<G4long> &data)
    {
        size_t nSize = data.size();
        ::fwrite(&nSize, sizeof(int), 1, pFileHandle);
        ::fwrite(&(data[0]), sizeof(G4long), nSize, pFileHandle);
        return ::ferror(pFileHandle) == 0 ? true : false;
    }
    /*size_t write(const void *ptr, size_t size, size_t nmemb)
    {
        return ::fwrite(ptr, size, nmemb, pFileHandle);
    }*/
    int flush()
    {
        return ::fflush(pFileHandle);
    }
    int close()
    {
        int id = ::fclose(pFileHandle);
        pFileHandle = NULL;
        return id;
    }

private:
    PctRawWriter() : pFileHandle(NULL), fFileIsOK(false) {
    }
    PctRawWriter(const PctRawWriter &);
    PctRawWriter &operator=(const PctRawWriter &);
};
