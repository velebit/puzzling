#include "UnorderedRoll.h"

#include <string>
#include <sstream>


// --- stringification ---

std::string dice::UnorderedRoll::toString() const
{
	std::ostringstream result;
	result << "{ ";
	for (int i = NUM_SIDES;  i > 0;  --i)
	{
		result << i << "x"
			   << static_cast<unsigned long>(m_dice[i]) << " ";
	}
	result << "}";
	return result.str();
}
