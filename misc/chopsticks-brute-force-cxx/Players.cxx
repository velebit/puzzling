#include "Players.h"

#include <cstdint>
#include <iostream>
#include <cassert>


Players::Players() :
	m_next(Fingers(1), Fingers(1)),
	m_other(Fingers(1), Fingers(1))
{
	check();
}

Players::Players(Hands a, Hands b) :
	m_next(a),
	m_other(b)
{
	check();
}

Players Players::FromSeq(SeqNum sn) {
	Hands::SeqNum next_sn = sn / (Hands::NUM_SEQ_NUMS-1);
	Hands::SeqNum other_sn = (sn % (Hands::NUM_SEQ_NUMS-1)) + 1;
	return Players(Hands::FromSeq(next_sn), Hands::FromSeq(other_sn));
}

Players::Players(const Players & other) :
	m_next(other.m_next),
	m_other(other.m_other)
{
	check();
}

Players & Players::operator=(const Players & other)
{
	m_next = other.m_next;
	m_other = other.m_other;
	check();
	return *this;
}

Hands Players::next() const
{
	return m_next;
}
Hands Players::other() const
{
	return m_other;
}

Players::SeqNum Players::seq() const
{
	check();
	SeqNum sn = ((m_next.seq() * (Hands::NUM_SEQ_NUMS-1)) + (m_other.seq())-1);
	//std::cout << *this << " -> " << sn << std::endl;
	return sn;
}

Players Players::addHighToHigh() const
{
	check();
	assert(m_next.isAlive());
	return Players(m_other.addToHigh(m_next.high()), m_next);
}
Players Players::addLowToHigh() const
{
	check();
	assert(m_next.isAlive());
	return Players(m_other.addToHigh(m_next.low()), m_next);
}
Players Players::addHighToLow() const
{
	check();
	assert(m_next.isAlive());
	return Players(m_other.addToLow(m_next.high()), m_next);
}
Players Players::addLowToLow() const
{
	check();
	assert(m_next.isAlive());
	return Players(m_other.addToLow(m_next.low()), m_next);
}

bool Players::operator==(const Players & other) const
{
	return ((m_next == other.m_next) && (m_other == other.m_other));
}

bool Players::operator!=(const Players & other) const
{
	return !(*this == other);
}

void Players::check() const
{
	assert(m_other.isAlive());
}


std::ostream & operator<<(std::ostream & s, const Players & p)
{
	s << p.next() << "|" << p.other();
	return s;
}

Players MakePlayers(Fingers::Value ah, Fingers::Value al,
					Fingers::Value bh, Fingers::Value bl)
{
	return Players(Hands(Fingers(ah), Fingers(al)),
				   Hands(Fingers(bh), Fingers(bl)));
}

void TestPlayers()
{
	Players h(MakeHands(1, 1), MakeHands(2, 2));
	assert(h.next() == MakeHands(1, 1));
	assert(h.other() == MakeHands(2, 2));
	Players k(Hands::Dead(), MakeHands(3, 3));
	assert(k.next() == Hands::Dead());
	assert(k.other() == MakeHands(3, 3));

	for (Players::SeqNum i = 0;  i < Players::NUM_SEQ_NUMS;  ++i)
	{
		Players p = Players::FromSeq(i);
		//std::cout << p << std::endl;
		assert(p.seq() == i);
	}

	//Players::FromSeq(Players::NUM_SEQ_NUMS);  // should abort!

	std::cout << __FUNCTION__ << ": passed" << std::endl;
}
