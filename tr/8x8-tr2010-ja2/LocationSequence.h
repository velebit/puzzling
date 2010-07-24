#ifndef LOCATION_SEQUENCE__H
#define LOCATION_SEQUENCE__H

#include "Location.h"

#include <vector>

class LocationSequence
{
private:
	static std::vector<Location> m_locationSequence;

protected:
	LocationSequence();

public:
	static const std::vector<Location>& get() { return m_locationSequence; }
};

#endif // LOCATION_SEQUENCE_BUILDER__H
