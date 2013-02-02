#include <stdio.h>

#include "digits.h"


#if defined(__GNUC__) && (__GNUC__ == 4) && (__GNUC_MINOR__ <= 6)
// HACK HACK HACK...  gcc 4.6 is refusing to use the member begin() and end()
inline Digits::iterator begin(const Digits& d) {
    return d.begin();
}
inline Digits::iterator end(const Digits& d) {
    return d.end();
}
#endif // gcc 4.6 and below


int main(int argc, const char** argv)
{
    Digits digits_for_L = Digits::all();
    for (int value_L : digits_for_L) {
	if (value_L < 1) {
	    continue;
	}
	Digits digits_for_O = digits_for_L.without(value_L);
	for (int value_O : digits_for_O) {
	    Digits digits_for_V = digits_for_O.without(value_O);
	    for (int value_V : digits_for_V) {
		Digits digits_for_E = digits_for_V.without(value_V);
		for (int value_E : digits_for_E) {
		    int word_LOVE = (1000*value_L + 100*value_O
				     + 10*value_V + value_E);
		    Digits digits_for_I = digits_for_E.without(value_E);
		    for (int value_I : digits_for_I) {
			if (value_I < 1) {
			    continue;
			}
			int expr_LOVExI = word_LOVE * value_I;
			if (expr_LOVExI >= 10000) {
			    continue;
			}
			Digits digits_for_S = digits_for_I.without(value_I);
			for (int value_S : digits_for_S) {
			    int expr_LOVExS = word_LOVE * value_S;
			    if (expr_LOVExS < 10000)
			    {
				continue;
			    }

			    int maybe_BLIND = 10*expr_LOVExI + expr_LOVExS;
			    if (maybe_BLIND >= 100000) {
				continue;
			    }
			    
			    int tmp = maybe_BLIND;
			    int maybe_D = tmp % 10;
			    tmp /= 10;
			    int maybe_N = tmp % 10;
			    tmp /= 10;
			    int maybe_I = tmp % 10;
			    tmp /= 10;
			    int maybe_L = tmp % 10;
			    tmp /= 10;
			    int maybe_B = tmp % 10;
			    tmp /= 10;

			    Digits digits_for_BND =
				digits_for_S.without(value_S);
			    if ((tmp > 0)
				|| ! digits_for_BND.eliminate(maybe_B)
				|| (maybe_L != value_L)
				|| (maybe_I != value_I)
				|| ! digits_for_BND.eliminate(maybe_N)
				|| ! digits_for_BND.eliminate(maybe_D))
			    {
				continue;
			    }

			    printf("%6d\n"
				   "x%5d\n"
				   "------\n"
				   "%6d\n"
				   "%5d\n"
				   "------\n"
				   "%6d\n\n",
				   word_LOVE, 10*value_I + value_S,
				   expr_LOVExS, expr_LOVExI,
				   maybe_BLIND);
			}
		    }
		}
	    }
	}
    }
}
