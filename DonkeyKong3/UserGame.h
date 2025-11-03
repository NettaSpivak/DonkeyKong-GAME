#pragma once
#include "Game.h"

class UserGame : public Game
{
	static constexpr int ESC = 27;
	static constexpr char BACK_TO_MENU = '2';
	static constexpr char INSTRUCTIONS = '8';
	static constexpr char STAGES = '3';
	static constexpr char EXIT = '9';

	bool isSave;


	int stage_menu();
	bool pause();
	void error_board();
	void instructions_menu();
	bool run_level(std::string fileName) override;
	void start() override;
	bool run(int start_stage, bool full_game);
	void set_level(int level) override;
	//void setFilesName(std::string fileName);

public:

	UserGame(bool is_save) : isSave(is_save) {}

};

