#pragma once

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <string>
#include <filesystem>

constexpr int ZERO = 0;

void ShowConsoleCursor(bool showFlag);

void Flush(); // clean the buffer

void getAllFileNames(std::vector<std::string>& vec_to_fill, std::string file_type);
int getScreenNumber(const std::string& filename);
void sortFiles(std::vector<std::string>& files);


