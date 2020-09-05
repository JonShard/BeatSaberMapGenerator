#include "../include/OK/Generator.hpp"

namespace OK {

    void Generator::GenerateMap(std::vector<Notation> notations, std::string songName) {
        Map map(songName);
        

        SaveMap(map);
    }

    void Generator::SaveMap(Map map) {

    }


} // namespace OK