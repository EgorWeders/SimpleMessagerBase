#include "client.h"

Client::Client() {}

bool Client::sendMessage(Client *reciever) {
	if (!reciever)
		return false;
	if (senderQueue.size() <= 0)
		return false;
	Message tempMsgd = senderQueue.back();
	if (this->sendMutator)
		tempMsgd.applyFilter(sendMutator);
	senderQueue.pop_back();
	return reciever->receiveMessage(tempMsgd);
}

size_t Client::sendAll(Client *reciever)
{
    int res = 0;
	;
	while (!senderQueue.empty()) {
		res += sendMessage(reciever);
	}
	return res;
}

bool Client::receiveMessage(const Message &msg) {
	if (!msg.isValid())
		return false;
	if (this->recvMutator) {
		Message tempMsgd = msg;
		tempMsgd.applyFilter(recvMutator);

		recvQueue.insert(recvQueue.begin(), tempMsgd);
	} else {
		recvQueue.insert(recvQueue.begin(), msg);
	}
	return true;
}

size_t Client::appendMessage(const char body[], size_t size) {
	char *id = new char[6];
    strlcpy(id, "dummy", 6);
    if (idGen) {
        delete[] id;
		id = idGen->newId();
    }
    if (size > Message::maxMessageSize()) {
		size_t counter = 0;
		size_t begin   = 0;

		while (size > 0) {
			size_t  msgSize = std::min(Message::maxMessageSize(), size);
			Message tempMsgd(id, body + begin, msgSize);
			if (tempMsgd.isValid()) {
				senderQueue.insert(senderQueue.begin(), tempMsgd);
				counter++;
			}

			begin += msgSize;
			size -= msgSize;
		}
		delete[] id;
		return counter;
	}
	Message tempMsgd(id, body, size);
	delete[] id;
	if (!tempMsgd.isValid()) {
		return false;
	}
	senderQueue.insert(senderQueue.begin(), tempMsgd);
	return true;
}

Message Client::getLastMessage() const {
	if (recvQueue.empty())
		return Message::NullMessage();
	return recvQueue.back();
}

Message Client::takeLastMessage() {
	if (recvQueue.empty())
		return Message::NullMessage();
	auto res = recvQueue.back();
	recvQueue.pop_back();
	return res;
}
