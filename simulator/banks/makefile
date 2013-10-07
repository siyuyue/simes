# Comment goes here.

INCLUDE_PATH=../core ../banks ../converters ../sources ../loads ../cti ../managers ../parser
CLASSES=Bank Grid LeadAcidBattery LiIonBattery SimpleBattery SuperCap

CC=g++
CFLAGS=-o2 -c $(addprefix -I, $(INCLUDE_PATH))

SOURCES=$(addsuffix .cpp, $(CLASSES)) BankCreate.cpp
HEADERS=$(addsuffix .h, $(CLASSES))
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
	@ mkdir obj