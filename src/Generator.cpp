#include "../include/OK/Generator.hpp"
#include "../include/OK/factories/RandomMatrixFactory.hpp"

namespace OK {

    void Generator::Init() {
        s_factories.push_back(new RandomMatrixFactory);
    }

    void Generator::GenerateMap(std::vector<Notation> notations, std::string songName) {
        Map map(songName + ".dat");
        printf("Generating map from nototions: %ld\n", notations.size());
        
        for (int i = 0; i < notations.size(); i++) {
            // TODO: operator override += :
            std::vector<Note> notes = s_factories[0]->produce(map);
            for (Note n : notes) {
                n.time = notations[i].time;
                map.m_map.push_back(n);
            }
        }
        
        map.save();
    }


} // namespace OK