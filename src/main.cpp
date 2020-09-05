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
    std::printf("Usage: generator.out [AUDIO FILE] [JSON ANNOTATION]\nExample: generator.out song.ogg annotation.json");
}


int main(int argc, char** argv) {
    if (argc < 2) {
        std::printf("Warning: Expected audio file.\n");
        printUsage();
    }

    // If argv 1 is directory:
        // Load thumbnail as sprite.
        // Load song
        // Load Annotation save
    // Else if argv is audiofile
        // Load audio file.
        // Assume Cover.jpg exist.

    OK::c_windowWidth;
    OK::c_windowHeight;

    OK::c_windowWidth = 1600;
    OK::c_windowHeight = 900;

    OK::Window window(OK::c_windowWidth, OK::c_windowHeight);
    OK::EditorPanel editorPanel;
    window.m_activePanel = &editorPanel;
    OK::Generator::Init();
    if (argc > 1) {
        if (editorPanel.openFromFile(argv[1])) {
            printf("Loaded song %s\n", argv[1]);
            editorPanel.play();
        } else {
            return 1;
        }
    }

    while(window.isOpen()) {
        window.update();
        window.draw();
	}
    
}