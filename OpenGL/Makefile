Inc_Dir = -I.././include -I../../Reader/include
Lib_Dir = .././lib
<<<<<<< HEAD
Libs = -lassimp -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
=======
Libs = -lfreetype -lassimp -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lz
>>>>>>> origin/ksusha_210223_graphics
СXX?= g++
CXXFLAGS?= $(Inc_Dir) -L $(Lib_Dir) $(Libs) 

export CXX CXXFLAGS


.PHONY: clean


subsystem:
	cd src && $(MAKE)

clean:
	cd src && $(MAKE) $@
