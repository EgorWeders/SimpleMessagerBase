#ifndef SPANMUTATOR_H
#define SPANMUTATOR_H

#include "base/mutator.h"
#include <ctype.h>
class LowerMutator : public Mutator {

public:
  LowerMutator() : Mutator("Lower mutator"){};
  virtual char apply(char &in) override { return tolower(in); }
};
class UpperMutator : public Mutator {

public:
  UpperMutator() : Mutator("Upper mutator"){};
  virtual char apply(char &in) override { return toupper(in); }
};
#endif // SPANMUTATOR_H
