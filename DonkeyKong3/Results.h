#pragma once
#include <iostream>
#include <list>
#include <string>
#include <conio.h>

class Results
{
public:
	enum ResultValue { Loss, Finish, NoResult };
	enum EndResult { WIN, FAIL };
private:
	std::list<std::pair<size_t, ResultValue>> results; // pair: iteration, result
	int score;
public:
	void loadResults(const std::string& filename);
	void saveResults(const std::string& filename, int score) const;
	void addResult(size_t iteration, ResultValue result) {
		results.push_back({ iteration, result });
	}
	std::pair<size_t, ResultValue> popResult() {
		if (results.empty()) return { 0, Results::NoResult };
		auto result = results.front();
		results.pop_front();
		return result;
	}
	bool isFinishedGame(size_t iteration) const {
		return results.empty() || results.back().first <= iteration;
	}
	size_t getNextBombIteration() const;
	void reportResultError(const std::string& message, std::string fileName, size_t iteration);

	void reportEndGameResult(EndResult gameRes, int end_score);

	void clearResults() {
		results.clear();
	}

	int getScore() {
		return score;
	}
};