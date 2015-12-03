#ifndef FINGERS_H
#define FINGERS_H

#include <cstdint>
#include <iostream>

class Fingers
{
public:
	typedef std::uint8_t Value;
	static const Value DEAD_VALUE = 0;
	static const Value LARGEST_VALUE = 4;
	static const Value NUM_VALUES = LARGEST_VALUE+1;

	explicit Fingers(Value num_fingers);

	static Fingers Dead();

	Fingers(const Fingers & other);
	Fingers & operator=(const Fingers & other);

	Value get() const;
	bool isAlive() const;

	Fingers & operator+=(const Fingers & other);
	Fingers operator+(const Fingers & other) const;

	bool operator==(const Fingers & other) const;
	bool operator<(const Fingers & other) const;

	bool operator!=(const Fingers & other) const;
	bool operator>(const Fingers & other) const;
	bool operator<=(const Fingers & other) const;
	bool operator>=(const Fingers & other) const;

	char asChar() const;
	std::string asString() const;

private:
	void check() const;
	static void check(Value v);
	static Value deadToLarge(Value v);
	static Value largeToDead(Value v);

	Value m_fingers;
};

std::ostream & operator<<(std::ostream & s, const Fingers & f);

void TestFingers();

#endif // FINGERS_H
