CFLAGS = -std=c++11 -O2 -Wall

all: main sfml-app imReader.py imWriter.py
	python3 imReader.py > test.txt
	./main < test.txt > ans.txt
	python3 imWriter.py < ans.txt
	./sfml-app

main: main.cpp CONSTS.cpp CONSTS.h WEIGHTED_BIPMATCH_V4.h
	g++ main.cpp CONSTS.cpp -o main $(CFLAGS)

sfml-app: displayImage.cpp CONSTS.cpp
	g++ displayImage.cpp CONSTS.cpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system $(CFLAGS)

clean:
	@rm -rf main sfml-app test.txt ans.txt
