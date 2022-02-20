#pragma once

#include "Json2Geant4Macro.hh"
#include <iostream>

G4String trim(const Json::Value& jvalue){
    G4String value(jvalue.asString());
    return value.strip(0, '"');
}

bool json2macro(const Json::Value &root, const std::string &outFile)
{
    bool isSucc = false;
    std::ofstream cmd(outFile, std::ios::out);
    if (cmd.good())
    {
        try
        {
            Json::String gps = root["beam"]["source"].asString();
            if ((gps == Json::String("gps")))
            {
                cmd << "/run/initialize\n";
                cmd << "/gps/particle ";
                cmd << trim(root["beam"]["setup"]["particle"]) << "\n";
                cmd << "/gps/direction ";
                cmd << trim(root["beam"]["setup"]["direction"]) << "\n";

                cmd << "/gps/ene/type ";
                cmd << trim(root["beam"]["setup"]["ene"]["type"]) << "\n";
                cmd << "/gps/ene/min ";
                cmd << trim(root["beam"]["setup"]["ene"]["min"]) << "\n";
                cmd << "/gps/ene/max ";
                cmd << trim(root["beam"]["setup"]["ene"]["max"]) << "\n";
                cmd << "/gps/ene/gradient ";
                cmd << trim(root["beam"]["setup"]["ene"]["gradient"]) << "\n";
                cmd << "/gps/ene/intercept ";
                cmd << trim(root["beam"]["setup"]["ene"]["intercept"]) << "\n";

                cmd << "/gps/pos/type ";
                cmd << trim(root["beam"]["setup"]["pos"]["type"]) << "\n";
                cmd << "/gps/pos/shape ";
                cmd << trim(root["beam"]["setup"]["pos"]["shape"]) << "\n";
                cmd << "/gps/pos/centre ";
                cmd << trim(root["beam"]["setup"]["pos"]["centre"]) << "\n";
                cmd << "/gps/pos/halfx ";
                cmd << trim(root["beam"]["setup"]["pos"]["halfx"]) << "\n";
                cmd << "/gps/pos/halfy ";
                cmd << trim(root["beam"]["setup"]["pos"]["halfy"]) << "\n";

                cmd << "/random/setSeeds ";
                cmd << trim(root["random_seed"]) << "\n";

                cmd << "/run/beamOn ";
                cmd << trim(root["beam_on"]) << "\n";

                isSucc = true;
            }
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            isSucc = false;
        }
        cmd.flush();
        cmd.close();
    }

    return isSucc;
}
