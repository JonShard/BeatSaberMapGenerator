#include "../include/OK/Generator.hpp"


namespace OK {

    bool Generator::Init() {
        if (Config::generator.factory.random.enabled) s_factories.push_back(new RandomFactory);
        if (Config::generator.factory.symmetrical.enabled) s_factories.push_back(new SymmetricalFactory);
        if (Config::generator.factory.markov.enabled) s_factories.push_back(new MarkovFactory);
        if (Config::generator.factory.line.enabled) s_factories.push_back(new LineFactory);
        if (Config::generator.validator.matrix.enabled) s_validators.push_back(new MatrixValidator);
        if (Config::generator.validator.doubleDown.enabled) s_validators.push_back(new DoubleDownValidator);
        if (Config::generator.validator.adjacent.enabled) s_validators.push_back(new AdjacentValidator);
        if (Config::generator.validator.clusterCurve.enabled) s_validators.push_back(new ClusterCurveValidator);
        s_validators.push_back(new OverlapValidator);
        if (s_factories.size() == 0) {
            printf("Error: there must be atleast one enabled factory\n");
            return false;
        }

        return true;
    }

    bool Generator::IsValid(Map map) {
        for (Note n : map.getNotes()) {
            if (!n.isValid()) { // Something is seriously wrong if the note itself thinks it's invalid.
                printf("Error: generated invalid note:");
                n.print();
                std::exit(4);
            }
        }
        for (Validator* v : s_validators) {
            if (!v->validate(map))
                return false;
        }
        return true;
    }
 
    void Generator::PrintReport(Map map) {
        for (Validator* v : s_validators) {
            v->printReport();
        }
        int blueCount = 0;
        int redCount = 0;
        for (Note n : map.getNotes()) {
            if (n.m_type == BLUE) blueCount++;
            else if (n.m_type == RED) redCount++;
        }
        printf("Blue to red ratio: %f\n\n", blueCount / (float)redCount);
    }

    Map Generator::GenerateMap(Notation notation) {
        printf("Generating map from nototion with keyframes: %ld\n", notation.m_keyframes.size());
        Map map(notation.m_name);
        int notesAddedLast = 0;

        for (int i = 0; i < notation.m_keyframes.size(); i++) {
            printf("Start keyframe %d \tTime: %f\t\tMap length: %d \tConcurrent: %d\tTime delta: %f\n", i, notation.m_keyframes[i].time, map.getNoteCount(), notation.m_keyframes[i].concurrent, (i) ? notation.m_keyframes[i].time - notation.m_keyframes[i-1].time: 0);
            int produceAttempts = 0;
            int randomizedCount = 0;
            Map mapNext;
            do {
                mapNext = map;
                int random;
                int failSafe = 0;
                do {
                    random = Util::rng(0, s_factories.size());
                    failSafe++;
                }while(!s_factories[random]->canProduceAmount(notation.m_keyframes[i].concurrent) && failSafe < 100000);
                Cluster cluster = s_factories[random]->produce(notation, map, notation.m_keyframes[i].concurrent);
                if (cluster.m_notes.size() == 0) {
                    produceAttempts++;
                    continue;
                }
                mapNext += cluster;
                produceAttempts++;
            } while (!IsValid(mapNext) && produceAttempts < Config::generator.factory.maxAttempts);
            // If the last note in map is an absorbing node that can't be transitioned away from, pop it, try again.
            if (map.getNoteCount() > 0 && produceAttempts >= Config::generator.factory.maxAttempts) {
                Generator::s_backtracks++;
                printf("\tRan out of max attempts: %d. Escaping absorbing state: \n", produceAttempts);
                for (int j = 0; j < notesAddedLast; j++) {
                    printf("\tAbsorbing note being removed: \t");
                    map.getNotes().back().print();
                   // map.get.pop_back(); Depricated, map.getNotes() returns copy.
                }
                for (int k = 0; k < mapNext.getNoteCount() - map.getNoteCount(); k++) {
                    printf("\tLast attempted transition:    \t");
                    mapNext.getNotes().back().print();
                    // mapNext.m_notes.pop_back(); Depricated, map.getNotes() returns copy.
                }
                printf("\n");
                PrintReport(map);
                i-= 2;
                continue;
            }
            notesAddedLast = mapNext.getNoteCount() - map.getNoteCount();
            map = mapNext;
            printf("End keyframe   %d\tProduce attempts: %d\tMap length: %d\t\tBacktracks: %lu\tFactory runs: %lu\tValidator passes: %lu, \tValidator fails: %lu\n\n", 
            i, produceAttempts, map.getNoteCount(), Generator::s_backtracks, Factory::getTotalProduceAttempts(), Validator::getTotalPasses(), Validator::getTotalFails());
        }
        PrintReport(map);
        return map;
    }


} // namespace OK