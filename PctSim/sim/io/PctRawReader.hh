#pragma once

#include <stdio.h>
#include "PctDataType.hh"


class PctRawReader
{
	private:
		static PctRawReader *pReaderHandle;
		FILE *pFileHandle;

	public:
		static PctRawReader *Instance();
		~PctRawReader(){};

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

		PCT::READER::PctEvent *next()
		{
			int num = -1;
			read(&num, sizeof(int), 1);
			if (num < 0) // this file reaches ending
				return NULL;
			data.resize(num);

			read(&data[0], sizeof(int), num);

			fevt->evt_id = data[0] & 0xFFF;
			fevt->run_id = data[0] >> 20;
			fevt->tracks.resize(data[1]);

			//int idxA = 2;
			int idx = 2;
			for (size_t ip = 0; ip < fevt->tracks.size(); ++ip)
			{
				fevt->tracks[ip].pdg_code = data[idx+0];  // pdg
				fevt->tracks[ip].track_id = data[idx+1];  // track id
				fevt->tracks[ip].parent_id = data[idx+2]; // parent
				fevt->tracks[ip].points.resize(data[idx+3]);//number of points
				for (size_t pos = 0; pos < fevt->tracks[ip].points.size(); ++pos)
				{
					fevt->tracks[ip].points[pos].x = data[pos*3+idx+4] / 100.;
					fevt->tracks[ip].points[pos].y = data[pos*3+idx+5] / 100.;
					fevt->tracks[ip].points[pos].z = data[pos*3+idx+6] / 100.;
				}
				idx += 4 + 3*fevt->tracks[ip].points.size() ;
			}
			//std::cout << "cross-check #" << num << ", " << idx << std::endl;
			if (num != idx){
				std::cerr<<"memory cross-check failed, return a NULL pointer to PCT::PctEvent"<<std::endl;
				return NULL;
			}
			return fevt;
		}
	private:
		std::vector<int> data;
		PCT::READER::PctEvent *fevt;


	private:
		PctRawReader() : pFileHandle(NULL)
	{
		fevt = new PCT::READER::PctEvent();
	}
		PctRawReader(const PctRawReader &);
		PctRawReader &operator=(const PctRawReader &);
};
