#include "Hands.h"

#include <cstdint>
#include <iostream>
#include <cassert>

Hands::Hands(Fingers a, Fingers b) :
	m_high(a),
	m_low(b)
{
	if (m_high < m_low) {
		std::swap(m_high, m_low);
	}
	check();
}

Hands Hands::Dead()
{
	return Hands(Fingers::Dead(), Fingers::Dead());
}

Hands Hands::FromSeq(Hands::SeqNum sn)
{
	Fingers::Value high = 0;
	while ((high < Fingers::LARGEST_VALUE) &&
		   (Hands(Fingers(high+1), Fingers(0)).seq() <= sn))
	{
		++high;
	}
	Fingers::Value low = sn - Hands(Fingers(high), Fingers(0)).seq();
	Hands result = Hands(Fingers(high), Fingers(low));
	assert(result.seq() == sn);
	return result;
}

Hands::Hands(const Hands & other) :
	m_high(other.m_high),
	m_low(other.m_low)
{
	check();
}

Hands & Hands::operator=(const Hands & other)
{
	m_high = other.m_high;
	m_low = other.m_low;
	check();
}

Fingers Hands::high() const
{
	return m_high;
}
Fingers Hands::low() const
{
	return m_low;
}

bool Hands::isAlive() const
{
	check();
	return m_high.isAlive();
}

Hands::SeqNum Hands::seq() const
{
	check();
	SeqNum high = m_high.get();
	SeqNum sn = (((high+1) * high / 2) + m_low.get());
	//std::cout << *this << " -> " << sn << std::endl;
	return sn;
}

Hands Hands::addToHigh(Fingers f) const
{
	check();
	assert(f.isAlive());
	return Hands(m_high + f, m_low);
}

Hands Hands::addToLow(Fingers f) const
{
	check();
	assert(f.isAlive());
	return Hands(m_low + f, m_high);
}

bool Hands::operator==(const Hands & other) const
{
	return ((m_high == other.m_high) && (m_low == other.m_low));
}

bool Hands::operator!=(const Hands & other) const
{
	return !(*this == other);
}

void Hands::check() const
{
	//m_high.check();
	//m_low.check();
	assert(m_high >= m_low);
}


std::ostream & operator<<(std::ostream & s, const Hands & h)
{
	s << h.high() << "," << h.low();
	return s;
}


Hands MakeHands(Fingers::Value a, Fingers::Value b)
{
	return Hands(Fingers(a), Fingers(b));
}

void TestHands()
{
	Hands h(Fingers(1), Fingers::Dead());
	assert(h.low() == Fingers::Dead());
	assert(h.high() == Fingers(1));
	Hands k(Fingers::Dead(), Fingers(2));
	assert(k.low() == Fingers::Dead());
	assert(k.high() == Fingers(2));

	assert(MakeHands(1, 2).high() == Fingers(2));
	assert(MakeHands(2, 1).high() == Fingers(2));
	assert(MakeHands(3, 3).high() == Fingers(3));
	assert(MakeHands(1, 2).low() == Fingers(1));
	assert(MakeHands(2, 1).low() == Fingers(1));
	assert(MakeHands(3, 3).low() == Fingers(3));

	for (Hands::SeqNum i = 0;  i < Hands::NUM_SEQ_NUMS;  ++i)
	{
		Hands h = Hands::FromSeq(i);
		//std::cout << h << std::endl;
		assert(h.seq() == i);
	}

	//Hands::FromSeq(Hands::NUM_SEQ_NUMS);  // should abort!

	std::cout << __FUNCTION__ << ": passed" << std::endl;
}
