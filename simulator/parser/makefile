# Comment goes here.

INCLUDE_PATH=../../tinyxml ../core ../banks ../converters ../sources ../loads ../cti ../managers ../parser

CC=g++
CFLAGS=-o2 -c $(addprefix -I, $(INCLUDE_PATH))

SOURCES=Parser.cpp
HEADERS=Parser.h
OBJECTS=$(addprefix obj/, $(SOURCES:.cpp=.o))

all: $(SOURCES) $(HEADERS) $(OBJECTS)
	@ echo "Done!"

obj/%.o: %.cpp
	@ echo "Compiling parser..."
	@ $(CC) $(CFLAGS) $< -o $@

clean:
	@ echo "Cleaning..."
	@ rm -f $(OBJECTS)

mkdir:
	@ mkdir -p obj