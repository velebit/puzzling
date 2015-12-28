#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <array>
#include <vector>
#include <set>
#include <memory>
#include <random>
#include <stdexcept>

#include "Fingers.h"
#include "Hands.h"
#include "Players.h"



static void TestAll()
{
	TestFingers();
	TestHands();
	TestPlayers();
}



enum class TriBool
{
	UNKNOWN,
	NO,
	YES
};

std::ostream & operator<<(std::ostream & s, TriBool t)
{
	s << ((t == TriBool::UNKNOWN) ? "UNKNOWN" :
		  ((t == TriBool::NO) ? "NO" : "YES"));
	return s;
}

inline TriBool AsTri(bool b)
{
	return (b ? TriBool::YES : TriBool::NO);
}
inline bool AsBool(TriBool t)
{
	return (t == TriBool::YES);
}



class PlayArray
{
public:
	struct Data
	{
		Players state;
		std::set<Players::SeqNum> next_regular_moves;
		std::set<Players::SeqNum> prev_regular_moves;
		TriBool is_directly_reachable;
		TriBool is_reachable;
		TriBool can_force_win;

		Data();
	};

	PlayArray();

	size_t size() const { return m_data.size(); }
	const Data & operator[](size_t i) const { return m_data[i]; }

private:
	void initializeStates();
	void initializeNextLinks();
	void initializePrevLinks();
	void initializeReachable();
	void initializeWinnable();

	std::array<Data, Players::NUM_SEQ_NUMS> m_data;
};


PlayArray::Data::Data() :
	state(),
	next_regular_moves(),
	prev_regular_moves(),
	is_directly_reachable(TriBool::UNKNOWN),
	is_reachable(TriBool::UNKNOWN),
	can_force_win(TriBool::UNKNOWN)
{
}

PlayArray::PlayArray() :
	m_data()
{
	initializeStates();
	initializeNextLinks();
	initializePrevLinks();
	initializeReachable();
	initializeWinnable();
}

void PlayArray::initializeStates()
{
	for (size_t i = 0;  i < m_data.size();  ++i) {
		m_data[i].state = Players::FromSeq(i);
	}
}

void PlayArray::initializeNextLinks()
{
	for (size_t i = 0;  i < m_data.size();  ++i) {
		m_data[i].next_regular_moves.clear();
	}

	for (size_t i = 0;  i < m_data.size();  ++i) {
		assert(m_data[i].state.other().isAlive());
		if (m_data[i].state.next().isAlive()) {
			assert(m_data[i].state.next().high().isAlive());
			assert(m_data[i].state.other().high().isAlive());
			m_data[i].next_regular_moves.insert(
				m_data[i].state.addHighToHigh().seq());
			if (m_data[i].state.other().low().isAlive()) {
				m_data[i].next_regular_moves.insert(
					m_data[i].state.addHighToLow().seq());
			}
			if (m_data[i].state.next().low().isAlive()) {
				m_data[i].next_regular_moves.insert(
					m_data[i].state.addLowToHigh().seq());
				if (m_data[i].state.other().low().isAlive()) {
					m_data[i].next_regular_moves.insert(
						m_data[i].state.addLowToLow().seq());
				}
			}
		}
	}
}

void PlayArray::initializePrevLinks()
{
	for (size_t i = 0;  i < m_data.size();  ++i) {
		m_data[i].prev_regular_moves.clear();
	}

	for (size_t i = 0;  i < m_data.size();  ++i) {
		for (auto nrmi = m_data[i].next_regular_moves.cbegin();
			 nrmi != m_data[i].next_regular_moves.cend();
			 ++nrmi) {
			m_data[*nrmi].prev_regular_moves.insert(i);
		}
	}
}

void PlayArray::initializeReachable()
{
	for (size_t i = 0;  i < m_data.size();  ++i) {
		m_data[i].is_directly_reachable =
			AsTri(! m_data[i].prev_regular_moves.empty());
	}

	std::vector<Players::SeqNum> reachables;
	std::vector<Players::SeqNum> next_reachables;
	reachables.push_back(MakePlayers(1, 1, 1, 1).seq());

	while (! reachables.empty()) {
		for (auto ri = reachables.cbegin();  ri != reachables.cend();  ++ri) {
			if (m_data[*ri].is_reachable == TriBool::UNKNOWN) {
				m_data[*ri].is_reachable = TriBool::YES;
				for (auto nrmi = m_data[*ri].next_regular_moves.cbegin();
					 nrmi != m_data[*ri].next_regular_moves.cend();
					 ++nrmi) {
					next_reachables.push_back(*nrmi);
				}
			}
		}
		std::swap(reachables, next_reachables);
		next_reachables.clear();
	}

	for (size_t i = 0;  i < m_data.size();  ++i) {
		if (m_data[i].is_reachable == TriBool::UNKNOWN) {
			m_data[i].is_reachable = TriBool::NO;
		}
	}
}

void PlayArray::initializeWinnable()
{
	std::vector<Players::SeqNum> updates;
	std::vector<Players::SeqNum> next_updates;

	for (size_t i = 0;  i < m_data.size();  ++i) {
		if (! m_data[i].state.next().isAlive()) {
			m_data[i].can_force_win = TriBool::NO;  // ...was just lost!
			updates.push_back(i);
		}
	}

	while (! updates.empty()) {
		for (auto ui = updates.cbegin();  ui != updates.cend(); ++ui) {
			for (auto prmi = m_data[*ui].prev_regular_moves.cbegin();
				 prmi != m_data[*ui].prev_regular_moves.cend();
				 ++prmi) {
				if (m_data[*prmi].can_force_win == TriBool::UNKNOWN) {
					bool saw_no = false;
					bool saw_maybe = false;

					for (auto nrmi = m_data[*prmi].next_regular_moves.cbegin();
						 nrmi != m_data[*prmi].next_regular_moves.cend();
						 ++nrmi) {
						auto can_force_win = m_data[*nrmi].can_force_win;
						if (can_force_win == TriBool::NO) {
							saw_no = true;
						} else if (can_force_win == TriBool::UNKNOWN) {
							saw_maybe = true;
						}
					}

					if (saw_no) {
						m_data[*prmi].can_force_win = TriBool::YES;
						next_updates.push_back(*prmi);
					} else if (! saw_maybe) {
						m_data[*prmi].can_force_win = TriBool::NO;
						next_updates.push_back(*prmi);
					}
				}
			}
		}

		std::swap(updates, next_updates);
		next_updates.clear();
	}

	for (size_t i = 0;  i < m_data.size();  ++i) {
		if (m_data[i].can_force_win == TriBool::UNKNOWN) {
			assert(m_data[i].is_reachable == TriBool::NO);
		}
	}
}



enum class PlayerIndex
{
	A,
	B
};

std::ostream & operator<<(std::ostream & s, PlayerIndex p)
{
	s << ((p == PlayerIndex::A) ? "A" : "B");
	return s;
}

PlayerIndex operator!(PlayerIndex p)
{
	return ((p == PlayerIndex::A) ? PlayerIndex::B : PlayerIndex::A);
}


class GameStats
{
public:
	explicit GameStats(std::shared_ptr<PlayArray> play_array);

	void showStats(int verbosity = 1) const;

private:
	void gatherStats();

	std::shared_ptr<PlayArray> m_data;
	size_t m_num_directly_reachable;
	size_t m_num_reachable;
	size_t m_num_r_alive;
	std::vector<size_t> m_num_next_moves;
	std::vector<size_t> m_num_prev_moves;
};


GameStats::GameStats(std::shared_ptr<PlayArray> play_array) :
	m_data(play_array),
	m_num_directly_reachable(0),
	m_num_reachable(0),
	m_num_r_alive(0),
	m_num_next_moves(),
	m_num_prev_moves()
{
	gatherStats();
}


void GameStats::gatherStats()
{
	for (size_t i = 0;  i < m_data->size();  ++i) {
		assert((*m_data)[i].state.other().isAlive());
		if (AsBool((*m_data)[i].is_directly_reachable)) {
			++m_num_directly_reachable;
		}
		if (AsBool((*m_data)[i].is_reachable)) {
			++m_num_reachable;
			if ((*m_data)[i].state.next().isAlive()) {
				++m_num_r_alive;
			}

			{
				size_t n = (*m_data)[i].next_regular_moves.size();
				if (n >= m_num_next_moves.size()) {
					m_num_next_moves.resize(n+1);
				}
				++(m_num_next_moves.at(n));
			}

			{
				size_t n = (*m_data)[i].prev_regular_moves.size();
				if (n >= m_num_prev_moves.size()) {
					m_num_prev_moves.resize(n+1);
				}
				++(m_num_prev_moves.at(n));
			}
		}
	}

	// UNFINISHED
	{
		std::vector<size_t> num_winning_with_next;
		for (size_t i = 0;  i < m_data->size();  ++i) {
			if (AsBool((*m_data)[i].is_reachable)) {
				if ((*m_data)[i].state.next().isAlive()) {
					//++m_num_r_alive;
				}

				{
					size_t n = (*m_data)[i].next_regular_moves.size();
					if (n >= num_winning_with_next.size()) {
						num_winning_with_next.resize(n+1);
					}
					++(num_winning_with_next.at(n));
				}

				{
#if 0  //XXX
					size_t n = (*m_data)[i].prev_regular_moves.size();
					if (n >= num_prev_moves.size()) {
						num_prev_moves.resize(n+1);
					}
					++(num_prev_moves.at(n));
#endif
				}
			}
		}
	}
}

void GameStats::showStats(int verbosity) const
{
	if (verbosity >= 1) {
		std::cout << "Total positions:    " << m_data->size() << std::endl;
		std::cout << "Directly reachable: " << m_num_directly_reachable
				  << " (+ start)" << std::endl;
		std::cout << "Reachable:          " << m_num_reachable << std::endl;
		std::cout << "Play is continuing: " << m_num_r_alive << std::endl;
	}
	if (verbosity >= 2) {
		for (size_t i = 0;  i < m_data->size();  ++i) {
			if (AsBool((*m_data)[i].is_reachable) &&
				(*m_data)[i].state.next().isAlive()) {
				std::cout << "  " << (*m_data)[i].state;
			}
		}
		std::cout << std::endl;
	}
	if (verbosity >= 1) {
		std::cout << "Play is done:       "
				  << (m_num_reachable - m_num_r_alive) << std::endl;
	}
	if (verbosity >= 2) {
		for (size_t i = 0;  i < m_data->size();  ++i) {
			if (AsBool((*m_data)[i].is_reachable) &&
				! (*m_data)[i].state.next().isAlive()) {
				std::cout << "  " << (*m_data)[i].state;
			}
		}
		std::cout << std::endl;
	}
	if (verbosity >= 2) {
		std::cout << "DR & !R:" << std::endl;
		for (size_t i = 0;  i < m_data->size();  ++i) {
			if (AsBool((*m_data)[i].is_directly_reachable) &&
				! AsBool((*m_data)[i].is_reachable)) {
				std::cout << "  " << (*m_data)[i].state;
			}
		}
		std::cout << std::endl;
	}
	if (verbosity >= 1) {
		std::cout << std::endl;
	}

	if (verbosity >= 1) {
		for (size_t j = 1;  j < m_num_next_moves.size();  ++j) {
			std::cout << "Can reach " << std::setw(2) << j
					  << " descendants:    " << m_num_next_moves.at(j)
					  << std::endl;
			if (verbosity >= 2) {
				for (size_t i = 0;  i < m_data->size();  ++i) {
					if (AsBool((*m_data)[i].is_reachable) &&
						((*m_data)[i].next_regular_moves.size() == j)) {
						std::cout << "  " << (*m_data)[i].state << " ->";
						for (auto nrmi = (*m_data)[i].next_regular_moves.cbegin();
							 nrmi != (*m_data)[i].next_regular_moves.cend();
							 ++nrmi) {
							std::cout << "  " << (*m_data)[*nrmi].state;
						}
						std::cout << std::endl;
					}
				}
			}
		}
		std::cout << std::endl;
	}

	if (verbosity >= 1) {
		for (size_t j = 0;  j < m_num_prev_moves.size();  ++j) {
			std::cout << "Reachable from " << std::setw(2) << j
					  << " ancestors: " << m_num_prev_moves.at(j)
					  << std::endl;
			if (verbosity >= 2) {
				for (size_t i = 0;  i < m_data->size();  ++i) {
					if (AsBool((*m_data)[i].is_reachable) &&
						((*m_data)[i].prev_regular_moves.size() == j)) {
						std::cout << "  " << (*m_data)[i].state << " <-";
						for (auto prmi = (*m_data)[i].prev_regular_moves.cbegin();
							 prmi != (*m_data)[i].prev_regular_moves.cend();
							 ++prmi) {
							std::cout << "  " << (*m_data)[*prmi].state;
						}
						std::cout << std::endl;
					}
				}
			}
		}
		std::cout << std::endl;
	}

//	std::cout << "Opponent's positions:" << std::endl;
}


class Game
{
public:
	explicit Game(std::shared_ptr<PlayArray> play_array);

	void readNextMove();
	void makeNextMove();

	void showTree() const;
	void showTree(Players play, PlayerIndex player) const;

	bool isAlive() const { return m_position.next().isAlive(); }

private:
	void showTree(size_t i, PlayerIndex player,
				  bool skip_winnable, size_t indent) const;
	void showState() const;

	std::shared_ptr<PlayArray> m_data;
	Players m_position;
	PlayerIndex m_next_player;
};

Game::Game(std::shared_ptr<PlayArray> play_array) :
	m_data(play_array),
	m_position(MakePlayers(1, 1, 1, 1)),
	m_next_player(PlayerIndex::A)
{
}


void Game::showTree() const
{
	showTree(m_position, m_next_player);
}

void Game::showTree(Players play, PlayerIndex player) const
{
	auto p = play.seq();
	std::cout << "Player starting from " << (*m_data)[p].state
			  << " is expected to win?  " << (*m_data)[p].can_force_win
			  << std::endl;
	showTree(p, player, ! AsBool((*m_data)[p].can_force_win), 0);
}

void Game::showTree(size_t i, PlayerIndex player,
					bool skip_winnable, size_t indent) const
{
	std::cout << "[" << player
			  << " " << (AsBool((*m_data)[i].can_force_win) ? "win" : "   ")
			  << " " << std::setw(2) << (indent+1)
			  << "]" << std::setw((4*indent)+1) << "" << (*m_data)[i].state;
	if (! (*m_data)[i].state.next().isAlive()) {
		std::cout << "  (player " << !player << " wins!)" << std::endl;
	} else if (skip_winnable && AsBool((*m_data)[i].can_force_win)) {
		std::cout << "  !!avoid!!" << std::endl;
	} else {
		std::cout << " ->" << std::endl;
		for (auto nrmi = (*m_data)[i].next_regular_moves.cbegin();
			 nrmi != (*m_data)[i].next_regular_moves.cend();
			 ++nrmi) {
			showTree(*nrmi, !player, !skip_winnable, indent+1);
		}
	}
}


void Game::showState() const
{
	std::cout << "You are currently showing:         "
			  << m_position.next().high();
	if (m_position.next().low().isAlive()) {
		std::cout << " and "
				  << m_position.next().low();
	}
	std::cout << std::endl;
	std::cout << "The computer is currently showing: "
			  << m_position.other().high();
	if (m_position.other().low().isAlive()) {
		std::cout << " and "
				  << m_position.other().low();
	}
	std::cout << std::endl;
}


void Game::readNextMove()
{
	std::cout << "======================== YOUR MOVE ========================"
			  << std::endl << std::endl;

	bool valid = false;
	while (! valid) {
		showState();
		{
			auto winning = (*m_data)[m_position.seq()].can_force_win;
			if (winning == TriBool::YES) {
				std::cout << "The crystal ball says:"
						  << " You should be able to win this one."
						  << std::endl << std::endl;
			} else if (winning == TriBool::NO) {
				std::cout << "The crystal ball says:"
						  << " It may not be possible to win this."
						  << std::endl << std::endl;
			} else {
				std::cout << "The crystal ball is unexpectedly cloudy."
						  << std::endl << std::endl;
			}
		}

		std::cout << "Enter a command in one of the following formats:\n"
				  << "    touch <NUM> to <NUM>    (example: touch 1 to 1)\n"
				  << "    tree\n"
				  << "Remember to type Enter at the end." << std::endl;
		std::cout << "> ";

		std::string line;
		while (line.empty()) {
			if (! std::getline(std::cin, line))
			{
				std::cout << "Bye!" << std::endl;
				exit(0);
			}
		}
		std::istringstream line_stream(line);
		line_stream.exceptions(std::istream::failbit | std::istream::badbit);
		std::cout << std::endl;
		try {
			std::string command;
			line_stream >> command;
			if ((command == "touch") || (command == "add")) {
				// "from" and "to" can't be Fingers::Value: that reads chars!!!
				int from = 0;
				std::string word;
				int to = 0;
				line_stream >> from >> word >> to;
				if (word != "to") {
					throw std::runtime_error("Bad join word '" + word + "'");
				}
				if (Fingers(from) == m_position.next().high()) {
					if (Fingers(to) == m_position.other().high()) {
						m_position = m_position.addHighToHigh();
						valid = true;
					} else if (Fingers(to) == m_position.other().low()) {
						m_position = m_position.addHighToLow();
						valid = true;
					} else {
						std::cout << "I'm sorry, the computer is not showing "
								  << Fingers(to) << ".  Please try again."
								  << std::endl << std::endl;
					}
				} else if (Fingers(from) == m_position.next().low()) {
					if (Fingers(to) == m_position.other().high()) {
						m_position = m_position.addLowToHigh();
						valid = true;
					} else if (Fingers(to) == m_position.other().low()) {
						m_position = m_position.addLowToLow();
						valid = true;
					} else {
						std::cout << "I'm sorry, the computer is not showing "
								  << Fingers(to) << ".  Please try again."
								  << std::endl << std::endl;
					}
				} else {
					std::cout << "I'm sorry, you are not showing "
							  << Fingers(from) << ".  Please try again."
							  << std::endl << std::endl;
				}
			} else if (command == "tree") {
				showTree();
			} else {
				throw std::runtime_error("Unknown command '" + command + "'");
			}
		} catch (...) {
			std::cout << "I'm sorry, I did not understand that."
					  << std::endl << std::endl;
		}
	}
}


void Game::makeNextMove()
{
	std::cout << "------------------- THE COMPUTER'S MOVE -------------------"
			  << std::endl << std::endl;

	typedef Players::SeqNum SeqNum;
	const Players old = m_position;
	const SeqNum p = m_position.seq();

	std::set<SeqNum> winning_moves;
	for (auto nrmi = (*m_data)[p].next_regular_moves.cbegin();
		 nrmi != (*m_data)[p].next_regular_moves.cend();
		 ++nrmi) {
		if (! AsBool((*m_data)[*nrmi].can_force_win)) {
			winning_moves.insert(*nrmi);
		}
	}

	// TODO: use a random element for fun, instead of first!
	showState();
	if (! winning_moves.empty()) {
		auto it = winning_moves.cbegin();
		m_position = (*m_data)[*it].state;
	} else if (! (*m_data)[p].next_regular_moves.empty()) {
		auto it = (*m_data)[p].next_regular_moves.cbegin();
		m_position = (*m_data)[*it].state;
		std::cout << "The computer says,"
				  << " \"I have a bad feeling about this.\""
				  << std::endl;
	} else {
		throw std::runtime_error("no next moves!?!");
	}

	if (m_position == old.addHighToHigh()) {
		std::cout << "The computer touches your "
				  << old.other().high() << " hand with its holographic "
				  << old.next().high() << " hand." << std::endl << std::endl;
	} else if (m_position == old.addHighToLow()) {
		std::cout << "The computer touches your "
				  << old.other().low() << " hand with its holographic "
				  << old.next().high() << " hand." << std::endl << std::endl;
	} else if (m_position == old.addLowToHigh()) {
		std::cout << "The computer touches your "
				  << old.other().high() << " hand with its holographic "
				  << old.next().low() << " hand." << std::endl << std::endl;
	} else if (m_position == old.addLowToLow()) {
		std::cout << "The computer touches your "
				  << old.other().low() << " hand with its holographic "
				  << old.next().low() << " hand." << std::endl << std::endl;
	} else {
		std::cout << "Something inexplicable just happened: "
				  << m_position << std::endl;
	}
}


int main()
{
	if (false) {
		TestAll();
	}

	auto plays = std::make_shared<PlayArray>();

	GameStats stats(plays);
	if (false) {
		stats.showStats();
	}

	int slot = -1;
	while ((slot != 1) && (slot != 2)) {
		std::cout << "Please enter 1 to go first, or 2 to go second.\n"
				  << "Remember to type Enter at the end." << std::endl;
		std::cin >> slot;
	}
	std::cout << std::endl << std::endl;

	Game game(plays);
	if (slot == 2) {
		game.makeNextMove();
	}
	while (1) {
		if (! game.isAlive()) {
			std::cout << "++++++++++ Game over: you lost ++++++++++"
					  << std::endl;
			break;
		}
		game.readNextMove();
		if (! game.isAlive()) {
			std::cout << "********** GAME OVER: YOU WON **********"
					  << std::endl;
			break;
		}
		game.makeNextMove();
	}
}
