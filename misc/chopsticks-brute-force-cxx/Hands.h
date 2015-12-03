#ifndef HANDS_H
#define HANDS_H

#include <cstdint>
#include <iostream>

#include "Fingers.h"


class Hands
{
public:
	typedef std::uint16_t SeqNum;
	static const SeqNum NUM_SEQ_NUMS = (((Fingers::NUM_VALUES+1) *
										  Fingers::NUM_VALUES) / 2);

	Hands(Fingers a, Fingers b);

	static Hands Dead();
	static Hands FromSeq(SeqNum sn);

	Hands(const Hands & other);
	Hands & operator=(const Hands & other);

	Fingers high() const;
	Fingers low() const;

	bool isAlive() const;

	SeqNum seq() const;

	Hands addToHigh(Fingers f) const;
	Hands addToLow(Fingers f) const;

	bool operator==(const Hands & other) const;
	bool operator!=(const Hands & other) const;

private:
	void check() const;

	Fingers m_high;
	Fingers m_low;
};

std::ostream & operator<<(std::ostream & s, const Hands & h);

Hands MakeHands(Fingers::Value a, Fingers::Value b);

void TestHands();

#endif // HANDS_H
