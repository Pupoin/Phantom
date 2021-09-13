//
// Created by Zhang Yulei on 8/11/21.
//

#ifndef PCTSIMULATION_PCTANALYZER_H
#define PCTSIMULATION_PCTANALYZER_H

#include "DataIO/PCTEvent.h"

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
class PCTAnalyzer {
public:
    PCTAnalyzer() = default;

    virtual ~PCTAnalyzer() = default;

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

    virtual void ProcessEvt(PCTEvent *) = 0;

    virtual void CheckEvt(PCTEvent *) = 0;

    virtual void End() = 0;

    // Overload output operator
    friend std::ostream &operator<<(std::ostream &os, const PCTAnalyzer &analyzer) {
        os << "name: " << analyzer.name;
        return os;
    }

protected:
    string name;
    string description;

    int Verbose{0};

private:
    MapAnaVar ana_var_col;
};

template<typename T>
void PCTAnalyzer::RegisterParameter(const string &var_name, T *var, T default_value, const string &var_description,
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
    if ((var_type) == "bool") *std::get<0>(var) = (yaml_node).as<bool>();                                                \
    else if ((var_type) == "int") *std::get<1>(var) = (yaml_node).as<int>();                                             \
    else if ((var_type) == "double") *std::get<2>(var) = (yaml_node).as<double>();                                       \
    else if ((var_type).find("string") != std::string::npos && (var_type).find("vector") == std::string::npos)      \
                                *std::get<3>(var) = (yaml_node).as<string>();                                       \
    else if ((var_type).find("bool") != std::string::npos && (var_type).find("vector") != std::string::npos) {      \
        std::vector<bool> tmp = {};                                                                                 \
        for (auto val : (yaml_node) ) tmp.push_back(val.as<bool>());                                        \
        *std::get<4>(var) = tmp;                                                                                         \
    } else if ((var_type).find("int") != std::string::npos && (var_type).find("vector") != std::string::npos) {     \
    std::vector<int> tmp = {};                                                                                      \
    for (auto val : (yaml_node) ) tmp.push_back(val.as<int>());                                             \
    *std::get<5>(var) = tmp;                                                                                             \
    } else if ((var_type).find("double") != std::string::npos && (var_type).find("vector") != std::string::npos) {  \
    std::vector<double> tmp = {};                                                                                   \
    for (auto val : (yaml_node) ) tmp.push_back(val.as<double>());                                          \
    *std::get<6>(var) = tmp;                                                                                             \
    } else if ((var_type).find("string") != std::string::npos && (var_type).find("vector") != std::string::npos) {  \
    std::vector<std::string> tmp = {};                                                                              \
    for (auto val : (yaml_node) ) tmp.push_back(val.as<std::string>());                                     \
    *std::get<7>(var) = tmp;                                                                                             \
    }

// Define the macro to Print AnaVar
#define printAnaVar(var, var_type)                                                                                  \
    if ((var_type) == "bool") printf("%d", *std::get<0>(var));                                                      \
    else if ((var_type) == "int") printf("%d", *std::get<1>(var));                                                  \
    else if ((var_type) == "double") printf("%f", *std::get<2>(var));                                               \
    else if ((var_type).find("string") != std::string::npos && (var_type).find("vector") == std::string::npos)      \
                                printf("\"%s\"", std::get<3>(var)->c_str());                                       \
    else if ((var_type).find("bool") != std::string::npos && (var_type).find("vector") != std::string::npos) {      \
        if (!std::get<4>(var)->empty()) {                                                                            \
            printf("[ %d", (int)std::get<4>(var)->front());                                                              \
            for (unsigned i = 1; i < std::get<4>(var)->size(); ++i) printf(", %d", (int)std::get<4>(var)->at(i));      \
            printf(" ]");                                                                                           \
        } else printf("[ ]");                                                                                       \
    }  else if ((var_type).find("int") != std::string::npos && (var_type).find("vector") != std::string::npos) {    \
        if (!std::get<5>(var)->empty()) {                                                                            \
            printf("[ %d", std::get<5>(var)->front());                                                              \
            for (unsigned i = 1; i < std::get<5>(var)->size(); ++i) printf(", %d", std::get<5>(var)->at(i));        \
            printf(" ]");                                                                                           \
        } else printf("[ ]");                                                                                       \
    }  else if ((var_type).find("double") != std::string::npos && (var_type).find("vector") != std::string::npos) { \
        if (!std::get<6>(var)->empty()) {                                                                            \
            printf("[ %f", std::get<6>(var)->front());                                                              \
            for (unsigned i = 1; i < std::get<6>(var)->size(); ++i) printf(", %f", std::get<6>(var)->at(i));        \
            printf(" ]");                                                                                           \
        } else printf("[ ]");                                                                                       \
    }  else if ((var_type).find("string") != std::string::npos && (var_type).find("vector") != std::string::npos) { \
        if (!std::get<7>(var)->empty()) {                                                                            \
            printf("[ \"%s\"", std::get<7>(var)->front().c_str());                                                  \
            for (unsigned i = 1; i < std::get<7>(var)->size(); ++i) printf(", \"%s\"", std::get<7>(var)->at(i).c_str());\
            printf(" ]");                                                                                           \
        } else printf("[ ]");                                                                                       \
    }

#endif //PCTSIMULATION_PCTANALYZER_H
