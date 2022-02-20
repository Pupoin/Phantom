//created by hujf@scnu.edu.cn @ 2021.10.22
//
#pragma once

#include "globals.hh"
#include "G4VUserActionInitialization.hh"
#include <stdio.h>


class PctActionInitialization : public G4VUserActionInitialization
{
public:
  PctActionInitialization();
  virtual ~PctActionInitialization();

  virtual void Build() const;
  virtual void BuildForMaster() const;

private:
};
