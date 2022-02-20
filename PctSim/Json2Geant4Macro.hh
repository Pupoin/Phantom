#pragma once
#include "json/reader.h"
#include "json/json.h"
#include "json/value.h"
#include "json/writer.h"
#include <fstream>
#include <exception>
#include "G4String.hh"

extern G4String trim(const Json::Value& jvalue);
extern bool json2macro(const Json::Value &root, const std::string &outFile);
