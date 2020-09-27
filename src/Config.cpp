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
        
            nlohmann::json jsRandomFactory = jsFactories["randomFactory"];
                jsRandomFactory.at("enabled").get_to(generator.factory.random.enabled);
            
            nlohmann::json jsSymmetricalFactory = jsFactories["symmetricalFactory"];
                jsSymmetricalFactory.at("enabled").get_to(generator.factory.symmetrical.enabled);
                jsSymmetricalFactory.at("centerPointMode").get_to(generator.factory.symmetrical.centerPointMode);
                jsSymmetricalFactory.at("horizontalMode").get_to(generator.factory.symmetrical.horizontalMode);
                jsSymmetricalFactory.at("verticalMode").get_to(generator.factory.symmetrical.verticalMode);
                jsSymmetricalFactory.at("allowOffsetPlane").get_to(generator.factory.symmetrical.allowOffsetPlane);
                jsSymmetricalFactory.at("allowNotesInCenter").get_to(generator.factory.symmetrical.allowNotesInCenter);

        nlohmann::json jsValidators = jsGenerator["validators"];
            jsValidators.at("validateTimeAfterNote").get_to(generator.validator.validateTimeAfterNote);
            
            nlohmann::json jsMatrixValidator = jsValidators["matrixValidator"];
                jsMatrixValidator.at("enabled").get_to(generator.validator.matrix.enabled);
                jsMatrixValidator.at("binaryMatrixFilePath").get_to(generator.validator.matrix.binaryMatrixFilePath);

            nlohmann::json jsDoubleDownValidator = jsValidators["doubleDownValidator"];
                jsDoubleDownValidator.at("enabled").get_to(generator.validator.doubleDown.enabled);
                jsDoubleDownValidator.at("angleToBeDoubleDown").get_to(generator.validator.doubleDown.angleToBeDoubleDown);


    nlohmann::json jsEditor = jsConfig["editor"];
        jsEditor.at("windowWidth").get_to(editor.windowWidth);
        jsEditor.at("windowHeight").get_to(editor.windowHeight);
    
    printf("Loaded config.json\n");
    return true;
}

} // Namespace OK
