#include "../include/OK/Window.hpp"
#include "../include/OK/EditorPanel.hpp"
#include "../include/OK/Input.hpp"
#include "../include/OK/factories/Factory.hpp"
#include "../include/OK/validators/Validator.hpp"


namespace OK {
// Constants
int c_windowWidth;
int c_windowHeight;
// Statics:
float Input::s_scrollDelta = 0;
std::vector<std::pair<float, sf::Event::KeyEvent>> Input::s_keys = std::vector<std::pair<float, sf::Event::KeyEvent>>();
std::vector<sf::Event::KeyEvent> Input::s_downKeys = std::vector<sf::Event::KeyEvent>();
int Factory::s_totalProduceAttempts = 0;
int Validator::s_passes = 0;
int Validator::s_fails = 0;
std::vector<Factory*> Generator::s_factories = std::vector<Factory*>();
std::vector<Validator*> Generator::s_validators = std::vector<Validator*>();
}


void printUsage() {
    std::printf("Usage: generator.out [AUDIO FILE] [JSON ANNOTATION]\nExample: generator.out song.ogg annotation.json\n");
}

void analyseMaps(std::vector<std::string> mapFiles) {
    // Verify only .dat files.
    printf("Analyse!\n");
}

void generateFromAnnotation(std::string annontationFile) {
    // Verify only .json file.
    printf("Generate!\n");
}

void openEditorWindow(std::string songFile) {
    OK::c_windowWidth;
    OK::c_windowHeight;

    OK::c_windowWidth = 1600;
    OK::c_windowHeight = 900;

    OK::Window window(OK::c_windowWidth, OK::c_windowHeight);
    OK::EditorPanel editorPanel;
    window.m_activePanel = &editorPanel;
    OK::Generator::Init();
    
    if (editorPanel.openFromFile(songFile)) {
        printf("Loaded song %s\n", songFile.data());
        editorPanel.play();
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
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    if (args.size() > 1 && std::find(args.begin(), args.end(), "-a") != args.end()) {
        std::vector<std::string> mapsList;
        for (int i = 2; i < args.size(); i++)
            mapsList.push_back(args[i]);
        analyseMaps(mapsList);
    }
    else if (args.size() > 1 && std::find(args.begin(), args.end(), "-g") != args.end()) {
        generateFromAnnotation(args[3]);
    }
    else {
        openEditorWindow(args[1]);
    }
}