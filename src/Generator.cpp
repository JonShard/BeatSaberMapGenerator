#include "../include/OK/Generator.hpp"
#include "../include/OK/factories/RandomMatrixFactory.hpp"
#include "../include/OK/validators/MatrixValidator.hpp"

namespace OK {

    void Generator::Init() {
        s_factories.push_back(new RandomMatrixFactory);
        s_validators.push_back(new MatrixValidator);
    }

    void Generator::GenerateMap(std::vector<Notation> notations, std::string songName) {
        Map map(songName + ".dat");
        printf("Generating map from nototions: %ld\n", notations.size());
        
        for (int i = 0; i < notations.size(); i++) {
            map += s_factories[0]->produce(map);
        }
        
        map.save();
    }


} // namespace OK