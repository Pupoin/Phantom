#pragma once

#include <vector>
#include <iostream>
#include "G4SystemOfUnits.hh"
#include "G4Types.hh"

namespace PCT
{
    struct PctPoint//refer to PctTrajectroyPoint
    {
        double x;
        double y;
        double z;
        double eDep;//energy deposit, keV
        double eTot;//keV
        int materialIndex;
        int processType;
        int status;
        //extension to more items, say energy depostion
        //static size_t size() {
        //    return 8;
        //}

        friend std::ostream &operator<<(std::ostream &os, const PctPoint &val);
    };
    struct PctDigi
    {
        int adcL;
        int tdcL;
        int adcR;
        int tdcR;//double-ended readout
        int detID;//detector ID

        //static size_t size() {
        //    return 5;
        //}

        friend std::ostream &operator<<(std::ostream &os, const PctPoint &val);
    };
    struct PctTrack 
    {
        PctTrack(){
            points.reserve(800);
        }
        ~PctTrack(){
            points.clear();
        }
        int pdg_code;
        int track_id;
        int parent_id;
        std::vector<PctPoint> points; //one track owns many interaction points

        //size_t size() const {
        //    return 3 + points.size()*PctPoint::size();
        //}

        friend std::ostream &operator<<(std::ostream &os, const PctTrack &val);
    };
    struct PctHit
    {
        ~PctHit(){
            //points.clear();
        }
        int detID;//which detector
        double dx;
        double dy;
        double dz;
        double dt;
        double dE;//energy depositon
        //static size_t size() {
        //    return 6 ;
        //}
        //std::vector<PctPoint> points; // interaction points

        friend std::ostream &operator<<(std::ostream &os, const PctHit& val);
    };

//to decode, please refer to how to encode.
    namespace READER
    {
        struct PctEvent
        {
            int run_id;
            int evt_id;
            std::vector<PctTrack> tracks;
            std::vector<PctHit> hits;
            std::vector<PctDigi> digis;

            PctEvent()
            {
                tracks.reserve(1000);
                hits.reserve(1000);
                digis.reserve(1000);
            }
            ~PctEvent()
            {
                tracks.clear();
                hits.clear();
                digis.clear();
            }
            void clear()
            {
                tracks.clear();
                hits.clear();
                digis.clear();
                run_id = evt_id = 0xEFFFFFFF;
            }
            void print(){
                std::cout<<"run="<<run_id<<", evt="<<evt_id<<std::endl;
                std::cout<<"tracks="<<tracks.size()<<", hits="<<hits.size()<<", digis="<<digis.size()<<std::endl;
            }
            void writeTo(std::vector<int>& databuff)
            {
                //size_t nn = GetSize();
                //databuff.push_back(nn);
                int run_evt = run_id << 20;
                run_evt |= evt_id;

                databuff.push_back(run_evt);
                databuff.push_back(tracks.size());

                size_t num = 2;

                for (size_t i = 0; i < tracks.size(); ++i)
                {
                    databuff.push_back(tracks[i].pdg_code);
                    databuff.push_back(tracks[i].track_id);
                    databuff.push_back(tracks[i].parent_id);
                    databuff.push_back(tracks[i].points.size());
                    //std::cout << "track #" << i << ", size#" << tracks[i].points.size() << std::endl;
                    for (size_t j = 0; j < tracks[i].points.size(); ++j)
                    {
                        databuff.push_back(G4int(100. * tracks[i].points[j].x / mm));
                        databuff.push_back(G4int(100. * tracks[i].points[j].y / mm));
                        databuff.push_back(G4int(100. * tracks[i].points[j].z / mm));
                        databuff.push_back(G4int(tracks[i].points[j].eDep / keV));
                        databuff.push_back(G4int(tracks[i].points[j].eTot / keV));
                        databuff.push_back(tracks[i].points[j].materialIndex);
                        databuff.push_back(tracks[i].points[j].processType);
                        databuff.push_back(tracks[i].points[j].status);
                        ++num;
                    }
                }

                databuff.push_back(hits.size());
                for (size_t i = 0; i < hits.size(); ++i)
                {
                    databuff.push_back(hits[i].detID);
                    databuff.push_back(G4int(100. * hits[i].dx / mm));
                    databuff.push_back(G4int(100. * hits[i].dy / mm));
                    databuff.push_back(G4int(100. * hits[i].dz / mm));
                    databuff.push_back(G4int(100. * hits[i].dt / ns));
                    databuff.push_back(G4int(hits[i].dE / keV));
                    ++num;
                }

                databuff.push_back(digis.size());
                for (size_t i = 0; i < digis.size(); ++i)
                {
                    databuff.push_back(digis[i].detID);
                    databuff.push_back(digis[i].adcL);
                    databuff.push_back(digis[i].tdcL);
                    databuff.push_back(digis[i].adcR);
                    databuff.push_back(digis[i].tdcR);
                    ++num;
                }
                // return num == databuff.size();
            }
            bool readFrom(const std::vector<int> &data)
            {
                // inverse operation for writeTo
                size_t idx = 0;
                evt_id = data[idx] & 0xFFF;
                run_id = data[idx++] >> 20;
                tracks.resize(data[idx++]);

                for (size_t ip = 0; ip < tracks.size(); ++ip)
                {
                    tracks[ip].pdg_code = data[idx++];     // pdg
                    tracks[ip].track_id = data[idx++];     // track id
                    tracks[ip].parent_id = data[idx++];    // parent
                    tracks[ip].points.resize(data[idx++]); // number of points
                    for (size_t pos = 0; pos < tracks[ip].points.size(); ++pos)
                    {
                        tracks[ip].points[pos].x = data[idx++] / 100.;
                        tracks[ip].points[pos].y = data[idx++] / 100.;
                        tracks[ip].points[pos].z = data[idx++] / 100.;
                        tracks[ip].points[pos].eDep = data[idx++] / 1.;
                        tracks[ip].points[pos].eTot = data[idx++] / 1.;
                        tracks[ip].points[pos].materialIndex = data[idx++];
                        tracks[ip].points[pos].processType = data[idx++];
                        tracks[ip].points[pos].status = data[idx++];
                    }
                }

                hits.resize(data[idx++]);
                for (size_t ih = 0; ih < hits.size(); ++ih)
                {
                    hits[ih].detID = data[idx++];
                    hits[ih].dx = data[idx++] / 100.; // to mm
                    hits[ih].dy = data[idx++] / 100.; //
                    hits[ih].dz = data[idx++] / 100.;
                    hits[ih].dt = data[idx++] / 100.; // to ns
                    hits[ih].dE = data[idx++] / 1.;   // to keV
                }

                digis.resize(data[idx++]);
                for (size_t id = 0; id < digis.size(); ++id)
                {
                    digis[id].detID = data[idx++];
                    digis[id].adcL = data[idx++];
                    digis[id].tdcL = data[idx++];
                    digis[id].adcR = data[idx++];
                    digis[id].tdcR = data[idx++];
                }
                // std::cout << "cross-check #" << num << ", " << idx << std::endl;
                if (data.size() != idx)
                {
                    std::cerr << "memory cross-check failed, this event is wrongly decoded." << std::endl;
                }
                return data.size() == idx;
            }

            friend std::ostream &operator<<(std::ostream &os, const PctEvent &val);
            friend class PctRawReader;
            friend class PctROOTReader;

        private:
            /*size_t GetSize() const
            {
                size_t nn = 0;
                for (size_t i = 0; i < tracks.size(); ++i)
                {
                    nn += tracks[i].size();
                }
                nn += hits.size() * PctHit::size();
                nn += digis.size() * PctDigi::size();
                return nn;
            }*/
        };
    }

    // to get a better output, we use a block data.
    namespace WRITER
    {
        struct PctEvent
        {
            std::vector<int> databuff;

            PctEvent()
            {
                databuff.reserve(10000);
            }
            ~PctEvent()
            {
                databuff.clear();
            }

            // friend std::ostream &operator<<(std::ostream &os, const PctEvent &val);
            friend class PctRawWriter;
            friend class PctROOTWriter;
        };
    }
}
