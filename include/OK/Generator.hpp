// Generator is the manager of all the factories and validators.
// It controls the order factories are called in to build the map.
#pragma once

#include "factories/Factory.hpp"
#include "factories/RandomMatrixFactory.hpp"

#include "validators/Validator.hpp"


namespace OK {

class Generator {
protected:
    static std::vector<Factory*> s_factories; // Weights
    static std::vector<Validator*> s_validators;

    static void SaveMap(Map map);
    static bool IsValid(Map map);
public:
    Generator();
    static void Init();

    static void GenerateMap(Notation notation);
};

} // namespace OK
