CXX      = g++
CXXFLAGS = -O2 -Wall -Wextra

all: illumination

illumination: illumination.o ppm.o
	$(CXX) $(CXXFLAGS) -o illumination illumination.o ppm.o -lm

illumination.o: illumination.cpp illumination.hpp ppm.hpp
	$(CXX) $(CXXFLAGS) -c illumination.cpp

ppm.o: ppm.cpp ppm.hpp
	$(CXX) $(CXXFLAGS) -c ppm.cpp

clean:
	rm -f illumination.o ppm.o illumination