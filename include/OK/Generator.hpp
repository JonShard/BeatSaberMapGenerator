#pragma once

#include "Map.hpp"
#include "factories/Factory.hpp"
#include "validators/Validator.hpp"

namespace OK {

class Generator {
protected:
    static std::vector<Factory*> s_factories; // Weigts
    static std::vector<Validator*> s_validators;

    static void SaveMap(Map map);

public:
    Generator();

    static void GenerateMap(std::vector<Notation> notations, std::string songName);
};

} // namespace OK
