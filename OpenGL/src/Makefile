SOURCES= postcard.cpp glad.c
OBJECTS= glad.o postcard.o
EXECUTABLE=OpenGL.exe

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(OBJECTS) -o $@ $(CXXFLAGS)

.c.o:
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -rf *.o *.exe
