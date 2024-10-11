#include "message.h"

Message::Message() {
	// dummy
	std::memset(_id, '\0', ID_SIZE);
	std::memset(_body, '\0', MSG_SIZE);
}

Message::Message(const char data[], const size_t &dataSize) : Message() {
	assert(data);
	if (!data)
		return;
	nulled = false;
	_size  = dataSize - ID_SIZE;
	std::copy(data, data + ID_SIZE - 1, _id);
	_size = _size % (MSG_SIZE + 1);
	std::copy(data + ID_SIZE - 1, data + ID_SIZE + _size - 1, _body);
}
Message::Message(const char id[], const char body[], const size_t &bodySize) : Message() {
	assert(id);
	assert(body);
	if (!body)
		return;
	nulled = false;
	std::copy(id, id + strlen(id), _id);
	_size = bodySize % (MSG_SIZE + 1);
	std::copy(body, body + _size, this->_body);
}

Message::~Message() {}

const char *Message::body() const {
	if (isValid())
		return _body;
	else
		return nullptr;
}

size_t Message::bodySize() const {
	if (isValid())
		return _size;
	else
		return MSG_SIZE;
}

bool Message::isValid() const { return _size > 0; }

std::string Message::stringBody() const {
	if (isValid())
		return std::string(_body, bodySize());
	else
		return {};
}

void Message::applyFilter(Mutator *mut) {
	if (isValid())
		for (int i = 0; i < _size; i++) {
			_body[i] = mut->apply(_body[i]);
		}
}

void Message::applyFilter(char (*mutator)(char &)) {
	if (isValid())
		for (int i = 0; i < _size; i++) {
			_body[i] = mutator(_body[i]);
		}
}

const size_t Message::maxMessageSize() { return MSG_SIZE; }
