#include "combinatorics.h"
#include "UnorderedRoll.h"
#include "SuccessesWithSkill.h"
#include "SimpleUnitSumStatistics.h"

#include <stdlib.h>
#include <math.h>

#include <vector>

using namespace combinatorics;
using namespace dice;
using std::vector;

#include <getopt.h>

// ===================================================================
// === command-line argument handling ================================

class CmdLineParser
{
public:
	CmdLineParser(int argc, char **argv)
		: m_dice(3), m_difficulty(7), m_skill(0),
		  m_skillROI(false), m_dieROI(false), m_histogram(false),
		  m_plotChancesForSkills(false), m_plotExpectedForSkills(false),
		  m_plotExpected(false), m_makePlot(true), m_plotArgs("")
	{
		processCommandLine(argc, argv);
	}

	unsigned int getDice() const         { return m_dice; }
	unsigned int getDifficulty() const   { return m_difficulty; }
	unsigned int getSkill() const        { return m_skill; }
	bool doShowSkillROI() const          { return m_skillROI; }
	bool doShowDieROI() const            { return m_dieROI; }
	bool doShowHistogram() const         { return m_histogram; }
	bool doPlotChancesForSkills() const  { return m_plotChancesForSkills; }
	bool doPlotExpectedForSkills() const { return m_plotExpectedForSkills; }
	bool doPlotExpected() const          { return m_plotExpected; }
	bool doMakePlot() const              { return m_makePlot; }
	const char* getPlotArgs() const      { return m_plotArgs; }

private:
	static void printUsage();

	void processCommandLine(int argc, char **argv);

	// Key values for options that don't have short options associated w/ them
	enum {
		OPT_ROI_SKILL           = 1000,
		OPT_ROI_DIE,
		OPT_PLOT_PROB_SKILLS    = 1100,
		OPT_PLOT_EXP_SKILLS,
		OPT_PLOT_EXP_2ARG,
		OPT_PLOT_ARGS,
		OPT_NO_PLOT,
		OPT_dummy
	} opt_values_t;

	// The long option list
	static const struct option long_opts[];

	// The short option list
	static const char* const short_opts;

	unsigned int m_dice;
	unsigned int m_difficulty;
	unsigned int m_skill;
	bool         m_skillROI;
	bool         m_dieROI;
	bool         m_histogram;
	bool         m_plotChancesForSkills;
	bool         m_plotExpectedForSkills;
	bool         m_plotExpected;
	bool         m_makePlot;
	const char*  m_plotArgs;
};

// The long option list
const struct option CmdLineParser::long_opts[] = {
	{"skill-return-on-investment",  no_argument,       0, OPT_ROI_SKILL},
	{"skill-roi",                   no_argument,       0, OPT_ROI_SKILL},
	{"sr",                          no_argument,       0, OPT_ROI_SKILL},
	{"die-return-on-investment",    no_argument,       0, OPT_ROI_DIE},
	{"die-roi",                     no_argument,       0, OPT_ROI_DIE},
	{"dr",                          no_argument,       0, OPT_ROI_DIE},
	{"return-on-investment",        no_argument,       0, 'r'},
	{"roi",                         no_argument,       0, 'r'},

	{"histogram",                   no_argument,       0, 'h'},

	{"plot-skill-probabilities",    no_argument,       0, OPT_PLOT_PROB_SKILLS},
	{"psp",                         no_argument,       0, OPT_PLOT_PROB_SKILLS},

	{"plot-skill-expected",         no_argument,       0, OPT_PLOT_EXP_SKILLS},
	{"pse",                         no_argument,       0, OPT_PLOT_EXP_SKILLS},

	{"plot-expected",               no_argument,       0, OPT_PLOT_EXP_2ARG},
	{"pe",                          no_argument,       0, OPT_PLOT_EXP_2ARG},

	{"dont-plot",                   no_argument,       0, OPT_NO_PLOT},
	{"no-plot",                     no_argument,       0, OPT_NO_PLOT},
	{"np",                          no_argument,       0, OPT_NO_PLOT},

	{"plot-arguments",              required_argument, 0, OPT_PLOT_ARGS},
	{"pa",                          required_argument, 0, OPT_PLOT_ARGS},

	{"help",                        no_argument,       0, '?'},
	{0, 0, 0, 0}
};

// The short option list
const char* const CmdLineParser::short_opts = "rh?";

void CmdLineParser::printUsage()
{
	fprintf(stderr,
			"Usage: soooperheroes_rolls [options...]"
			" dice [difficulty [skill]]\n"
			"Options:\n"
			"skill-return-on-investment (-sr)"
			"  Print extra successes for 1 extra skill lvl.\n"
			"die-return-on-investment   (-dr)"
			"  Print extra successes for 1 extra stat die.\n"
			"histogram                   (-h)"
			"  Display a histogram.\n"
			"\n"
			"Plotting options:\n"
			"no-plot                    (-np)"
			"  Generate output file, but no visual plot.\n"
			"plot-skill-probabilities  (-psp)"
			"  (make a plot)\n"
			"plot-skill-expected       (-pse)"
			"  (make a plot)\n"
			"plot-expected              (-pe)"
			"  (make a plot)\n"
			"plot-arguments             (-pa)"
			"  Extra args for the plot script.\n"
		);
	exit(1);
}

void CmdLineParser::processCommandLine(int argc, char **argv)
{
	// collect parameters from the command line
	while (1) {
		int opt_index = -1;
		int c = getopt_long_only(argc, argv, short_opts, long_opts, &opt_index);
		if (c == -1)
			break;

		switch (c)
		{
		case OPT_ROI_SKILL:   // "skill-return-on-investment", "skill-roi", "sr"
			m_skillROI = true;
			break;
		case OPT_ROI_DIE:         // "die-return-on-investment", "die-roi", "dr"
			m_dieROI = true;
			break;
		case 'r':                   // "return-on-investment", "roi", "r"
			m_skillROI = m_dieROI = true;
			break;

		case 'h':                   // "histogram", "h"
			m_histogram = true;
			break;

		case OPT_PLOT_PROB_SKILLS:  // "plot-skill-probabilities", "psp"
			m_plotChancesForSkills = true;
			break;
		case OPT_PLOT_EXP_SKILLS:   // "plot-skill-expected", "pse"
			m_plotExpectedForSkills = true;
			break;
		case OPT_PLOT_EXP_2ARG:     // "plot-expected", "pe"
			m_plotExpected = true;
			break;

		case OPT_NO_PLOT:           // "dont-plot", "no-plot", "np"
			m_makePlot = false;
			break;

		case OPT_PLOT_ARGS:         // "plot-arguments", "pa"
			m_plotArgs = optarg;
			break;

		case '?':
			// if an error, a message was already printed by getopt_long
			printUsage();

		default:
			fprintf(stderr, "INTERNAL ERROR: Option code %d isn't"
					" handled by command-line code.\n", c);
			if (opt_index >= 0)
				fprintf(stderr, "\t\tLong option name is '%s'.\n",
						long_opts[opt_index].name);
			if (isprint(c))
				fprintf(stderr, "\t\tOption character is '%c'.\n", c);
			abort();
		}
	}

	// handle non-option arguments
	if (argc < optind+1)
	{
		// whoops, we expected at least one argument
		printUsage();
	}
	m_dice = atoi(argv[optind]);
	if (argc >= optind+2)
		m_difficulty = atoi(argv[optind+1]);
	if (argc >= optind+3)
		m_skill = atoi(argv[optind+2]);

	if (argc >= optind+4)
	{
		// whoops, too many arguments
		printUsage();
	}
}

// ===================================================================
// === Putting it all together =======================================

// --- simple helpers ---

static inline int iround(real_t value)
{
	return static_cast<int>(round(value));
}

static inline unsigned int uiround(real_t value)
{
	return static_cast<unsigned int>(round(value));
}

static inline void do_system(const char* cmd)
{
	printf("> %s\n", cmd);
	system(cmd);
}

template <typename T>
static inline void swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

// --- histogram output helpers ---

static inline void print_block(int numMarks, char ch)
{
	for (int i = 0;  i < numMarks;  ++i)
		putchar(ch);
}

static inline void print_bar(int width, int numMarks, char ch = '*')
{
	if (numMarks > width)
	{
		print_block(width-1, ch);
		print_block(1, '>');
	}
	else
	{
		print_block(numMarks, ch);
		print_block(width - numMarks, ' ');
	}
}

static inline void print_bar_minus_plus(int width, int numMarks,
										char chM = '-', char chP = '+')
{
	if (numMarks < -width)
	{
		print_block(1,       '<');
		print_block(width-1, chM);
		print_block(width,   ' ');
	}
	else if (numMarks > width)
	{
		print_block(width,   ' ');
		print_block(width-1, chP);
		print_block(1,       '>');
	}
	else if (numMarks < 0)
	{
		print_block(width + numMarks, ' ');
		print_block(-numMarks,        chM);
		print_block(width,            ' ');
	}
	else
	{
		print_block(width,            ' ');
		print_block(numMarks,         chP);
		print_block(width - numMarks, ' ');
	}
}

static inline void print_bar_delta(int width, int numMarks1, int numMarks2,
								   char ch = '*',
								   char chM = '-', char chP = '+')
{
	if (numMarks1 > numMarks2)
	{
		swap(numMarks1, numMarks2);
		chP = chM;
	}

	if (numMarks1 > width)
	{
		print_block(width-1, ch);
		print_block(1,       '>');
	}
	else if (numMarks2 > width)
	{
		print_block(numMarks1,             ch);
		print_block(width - numMarks1 - 1, chP);
		print_block(1,                     '>');
	}
	else
	{
		print_block(numMarks1,             ch);
		print_block(numMarks2 - numMarks1, chP);
		print_block(width - numMarks2,     ' ');
	}
}

// --- calculate and print "rated" results for a given roll ---

static void calculate_stats(const CmdLineParser& options)
{
	unsigned int dice       = options.getDice();
	unsigned int difficulty = options.getDifficulty();
	unsigned int skill      = options.getSkill();

	printf("(%d dice, for %ds, skill %d)\n", dice, difficulty, skill);

	SimpleUnitSumStatistics statSuccesses;
	SimpleUnitSumStatistics statUnused;
	SimpleUnitSumStatistics statWanted;
	SimpleUnitSumStatistics statSkillROI;
	vector<real_t>          successHistogram(dice+1);
	vector<real_t>          skillROIHistogram(dice+1);

	UnorderedRoll r = UnorderedRoll::begin(dice);
	bool keepGoing = true;
	while (keepGoing)
	{
		SuccessesWithSkill result(r, difficulty, skill);
		statSuccesses.addData(r.getWeight(), result.getSuccesses());
		statUnused.addData(r.getWeight(),    result.getUnusedSkill());
		statWanted.addData(r.getWeight(),    result.getWantedSkill());
		statSkillROI.addData(r.getWeight(),  result.getWantedSkill() == 1);
		successHistogram[result.getSuccesses()] += r.getWeight();
		if (result.getWantedSkill() == 1)
		{
			skillROIHistogram[result.getSuccesses()]   -= r.getWeight();
			skillROIHistogram[result.getSuccesses()+1] += r.getWeight();
		}
		keepGoing = r.increment();
	}
	
	printf("Average number of successes: %10.6Lf (+/- %6.3Lf)\n",
		   statSuccesses.getAverage(), statSuccesses.getStandardDeviation());
	printf("Average unused skill:        %10.6Lf (+/- %6.3Lf)\n",
		   statUnused.getAverage(), statUnused.getStandardDeviation());
	printf("Average wanted skill:        %10.6Lf (+/- %6.3Lf)\n",
		   statWanted.getAverage(), statWanted.getStandardDeviation());

	if (options.doShowHistogram())
	{
		printf("\n");
		printf("%-7s %-8s %-7s\n", "# Succ.", "At least", "Exactly");
		printf("%-7s %-8s %-7s\n", "-------", "--------", "-------");
		real_t atLeastThisMany = 1;
		for (size_t i = 0;  i < successHistogram.size();  ++i)
		{
			char atLeastBuf[64];
			sprintf(atLeastBuf, "(%.0Lf%%)", 100*atLeastThisMany);
			printf("%4d %10s %5.0Lf%%   |",
				   i, atLeastBuf, 100*successHistogram[i]);
			const int WIDTH = 50;
			print_bar(WIDTH, iround(WIDTH * successHistogram[i]));
			printf("|\n");
			atLeastThisMany -= successHistogram[i];
		}
		printf("\n");
	}

	if (options.doShowDieROI())
	{
		SimpleUnitSumStatistics statSuccessesNext;
		vector<real_t>          successHistogramNext(dice+2);
		UnorderedRoll r = UnorderedRoll::begin(dice+1);
		bool keepGoing = true;
		while (keepGoing)
		{
			SuccessesWithSkill result(r, difficulty, skill);
			statSuccessesNext.addData(r.getWeight(), result.getSuccesses());
			successHistogramNext[result.getSuccesses()] += r.getWeight();
			keepGoing = r.increment();
		}

		printf("Average next die ROI:        %10.6Lf\n",
			   statSuccessesNext.getAverage() - statSuccesses.getAverage());

		if (options.doShowHistogram())
		{
			printf("\n");
			printf("%-7s %-8s %-7s\n", "# Succ.", "At least", "Exactly");
			printf("%-7s %-8s %-7s\n", "-------", "--------", "-------");
			real_t atLeastThisMany = 0;
			for (size_t i = 0;  i < successHistogramNext.size();  ++i)
			{
				real_t nextHist = successHistogramNext[i];
				real_t currHist = 0;
				if (i < successHistogram.size())
					currHist = successHistogram[i];

				char atLeastBuf[64];
				sprintf(atLeastBuf, "(%+.0Lf%%)", 100*atLeastThisMany);
				printf("%4d %10s %+5.0Lf%%   |",
					   i, atLeastBuf, 100*(nextHist - currHist));

				const int WIDTH = 25;
				print_bar_minus_plus(WIDTH,
									 iround(5 * WIDTH*(nextHist - currHist)));

				printf("|\n");
				atLeastThisMany -= (nextHist - currHist);
			}
			printf("\n");
		}
		if (options.doShowHistogram())
		{
			printf("\n");
			printf("%-7s %-8s %-7s\n", "# Succ.", "At least", "Exactly");
			printf("%-7s %-8s %-7s\n", "-------", "--------", "-------");
			real_t atLeastThisMany = 1;
			for (size_t i = 0;  i < successHistogramNext.size();  ++i)
			{
				real_t nextHist = successHistogramNext[i];
				real_t currHist = 0;
				if (i < successHistogramNext.size())
					currHist = successHistogram[i];

				char atLeastBuf[64];
				sprintf(atLeastBuf, "(%.0Lf%%)", 100*atLeastThisMany);
				printf("%4d %10s %5.0Lf%%   |",
					   i, atLeastBuf, 100*nextHist);

				const int WIDTH = 50;
				print_bar_delta(WIDTH,
								iround(WIDTH * currHist),
								iround(WIDTH * nextHist));

				printf("|\n");
				atLeastThisMany -= nextHist;
			}
			printf("\n");
		}
	}

	if (options.doShowSkillROI())
	{
		printf("Average next skill lvl ROI:  %10.6Lf (+/- %6.3Lf)\n",
			   statSkillROI.getAverage(), statSkillROI.getStandardDeviation());

		if (options.doShowHistogram())
		{
			printf("\n");
			printf("%-7s %-8s %-7s\n", "# Succ.", "At least", "Exactly");
			printf("%-7s %-8s %-7s\n", "-------", "--------", "-------");
			real_t atLeastThisMany = 0;
			for (size_t i = 0;  i < skillROIHistogram.size();  ++i)
			{
				char atLeastBuf[64];
				sprintf(atLeastBuf, "(%+.0Lf%%)", 100*atLeastThisMany);
				printf("%4d %10s %+5.0Lf%%   |",
					   i, atLeastBuf, 100*skillROIHistogram[i]);

				const int WIDTH = 25;
				print_bar_minus_plus(WIDTH,
									 iround(5 * WIDTH*skillROIHistogram[i]));

				printf("|\n");
				atLeastThisMany -= skillROIHistogram[i];
			}
			printf("\n");
		}
		if (options.doShowHistogram())
		{
			printf("\n");
			printf("%-7s %-8s %-7s\n", "# Succ.", "At least", "Exactly");
			printf("%-7s %-8s %-7s\n", "-------", "--------", "-------");
			real_t atLeastThisMany = 1;
			for (size_t i = 0;  i < skillROIHistogram.size();  ++i)
			{
				real_t nextHist = successHistogram[i] + skillROIHistogram[i];
				char atLeastBuf[64];
				sprintf(atLeastBuf, "(%.0Lf%%)", 100*atLeastThisMany);
				printf("%4d %10s %5.0Lf%%   |",
					   i, atLeastBuf, 100*nextHist);

				const int WIDTH = 50;
				print_bar_delta(WIDTH,
								iround(WIDTH * successHistogram[i]),
								iround(WIDTH * nextHist));

				printf("|\n");
				atLeastThisMany -= nextHist;
			}
			printf("\n");
		}
	}
}

// --- plot probabilities of number of successes, as a function of skill ---

static void plot_chances_for_skills(const CmdLineParser& options)
{
	unsigned int dice       = options.getDice();
	unsigned int difficulty = options.getDifficulty();
	unsigned int maxSkill   = options.getSkill();

	assert(options.doPlotChancesForSkills());
	printf("(%d dice, for %ds, skill 0-%d)\n", dice, difficulty, maxSkill);

	char fileName[128];
	sprintf(fileName, "srplot_prob_f%d_d%d_s0-%d.csv",
			difficulty, dice, maxSkill);
	FILE* out = fopen(fileName, "w");
	if (! out)
	{
		perror("fopen");
		return;
	}
	fprintf(out, "!plot_title='(%d dice, for %ds, skill 0-%d)'\n",
			dice, difficulty, maxSkill);
	fprintf(out, "!x_label='successes'\n");
	fprintf(out, "!y_label='%% chance'\n");

	vector< vector<real_t> > successTable(maxSkill+1);

	for (unsigned int skill = 0;  skill <= maxSkill;  ++skill)
	{
		// TODO: This is woefully inefficient.  Leaving alone for now.

		vector<real_t>& successHistogram = successTable[skill];
		successHistogram.resize(dice+1);

		UnorderedRoll r = UnorderedRoll::begin(dice);
		bool keepGoing = true;
		while (keepGoing)
		{
			SuccessesWithSkill result(r, difficulty, skill);
			successHistogram[result.getSuccesses()] += r.getWeight();
			keepGoing = r.increment();
		}
	}

	for (unsigned int skill = 0;  skill <= maxSkill;  ++skill)
	{
		for (size_t i = 0;  i < successTable[skill].size();  ++i)
		{
			fprintf(out, "%.3Lf,", 100*successTable[skill][i]);
		}
		fprintf(out, "0\n");
	}

	fclose(out);

	if (options.doMakePlot())
	{
		char plotCmd[1024];
		sprintf(plotCmd, "./plot-lines.py '%s' %s &",
				fileName, options.getPlotArgs());
		do_system(plotCmd);
	}
}

// --- plot probabilities of number of successes, as a function of skill ---

static void plot_expected_for_skills(const CmdLineParser& options)
{
	unsigned int dice       = options.getDice();
	unsigned int difficulty = options.getDifficulty();
	unsigned int maxSkill   = options.getSkill();

	assert(options.doPlotExpectedForSkills());
	printf("(%d dice, for %ds, skill 0-%d)\n", dice, difficulty, maxSkill);

	char fileName[128];
	sprintf(fileName, "srplot_exps_f%d_d%d_s0-%d.csv",
			difficulty, dice, maxSkill);
	FILE* out = fopen(fileName, "w");
	if (! out)
	{
		perror("fopen");
		return;
	}
	fprintf(out, "!plot_title='(%d dice, for %ds, skill 0-%d)'\n",
			dice, difficulty, maxSkill);
	fprintf(out, "!x_label='skill'\n");
	fprintf(out, "!y_label='average expected successes'\n");

	vector<real_t> expectedSuccesses(maxSkill+1);

	for (unsigned int skill = 0;  skill <= maxSkill;  ++skill)
	{
		// TODO: This is woefully inefficient.  Leaving alone for now.

		SimpleUnitSumStatistics statSuccesses;

		UnorderedRoll r = UnorderedRoll::begin(dice);
		bool keepGoing = true;
		while (keepGoing)
		{
			SuccessesWithSkill result(r, difficulty, skill);
			statSuccesses.addData(r.getWeight(), result.getSuccesses());
			keepGoing = r.increment();
		}

		expectedSuccesses[skill] = statSuccesses.getAverage();
	}

	const char* sep = "";
	for (unsigned int skill = 0;  skill <= maxSkill;  ++skill)
	{
		fprintf(out, "%s%.3Lf", sep, expectedSuccesses[skill]);
		sep = ",";
	}
	fprintf(out, "\n");

	fclose(out);

	if (options.doMakePlot())
	{
		char plotCmd[1024];
		sprintf(plotCmd, "./plot-lines.py '%s' %s &",
				fileName, options.getPlotArgs());
		do_system(plotCmd);
	}
}

// --- plot probabilities of number of successes, as a function of skill ---

static void plot_expected_for_dice_and_skill(const CmdLineParser& options)
{
	unsigned int maxDice       = options.getDice();
	unsigned int difficulty = options.getDifficulty();
	unsigned int maxSkill   = options.getSkill();

	assert(options.doPlotExpected());
	printf("(1-%d dice, for %ds, skill 0-%d)\n",
		   maxDice, difficulty, maxSkill);

	char fileName[128];
	sprintf(fileName, "srplot_expc_d1-%d_s0-%d_f%d.csv",
			maxDice, maxSkill, difficulty);
	FILE* out = fopen(fileName, "w");
	if (! out)
	{
		perror("fopen");
		return;
	}
	fprintf(out, "!plot_title='(1-%d dice, for %ds, skill 0-%d)'\n",
			maxDice, difficulty, maxSkill);

	for (unsigned int dice = 1;  dice <= maxDice;  ++dice)
	{
		vector<real_t> expectedSuccesses(maxSkill+1);

		for (unsigned int skill = 0;  skill <= maxSkill;  ++skill)
		{
			// TODO: This is woefully inefficient.  Leaving alone for now.

			SimpleUnitSumStatistics statSuccesses;

			UnorderedRoll r = UnorderedRoll::begin(dice);
			bool keepGoing = true;
			while (keepGoing)
			{
				SuccessesWithSkill result(r, difficulty, skill);
				statSuccesses.addData(r.getWeight(), result.getSuccesses());
				keepGoing = r.increment();
			}

			expectedSuccesses[skill] = statSuccesses.getAverage();
		}

		const char* sep = "";
		for (unsigned int skill = 0;  skill <= maxSkill;  ++skill)
		{
			fprintf(out, "%s%.3Lf", sep, expectedSuccesses[skill]);
			sep = ",";
		}
		fprintf(out, "\n");
	}

	fclose(out);

	if (options.doMakePlot())
	{
		char plotCmd[1024];
		sprintf(plotCmd, "./plot-contours-sd.py '%s' %s &",
				fileName, options.getPlotArgs());
		do_system(plotCmd);
	}
}

// --- main ---

int main(int argc, char** argv)
{
	CmdLineParser options(argc, argv);

	if (options.doPlotChancesForSkills())
	{
		plot_chances_for_skills(options);
	}
	else if (options.doPlotExpectedForSkills())
	{
		plot_expected_for_skills(options);
	}
	else if (options.doPlotExpected())
	{
		plot_expected_for_dice_and_skill(options);
	}
	else
	{
		calculate_stats(options);
	}
	return 0;
}
