CC      = g++
CFLAGS  = -std=c++11

all: a.out create_output_dir

a.out: *.cpp *.h
	$(CC) $(CFLAGS) *.cpp

create_output_dir:
	mkdir output_images

clean:
	rm a.out
	rm -Rf output_images
