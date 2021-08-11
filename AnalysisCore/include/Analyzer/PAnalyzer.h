//
// Created by Zhang Yulei on 8/11/21.
//

#ifndef PCTSIMULATION_PANALYZER_H
#define PCTSIMULATION_PANALYZER_H

#include "DataIO/PEvent.h"

#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <map>
#include <tuple>

using std::cout, std::endl;
using std::string;
using std::vector;
using std::map, std::pair, std::tuple;

using AnaVar = std::variant<bool *, int *, double *, std::string *, vector<bool> *, vector<int> *, vector<double> *, vector<std::string> *>;
using MapAnaVar = map<string, tuple<string, AnaVar, string>>;

// Abstract class of analyzers
class PAnalyzer {
public:
    PAnalyzer() = default;

    virtual ~PAnalyzer() = default;

    [[nodiscard]] const MapAnaVar &getAnaVarCol() const { return ana_var_col; }

    [[nodiscard]] const string &getName() const { return name; }

    [[nodiscard]] const string &getDescription() const { return description; }

    [[nodiscard]] int getVerbose() const { return Verbose; }

    template<typename T>
    void RegisterParameter(const string &var_name, T *var, T default_value, const string &var_description,
                           const string &var_type);

    /*
     * Workflow of an algo:
     *      begin of run -> [loop of evt: process -> check  ] -> end of run
     */

    virtual void Begin() = 0;

    virtual void ProcessEvt(PEvent *) = 0;

    virtual void CheckEvt(PEvent *) = 0;

    virtual void End() = 0;

protected:
    string name;
    string description;

    int Verbose{0};

private:
    MapAnaVar ana_var_col;
};

template<typename T>
void PAnalyzer::RegisterParameter(const string &var_name, T *var, T default_value, const string &var_description,
                                  const string &var_type) {
    if (ana_var_col.count(var_name) != 0)
        std::cerr << "[WARNING] ==> Parameter " + var_name + " already exists." << std::endl;
    else {
        *var = default_value;
        AnaVar tmp_v = var;
        std::tuple<std::string, AnaVar, std::string> tmp(var_type, tmp_v, var_description);
        ana_var_col.insert(std::pair<std::string, std::tuple<std::string, AnaVar, std::string> >(var_name, tmp));

        if (Verbose > 1) {
            std::cout << "[Parameter REGISTER] ==> Parameter " + var_name + " has been successfully added."
                      << std::endl;
        }
    }
}

// Define the macro to read AnaVar
#define assignAnaVar(var, var_type, yaml_node)                                                                      \
    if ((var_type) == "bool") *get<0>(var) = (yaml_node).as<bool>();                                                \
    else if ((var_type) == "int") *get<1>(var) = (yaml_node).as<int>();                                             \
    else if ((var_type) == "double") *get<2>(var) = (yaml_node).as<double>();                                       \
    else if ((var_type) == "string") *get<3>(var) = (yaml_node).as<string>();                                       \
    else if ((var_type).find("bool") != std::string::npos && (var_type).find("vector") != std::string::npos) {      \
        std::vector<bool> tmp = {};                                                                                 \
        for (auto val : (yaml_node) ) tmp.push_back((yaml_node).as<bool>());                                        \
        *get<4>(var) = tmp;                                                                                         \
    } else if ((var_type).find("int") != std::string::npos && (var_type).find("vector") != std::string::npos) {     \
    std::vector<int> tmp = {};                                                                                      \
    for (auto val : (yaml_node) ) tmp.push_back((yaml_node).as<int>());                                             \
    *get<5>(var) = tmp;                                                                                             \
    } else if ((var_type).find("double") != std::string::npos && (var_type).find("vector") != std::string::npos) {  \
    std::vector<double> tmp = {};                                                                                   \
    for (auto val : (yaml_node) ) tmp.push_back((yaml_node).as<double>());                                          \
    *get<6>(var) = tmp;                                                                                             \
    } else if ((var_type).find("string") != std::string::npos && (var_type).find("vector") != std::string::npos) {  \
    std::vector<std::string> tmp = {};                                                                              \
    for (auto val : (yaml_node) ) tmp.push_back((yaml_node).as<std::string>());                                     \
    *get<7>(var) = tmp;                                                                                             \
    }

#endif //PCTSIMULATION_PANALYZER_H
