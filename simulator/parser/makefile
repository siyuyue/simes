# Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
# University of Southern California

INCLUDE_PATH=../../tinyxml ../core ../banks ../converters ../sources ../loads ../cti ../managers ../parser

CC=g++
CFLAGS=-O2 -c $(addprefix -I, $(INCLUDE_PATH))

SOURCES=Parser.cpp
HEADERS=Parser.h
OBJECTS=$(addprefix obj/, $(SOURCES:.cpp=.o))
DEPS=$(OBJECTS:.o=.d)

all: mkdir $(SOURCES) $(HEADERS) $(OBJECTS)
	@ echo "Done!"

obj/%.o: %.cpp
	@ echo "Compiling parser..."
	@ $(CC) $(CFLAGS) -MMD $< -o $@

clean:
	@ echo "Cleaning..."
	@ rm -f $(OBJECTS)
	@ rm -f $(DEPS)

mkdir:
	@ mkdir -p obj

.PHONY: clean
.PHONY: mkdir

-include $(DEPS)