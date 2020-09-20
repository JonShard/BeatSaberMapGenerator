#include "../include/OK/Generator.hpp"


namespace OK {

    bool Generator::Init() {
        if (Config::generator.factory.random.enabled) s_factories.push_back(new RandomFactory);
        if (Config::generator.validator.matrix.enabled) s_validators.push_back(new MatrixValidator);
        if (Config::generator.validator.doubleDown.enabled) s_validators.push_back(new DoubleDownValidator);
        
        if (s_factories.size() == 0) {
            printf("Error: there must be atleast one enabled factory\n");
            return false;
        }

        return true;
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
                printf("Ran out of max attempts: %d. Escaping absorbing state: \n", produceAttempts);
                printf("Absorbing note being removed: ");
                map.m_notes.back().print();
                printf("Last attempted transition:    ");
                mapNext.m_notes.back().print();
                map.m_notes.pop_back();
                printf("Map length after removing absorbing note: %ld\n\n", map.m_notes.size());
                i--;
                continue;
            }

            map = mapNext;
            printf("Processed keyframe %d at time: %f \tMap length: %ld \tProduce attempts: %d\n", i, notation.m_keyframes[i].time, map.m_notes.size(), produceAttempts);
        }
        return map;
    }


} // namespace OK