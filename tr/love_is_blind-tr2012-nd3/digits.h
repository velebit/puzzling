#ifndef DIGITS_H
#define DIGITS_H

/// A class that implements a set of unique digits (0-9).
class Digits {
public:
    Digits() : m_mask(0) {};

    Digits(const Digits& d)
	: m_mask(d.m_mask) {};

    Digits& operator=(const Digits& d) {
	m_mask = d.m_mask;
	return *this;
    }

    static Digits range(int from, int to) {
	Digits value;
	value.m_mask = (1U << (to+1)) - (1U << from);  // set all bits from-to
	return value;
    }

    static Digits all() {
	return range(0, 9);
    }

    static Digits none() {
	return Digits();
    }

    bool contains(int i) const {
	return (m_mask & (1U << i));
    }

    Digits& add(int i) {
	m_mask = m_mask | (1U << i);
	return *this;
    }

    Digits& remove(int i) {
	m_mask = m_mask & ~(1U << i);
	return *this;
    }

    bool inject(int i) {
	if (contains(i)) {
	    return false;
	} else {
	    add(i);
	    return true;
	}
    }

    bool eliminate(int i) {
	if (! contains(i)) {
	    return false;
	} else {
	    remove(i);
	    return true;
	}
    }

    Digits with(int i) const {
	return Digits(*this).add(i);
    }

    Digits without(int i) const {
	return Digits(*this).remove(i);
    }


    class iterator;

    inline iterator begin() const;
    inline iterator end() const;

private:
    unsigned int m_mask;
};



class Digits::iterator {
protected:
    friend class Digits;
    iterator(const Digits& digits, int i)
	: m_digits(digits), m_current(i)
    {
	while ((m_current < 10) && ! m_digits.contains(m_current)) {
	    ++m_current;
	}
    }

public:
    iterator(const iterator& it)
	: m_digits(it.m_digits), m_current(it.m_current) {};

    iterator& operator=(const iterator& it) {
	m_digits = it.m_digits;
	m_current = it.m_current;
	return *this;
    }

    int operator*() const {
	return m_current;
    }

    /// Prefix increment
    iterator operator++() {
	++m_current;
	while ((m_current < 10) && ! m_digits.contains(m_current)) {
	    ++m_current;
	}
	return *this;
    }

    /// Postfix increment
    iterator operator++(int) {
	iterator old(*this);
	++(*this);
	return old;
    }

    bool operator==(const iterator& o) const {
	return m_current == o.m_current;
    }

    bool operator!=(const iterator& o) const {
	return m_current != o.m_current;
    }

private:
    Digits m_digits;
    int m_current;
};

inline Digits::iterator Digits::begin() const {
    return iterator(*this, 0);
}

inline Digits::iterator Digits::end() const {
    return iterator(*this, 10);
}

#endif // DIGITS_H
