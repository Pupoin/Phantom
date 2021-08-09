//
// Created by Zhang Yulei on 9/24/20.
//

#ifndef PSIM_DEVENTLINKDEF_H
#define PSIM_DEVENTLINKDEF_H

#include <memory>
#include <vector>
#include <map>
#include <string>

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

#pragma link C++ class PStep+;
#pragma link C++ class PHit+;
#pragma link C++ class MCParticle+;

#pragma link C++ class std::vector<PStep *>+;
#pragma link C++ class std::vector<PHit *>+;
#pragma link C++ class std::vector<MCParticle *>+;

#pragma link C++ class std::map<TString, std::vector<PStep * >>+;
#pragma link C++ class std::map<TString, std::vector<PHit * >>+;
#pragma link C++ class std::map<TString, std::vector<MCParticle * >>+;

#pragma link C++ class PEvent+;


#endif

#endif //PSIM_DEVENTLINKDEF_H
