#pragma once
#include <fstream>
#include <unordered_set>
#include <random>
#include <time.h>

class id
{
private:
    std::string letters = "";

    static bool initialized;
    static std::unordered_set<std::string> usedIds;
    std::string static generate();

public:
    id();
    bool static validate(std::string s);
    bool static init(std::ifstream *f);
    void static remove(std::string id);
    std::string getValue();
};
