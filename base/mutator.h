#ifndef MUTATOR_H
#define MUTATOR_H
/*
 * Base class used to change Message content char by char
 * can be used as enryption util
 */
#include <assert.h>
#include <cstring>
#include <string>
#include <vector>
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
  Mutator(const std::string &id) : _id(id) { assert(id.size() > 0); }
  Mutator(const Mutator &mut)                = delete;
  Mutator     &operator=(const Mutator &mut) = delete;
  virtual bool isNull() const { return false; }
  virtual ~Mutator() {}
  virtual void reset() {}
  virtual char apply(char &in) { return in; }
  virtual bool checkMsg(const Message &msg) = 0;
  bool checkFlag(int flags) { return (this->_flags & flags) == flags; }
  int setFlag(int flags) {
    this->_flags |= flags;
    return _flags;
  }
  int unsetFlag(int flags) {
    this->_flags &= ~flags;
    return _flags;
  }
  bool isSkippable() const { return skippable; }
  void setSkippable(bool value) { skippable = value; }
  std::string id() const
  {
    return _id;
  }
};

// NullMutator only for using instead of nullptr for return and reduce heap usage

class NullMutator : public Mutator {
  // Mutator interface
public:
  NullMutator() : Mutator("NULL mutator") {}
  void reset() override{};
  char apply(char &in) override { return in; };
  bool checkMsg(const Message &msg) override { return false; };
  bool isNull() const override { return true; }
};
class MutatorQueue {
private:
  std::vector<Mutator *> _mutVec;
  Mutator::CallCase      _type;

public:
  MutatorQueue(Mutator::CallCase type = Mutator::CallCase::SendRecieve) : _type{type} {}
  // use position param for concurrent mutators use
  size_t            size() { return _mutVec.size(); }
  bool              atEnd(size_t pos) const { return pos > _mutVec.size(); }
  Mutator::CallCase getType() const { return _type; }
  int               indexOf(std::string id, size_t startPos = 0) {
                  Mutator *curMut = nullptr;
                  for (size_t i = startPos; i < _mutVec.size(); i++) {
                    curMut = _mutVec.at(i);
                    if (curMut && curMut->id() == id) {
                      return i;
      }
    }
                  return -1;
  }
  void insert(Mutator *mut, size_t index) {
    if (index >= _mutVec.size()) {
      _mutVec.push_back(mut);
    } else {
      _mutVec.insert(_mutVec.begin() + index, mut);
    }
  }
  void append(Mutator *mut) { _mutVec.push_back(mut); }
  bool remove(size_t index) {
    if (index >= _mutVec.size()) {
      return false;
    } else {
      _mutVec.erase(_mutVec.begin() + index);
      return true;
    }
  }
  Mutator *getAt(size_t index) const {
    if (index >= _mutVec.size()) {
      return nullptr;
    } else {
      return _mutVec.at(index);
    }
  }
  char apply(char input) {
    Mutator *curMut = nullptr;
    for (size_t i = 0; i < _mutVec.size(); i++) {
      curMut = _mutVec.at(i);
      if (curMut) {
        input = curMut->apply(input);
      }
    }
    return input;
  }
};

#endif // MUTATOR_H
