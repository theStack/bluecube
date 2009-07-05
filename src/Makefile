OBJECTS = main.o box.o pieces.o credits.o grafix.o font.o particle.o sound.o
SDL_INCLUDE = `sdl-config --cflags`
SDL_LINK = `sdl-config --libs`

../bluecube: $(OBJECTS)
	gcc -o ../bluecube $(OBJECTS) $(SDL_LINK)

main.o: main.c
	gcc -c main.c $(SDL_INCLUDE) -Wall -ansi

box.o: box.c
	gcc -c box.c $(SDL_INCLUDE) -Wall -ansi

pieces.o: pieces.c
	gcc -c pieces.c -Wall -ansi

credits.o: credits.c
	gcc -c credits.c $(SDL_INCLUDE) -Wall -ansi

grafix.o: grafix.c
	gcc -c grafix.c $(SDL_INCLUDE) -Wall -ansi

font.o: font.c
	gcc -c font.c $(SDL_INCLUDE) -Wall -ansi

particle.o: particle.c
	gcc -c particle.c $(SDL_INCLUDE) -Wall -ansi

sound.o: sound.c
	gcc -c sound.c $(SDL_INCLUDE) -Wall -ansi
