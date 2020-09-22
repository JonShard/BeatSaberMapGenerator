#include "../include/OK/Generator.hpp"


namespace OK {

    bool Generator::Init() {
        if (Config::generator.factory.random.enabled) s_factories.push_back(new RandomFactory);
        if (Config::generator.factory.symmetrical.enabled) s_factories.push_back(new SymmetricalFactory);
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
        printf("Generating map from nototions: %ld\n", notation.m_keyframes.size());
        Map map(Util::removeFileExtention(notation.getName(), "_notation.json") + ".dat");
        int notesAddedLast = 0;

        for (int i = 0; i < notation.m_keyframes.size(); i++) {
            printf("Start keyframe %d \tTime: %f\t\tMap length: %ld \tConcurrent %d\n", i, notation.m_keyframes[i].time, map.m_notes.size(), notation.m_keyframes[i].concurrent);
            int produceAttempts = 0;
            Map mapNext;
            do {
                mapNext = map;
                if (notation.m_keyframes[i].concurrent == 2) {
                    mapNext += s_factories[1]->produce(notation, map);
                }
                else {
                    mapNext += s_factories[0]->produce(notation, map);
                }
                produceAttempts++;
            } while (!IsValid(mapNext) && produceAttempts < Config::generator.factory.maxAttempts);
            
            // If the last note in map is an absorbing node that can't be transitioned away from, pop it, try again.
            if (map.m_notes.size() > 0 && produceAttempts >= Config::generator.factory.maxAttempts) {
                printf("\tRan out of max attempts: %d. Escaping absorbing state: \n", produceAttempts);
                for (int j = 0; j < notesAddedLast; j++) {
                    printf("\tAbsorbing note being removed: \t%s" ,map.m_notes.back().toString().data());
                    map.m_notes.pop_back();
                }
                for (int k = 0; k < mapNext.m_notes.size() - map.m_notes.size(); k++) {
                    printf("\tLast attempted transition:    \t%s\n", mapNext.m_notes.back().toString().data());
                    mapNext.m_notes.pop_back();
                }
                i--;
                continue;
            }
            notesAddedLast = mapNext.m_notes.size() - map.m_notes.size();
            map = mapNext;
             printf("End keyframe   %d\tProduce attempts: %d\tMap length: %ld\n\n", i, produceAttempts, map.m_notes.size());
        }
        return map;
    }


} // namespace OK