#include "Fingers.h"

#include <cstdint>
#include <iostream>
#include <cassert>

Fingers::Fingers(Value num_fingers) :
	m_fingers(num_fingers)
{
	check();
}

Fingers Fingers::Dead()
{
	return Fingers(DEAD_VALUE);
}

Fingers::Fingers(const Fingers & other) :
	m_fingers(other.m_fingers)
{
	check();
}

Fingers & Fingers::operator=(const Fingers & other)
{
	m_fingers = other.m_fingers;
	check();
	return *this;
}

Fingers::Value Fingers::get() const
{
	return m_fingers;
}

bool Fingers::isAlive() const
{
	check();
	return (m_fingers != DEAD_VALUE);
}

Fingers & Fingers::operator+=(const Fingers & other)
{
	check();
	other.check();
	m_fingers = largeToDead(deadToLarge(m_fingers) +
							deadToLarge(other.m_fingers));
	check();
	return *this;
}

Fingers Fingers::operator+(const Fingers & other) const
{
	Fingers result(*this);
	result += other;
	return result;
}

bool Fingers::operator==(const Fingers & other) const
{
	return (m_fingers == other.m_fingers);
}
bool Fingers::operator<(const Fingers & other) const
{
	return (m_fingers < other.m_fingers);
}

bool Fingers::operator!=(const Fingers & other) const
{
	return !(*this == other);
}
bool Fingers::operator>(const Fingers & other) const
{
	return other < *this;
}
bool Fingers::operator<=(const Fingers & other) const
{
	return !(other < *this);
}
bool Fingers::operator>=(const Fingers & other) const
{
	return !(*this < other);
}

char Fingers::asChar() const {
	static char letters[NUM_VALUES+1] = "x1234";
	check();
	return letters[m_fingers];
}

std::string Fingers::asString() const {
	std::string result = " ";
	result[0] = asChar();
	return result;
}

void Fingers::check() const {
	check(m_fingers);
}
void Fingers::check(Value v) {
	assert(v <= LARGEST_VALUE);
}

Fingers::Value Fingers::deadToLarge(Value v) {
	check(v);
	return ((v != DEAD_VALUE) ? v : 100);
}
Fingers::Value Fingers::largeToDead(Value v) {
	Value w = ((v <= LARGEST_VALUE) ? v : DEAD_VALUE);
	check(w);
	return w;
}

std::ostream & operator<<(std::ostream & s, const Fingers & f)
{
	s << f.asChar();
	return s;
}


void TestFingers()
{
	Fingers f(1);
	f.asString();
	assert(f.asString() == "1");
	assert(f.isAlive());

	//Fingers x(88);  // should abort!

	f += f;
	assert(f.asString() == "2");
	assert(f.isAlive());

	Fingers g(f + f);
	assert(g.asString() == "4");
	assert(g.isAlive());

	Fingers h = g;
	assert(h.asString() == "4");
	assert(h.isAlive());

	h += f;
	assert(h.asString() == "x");
	assert(! h.isAlive());

	f = g + Fingers(1);
	assert(f.asString() == "x");
	assert(! f.isAlive());

	f += h;
	assert(f.asString() == "x");
	assert(! f.isAlive());

	assert(Fingers::Dead().asString() == "x");


	assert((Fingers(2) == Fingers(4)) == false);
	assert((Fingers(2) != Fingers(4)) == true);
	assert((Fingers(2) < Fingers(4)) == true);
	assert((Fingers(2) > Fingers(4)) == false);
	assert((Fingers(2) <= Fingers(4)) == true);
	assert((Fingers(2) >= Fingers(4)) == false);

	assert((Fingers(4) == Fingers(4)) == true);
	assert((Fingers(4) != Fingers(4)) == false);
	assert((Fingers(4) < Fingers(4)) == false);
	assert((Fingers(4) > Fingers(4)) == false);
	assert((Fingers(4) <= Fingers(4)) == true);
	assert((Fingers(4) >= Fingers(4)) == true);

	assert((Fingers::Dead() == Fingers(4)) == false);
	assert((Fingers::Dead() != Fingers(4)) == true);
	assert((Fingers::Dead() < Fingers(4)) == true);
	assert((Fingers::Dead() > Fingers(4)) == false);
	assert((Fingers::Dead() <= Fingers(4)) == true);
	assert((Fingers::Dead() >= Fingers(4)) == false);

	assert((Fingers::Dead() == Fingers::Dead()) == true);
	assert((Fingers::Dead() != Fingers::Dead()) == false);
	assert((Fingers::Dead() < Fingers::Dead()) == false);
	assert((Fingers::Dead() > Fingers::Dead()) == false);
	assert((Fingers::Dead() <= Fingers::Dead()) == true);
	assert((Fingers::Dead() >= Fingers::Dead()) == true);


	std::cout << __FUNCTION__ << ": passed" << std::endl;
}
