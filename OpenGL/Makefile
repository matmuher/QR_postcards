Inc_Dir = .././include
Lib_Dir = .././lib
Libs = -lassimp -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
СXX?= g++
CXXFLAGS?= -I $(Inc_Dir) -L $(Lib_Dir) $(Libs) 

export CXX CXXFLAGS


.PHONY: clean


subsystem:
	cd src && $(MAKE)

clean:
	cd src && $(MAKE) $@