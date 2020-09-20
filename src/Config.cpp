#include "../include/OK/Config.hpp"

namespace OK {

bool Config::Load() {
    std::ifstream file("config.json");
    if (file.fail()) {
        return false;
    }
    nlohmann::json jsConfig;
    file >> jsConfig;
    
    nlohmann::json jsGenerator = jsConfig["generator"];
            jsGenerator.at("noteClusterTime").get_to(generator.noteClusterTime);
        nlohmann::json jsFactories = jsGenerator["factories"];
            jsFactories.at("maxAttempts").get_to(generator.factory.maxAttempts);
        nlohmann::json jsValidators = jsGenerator["validators"];
            jsValidators.at("validateTimeAfterNote").get_to(generator.validator.validateTimeAfterNote);
            nlohmann::json jsDoubleDownValidator = jsValidators["doubleDownValidator"];
                jsDoubleDownValidator.at("angleToBeDoubleDown").get_to(generator.validator.doubleDown.angleToBeDoubleDown);
    nlohmann::json jsEditor = jsConfig["editor"];
        jsEditor.at("windowWidth").get_to(editor.windowWidth);
        jsEditor.at("windowHeight").get_to(editor.windowHeight);
    
    printf("Loaded config.json\n");
    return true;
}

} // Namespace OK
