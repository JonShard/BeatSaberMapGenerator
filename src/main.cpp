#include "../include/OK/Window.hpp"
#include "../include/OK/EditorPanel.hpp"

const int c_windowWidth = 1600;
const int c_windowHeight = 900;


void printUsage() {
    std::printf("Usage: generator.out [AUDIO FILE] [JSON ANNOTATION]\nExample: generator.out song.ogg annotation.json");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::printf("Warning: Expected audio file.\n");
        printUsage();
    }

    OK::Window window(c_windowWidth, c_windowHeight);
    OK::EditorPanel editorPanel;
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