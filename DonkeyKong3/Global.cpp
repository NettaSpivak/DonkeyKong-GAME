#include "Global.h"



void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}


void Flush() {
    while (_kbhit()) {
        _getch();
    }
}


void getAllFileNames(std::vector<std::string>& vec_to_fill, std::string file_type) {
    namespace fs = std::filesystem;
    for (const auto& entry : std::filesystem::directory_iterator(fs::current_path())) {
        auto filename = entry.path().filename();
        auto filenameStr = filename.string();
        if (filenameStr.substr(0, 5) == "dkong" && filename.extension() == file_type && fs::is_regular_file(entry.path())) {
            vec_to_fill.push_back(filenameStr);
        }
    }
    sortFiles(vec_to_fill);
}

// Function that takes a filename and returns the number after "dkong_"
int getScreenNumber(const std::string& filename) {
    size_t underscorePos = filename.find('_'); // Find position of "_" in filename
    size_t dotPos = filename.find(".screen"); // Find position of ".screen"

    if (underscorePos != std::string::npos && dotPos != std::string::npos) { // If we found both "_" and ".screen"
        // Extract the part between "_" and ".screen" and convert it to number
        std::string numStr = filename.substr(underscorePos + 1, dotPos - underscorePos - 1);
        return std::stoi(numStr);
    }
    return -1;  // If not found, return -1
}

void sortFiles(std::vector<std::string>& files) {
    std::sort(files.begin(), files.end(),
        [](const std::string& a, const std::string& b) {
            return getScreenNumber(a) < getScreenNumber(b);
        });
}