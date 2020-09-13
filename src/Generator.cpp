#include "../include/OK/Generator.hpp"
#include "../include/OK/factories/RandomMatrixFactory.hpp"
#include "../include/OK/validators/MatrixValidator.hpp"

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
 
    void Generator::GenerateMap(Notation notation) {
        Map map(notation.getName() + ".dat");
        printf("Generating map from nototions: %ld\n", notation.m_keyframes.size());
        for (int i = 0; i < notation.m_keyframes.size(); i++) {
            int produceAttempts = 0;
            printf("Processing keyframe %d at time: %f \tMap length: %ld \tProduce attempts: %d\n", i, notation.m_keyframes[i].time, map.m_notes.size(), produceAttempts);
            Map mapNext;
            do {
                mapNext = map;
                mapNext += s_factories[0]->produce(notation.m_keyframes, map);
                produceAttempts++;
            } while (!IsValid(mapNext) && produceAttempts < 100000);
            map = mapNext;
        }
        map.save();
    }


} // namespace OK