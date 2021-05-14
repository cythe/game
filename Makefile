# We just need base opencv library and head files
INC=-I/usr/include/opencv4 -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_core

SRC=src/*.cpp

all:
	g++ -g ${INC} ${SRC} -o a.out
	ln -sf a.out water
	ln -sf a.out stars
clean:
	rm -f a.out
	rm -f water stars

dist: clean
	rm -f GPATH GRTAGS GTAGS tags cscope*
