#include "../include/OK/Generator.hpp"


namespace OK {

    bool Generator::Init() {
        if (Config::generator.factory.random.enabled) s_factories.push_back(new RandomFactory);
        if (Config::generator.factory.symmetrical.enabled) s_factories.push_back(new SymmetricalFactory);
        if (Config::generator.validator.matrix.enabled) s_validators.push_back(new MatrixValidator);
        if (Config::generator.validator.doubleDown.enabled) s_validators.push_back(new DoubleDownValidator);
        if (Config::generator.validator.adjacent.enabled) s_validators.push_back(new AdjacentValidator);
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
 
    void Generator::PrintReport() {
        for (Validator* v : s_validators) {
            v->printReport();
        }
        printf("\n");
    }


    Map Generator::GenerateMap(Notation notation) {
        printf("Generating map from nototions: %ld\n", notation.m_keyframes.size());
        Map map(Util::removeFileExtention(notation.getName(), "_notation.json") + ".dat");
        int notesAddedLast = 0;

        for (int i = 0; i < notation.m_keyframes.size(); i++) {
            printf("Start keyframe %d \tTime: %f\t\tMap length: %ld \tConcurrent: %d\n", i, notation.m_keyframes[i].time, map.m_notes.size(), notation.m_keyframes[i].concurrent);
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
                Generator::s_backtracks++;
                printf("\tRan out of max attempts: %d. Escaping absorbing state: \n", produceAttempts);
                for (int j = 0; j < notesAddedLast; j++) {
                    printf("\tAbsorbing note being removed: \t%s\n" ,map.m_notes.back().toString().data());
                    map.m_notes.pop_back();
                }
                for (int k = 0; k < mapNext.m_notes.size() - map.m_notes.size(); k++) {
                    printf("\tLast attempted transition:    \t%s\n", mapNext.m_notes.back().toString().data());
                    mapNext.m_notes.pop_back();
                }
                printf("\n");
                PrintReport();
                i-= 2;
                continue;
            }
            notesAddedLast = mapNext.m_notes.size() - map.m_notes.size();
            map = mapNext;
             printf("End keyframe   %d\tProduce attempts: %d\tMap length: %ld\t\tBacktracks: %lu\tFactory runs: %lu\tValidator passes: %lu, \tValidator fails: %lu\n\n", 
             i, produceAttempts, map.m_notes.size(), Generator::s_backtracks, Factory::getTotalProduceAttempts(), Validator::getTotalPasses(), Validator::getTotalFails());
        }
        PrintReport();
        return map;
    }


} // namespace OK