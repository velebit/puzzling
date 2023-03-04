#ifndef FACTORIAL_CACHE__H
#define FACTORIAL_CACHE__H

#include "combinatorics_common.h"

#include <assert.h>

namespace combinatorics
{
	namespace factorial
	{

		// Helper class for cached factorial values (DO NOT USE DIRECTLY!)

		template <typename T>
		class Cache {
		public:
			Cache(int size);
			~Cache();

			T get(int i) const { assert(i < m_size);  return m_cache[i]; }

		private:
			static T* buildCache(int size);

		private:
			const int      m_size;
			const T* const m_cache;
		};

	};
};

#endif // FACTORIAL_CACHE__H
