#pragma once
#include <fstream>
#include <iostream>
#include <string>

#include "../nlohmann/json.hpp"

namespace OK {

struct FactoryConfig {
    int maxAttempts;
};

struct ValidatorConfig {
    float validateTimeAfterNote; // Time after a note in which the validator will care if an illegal note is placed (sec).
};

struct GeneratorConfig {
    float noteClusterTime;  // Time that defines how close notes have to be to be considered in the same cluser or "frame".
    FactoryConfig factory;
    ValidatorConfig validator;
};

struct EditorConfig {
    int windowWidth;
    int windowHeight;
};


struct Config {
    static GeneratorConfig generator;
    static EditorConfig editor;
    static bool Load();
};


} // Namespace OK
