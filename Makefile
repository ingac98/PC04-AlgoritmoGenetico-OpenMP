CXX = g++

CXXFLAGS = -std=c++17 -O2 -fopenmp

INCLUDES = -Iinclude

SRC = $(wildcard src/*.cpp)

TARGET = genetic.exe

all:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(TARGET)

run:
	./$(TARGET)

clean:
	del /Q $(TARGET)