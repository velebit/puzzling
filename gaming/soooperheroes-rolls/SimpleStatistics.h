#ifndef SIMPLE_STATISTICS__H
#define SIMPLE_STATISTICS__H

#include <math.h>

namespace combinatorics
{
	// === simple statistics (average and standard deviation) ============

	class SimpleStatistics
	{
	public:

		SimpleStatistics() : m_scale(0), m_sum(0), m_sumSq(0)
		{
		}
		
		void addData(real_t weight, real_t value)
		{
			m_scale += weight;
			m_sum   += weight * value;
			m_sumSq += weight * value * value;
		}

		void addData(real_t value)  { addData(1, value); }

		real_t getAverage() const
		{
			return m_sum / m_scale;
		}

		real_t getStandardDeviation() const
		{
			return sqrt(m_sumSq/m_scale - square(m_sum/m_scale));
		}

	private:
		static real_t square(real_t value) { return value*value; }

	private:
		real_t m_scale;
		real_t m_sum;
		real_t m_sumSq;
	};
}

#endif // SIMPLE_STATISTICS__H
