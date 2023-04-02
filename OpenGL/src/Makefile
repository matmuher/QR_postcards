SOURCES= postcard.cpp glad.c ../../Reader/src/MaPrinter.cpp ../../Reader/src/Tokenizer.cpp
OBJECTS= glad.o postcard.o ../../Reader/src/MaPrinter.o ../../Reader/src/Tokenizer.o
EXECUTABLE=OpenGL.exe

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(OBJECTS) -o $@ $(CXXFLAGS)

.c.o:
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -rf *.o *.exe
