INC=-I/usr/include/opencv4 -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_core
all:
	#gcc -g main.cpp  tube.cpp stack.cpp color.cpp load.cpp
	g++ -g ${INC} main.cpp config.c tube.cpp stack.cpp load.cpp color.cpp water.cpp -o a.out
	ln -s a.out water
	ln -s a.out stars
clean:
	rm -f a.out
	#g++ `pkg-config opencv4 --cflags --libs` main.cpp -o a.out
