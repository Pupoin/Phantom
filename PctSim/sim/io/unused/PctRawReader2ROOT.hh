#pragma once

#include <stdio.h>

#include "DataIO/PCTEvent.h"
#include "DataIO/PCTStep.h"

class PctRawReader2ROOT
{
	private:
		static PctRawReader2ROOT *pReaderHandle;
		FILE *pFileHandle;

	public:
		static PctRawReader2ROOT *Instance();
		~PctRawReader2ROOT(){};

		bool open(const char *fileName)
		{
			if (!pFileHandle)
				pFileHandle = ::fopen(fileName, "r");
			return ::ferror(pFileHandle) == 0 ? true : false;
		}
		size_t read(void *ptr, size_t size, size_t nmemb)
		{
			return ::fread(ptr, size, nmemb, pFileHandle);
		}
		int eof()
		{
			return feof(pFileHandle);
		}
		int close()
		{
			int id = ::fclose(pFileHandle);
			pFileHandle = NULL;
			return id;
		}

	public:
		PCTEvent *next()
		{
			int num = -1;
			read(&num, sizeof(int), 1);
			if (num < 0) // this file reaches ending
				return NULL;
			data.resize(num);

			read(&data[0], sizeof(int), num);

			int runID = data[0] >> 20;
			int evtID = data[0] & 0b111111111111;
			int ntrj = data[1];
			fevt->setRunId(runID);
			fevt->setEventId(evtID);
			std::vector<PCTStep *> *stepVec = fevt->GetData("PCTStep", ParticleStep_DataType::COL);
			int idxA = 2;
			for (int ip = 0; ip < ntrj; ++ip)
			{
				int pdg = data[idxA++]; // pdg
				int tid = data[idxA++]; // track id
				int pid = data[idxA++]; // parent
				int npoints = data[idxA++];
				for (int pos = 0; pos < npoints; ++pos)
				{
					PCTStep *aStep = new PCTStep();
					aStep->setX(data[idxA++] / 100.);
					aStep->setY(data[idxA++] / 100.);
					aStep->setY(data[idxA++] / 100.);
					stepVec->push_back(aStep);
				}
			}
			// std::cout << "cross-check #" << num << ", " << idxA << std::endl;
			if (num != idxA)
				return NULL;
		}
	private:
		std::vector<int> data;
		PCTEvent *fevt;


	private:
		PctRawReader2ROOT() : pFileHandle(NULL)
	{
		fevt = new PCTEvent();
		fevt->RegisterCollection("PCTStep", Phantom_DataType::ParticleStep);
		// fevt->RegisterCollection("MCParticle", MCParticle_DataType::COL);
	}
		PctRawReader2ROOT(const PctRawReader2ROOT &);
		PctRawReader2ROOT &operator=(const PctRawReader2ROOT &);
};
