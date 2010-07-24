#include "LocationSequence.h"
#include "LocationSequenceBuilder.h"

std::vector<Location> LocationSequence::m_locationSequence =
	LocationSequenceBuilder().getLocations();
