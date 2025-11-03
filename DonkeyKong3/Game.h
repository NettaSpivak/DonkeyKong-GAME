#pragma once

#include <iostream>
#include <conio.h>
#include <vector>
#include "Mario.h"
#include "Barrel.h"
#include "Climbed_Ghost.h"
#include "Steps.h"
#include "Results.h"

class Game
{
	bool isSilent = false;

protected:
	static constexpr char PLAY = '1';
	static constexpr int HAMMER_SCORE = 600;
	static constexpr int GRAB_HAMMER_SCORE = 600;
	static constexpr int TAKE_LIFE_SCORE = 600;
	static constexpr int GAME_LOOP_SCORE = 10000;
	static constexpr int NUM_OF_BARRELS = 30; // how many barrels can be on board at the same time in game
	static constexpr int GAME_LIFE = 3; // num of life for player
	static constexpr int COMMON_START_BARREL = 15;

	Board b;
	Steps steps;
	Results results;
	std::vector<std::string> stages;
	std::vector<Ghost*> ghosts;
	Mario player{ &b , GAME_LIFE };
	Barrel barrels[NUM_OF_BARRELS];
	int score = ZERO;
	long random_seed;
	bool is_p_pressed = false;

	// array order is exactly the same as the directions array - must keep it that way
	static constexpr char keys[] = { 'w', 'a', 'x', 'd', 's' }; // keys options (up, left, down, right, stay)
	static constexpr size_t numKeys = sizeof(keys) / sizeof(keys[0]); // keys options array size

public:

	Game() : b(GAME_LIFE) {}
	Game(bool is_silent) : b(GAME_LIFE), isSilent(is_silent) {}

	~Game() {
		for (int i = 0; i < ghosts.size(); i++) {
			delete ghosts[i];
		}
	}

	void set_barrels() {
		for (size_t i = 0; i < NUM_OF_BARRELS; i++)
			barrels[i] = { b.barrel_ch, &b, b.DONKEY_KONG_POS };
	}
	virtual bool run_level(std::string fileName) = 0;
	bool run(int start_stage, bool full_game);
	virtual void set_level(int level);
	int calc_win_score(int game_loop); // clac score addition according to game time
	bool keyPressed(char key); // if the pressed key is a moving key then change direction + check if the key is a known key
	void restart_game();
	virtual void start() = 0;
	bool win_stage();
	void victory();
	void fail();
	bool Is_mario_and_enemy_switch_and_hit() const;
	//Barrel_Arr func-> Game func:
	void find_availble_barrel();
	void draw_enemys();
	void restore_enemys();
	void enemys_movement();
	void ghost_hit_ghost();
	bool enemys_hit_something(Point mario_p) const;
	void enemys_restart();
	bool IsAnyEnemyPrevPosHitPoint(Point p, Point p_prev) const;
	bool checksAndPreparesGame();
	void p_pressed();
	bool hammer_hit_enemy(Point hammer);

};

