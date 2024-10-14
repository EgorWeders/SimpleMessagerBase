#ifndef IDGENERATOR_H
#define IDGENERATOR_H
#include <atomic>
#include <cstring>
#include <string>
class IdGenerator {

public:
  IdGenerator(){};
  virtual char *newId() = 0;
  virtual char *newId(char *idDst) = 0;
  virtual size_t idSize() = 0;
  virtual void reset() = 0;
};
class IdCounterGenerator : public IdGenerator {
  std::atomic_int counter = 0;

public:
  IdCounterGenerator(){};
  char *newId() override {
    char *temp = new char[idSize() + 1];
    return newId(temp);
  }
  char *newId(char *idDst) override {
    std::string strData = std::to_string(counter);
    memcpy(idDst, strData.data(), strData.size() + 1); // null term
    counter++;
    return idDst;
  }
  size_t idSize() override { return std::to_string(counter).size(); }
  void reset() override { counter = 0; };
};

#endif // IDGENERATOR_H
