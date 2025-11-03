#include "Results.h"
#include <fstream>

void Results::loadResults(const std::string& filename) {
	std::ifstream results_file(filename);
	size_t size;
	results_file >> score;
	results_file >> size;
	while (!results_file.eof() && size-- != 0) {
		size_t iteration;
		int result;
		results_file >> iteration >> result;
		addResult(iteration, static_cast<ResultValue>(result));
	}
	results_file.close();
}

void Results::saveResults(const std::string& filename, int score) const {
	std::ofstream results_file(filename);
	results_file << score << '\n';
	results_file << results.size();
	for (const auto& result : results) {
		results_file << '\n' << result.first << ' ' << result.second;
	}
	results_file.close();
}

size_t Results::getNextBombIteration() const {
	if (!results.empty() && results.front().second == Loss) {
		return results.front().first;
	}
	else return 0;
	// we use the fact that on iteration zero we cannot hit a bomb
	// to indicate that there isn't any result left with a hit bomb event
}

void Results::reportResultError(const std::string& message, std::string fileName, size_t iteration) {
	system("cls");
	std::cout << "Screen " << fileName << " - " << message << '\n';
	std::cout << "Iteration: " << iteration << '\n';
	std::cout << "Press any key to continue to next screens (if any)" << std::endl;
	_getch();
}

void Results::reportEndGameResult(EndResult gameRes, int end_score) {
	system("cls");
	std::cout << "Game working successfully!" << '\n';
	std::cout << "Player " << (gameRes == EndResult::WIN ? "win" : "fail") << " this game!" << '\n';
	std::cout << "Score: " << end_score << '\n';
	std::cout << "Press any key to continue to next screens (if any)" << std::endl;
	_getch();
	return;
}






