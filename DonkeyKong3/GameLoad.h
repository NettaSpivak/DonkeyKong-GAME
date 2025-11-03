#pragma once
#include "Game.h"


class GameLoad : public Game
{
	static constexpr int first_stage = 1;
	bool isSilent;
	std::vector<std::string> steps_file;
	std::vector<std::string> results_file;

	void start() override;
	bool run_level(std::string fileName) override;
	bool run(int start_stage);
	void set_level(int level) override;
	bool checkUnMatchFinishGame(int iteration, std::string fileName);
	bool checkUnMatchMidGameResult(int iteration, std::string fileName);
	void getSavedScreensFileNames();

public:
	GameLoad(bool is_silent) : Game(is_silent), isSilent(is_silent) { // false for isSave
		b.set_isSilent(is_silent);
	}

};

