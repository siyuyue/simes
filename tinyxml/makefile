# Comment goes here.

CC=g++
CFLAGS=-o2 -c -Wall -I include/

AR=ar
ARFLAGS=rc

SOURCES=tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp
HEADERS=$(addprefix include/, tinystr.h tinyxml.h)
OBJECTS=$(addprefix obj/, $(SOURCES:.cpp=.o))
LIBRARY=$(addprefix lib/, libtinyxml.a)

all: $(SOURCES) $(HEADERS) $(LIBRARY)
	@ echo "Done!"

$(LIBRARY): $(OBJECTS)
	@ echo "Compiling tinyxml library..."
	@ $(AR) $(ARFLAGS) $(LIBRARY) $(OBJECTS)

obj/%.o: %.cpp
	@ $(CC) $(CFLAGS) $< -o $@

clean:
	@ echo "Cleaning..."
	@ rm -f $(OBJECTS)
	@ rm -f $(LIBRARY)

mkdir:
	@ mkdir library
	@ mkdir obj