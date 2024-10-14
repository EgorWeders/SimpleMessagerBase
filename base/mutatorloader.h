#ifndef MUTATORLOADER_H
#define MUTATORLOADER_H
#include "base/mutator.h"
#include <dlfcn.h>
#include <iostream>
#include <string>
#include <vector>
typedef Mutator *(*CreateMutatorInstance)();
class MutatorLib {
  void *_handle;
  CreateMutatorInstance _func;
  std::string _path;
  friend class MutatorLoader;

public:
  MutatorLib(const char filepath[]) {
    _handle = dlopen(filepath, RTLD_LAZY);
    if (!_handle) {
      std::cerr << dlerror() << std::endl;
    }
    _func = reinterpret_cast<CreateMutatorInstance>(
        dlsym(_handle, "createInstance"));
    if (!_func) {
      std::cerr << dlerror() << std::endl;
    }
  }
  ~MutatorLib() {

    if (_handle && dlclose(_handle) != 0) {
      std::cerr << dlerror() << std::endl;
    }
  }
};
class MutatorLoader {
  std::vector<MutatorLib *> _libs;

public:
  MutatorLoader();
  size_t size() { return _libs.size(); }
  bool load(const char filepath[]) {
    MutatorLib *lib = new MutatorLib(filepath);
    if (lib->_handle != nullptr && lib->_func != nullptr) {
      _libs.push_back(lib);
      return true;
    } else {
      delete lib;
    }
    return false;
  }
  Mutator *createFrom(size_t index) { return _libs.at(index)->_func(); }
  ~MutatorLoader() {}
};

#endif // MUTATORLOADER_H
