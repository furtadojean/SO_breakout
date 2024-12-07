build: ball.cpp bar.cpp blocks.cpp draw.cpp game.cpp input.cpp object.cpp main.cpp
	g++ -g3 -o breakout ball.cpp bar.cpp blocks.cpp draw.cpp game.cpp input.cpp object.cpp main.cpp
test: ball.cpp bar.cpp blocks.cpp draw.cpp game.cpp input.cpp object.cpp test.cpp
	g++ -g3 -o test ball.cpp bar.cpp blocks.cpp draw.cpp input.cpp object.cpp test.cpp
