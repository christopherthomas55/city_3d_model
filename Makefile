#from lazyfoo tutorials
OBJS = main.cpp ../glad/src/glad.c shader.cpp

CXX = g++

COMPILER_FLAGS = -w

LINKER_FLAGS = -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl

LIBRARY_PATH = includes/

OBJ_NAME = main 

#test: test.cpp
	#$(CXX) test.cpp -I$(LIBRARY_PATH) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o test

all: $(OBJS)
	$(CXX) $(OBJS) -I$(LIBRARY_PATH) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
