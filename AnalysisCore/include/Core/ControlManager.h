#ifndef PCTSIMULATION_CONTROLMANAGER_H
#define PCTSIMULATION_CONTROLMANAGER_H

#include "EventReader.h"
#include "ConfigManager.h"

class ControlManager {
public:
    static ControlManager *CreateInstance();

    void run();

    void generate_config();

private:
    ControlManager();

public:
    // Delete Copy constructor
    ControlManager(const ControlManager &) = delete;

    ControlManager &operator=(ControlManager const &) = delete;
};

extern ControlManager *pControlMgr;

#endif //PCTSIMULATION_CONTROLMANAGER_H
