#include "../include/OK/Generator.hpp"


namespace OK {

    void Generator::Init() {
        s_factories.push_back(new RandomFactory);
        s_validators.push_back(new MatrixValidator);
    }

    bool Generator::IsValid(Map map) {
        for (Validator* v : s_validators) {
            if (!v->validate(map)) 
                return false;
        }
        return true;
    }
 
    Map Generator::GenerateMap(Notation notation) {
        Map map(Util::removeFileExtention(notation.getName(), "_notation.json") + ".dat");
        printf("Generating map from nototions: %ld\n", notation.m_keyframes.size());
        for (int i = 0; i < notation.m_keyframes.size(); i++) {
            int produceAttempts = 0;
            Map mapNext;
            do {
                mapNext = map;
                mapNext += s_factories[0]->produce(notation, map);
                produceAttempts++;
            } while (!IsValid(mapNext) && produceAttempts < Config::generator.factory.maxAttempts);
            
            // If the last note in map is an absorbing node that can't be transitioned away from, pop it, try again.
            if (map.m_notes.size() > 0 && produceAttempts >= Config::generator.factory.maxAttempts) {
                map.m_notes.pop_back();
                i--;
                continue;
            }

            map = mapNext;
            printf("Processed keyframe %d at time: %f \tMap length: %ld \tProduce attempts: %d\n", i, notation.m_keyframes[i].time, map.m_notes.size(), produceAttempts);
        }
        return map;
    }


} // namespace OK