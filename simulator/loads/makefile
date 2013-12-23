# Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
# University of Southern California

INCLUDE_PATH=../core ../banks ../converters ../sources ../loads ../cti ../managers ../parser $(BOOST_ROOT)
CLASSES=Load ConstantLoad ProfileLoad

CC=g++
CFLAGS=-O2 -c $(addprefix -I, $(INCLUDE_PATH))

SOURCES=$(addsuffix .cpp, $(CLASSES))
HEADERS=$(addsuffix .h, $(CLASSES))
OBJECTS=$(addprefix obj/, $(SOURCES:.cpp=.o))
DEPS=$(OBJECTS:.o=.d)

all: mkdir $(SOURCES) $(HEADERS) $(OBJECTS)
	@ echo "Done!"

obj/%.o: %.cpp
	@ echo "Compiling $<"
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