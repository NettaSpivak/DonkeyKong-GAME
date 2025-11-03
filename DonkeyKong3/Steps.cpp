#include "Steps.h"
#include <fstream>


void Steps::loadSteps(const std::string& filename) {
	//Steps steps;
	std::ifstream steps_file(filename); // open file
	steps_file >> randomSeed; // read from file the seed (data member)
	size_t size;
	steps_file >> size; // how many steps in the file
	while (!steps_file.eof() && size-- != 0) {
		size_t iteration;
		std::array<char, 2> step;
		steps_file >> iteration >> step[0];
		steps_file.get(step[1]); // if there is only one step then file has ' ' at step[1]

		addStep(iteration, step); // enter step to steps - data member of LoadGame 
	}
	steps_file.close();
}

void Steps::saveSteps(const std::string& filename) const {
	std::ofstream steps_file(filename);
	steps_file << randomSeed << '\n' << steps.size();
	for (const auto& step : steps) {
		steps_file << '\n' << step.first << ' ' << step.second[0] << step.second[1];
	}
	steps_file.close();
}

std::array<char, 2> Steps::popStep(size_t iteration) {
	std::array<char, 2 > step;
	step = steps.front().second;
	steps.pop_front();
	return step;
}

