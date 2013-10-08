# Comment goes here.

INCLUDE_PATH=../core ../banks ../converters ../sources ../loads ../cti ../managers ../parser
CLASSES=Bank Grid LeadAcidBattery LiIonBattery SimpleBattery SuperCap

CC=g++
CFLAGS=-O2 -c $(addprefix -I, $(INCLUDE_PATH))

SOURCES=$(addsuffix .cpp, $(CLASSES)) BankCreate.cpp
HEADERS=$(addsuffix .h, $(CLASSES))
OBJECTS=$(addprefix obj/, $(SOURCES:.cpp=.o))
DEPS=$(OBJECTS:.o=.d)

all: $(SOURCES) $(HEADERS) $(OBJECTS)
	@ echo "Done!"

obj/%.o: %.cpp
	@ echo "Compiling $<"
	@ $(CC) $(CFLAGS) -MMD $< -o $@

clean:
	@ echo "Cleaning..."
	@ rm -f $(OBJECTS)

mkdir:
	@ mkdir -p obj

-include $(DEPS)