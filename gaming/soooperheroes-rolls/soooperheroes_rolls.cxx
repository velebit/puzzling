#include "combinatorics.h"
#include "UnorderedRoll.h"
#include "Successes.h"

#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>

using namespace combinatorics;
using namespace dice;

#include <getopt.h>

// ===================================================================
// === command-line argument handling ================================

class CmdLineParser
{
public:
	CmdLineParser(int argc, char **argv)
		: m_dice(3), m_difficulty(7), m_skill(0),
		  m_skillROI(false), m_dieROI(false)
	{
		processCommandLine(argc, argv);
	}

	unsigned int getDice() const       { return m_dice; }
	unsigned int getDifficulty() const { return m_difficulty; }
	unsigned int getSkill() const      { return m_skill; }
	bool         showSkillROI() const  { return m_skillROI; }
	bool         showDieROI() const    { return m_dieROI; }

private:
	static void printUsage();

	void processCommandLine(int argc, char **argv);

	// Key values for options that don't have short options associated w/ them
	enum {
		OPT_ROI_SKILL           = 1000,
		OPT_ROI_DIE
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

	{"help",                        no_argument,       0, '?'},
	{0, 0, 0, 0}
};

// The short option list
const char* const CmdLineParser::short_opts = "r?";

void CmdLineParser::printUsage()
{
	fprintf(stderr, "Usage: soooperheroes_rolls [options...]"
			" dice [difficulty [skill]]\n");
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
		case OPT_ROI_DIE:     // "die-return-on-investment", "die-roi", "dr"
			m_dieROI = true;
			break;
		case 'r':             // "return-on-investment", "roi", "r"
			m_skillROI = m_dieROI = true;
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
// === simple statistics (average and standard deviation) ============

class Statistics
{
public:
	Statistics() : m_scale(0), m_sum(0), m_sumSq(0)
	{
	}

	void addData(real_t weight, real_t value)
	{
		m_scale += weight;
		m_sum   += weight * value;
		m_sumSq += weight * value * value;
	}

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

// ===================================================================
// === Putting it all together =======================================

// --- calculate and print "rated" results for a given roll ---

static void calculate_stats(const CmdLineParser& options)
{
	unsigned int dice       = options.getDice();
	unsigned int difficulty = options.getDifficulty();
	unsigned int skill      = options.getSkill();

	printf("(%d dice, for %ds, skill %d)\n", dice, difficulty, skill);

	Statistics statSuccesses;
	Statistics statUnused;
	Statistics statWanted;
	Statistics statSkillROI;

	UnorderedRoll r = UnorderedRoll::begin(dice);
	bool keepGoing = true;
	while (keepGoing)
	{
		Successes result(r, difficulty, skill);
		statSuccesses.addData(r.getWeight(), result.getSuccesses());
		statUnused.addData(r.getWeight(),    result.getUnusedSkill());
		statWanted.addData(r.getWeight(),    result.getWantedSkill());
		statSkillROI.addData(r.getWeight(),  result.getWantedSkill() == 1);
		keepGoing = r.increment();
	}

	printf("Average number of successes: %10.6Lf (+/- %6.3Lf)\n",
		   statSuccesses.getAverage(), statSuccesses.getStandardDeviation());
	printf("Average unused skill:        %10.6Lf (+/- %6.3Lf)\n",
		   statUnused.getAverage(), statUnused.getStandardDeviation());
	printf("Average wanted skill:        %10.6Lf (+/- %6.3Lf)\n",
		   statWanted.getAverage(), statWanted.getStandardDeviation());

	if (options.showDieROI())
	{
		Statistics statSuccessesNext;
		UnorderedRoll r = UnorderedRoll::begin(dice+1);
		bool keepGoing = true;
		while (keepGoing)
		{
			Successes result(r, difficulty, skill);
			statSuccessesNext.addData(r.getWeight(), result.getSuccesses());
			keepGoing = r.increment();
		}

		printf("Average next die ROI:        %10.6Lf\n",
			   statSuccessesNext.getAverage() - statSuccesses.getAverage());
	}
	if (options.showSkillROI())
	{
		printf("Average next skill pt ROI:   %10.6Lf (+/- %6.3Lf)\n",
			   statSkillROI.getAverage(), statSkillROI.getStandardDeviation());
	}
}

// --- main ---

int main(int argc, char** argv)
{
	CmdLineParser options(argc, argv);

	calculate_stats(options);
	return 0;
}
