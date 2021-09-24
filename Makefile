all: start clean

start: view.o game.o
	g++ game.o view.o -o game.out -lsfml-graphics -lsfml-system -lsfml-window

view.o:
	g++ -c g_view/view.cpp

game.o:
	g++ -c game.cpp

clean:
	rm -rf *.o Game_runforyourdreamgirl
