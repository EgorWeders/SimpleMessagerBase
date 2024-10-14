#ifndef CLIENT_H
#define CLIENT_H

#include "base/idgenerator.h"
#include "message.h"
#include "mutator.h"
#include <algorithm>
#include <vector>
const int CLIENT_ID_SIZE = 64;
/*
 * Base client class - provides queues Message objects (to send/ received)
 *
 * Can use Mutators (applying on char)
 */
class Client {
protected:
  std::vector<Message> senderQueue;
  std::vector<Message> recvQueue;
  Mutator *sendMutator = nullptr;
  Mutator *recvMutator = nullptr;
  IdGenerator *idGen = nullptr;

public:
  Client();
  ~Client() {}
  virtual void setSendMutator(Mutator *mut) { this->sendMutator = mut; }
  virtual void setRecvMutator(Mutator *mut) {
    this->recvMutator = mut;
  } // can be nullptr
  virtual bool sendMessage(Client *reciever);
  virtual bool sendMessage() { // Not implemented
    return false;
  };
  size_t sendAll(Client *reciever);
  size_t sendCount() { return senderQueue.size(); }
  size_t recvCount() { return recvQueue.size(); }
  virtual size_t sendAll() {
    size_t res = 0;
    ;
    while (!senderQueue.empty()) {
      res += sendMessage();
    }
    return res;
  };
  virtual bool receiveMessage(const Message &msg);
  virtual int receiveAll() { return false; };
  size_t appendMessage(const char body[], size_t size);
  Message getLastMessage() const;
  Message takeLastMessage();
  virtual bool hasMessage() { return !recvQueue.empty(); }
  void discardSendingMessages() { senderQueue.clear(); }
  void discardReceivedMessages() { recvQueue.clear(); }
};

#endif // CLIENT_H
