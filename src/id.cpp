#include "id.h"
using namespace std;

//static member
unordered_set<string> id::usedIds;
bool id::initialized = false;

//private static methods
string id::generate()
{
    string res = "";
    for (int i = 0; i < 3; i++)
        res += (char)((rand() % 26) + 65);
    return res;
}

bool id::validate(string s)
{
    if (s.length() != 3)
        return false;
    for (int i = 0; i < 3; i++)
    {
        int temp = ((int)s.at(i) - 65);
        if (temp < 0 || temp > 25)
            return false;
    }
    return true;
}

//public methods
id::id()
{
    if (!initialized)
    {
        throw runtime_error("Please initialize ids by calling id::init.");
        return;
    }
    do
        this->letters = generate();
    while (usedIds.find(this->letters) != usedIds.end());
    usedIds.insert(this->letters);
}

//static methods
bool id::init(ifstream *f)
{
    if (initialized)
    {
        throw runtime_error("Error: The list is already initialized");
        return false;
    }
    string currentLine;
    int i = 0;
    while (getline(*f, currentLine))
    {
        string id = currentLine.substr(0, 3);
        if (!validate(id))
        {
            throw runtime_error("Error: id on line " + to_string(i) + " is invalid.");
            id::usedIds.clear();
            return false;
        }
        if (id::usedIds.find(id) != id::usedIds.end())
        {
            throw runtime_error("Error: duplicated id on line " + to_string(i));
            id::usedIds.clear();
            return false;
        }
        id::usedIds.insert(id);
        i++;
    }

    initialized = true;
    return true;
}
void id::remove(string id)
{
    id::usedIds.erase(id);
}
string id::getValue()
{
    return this->letters;
}