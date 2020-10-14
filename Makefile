all:
	mkdir -p .build
	mkdir -p songs
	mkdir -p imports
	mkdir -p exports
	g++ -g -c src/Main.cpp -o .build/Main.o
	g++ -g -c src/Config.cpp -o .build/Config.o
	g++ -g -c src/Note.cpp -o .build/Note.o
	g++ -g -c src/Map.cpp -o .build/Map.o
	g++ -g -c src/Notation.cpp -o .build/Notation.o
	g++ -g -c src/Song.cpp -o .build/Song.o
	g++ -g -c src/Generator.cpp -o .build/Generator.o
	g++ -g -c src/TransitionMatrix.cpp -o .build/TransitionMatrix.o
	g++ -g -c src/Utilities.cpp -o .build/Utilities.o
	g++ -g -c src/MapAnalyzer.cpp -o .build/MapAnalyzer.o

	g++ -g -c src/EditorPanel.cpp -o .build/EditorPanel.o -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
	g++ -g -c src/Window.cpp -o .build/Window.o -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
	g++ -g -c src/Input.cpp -o .build/Input.o -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

	g++ .build/Main.o -o generator.out \
		.build/Config.o  \
		.build/EditorPanel.o  \
		.build/Note.o \
		.build/Map.o \
		.build/Notation.o \
		.build/Song.o \
		.build/Window.o \
		.build/Input.o \
		.build/Generator.o \
		.build/TransitionMatrix.o \
		.build/Utilities.o \
		.build/MapAnalyzer.o \
		-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
	rm -r .build