#include "mutator.h"
#include "message.h"

bool Mutator::checkMsg(const Message &msg) {
  if (msg.isNull() || !msg.isValid()) {
    return false;
  }
  // some checks
  return true;
}
