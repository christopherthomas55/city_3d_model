#from lazyfoo tutorials
OBJS = main.cpp shader.cpp Mesh.cpp glad.c

CXX = g++

COMPILER_FLAGS = -w -g

LINKER_FLAGS = -lglfw -lGLU -lGL -ldl
LIBRARY_PATH = includes/

OBJ_NAME = main 

#test: test.cpp
	#$(CXX) test.cpp -I$(LIBRARY_PATH) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o test

all: $(OBJS)
	$(CXX) $(OBJS) -I$(LIBRARY_PATH) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
