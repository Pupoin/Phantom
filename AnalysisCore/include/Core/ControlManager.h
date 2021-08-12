#ifndef PCTSIMULATION_CONTROLMANAGER_H
#define PCTSIMULATION_CONTROLMANAGER_H

#include "EventReader.h"
#include "ConfigManager.h"

class ControlManager {
public:
    static ControlManager *CreateInstance();

    void initialize();

    void run();

    static void generate_config();

    void setVerbose(int verbose) { Verbose = verbose; }

private:
    ControlManager();

    friend class ConfigManager;

    std::string file_name;
    std::string geom_name;
    std::string input_tree;
    std::string outfile_name;

    int Verbose{0};

public:
    // Delete Copy constructor
    ControlManager(const ControlManager &) = delete;

    ControlManager &operator=(ControlManager const &) = delete;
};

extern ControlManager *pControlMgr;

#endif //PCTSIMULATION_CONTROLMANAGER_H
