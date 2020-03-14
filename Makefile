all:
		clear
		mkdir -p .build
		g++ -c src/main.cpp -o .build/main.o

		g++ -c src/Window.cpp -o .build/Window.o -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
		g++ -c src/Map.cpp -o .build/Map.o

		g++ .build/main.o -o generator.out .build/Map.o .build/Window.o -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system