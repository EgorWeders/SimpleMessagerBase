#ifndef MUTATOR_H
#define MUTATOR_H
/*
 * Base class used to change Message content char by char
 * can be used as enryption util
 */
#include <assert.h>
#include <cstring>
#include <string>
class Message;
class Mutator {
public:
  enum CallCase {
    Recieve = 0b001,
    Send = 0b010,
    SendRecieve = 0b011,
    Special = 0b100 // need to call if msg meets conditions from checkMsg
  };
  bool skippable = false;
  int _flags;

private:
  std::string _id;

public:
  Mutator(const std::string &id) : _id(id) { assert(id.size() > 0); };
  virtual ~Mutator() {}
  virtual std::string id() { return _id; }
  virtual void reset() {}
  virtual char apply(char &in) { return in; }
  virtual bool checkMsg(const Message &msg);
  bool checkFlag(int flags) { return (this->_flags & flags) == flags; }
  int csdn_paged_array_set_flag(int flags) {
    this->_flags |= flags;
    return _flags;
  }
  int csdn_paged_array_unset_flag(int flags) {
    this->_flags &= ~flags;
    return _flags;
  }
  bool isSkippable() const { return skippable; }
  void setSkippable(bool value) { skippable = value; }
};

#endif // MUTATOR_H
