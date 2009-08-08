#ifndef SUCCESSES__H
#define SUCCESSES__H

#include "UnorderedRoll.h"

namespace dice
{

	// Calculate the number of successes, and other related parameters.

	class Successes
	{
	public:
		// --- construction ---

		Successes(const UnorderedRoll& roll,
				  unsigned int rollingFor = 7, unsigned int skill = 0)
		{
			if (rollingFor < 2)
				rollingFor = 2;

			int succ = 0;
			for (unsigned int i = UnorderedRoll::NUM_SIDES;
				 i >= rollingFor;
				 --i)
			{
				succ += roll[i];
			}
			m_naturalSuccesses = succ;

			m_wantedSkill = 0;
			for (unsigned int i = rollingFor-1;  i > 1;  --i)
			{
				if (i > UnorderedRoll::NUM_SIDES)
					continue;
				unsigned int dice     = roll[i];
				if (! dice)
					continue;

				unsigned int gap      = rollingFor - i;
				unsigned int canCover = skill / gap;
				if (canCover >= dice)
				{
					// Use skill to upgrade all of these dice.
					succ  += dice;
					skill -= dice * gap;
				}
				else
				{
					// Can't use skill to upgrade all of these dice.
					succ  += canCover;
					skill -= canCover * gap;
					m_wantedSkill = gap - skill;
					break;
				}
			}
			m_successes   = succ;
			m_unusedSkill = skill;
		}

	private:
		// --- copy construction and assignment ---

		// Prevent copy construction
	    Successes(const Successes& successes);

		// Prevent assignment
	    Successes& operator=(const Successes& successes);

	public:
		// --- results access ---

		unsigned int getNaturalSuccesses() const
		{
			return m_naturalSuccesses;
		}

		unsigned int getSuccesses() const
		{
			return m_successes;
		}

		unsigned int getUnusedSkill() const
		{
			return m_unusedSkill;
		}

		unsigned int getWantedSkill() const
		{
			return m_wantedSkill;
		}

	private:
		unsigned int m_naturalSuccesses;
		unsigned int m_successes;
		unsigned int m_unusedSkill;
		unsigned int m_wantedSkill;
	};
}

#endif // SUCCESSES__H
