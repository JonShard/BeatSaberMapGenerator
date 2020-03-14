#include "../include/OK/Map.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::printf("Error: Expected 2 perameters. Json file and audio file.\n");
        return 1;
    }

    OK::Map* map = new OK::Map(std::string(argv[1]));
    map->Print();
    
}