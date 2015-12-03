#ifndef PLAYERS_H
#define PLAYERS_H

#include <cstdint>
#include <iostream>

#include "Fingers.h"
#include "Hands.h"


class Players
{
public:
	typedef std::uint16_t SeqNum;
	static const SeqNum NUM_SEQ_NUMS = (Hands::NUM_SEQ_NUMS *
										(Hands::NUM_SEQ_NUMS-1));

	Players();
	Players(Hands a, Hands b);

	static Players FromSeq(SeqNum sn);

	Players(const Players & other);
	Players & operator=(const Players & other);

	Hands next() const;
	Hands other() const;

	SeqNum seq() const;

	Players addHighToHigh() const;
	Players addLowToHigh() const;
	Players addHighToLow() const;
	Players addLowToLow() const;

	bool operator==(const Players & other) const;
	bool operator!=(const Players & other) const;

private:
	void check() const;

	Hands m_next;
	Hands m_other;
};

std::ostream & operator<<(std::ostream & s, const Players & p);

Players MakePlayers(Fingers::Value ah, Fingers::Value al,
					Fingers::Value bh, Fingers::Value bl);

void TestPlayers();

#endif // PLAYERS_H
