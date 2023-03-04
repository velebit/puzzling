#ifndef TUPLE__H
#define TUPLE__H

#include <assert.h>

namespace combinatorics
{

	// A N-tuple of type T, with indices starting at Z

	template <int N, typename T, int Z = 0>
	class Tuple
	{
	public:
		static const int NUM_ELEMENTS  = N;
		static const int INITIAL_INDEX = Z;
	    typedef T element_type;

	    // --- construction and destruction ---

		// Create an empty tuple
		Tuple()
		{
			for (int i = 0;  i < N;  ++i)
			{
				m_elements[i] = static_cast<element_type>(0);
			}
		}

		// --- copy construction and assignment ---

		// Copy constructor (allows element type conversion)
	    template <typename O>
	    Tuple(const Tuple<N,O,Z>& tuple)
		{
			for (int i = 0;  i < N;  ++i)
			{
				m_elements[i] = tuple.m_elements[i];
			}
		}

		// Assignment (allows element type conversion)
	    template <typename O>
	    Tuple& operator=(const Tuple<N,O,Z>& tuple)
		{
			for (int i = 0;  i < N;  ++i)
			{
				m_elements[i] = tuple.m_elements[i];
			}
		}

		// --- Element access ---

	    element_type operator[](int i) const
	    {
			i -= INITIAL_INDEX;
			assert((i >= 0) && (i < NUM_ELEMENTS));
			return m_elements[i];
		}

	    element_type& operator[](int i)
	    {
			i -= INITIAL_INDEX;
			assert((i >= 0) && (i < NUM_ELEMENTS));
			return m_elements[i];
		}

	private:
		// Elements of the tuple
		element_type m_elements[NUM_ELEMENTS];
	};
}

#endif // TUPLE__H
