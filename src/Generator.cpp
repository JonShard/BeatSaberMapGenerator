#include "../include/OK/Generator.hpp"


namespace OK {

    bool Generator::Init() {
        s_factories.clear();
        for (int i = 1; i <= c_maxNotesInCluster; i++) {
            s_factories.insert({i, std::vector<Factory*>()});
        }

        if (Config::generator.factory.random.enabled) RegisterFactory(new RandomFactory);
        if (Config::generator.factory.symmetrical.enabled) RegisterFactory(new SymmetricalFactory);
        if (Config::generator.factory.markov.enabled) RegisterFactory(new MarkovFactory);
        if (Config::generator.factory.line.enabled) RegisterFactory(new LineFactory);
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

    void Generator::RegisterFactory(Factory* factory) {
        for (int i = 1; i <= c_maxNotesInCluster; i++) {
            if (factory->canProduceAmount(i)) {
                s_factories.at(i).push_back(factory);
            }
        }
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
        printf("\n=== Report ===\nFactories for each custer size:\n");
        for (int i = 1; i <= c_maxNotesInCluster; i++) {
            printf("%d - ", i);
            for (Factory* f : s_factories.at(i)) {
                printf("%s, ",f->getName().c_str());
            }
            printf("\n");
        }
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

    Factory* Generator::PickFactoryWithDesiredSize(int desiredSize) {
        for (int i = desiredSize; i > 0; i--) {
            if (s_factories.at(i).size() > 0) { 
                if (i != desiredSize) {
                    printf("\tWarning: No factories for desired cluster size of %d, using next best: %d", desiredSize, i);
                }
                return s_factories.at(i)[Util::rng(0, s_factories.at(i).size())];
            }
        }
        return nullptr;
    }

    Map Generator::GenerateMap(Notation notation) {
        printf("Generating map from nototion with keyframes: %ld\n", notation.m_keyframes.size());
        Map map(notation.m_name + "_generated");

        Keyframe previousKeyframe{};
        for (int i = 0; i < notation.m_keyframes.size(); i++) {
            Keyframe k = notation.m_keyframes[i];
            printf("Start keyframe id: %ld \tTime: %f\t\tMap length: %d \tConcurrent: %d\tTime delta: %f\n", k.id, k.time, map.getNoteCount(), k.concurrent, k.time - previousKeyframe.time);
            int produceAttempts = 0;
            Map mapNext;
            do {
                produceAttempts++;
                mapNext = map;
                Factory* factory = PickFactoryWithDesiredSize(k.concurrent);
                if (factory == nullptr) {
                    printf("Error: Could not find a factory that can produce a cluster of size %d or smaller. Unable to continue\n", k.concurrent);
                    PrintReport(map);
                    return map;
                }
                Cluster cluster = factory->produce(notation, map, k.concurrent);
                if (cluster.m_notes.size() == 0) {
                    produceAttempts++;
                    printf("\tWarning: %s produced an empty cluster", factory->getName().c_str());
                    continue;
                }
                printf("\tGenerated cluster(keyframe id: %ld):\n", k.id);
                cluster.print("\t");
                mapNext += cluster;
            } while (!IsValid(mapNext) && produceAttempts < Config::generator.factory.maxAttempts);
            
            // If the last note in map is an absorbing state that can't be transitioned away from, pop it, try again.
            if (map.m_clusters.size() > 0 && produceAttempts >= Config::generator.factory.maxAttempts) {
                Generator::s_backtracks++;
                printf("\tFailed to produce valid cluster in attempts: %d. Removing absorbing cluster: \n", produceAttempts);
                map.m_clusters.back().print("\t");
                map.m_clusters.pop_back();
                map.m_clusters.pop_back();

                printf("\tLast attempted cluster:\n");
                mapNext.getNotes().back().print();
                PrintReport(map);
                
                i -= 2;
                if (i < 0) {
                    i = 0;
                    previousKeyframe = Keyframe{};
                    map.m_clusters.clear();
                }
                continue;
            }
            printf("End keyframe   id: %ld\tProduce attempts: %d\tMap length: %d\t\tBacktracks: %lu\tFactory runs: %lu\tValidator passes: %lu, \tValidator fails: %lu\n\n", 
                k.id, produceAttempts, map.getNoteCount(), Generator::s_backtracks, Factory::getTotalProduceAttempts(), Validator::getTotalPasses(), Validator::getTotalFails());
            
            map = mapNext;
            previousKeyframe = k;
        }
        PrintReport(map);
        return map;
    }


} // namespace OK