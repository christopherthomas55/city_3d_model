#from lazyfoo tutorials
OBJS = main.cpp src/shader.cpp src/Mesh.cpp glad.c src/BuildingHandler.cpp src/Building.cpp

CXX = g++

COMPILER_FLAGS = -w -g -std=c++11
LINKER_FLAGS = -lglfw -lGLU -lGL -ldl -lgmp -lCGAL
LIBRARY_PATH = -Iincludes/ -I/usr/include/

OBJ_NAME = main

#test: test.cpp
	#$(CXX) test.cpp -I$(LIBRARY_PATH) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o test


all: $(OBJS)
	$(CXX) $(OBJS) $(LIBRARY_PATH) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)


btest: $(OBJS)
	$(CXX) test_building.cpp  glad.c src/shader.cpp src/Building.cpp src/BuildingHandler.cpp  $(LIBRARY_PATH) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o test_building
