#include "../include/OK/Map.hpp"
#include "../include/OK/Window.hpp"

void printUsage() {
    std::printf("Usage: generator.out [AUDIO FILE] [JSON ANNOTATION]\nExample: generator.out song.ogg annotation.json");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::printf("Warning: Expected audio file.\n");
        printUsage();
    }

    OK::Window window(1600, 900);
    
    while(window.isOpen()) {
        window.update();
        window.draw();
	} 
    
}