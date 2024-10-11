#ifndef IDGENERATOR_H
#define IDGENERATOR_H
#include <atomic>
#include <cstring>
#include <string>
class IdGenerator {

public:
	IdGenerator(){};
	virtual char *newId() = 0;
	virtual void  reset() = 0;
};
class IdCounterGenerator : public IdGenerator {
	std::atomic_int counter = 0;

public:
    IdCounterGenerator(){};
    char *newId() override {
		std::string strData = std::to_string(counter++);
		char *      temp    = new char[strData.length() + 1];
        strlcpy(temp, strData.c_str(), strData.size() + 1);
        return temp;
    }
	void reset() override { counter = 0; };
};

#endif // IDGENERATOR_H
