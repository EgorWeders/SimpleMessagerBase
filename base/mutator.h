#ifndef MUTATOR_H
#define MUTATOR_H
/*
 * Base class used to change Message content char by char
 * can be used as enryption util
 */
class Mutator {

public:
	Mutator();

	virtual void reset() {}
	virtual char apply(char &in) { return in; }
};

#endif // MUTATOR_H
