CC=g++
BUILD_FOLDER=./build
FLAGS=-Wall -Weffc++ -Wextra -Wsign-conversion -pedantic-errors -Werror -std=c++17
SOFT_FLAGS=-std=c++17

default:
	$(CC) ./main.cpp ./src/*/*.cpp ./src/*/*/*.cpp -o $(BUILD_FOLDER)/interpreter $(FLAGS)

no-warn:
	$(CC) ./main.cpp ./src/*/*.cpp ./src/*/*/*.cpp -o $(BUILD_FOLDER)/interpreter $(SOFT_FLAGS)

ast:
	$(CC) ./tools/*.cpp -o $(BUILD_FOLDER)/generateAst $(FLAGS)