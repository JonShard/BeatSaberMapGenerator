#pragma once
#include <fstream>
#include <iostream>
#include <string>

#include "../nlohmann/json.hpp"

namespace OK {


struct SymmeticalFactoryConfig {
    bool enabled;
    bool centerPointMode; // Note: One of the three symmetry modes must be enabled. Allow outcomes where the pair of notes are complete opposites, mirrored accross a center point. If one note is top left with direction up, the other will be bottom right with direction down. 
    bool horizontalMode; // Note: One of the three symmetry modes must be enabled. Allow outcomes where the generated notes are symmertrical along a vertical plane. Two notes on same layer with same direction, but different lanes.
    bool verticalMode; // Note: One of the three symmetry modes must be enabled. Allow outcomes where the generated notes are symmetrical along a horizontal plane. Two notes on same lane, top and bottom. 
    bool allowOffsetPlane; // Allow the symmmetry plane be offset one note left or right (only applicable to vertical mode).
    bool allowNotesInCenter; // Allow notes to be generatred in the two ceneter positions.
};

struct RandomFactoryConfig {
    bool enabled;
};

struct FactoryConfig {
    int maxAttempts; // The maximum amount of attempts to generate from note A, a valid note B before backtracking by removing note A.
    RandomFactoryConfig random;
    SymmeticalFactoryConfig symmetrical;
};


struct AdjacentValidatorConfig {
    bool enabled;
    float timeToBeAdjacent; // The maximum a time difference between two notes in order for them to be considered adjacent.
};

struct DoubleDownValidatorConfig {
    bool enabled;
    int angleToBeDoubleDown; // The difference in angles two notes have to be for theirs transition to qualify as a double down. Possible values: 0, 45, 90, 135. 180 would deny all notes always.
};

struct MatrixValidatorConfig {
    bool enabled;
    std::string binaryMatrixFilePath; // Path to ascii file containing binary transition matrix containing only 1, 0 and whitespace. 
};

struct ValidatorConfig {
    float validateTimeAfterNote; // Time after a note in which the validator will care if an illegal note is placed (sec).
    MatrixValidatorConfig matrix;
    DoubleDownValidatorConfig doubleDown;
    AdjacentValidatorConfig adjacent;
};

struct GeneratorConfig {
    float noteClusterTime;  // Time that defines how close notes have to be to be considered in the same cluser or "frame".
    FactoryConfig factory;
    ValidatorConfig validator;
};

struct EditorConfig {
    bool autosaveEnabled;
    int windowWidth;    // Amount of pixles of the editor window in X direction.
    int windowHeight;   // Amount of pixles of the editor window in Y direction.
};


struct Config {
    static GeneratorConfig generator;
    static EditorConfig editor;
    static bool Load();
};


} // Namespace OK
