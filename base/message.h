#ifndef MESSAGE_H
#define MESSAGE_H
#include "mutator.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <string.h>
#include <string>

#define MSG_SIZE 3 // fixed size
#define ID_SIZE  64
/*
 * Base class for Message
 * Wraps body of char array with fixed MSG_SIZE
 * Useful content size stored in _size var
 */
class Message final {
private:
	char   _body[MSG_SIZE];
	size_t _size  = 0;
	bool   nulled = true;
	char   _id[ID_SIZE];

public:
	Message();
	Message(const char data[], const size_t &dataSize);
	Message(const char id[], const char body[], const size_t &bodySize);
	~Message();
	const char *body() const;
	size_t      bodySize() const;
	bool        isValid() const;
	bool        isNull() const { return nulled; };
	std::string stringBody() const;
	void        applyFilter(Mutator *mut);
	char *      data() const {
		char *_data = new char[_size + ID_SIZE];
		std::memset(_data, '0', _size + ID_SIZE);
		std::copy(_id, _id + ID_SIZE - 1, _data);
		std::copy(_body, _body + _size, _data + ID_SIZE - 1);
		return _data;
	}
	const char *        id() { return _id; }
	size_t              dataSize() { return _size + ID_SIZE; }
	void                applyFilter(char (*mutator)(char &));
	static const size_t maxMessageSize();
	static Message      NullMessage() { return Message(); }
};

#endif // MESSAGE_H
