projectdir=/home/robert/Projects/trog
CPPFLAGS=-g -std=c++17 -pthread -I${projectdir}/vendor/include -I${projectdir}/sample-app -I${projectdir}/include
LDFLAGS=-g
LDLIBS=

sample: main.o class_a.o class_b.o
	g++ $(LDFLAGS) -o main main.o class_a.o class_b.o $(LDLIBS)

main.o: main.cpp class_a.cpp class_a.hpp class_b.cpp class_b.hpp class_c.hpp class_d.hpp
	g++ $(CPPFLAGS) -c main.cpp

class_a.o: class_a.cpp class_a.hpp class_c.hpp class_d.hpp
	g++ $(CPPFLAGS) -c class_a.cpp

class_b.o: class_b.cpp class_b.hpp class_c.hpp class_d.hpp
	g++ $(CPPFLAGS) -c class_b.cpp