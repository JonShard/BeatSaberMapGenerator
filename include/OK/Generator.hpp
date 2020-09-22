#pragma once

#include "factories/RandomFactory.hpp"
#include "factories/SymmetricalFactory.hpp"
#include "validators/MatrixValidator.hpp"
#include "validators/DoubleDownValidator.hpp"

namespace OK {

// Generator is the manager of all the factories and validators.
// It controls the order factories are called in to build the map.
class Generator {
protected:
    static std::vector<Factory*> s_factories; // Weights
    static std::vector<Validator*> s_validators;

    static bool IsValid(Map map);
public:
    Generator();
    static bool Init();

    static Map GenerateMap(Notation notation);
};

} // namespace OK
