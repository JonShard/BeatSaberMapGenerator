all:
		clear
		mkdir -p .build
		g++ -g -c src/main.cpp -o .build/main.o

		g++ -g -c src/Editor.cpp -o .build/Editor.o -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
		g++ -g -c src/EditorPanel.cpp -o .build/EditorPanel.o -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
		g++ -g -c src/Window.cpp -o .build/Window.o -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
		g++ -g -c src/Input.cpp -o .build/Input.o -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

		g++ -g -c src/Map.cpp -o .build/Map.o

		g++ .build/main.o -o generator.out .build/EditorPanel.o .build/Map.o .build/Window.o .build/Editor.o .build/Input.o -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system