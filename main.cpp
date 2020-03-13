#include "include/nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using json = nlohmann::json;
using namespace std;

enum Type { BLUE, RED };
enum CutDirection {UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT};

struct Note {
    float time;
    int lineIndex;
    int lineLayer;
    int type;
    int cutDirection;
};

vector<Note> importMap(string fileName) {
   // read a JSON file
    std::ifstream file(fileName);
    json jsonMap;
    file >> jsonMap;
    
    cout << jsonMap.at("_version")  << endl;
    
    json jsonNotes = jsonMap["_notes"];

    vector<Note> map = vector<Note>();
    for (json jn : jsonNotes) {
        Note n {
            jn.at("_time").get_to(n.time),
            jn.at("_lineIndex").get_to(n.lineIndex),
            jn.at("_lineLayer").get_to(n.lineLayer),
            jn.at("_type").get_to(n.type),
            jn.at("_cutDirection").get_to(n.cutDirection)
        };
        map.push_back(n);
    }
    return map;
}
void exportMap(string fileName, vector<Note> map) {
    json jsNotes;
    json jsonMap;
    
    for (Note n : map) {
        json jn;
        jn["_time"] = n.time;
        jn["_lineIndex"] = n.lineIndex;
        jn["_lineLayer"] = n.lineLayer;
        jn["_type"] = n.type;
        jn["_cutDirection"] = n.cutDirection;
        jsNotes.push_back(jn);
    }

    jsonMap["_version"] = "2.0.0";
    jsonMap["_notes"] = jsNotes;
    std::ofstream o(fileName);
    o << jsonMap;
    o.close();
}

void printMap(vector<Note> map) {
    string dir = "";
    for (Note n : map) {
        switch (n.cutDirection)
        {
        case CutDirection::UP: dir = "UP"; break;
        case CutDirection::DOWN: dir = "DOWN"; break;
        case CutDirection::LEFT: dir = "LEFT"; break;
        case CutDirection::RIGHT: dir = "RIGHT"; break;
        case CutDirection::UP_LEFT: dir = "UP LEFT"; break;
        case CutDirection::UP_RIGHT: dir = "UP RIGHT"; break;
        case CutDirection::DOWN_LEFT: dir = "DOWN LEFT"; break;
        case CutDirection::DOWN_RIGHT: dir = "DOWN RIGHT"; break;
        default: dir = "UNKNOW"; break;
        }
        printf("Note: %f\t%s\t%s\t\n", n.time, (n.type == Type::BLUE)?"BLUE":"RED", dir.data());
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::printf("Error: Expected 2 perameters. Json file and audio file.\n");
        return 1;
    }

    vector<Note> map = importMap(argv[1]);
    printMap(map);
    exportMap("out.json", map);

}