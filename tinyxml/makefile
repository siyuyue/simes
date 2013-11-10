CC=g++
CFLAGS=-o2 -c -Wall

SOURCES=tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp
HEADERS=tinystr.h tinyxml.h
OBJECTS=$(addprefix obj/, $(SOURCES:.cpp=.o))

all: mkdir $(SOURCES) $(HEADERS) $(OBJECTS)
	@ echo "Done!"

obj/%.o: %.cpp
	@ echo "Compiling $<"
	@ $(CC) $(CFLAGS) $< -o $@

clean:
	@ echo "Cleaning..."
	@ rm -f $(OBJECTS)

mkdir:
	@ mkdir -p obj

.PHONY: clean
.PHONY: mkdir