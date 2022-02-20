
//
// Created by hujf@mail.scnu.edu.cn on 11/23/2021.
//

#ifndef PSIM_PCTHIT_H
#define PSIM_PCTHIT_H

#include <string>
#include <ostream>

#include "TObject.h"
#include "TString.h"

class PCTHit : public TObject 
{
public:
    PCTHit() = default;

    PCTHit(const PCTHit &rhs) : TObject(rhs)
    {
        *this = rhs;
    }
    ~PCTHit() override = default;

    // Operators
    bool operator==(const PCTHit &rhs) const
    {
        return id == rhs.id &&
               X == rhs.X &&
               Y == rhs.Y &&
               Z == rhs.Z &&
               T == rhs.T &&
               E == rhs.E &&
               PVName == rhs.PVName &&
               ProcessName == rhs.ProcessName;
    }

    bool operator!=(const PCTHit &rhs) const
    {
        return !(rhs == *this);
    }

    PCTHit &operator=(const PCTHit &rhs)
    {
        if (&rhs != this)
        {
            id = rhs.id;
            X = rhs.X;
            Y = rhs.Y;
            Z = rhs.Z;
            E = rhs.E;
            PVName = rhs.PVName;
            ProcessName = rhs.ProcessName;
        }
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const PCTHit &step)
    {
        TString str(
            Form("|  %-5d  |  %8.3f, %8.3f, %8.3f, %8.3f, |  %10.5f,  |   %-21s   %-15s  |",
                 step.id, step.X, step.Y, step.Z, step.T, 
                 step.E, step.PVName.data(), step.ProcessName.data()));
        os << str;
        return os;
    }

    // Get Methods
    int getId() const
    {
        return id;
    }

    double getX() const
    {
        return X;
    }

    double getY() const
    {
        return Y;
    }

    double getZ() const
    {
        return Z;
    }
    double getT() const
    {
        return T;
    }

    double getE() const
    {
        return E;
    }

    const std::string &getPVName() const
    {
        return PVName;
    }

    const std::string &getProcessName() const
    {
        return ProcessName;
    }

    // Set Methods
    void setId(int ID)
    {
        id = ID;
    }

    void setX(double x)
    {
        X = x;
    }

    void setY(double y)
    {
        Y = y;
    }

    void setZ(double z)
    {
        Z = z;
    }

    void setT(double t)
    {
        T = t;
    }

    void setE(double e)
    {
        E = e;
    }

    void setPVName(const std::string &pvName)
    {
        PVName = pvName;
    }

    void setProcessName(const std::string &processName)
    {
        ProcessName = processName;
    }

private:
    int id{0};
    double X{0.}; // hit position x component
    double Y{0.};
    double Z{0.};
    double T{0.}; // time
    double E{0.}; // total energy depositi
    std::string PVName;
    std::string ProcessName;

    ClassDefOverride(PCTHit, 1);
};

#endif // PSIM_PCTHIT_H