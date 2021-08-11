#ifndef PCTSIMULATION_CONTROLMANAGER_H
#define PCTSIMULATION_CONTROLMANAGER_H

#include "EventReader.h"
#include "ConfigManager.h"

class ControlManager {
public:
    static ControlManager *CreateInstance();

    void initialize();

    void run();

    void generate_config();

private:
    ControlManager();

    int Verbose{0};

public:
    // Delete Copy constructor
    ControlManager(const ControlManager &) = delete;

    ControlManager &operator=(ControlManager const &) = delete;
};

extern ControlManager *pControlMgr;

#endif //PCTSIMULATION_CONTROLMANAGER_H
