#pragma once
#include <fstream>
#include <iostream>
#include <string>

#include "../nlohmann/json.hpp"

namespace OK {

struct FactoryConfig {
    int maxAttempts; // The maximum amount of attempts to generate from note A, a valid note B before backtracking by removing note A.
};

struct DoubleDownValidatorConfig {
    int angleToBeDoubleDown = 90; // The difference in angles two notes have to be for theirs transition to qualify as a double down. Possible values: 0, 45, 90, 135. 180 would deny all notes always.
};

struct ValidatorConfig {
    float validateTimeAfterNote; // Time after a note in which the validator will care if an illegal note is placed (sec).
    DoubleDownValidatorConfig doubleDown;
};

struct GeneratorConfig {
    float noteClusterTime;  // Time that defines how close notes have to be to be considered in the same cluser or "frame".
    FactoryConfig factory;
    ValidatorConfig validator;
};

struct EditorConfig {
    int windowWidth;    // Amount of pixles of the editor window in X direction.
    int windowHeight;   // Amount of pixles of the editor window in Y direction.
};


struct Config {
    static GeneratorConfig generator;
    static EditorConfig editor;
    static bool Load();
};


} // Namespace OK
