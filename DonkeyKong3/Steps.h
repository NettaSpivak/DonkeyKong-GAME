#pragma once
#include <list>
#include <string>
#include <array>

class Steps {

	long randomSeed = 0;
	std::list < std::pair < size_t, std::array<char, 2>>>steps; // pair: iteration, step (has place for 2 keys) instelaize to default

public:

	//static constexpr char def_no_chars = ' ';

	//static Steps
	void loadSteps(const std::string& filename);
	void saveSteps(const std::string& filename) const;
	long getRandomSeed() const {
		return randomSeed;
	}
	void setRandomSeed(long seed) {
		randomSeed = seed;
	}
	void addStep(size_t iteration, const std::array<char, 2>& step) {
		steps.push_back({ iteration, step }); // add step
	}
	bool isNextStepOnIteration(size_t iteration) const {
		return !steps.empty() && steps.front().first == iteration;
	}

	std::array<char, 2 > popStep(size_t iteration);

	void clearSteps() {
		steps.clear();
	}
};

