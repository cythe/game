INC=-I/usr/include/opencv4 -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_core
all:
	#gcc -g main.cpp  tube.cpp stack.cpp color.cpp load.cpp
	g++ -g ${INC} main.cpp config.c tube.cpp stack.cpp load.cpp color.cpp -o a.out
clean:
	rm -f a.out
	#g++ `pkg-config opencv4 --cflags --libs` main.cpp -o a.out
