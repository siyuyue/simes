# Comment goes here.

INCLUDE_PATH=../core ../banks ../converters ../sources ../loads ../cti ../managers ../parser
CLASSES=Component Port LookUpTable Sensor SimException Simulator

CC=g++
CFLAGS=-o2 -c $(addprefix -I, $(INCLUDE_PATH))

SOURCES=$(addsuffix .cpp, $(CLASSES))
HEADERS=$(addsuffix .h, $(CLASSES)) config.h
OBJECTS=$(addprefix obj/, $(SOURCES:.cpp=.o))

all: $(SOURCES) $(HEADERS) $(OBJECTS)
	@ echo "Done!"

obj/%.o: %.cpp
	@ echo "Compiling $<"
	@ $(CC) $(CFLAGS) $< -o $@

clean:
	@ echo "Cleaning..."
	@ rm -f $(OBJECTS)

mkdir:
	@ mkdir -p obj
