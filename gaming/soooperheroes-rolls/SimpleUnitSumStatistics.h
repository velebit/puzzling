#ifndef SIMPLE_UNIT_SUM_STATISTICS__H
#define SIMPLE_UNIT_SUM_STATISTICS__H

#include <math.h>

namespace combinatorics
{
	// === simple statistics that assume the sum of weights is 1 =========

	class SimpleUnitSumStatistics
	{
	public:

		SimpleUnitSumStatistics() : m_sum(0), m_sumSq(0)
		{
		}
		
		void addData(real_t weight, real_t value)
		{
			m_sum   += weight * value;
			m_sumSq += weight * value * value;
		}

		real_t getAverage() const
		{
			return m_sum;
		}

		real_t getStandardDeviation() const
		{
			return sqrt(m_sumSq - square(m_sum));
		}

	private:
		static real_t square(real_t value) { return value*value; }

	private:
		real_t m_sum;
		real_t m_sumSq;
	};
}

#endif // SIMPLE_UNIT_SUM_STATISTICS__H
