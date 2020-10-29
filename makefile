CC=g++

default:
	$(CC) ./main.cpp ./src/*/*.cpp -o ./build/interpreter -Wall -Weffc++ -Wextra -Wsign-conversion -pedantic-errors -Werror -std=c++17