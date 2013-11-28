# Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
# University of Southern California

INCLUDE_PATH=../tinyxml core banks converters sources loads cti managers parser
SUBDIRS=$(INCLUDE_PATH)

OBJECTS=$(addsuffix /obj/*.o, $(SUBDIRS))

BUILDDIRS = $(SUBDIRS:%=build-%)
CLEANDIRS = $(SUBDIRS:%=clean-%)
MKDIRDIRS = $(SUBDIRS:%=mkdir-%)

CC=g++
CFLAGS=-o2 -static $(addprefix -I, $(INCLUDE_PATH))

AR=ar
ARFLAGS=rcf

LIBRARY=$(addprefix lib/, libsimulator.a)
EXECUTABLE=$(addprefix bin/, simulator)

all: mkdir $(BUILDDIRS) $(LIBRARY) $(EXECUTABLE) bin/simhybrid
	@ echo "Done!"

makelib: $(BUILDDIRS) $(LIBRARY)
	@ echo "Done!"

$(BUILDDIRS):
	@ echo "Entering directory $(@:build-%=%)..."
	@ $(MAKE) --no-print-directory -C $(@:build-%=%) all

$(CLEANDIRS):
	@ echo "Entering directory $(@:clean-%=%)..."
	@ $(MAKE) --no-print-directory -C $(@:clean-%=%) clean

$(LIBRARY):
	@ echo "Compiling simulator library..."
	@ $(AR) $(ARFLAGS) $(LIBRARY) $(OBJECTS)

$(EXECUTABLE): $(LIBRARY)
	@ echo "Compiling simulator..."
	@ $(CC) $(CFLAGS) main.cpp -Llib -lsimulator -o $@

bin/simhybrid: $(LIBRARY)
	@ echo "Compiling simhybrid..."
	@ $(CC) $(CFLAGS) mainhybrid.cpp -Llib -lsimulator -o $@

clean: $(CLEANDIRS)
	@ echo "Cleaning..."
	@ rm -f $(LIBRARY)
	@ rm -f $(EXECUTABLE)

mkdir:
	@ mkdir -p lib
	@ mkdir -p bin

.PHONY: $(BUILDDIRS)
.PHONY: $(CLEANDIRS)
.PHONY: $(LIBRARY)
.PHONY: clean
.PHONY: mkdir
