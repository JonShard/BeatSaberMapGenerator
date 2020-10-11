#include "../include/OK/Window.hpp"
#include "../include/OK/EditorPanel.hpp"
#include "../include/OK/Input.hpp"
#include "../include/OK/factories/Factory.hpp"
#include "../include/OK/validators/Validator.hpp"
#include "../include/OK/Song.hpp"
#include "../include/OK/MapAnalyzer.hpp"
#include "../include/OK/Config.hpp"

enum ExitCode { DONE, MISSING_ARGUMENTS, BAD_CONFIG };

namespace OK {
const std::string c_binaryMatrixFile = "binaryTransitionMatrix.data";
// Statics:
float Input::s_scrollDelta = 0;
std::vector<std::pair<float, sf::Event::KeyEvent>> Input::s_keys = std::vector<std::pair<float, sf::Event::KeyEvent>>();
std::vector<sf::Event::KeyEvent> Input::s_downKeys = std::vector<sf::Event::KeyEvent>();
unsigned long Factory::s_totalProduceAttempts = 0;
unsigned long Validator::s_totalPasses = 0;
unsigned long Validator::s_totalFails = 0;
unsigned long Generator::s_backtracks = 0;
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
    int countStart = matrix.getNonZeroCount();
    for (std::string file : mapFiles) {
        if (!OK::Util::isFileExtention(file, ".dat")) {
            printf("Error: Unexpected file extention: %s\nExpected .dat\n", file.data());
            return;
        }
        printf("\n\nLoading mapFile %s\n\n", file.data());
        OK::Map map;
        map.load(file, 120);
        if (map.m_notes.size() == 0) {
            printf("Failed to load map: %s\n", file.data());
            continue;
        }
        matrix += OK::MapAnalyzer::RegisterTransitionsInMap(map);
        printf("done\n");
    }
    printf("\n##### Result #####\nTransition count before: %d\nTrasition count after: %d\nDifference: %d\n", 
    countStart, matrix.getNonZeroCount(), matrix.getNonZeroCount() - countStart);
    matrix.saveToFile(OK::c_binaryMatrixFile);
}

void generateMapFromFile(std::string file) {
    OK::Song song(file);
    int notationIndex = song.loadNotation(file);
    if (notationIndex == -1) {
        notationIndex = song.createNotationFromMap(song.getMap(song.loadMap(file)));  
    }
    if (notationIndex == -1) {
        printf("Error: Invalid file extention or invalid file content: %s\n", file.data());
        return;
    }

    int mapIndex = song.addMap(OK::Generator::GenerateMap(song.getNotation(notationIndex)));
    song.saveMap(mapIndex);
}

void openEditorWindow(std::string songFile, std::string notationFile = "") {
    if (!OK::Util::isFileExtention(songFile, ".ogg")) {
        printf("Error: Unexpected file extention: %s\nExpected .ogg\n", songFile.data());
        return;
    }

    OK::Window window(OK::Config::editor.windowWidth, OK::Config::editor.windowHeight);
    OK::EditorPanel editorPanel;
    window.m_activePanel = &editorPanel;
    
    if (editorPanel.loadMusic(songFile)) {
        printf("Loaded song %s\n", songFile.data());
    }
    if (notationFile.length() > 0) {
        if (!OK::Util::isFileExtention(notationFile, ".json")) {
            printf("Error: Unexpected file extention: %s\nExpected .json\n", notationFile.data());
            return;
        }
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
        return ExitCode::MISSING_ARGUMENTS;
    }
    if (!OK::Config::Load()) {
        printf("Error: configuration file config.json not found\n");
        return ExitCode::BAD_CONFIG;
    }
    if (!OK::Generator::Init()) {
        return ExitCode::BAD_CONFIG;
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
        
        if (args.size() > 3 && std::find(args.begin(), args.end(), "--seed") != args.end()) {
            if (args.size() > 4) {
                srandom(OK::Util::integerSumString(args[4]));
            }
            else {
                printf("Error: expected value for parameter --seed\n");
                return ExitCode::MISSING_ARGUMENTS;
            }

        }
        generateMapFromFile(args[2]);
    }
    else if (args.size() > 2){
        openEditorWindow(args[1], args[2]);
    }
    else {
        openEditorWindow(args[1]);
    }

    return ExitCode::DONE;
}