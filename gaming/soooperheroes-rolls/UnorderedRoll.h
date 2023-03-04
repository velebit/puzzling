#ifndef UNORDERED_ROLL__H
#define UNORDERED_ROLL__H

#include "Tuple.h"
#include "factorial.h"

#include <string>

namespace dice
{

	// An unordered roll of D dice with N sides each

	class UnorderedRoll
	{
	public:
		typedef combinatorics::real_t weight_t;
		typedef unsigned int          uint_t;

		static const uint_t NUM_SIDES = 10;

		// --- construction and destruction ---

		// Create a specific roll
		UnorderedRoll(uint_t n1, uint_t n2, uint_t n3, uint_t n4, uint_t n5,
					  uint_t n6, uint_t n7, uint_t n8, uint_t n9, uint_t n10)
			: m_dice()
		{
			m_dice[1]  = n1;
			m_dice[2]  = n2;
			m_dice[3]  = n3;
			m_dice[4]  = n4;
			m_dice[5]  = n5;
			m_dice[6]  = n6;
			m_dice[7]  = n7;
			m_dice[8]  = n8;
			m_dice[9]  = n9;
			m_dice[10] = n10;

			m_weight = computeWeight();
		}

		// Create an empty roll
	    UnorderedRoll()
			: m_dice(), m_weight(1)
		{
		}

		// --- copy construction and assignment ---

		// Copy constructor
	    UnorderedRoll(const UnorderedRoll& roll)
			: m_dice(roll.m_dice), m_weight(roll.m_weight)
		{
		}

		// Assignment
	    UnorderedRoll& operator=(const UnorderedRoll& roll)
		{
			m_dice   = roll.m_dice;
			m_weight = roll.m_weight;
			return *this;
		}

		// --- iteration ---

		// Initial value for iteration
		static UnorderedRoll begin(uint_t numDice)
		{
			return UnorderedRoll(numDice, 0,0,0,0,0,0,0,0,0);
		}
			
		bool increment()
		{
			// TODO: move implementation to .cxx?  [2-3% slower]
			if (m_dice[1])
			{
				++(m_dice[2]);
				m_weight = m_weight * m_dice[1] / m_dice[2];
				--(m_dice[1]);
			} else {
				uint_t nonzeroIndex = 2;
				while (m_dice[nonzeroIndex] == 0)
				{
					++nonzeroIndex;
					if (nonzeroIndex >= NUM_SIDES)
					{
						return false;
					}
				}

				++(m_dice[nonzeroIndex+1]);
				m_weight = m_weight * m_dice[nonzeroIndex]
									/ m_dice[nonzeroIndex+1];
				m_dice[1]            = m_dice[nonzeroIndex] - 1;
				m_dice[nonzeroIndex] = 0;
			}
			return true;
		}

		// --- stringification ---

		std::string toString() const;

		// --- element access (read-only) ---

	    uint_t operator[](uint_t i) const
	    {
			return m_dice[i];
		}

		// --- aggregate information access ---

		uint_t getNumDice() const
		{
			uint_t n = 0;
			for (uint_t i = 1;  i <= NUM_SIDES;  ++i)
			{
				n += m_dice[i];
			}
			return n;
		}

		weight_t getWeight() const { return m_weight; }

	private:
		weight_t computeWeight()
		{
			// weight = ((sum d_i)! / prod(d_i !)) / 10^(sum d_i)
			//
			// We can always compute this relatively efficiently since
			// we don't have to worry about overflow of real numbers.

			uint_t numDice = getNumDice();
			weight_t weight = (combinatorics::rfact(numDice) *
							   combinatorics::rpower(1./NUM_SIDES, numDice));
			for (uint_t i = 1;  i <= NUM_SIDES;  ++i)
			{
				weight = weight / combinatorics::rfact(m_dice[i]);
			}
			return weight;
		}

	private:
		typedef combinatorics::Tuple<NUM_SIDES, unsigned char, 1> tuple_t;
		tuple_t  m_dice;
		weight_t m_weight;
	};
}

#endif // UNORDERED_ROLL__H
