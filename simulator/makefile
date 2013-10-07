# Comment goes here.

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
EXECUTABLE=simulator

all: $(BUILDDIRS) $(LIBRARY) $(EXECUTABLE)
	@ echo "Done!"

makelib: $(BUILDDIRS) $(LIBRARY)
	@ echo "Done!"

$(BUILDDIRS):
	@ echo "Entering directory $@..."
	@ $(MAKE) --no-print-directory -C $(@:build-%=%) all

$(CLEANDIRS):
	@ echo "Entering directory $@..."
	@ $(MAKE) --no-print-directory -C $(@:clean-%=%) clean

$(MKDIRDIRS):
	@ echo "Entering directory $@..."
	@ $(MAKE) --no-print-directory -C $(@:mkdir-%=%) mkdir

$(LIBRARY):
	@ echo "Compiling simulator library..."
	@ $(AR) $(ARFLAGS) $(LIBRARY) $(OBJECTS)

$(EXECUTABLE): $(LIBRARY)
	@ echo "Compiling simulator..."
	@ $(CC) $(CFLAGS) main.cpp -Llib -lsimulator -o $@

clean: $(CLEANDIRS)
	@ echo "Cleaning..."
	@ rm -f $(LIBRARY)
	@ rm -f $(EXECUTABLE)

mkdir: $(MKDIRDIRS)
	@ mkdir -p lib

.PHONY: $(BUILDDIRS)
.PHONY: $(CLEANDIRS)
.PHONY: $(MKDIRDIRS)