# Testing ground for a messenger based on qt and mutator mechanism (a kind of visitor boost) 

(The project was created for testing and training) 
# Goal: 
To make a messeger for transfers through third-party applications and protocols (SMTP, temporary google-drive folders, other messagers and other places not intended for this) and give user a choice how to encrypt that messages with versatile mutator mechanism (idk how to name it other way).

## Use case: 
You and your friend (Bob and Alice probably) agree on the same encryption algorithms and libraries (perhaps even self-written). You exchange them (even offline). Choose an third-party app or protocol to store and transmit message body (encrypted) (my application is not designed to store messages, only to send and receive,storage may be implemented through mutators, by the way). And you communicate through a single interface. Moreover, any step can be repeated and this should not change the behavior under the conditions: you use the correct mutators for each other and use the same application or protocol for transmission

# Important!

All files in other directories than 'base' will be removed or moved to other repos.
