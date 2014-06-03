OBJS = Random.o DetectorGeometry.o StripSet.o DataGenModule.o dataGen.o
NOTES = notes/
CC = g++
DEBUG = -g
INCDIRS = -I${ROOTSYS}include
CFLAGS = -std=c++11 -Wall -c $(DEBUG) `root-config --cflags`
LDIRS = -L$(ROOTSYS)lib
LOBJS = $(ROOTSYS)lib/libGeom.so
LFLAGS = -std=c++11 -Wall $(DEBUG) `root-config --glibs`

dataGen : $(OBJS)
	$(CC) $(LFLAGS) $(LOBJS) $(OBJS) -o dataGen

Random.o : Random.cc Random.hh
	$(CC) $(CFLAGS) Random.cc

DetectorGeometry.o : DetectorGeometry.cc DetectorGeometry.hh
	$(CC) $(CFLAGS) DetectorGeometry.cc

StripSet.o : StripSet.cc StripSet.hh
	$(CC) $(CFLAGS) StripSet.cc

DataGenModule.o : DataGenModule.cc DataGenModule.hh
	$(CC) $(CFLAGS) DataGenModule.cc

dataGen.o : dataGen.cc
	$(CC) $(CFLAGS) dataGen.cc

clean:
	\rm *.o *~ dataGen 

tar:
	tar -z --create --file day0.tgz dataGen.cc DataGenModule.cc DataGenModule.hh \
	DetectorGeometry.cc DetectorGeometry.hh StripSet.cc StripSet.hh \
        commands0.txt sensorgeometry.txt makefile doxygen.cfg \
        doc
