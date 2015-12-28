#include <iostream>
#include <iomanip>
#include <cassert>
#include <array>
#include <vector>
#include <set>

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



void ShowVictory(const PlayArray & data, size_t i, PlayerIndex player,
				 bool skip_winnable, size_t indent = 0)
{
	std::cout << "[" << player
			  << " " << (AsBool(data[i].can_force_win) ? "win" : "   ")
			  << " " << std::setw(2) << (indent+1)
			  << "]" << std::setw((4*indent)+1) << "" << data[i].state;
	if (! data[i].state.next().isAlive()) {
		std::cout << "  (player " << !player << " wins!)" << std::endl;
	} else if (skip_winnable && AsBool(data[i].can_force_win)) {
		std::cout << "  !!avoid!!" << std::endl;
	} else {
		std::cout << " ->" << std::endl;
		for (auto nrmi = data[i].next_regular_moves.cbegin();
			 nrmi != data[i].next_regular_moves.cend();
			 ++nrmi) {
			ShowVictory(data, *nrmi, !player, !skip_winnable, indent+1);
		}
	}
}



int main()
{
	if (false) {
		TestAll();
	}


	PlayArray data;

	size_t num_directly_reachable = 0;
	size_t num_reachable = 0;
	size_t num_r_alive = 0;
	std::vector<size_t> num_next_moves;
	std::vector<size_t> num_prev_moves;
	for (size_t i = 0;  i < data.size();  ++i) {
		assert(data[i].state.other().isAlive());
		if (AsBool(data[i].is_directly_reachable)) {
			++num_directly_reachable;
		}
		if (AsBool(data[i].is_reachable)) {
			++num_reachable;
			if (data[i].state.next().isAlive()) {
				++num_r_alive;
			}

			{
				size_t n = data[i].next_regular_moves.size();
				if (n >= num_next_moves.size()) {
					num_next_moves.resize(n+1);
				}
				++(num_next_moves.at(n));
			}

			{
				size_t n = data[i].prev_regular_moves.size();
				if (n >= num_prev_moves.size()) {
					num_prev_moves.resize(n+1);
				}
				++(num_prev_moves.at(n));
			}
		}
	}

	std::cout << "Total positions:    " << data.size() << std::endl;
	std::cout << "Directly reachable: " << num_directly_reachable
			  << " (+ start)" << std::endl;
	std::cout << "Reachable:          " << num_reachable << std::endl;
	std::cout << "Play is continuing: " << num_r_alive << std::endl;
	if (false) {
		for (size_t i = 0;  i < data.size();  ++i) {
			if (AsBool(data[i].is_reachable) &&
				data[i].state.next().isAlive()) {
				std::cout << "  " << data[i].state;
			}
		}
		std::cout << std::endl;
	}
	std::cout << "Play is done:       "
			  << (num_reachable - num_r_alive) << std::endl;
	if (false) {
		for (size_t i = 0;  i < data.size();  ++i) {
			if (AsBool(data[i].is_reachable) &&
				! data[i].state.next().isAlive()) {
				std::cout << "  " << data[i].state;
			}
		}
		std::cout << std::endl;
	}
	if (false) {
		std::cout << "DR & !R:" << std::endl;
		for (size_t i = 0;  i < data.size();  ++i) {
			if (AsBool(data[i].is_directly_reachable) &&
				! AsBool(data[i].is_reachable)) {
				std::cout << "  " << data[i].state;
			}
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	for (size_t j = 1;  j < num_next_moves.size();  ++j) {
		std::cout << "Can reach " << std::setw(2) << j << " descendants:    "
				  << num_next_moves.at(j) << std::endl;
		if (false) {
			for (size_t i = 0;  i < data.size();  ++i) {
				if (AsBool(data[i].is_reachable) &&
					(data[i].next_regular_moves.size() == j)) {
					std::cout << "  " << data[i].state << " ->";
					for (auto nrmi = data[i].next_regular_moves.cbegin();
						 nrmi != data[i].next_regular_moves.cend();
						 ++nrmi) {
						std::cout << "  " << data[*nrmi].state;
					}
					std::cout << std::endl;
				}
			}
		}
	}

	std::cout << std::endl;
	for (size_t j = 0;  j < num_prev_moves.size();  ++j) {
		std::cout << "Reachable from " << std::setw(2) << j << " ancestors: "
				  << num_prev_moves.at(j) << std::endl;
		if (false) {
			for (size_t i = 0;  i < data.size();  ++i) {
				if (AsBool(data[i].is_reachable) &&
					(data[i].prev_regular_moves.size() == j)) {
					std::cout << "  " << data[i].state << " <-";
					for (auto prmi = data[i].prev_regular_moves.cbegin();
						 prmi != data[i].prev_regular_moves.cend();
						 ++prmi) {
						std::cout << "  " << data[*prmi].state;
					}
					std::cout << std::endl;
				}
			}
		}
	}

	std::cout << std::endl;
	{
		auto p = MakePlayers(1, 1, 1, 1).seq();
		std::cout << "Player starting from " << data[p].state
				  << " is expected to win?  " << data[p].can_force_win
				  << std::endl;
		ShowVictory(data, p, PlayerIndex::A, ! AsBool(data[p].can_force_win));
	}

	std::cout << std::endl;
	{
		std::vector<size_t> num_winning_with_next;
		for (size_t i = 0;  i < data.size();  ++i) {
			if (AsBool(data[i].is_reachable)) {
				if (data[i].state.next().isAlive()) {
					++num_r_alive;
				}

				{
					size_t n = data[i].next_regular_moves.size();
					if (n >= num_winning_with_next.size()) {
						num_winning_with_next.resize(n+1);
					}
					++(num_winning_with_next.at(n));
				}

				{
					size_t n = data[i].prev_regular_moves.size();
					if (n >= num_prev_moves.size()) {
						num_prev_moves.resize(n+1);
					}
					++(num_prev_moves.at(n));
				}
			}
		}
	}
//		std::cout << "Opponent's positions:" << std::endl;
//	}
}
