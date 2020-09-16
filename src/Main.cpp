#include "../include/OK/Window.hpp"
#include "../include/OK/EditorPanel.hpp"
#include "../include/OK/Input.hpp"
#include "../include/OK/factories/Factory.hpp"
#include "../include/OK/validators/Validator.hpp"
#include "../include/OK/MapAnalyzer.hpp"
#include "../include/OK/Config.hpp"

namespace OK {
// Constants
int c_windowWidth;
int c_windowHeight;
const std::string c_binaryMatrixFile = "binaryTransitionMatrix.data";
// Statics:
float Input::s_scrollDelta = 0;
std::vector<std::pair<float, sf::Event::KeyEvent>> Input::s_keys = std::vector<std::pair<float, sf::Event::KeyEvent>>();
std::vector<sf::Event::KeyEvent> Input::s_downKeys = std::vector<sf::Event::KeyEvent>();
int Factory::s_totalProduceAttempts = 0;
int Validator::s_passes = 0;
int Validator::s_fails = 0;
std::vector<Factory*> Generator::s_factories = std::vector<Factory*>();
std::vector<Validator*> Generator::s_validators = std::vector<Validator*>();
GeneratorConfig Config::generator = {};
EditorConfig Config::editor = {};
}


void printUsage() {
    std::printf("Usage: generator.out [AUDIO FILE] [JSON ANNOTATION]\nExample: generator.out song.ogg keyframe.json\n");
}

void analyseMaps(std::vector<std::string> mapFiles, bool append) {
    OK::TransitionMatrix<bool> matrix;
    if (append) {
        matrix.loadFromFile(OK::c_binaryMatrixFile);
    }
    for (std::string file : mapFiles) {
        if (!OK::Util::isFileExtention(file, ".dat")) {
            printf("Error: Unexpected file extention: %s\nExpected .dat\n", file.data());
            return;
        }
        OK::Map map;
        map.load(file);
        if (map.m_notes.size() == 0) {
            printf("Failed to load map: %s\n", file.data());
            continue;
        }
        matrix += OK::MapAnalyzer::AnalyzeMap(map);
    }
    matrix.saveToFile(OK::c_binaryMatrixFile);
}

void generateFromKeyframe(std::string annontationFile) {
    if (!OK::Util::isFileExtention(annontationFile, ".json")) {
        printf("Error: Unexpected file extention: %s\nExpected .json\n", annontationFile.data());
        return;
    }
    printf("Generate!\n");
}

void openEditorWindow(std::string songFile, std::string notationFile = "") {
    if (!OK::Util::isFileExtention(songFile, ".ogg")) {
        printf("Error: Unexpected file extention: %s\nExpected .ogg\n", songFile.data());
        return;
    }

    OK::c_windowWidth = 1600;
    OK::c_windowHeight = 900;

    OK::Window window(OK::c_windowWidth, OK::c_windowHeight);
    OK::EditorPanel editorPanel;
    window.m_activePanel = &editorPanel;
    OK::Generator::Init();
    
    if (editorPanel.loadMusic(songFile)) {
        printf("Loaded song %s\n", songFile.data());
    }
    if (notationFile.length() > 0) {
        editorPanel.loadNotation(notationFile);
        printf("Loaded notation %s\n", notationFile.data());
    }
    while(window.isOpen()) {
        window.update();
        window.draw();
	}
}

int main(int argc, char** argv) {
    if (argc == 1) {
        std::printf("Error: expected arguments.\n");
        printUsage();
        return 1;
    }
    if (!OK::Config::Load()) {
        printf("Error: configuration file config.json not found\n");
        return 2;
    }
    
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    if (args.size() > 1 && std::find(args.begin(), args.end(), "-a") != args.end()) {
        std::vector<std::string> mapsList;
        bool append = true;
        for (int i = 2; i < args.size(); i++) {
            if (std::strcmp(args[i].data(), "--clean") == 0) {
                append = false;
                continue;
            }
            mapsList.push_back(args[i]);
        }
        analyseMaps(mapsList, append);
    }
    else if (args.size() > 1 && std::find(args.begin(), args.end(), "-g") != args.end()) {
        generateFromKeyframe(args[3]);
    }
    else if (args.size() > 2){
        openEditorWindow(args[1], args[2]);
    }
    else {
        openEditorWindow(args[1]);
    }
}