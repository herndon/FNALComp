OBJS = Random.o DetectorGeometry.o StripSet.o Hit.o HitSet.o TNGeoHelix.o Track.o TrackSet.o TrackGenModule.o HitStripGenModule.o DataOutputModule.o dataGen.o
NOTES = notes/
CC = g++
DEBUG = -g -O0
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

Hit.o : Hit.cc Hit.hh
	$(CC) $(CFLAGS) Hit.cc

HitSet.o : HitSet.cc HitSet.hh
	$(CC) $(CFLAGS) HitSet.cc

TNGeoHelix.o : TNGeoHelix.cc TNGeoHelix.hh
	$(CC) $(CFLAGS) TNGeoHelix.cc

Track.o : Track.cc Track.hh
	$(CC) $(CFLAGS) Track.cc

TrackSet.o : TrackSet.cc TrackSet.hh
	$(CC) $(CFLAGS) TrackSet.cc

TrackGenModule.o : TrackGenModule.cc TrackGenModule.hh
	$(CC) $(CFLAGS) TrackGenModule.cc

HitStripGenModule.o : HitStripGenModule.cc HitStripGenModule.hh
	$(CC) $(CFLAGS) HitStripGenModule.cc

DataOutputModule.o : DataOutputModule.cc DataOutputModule.hh
	$(CC) $(CFLAGS) DataOutputModule.cc

dataGen.o : dataGen.cc
	$(CC) $(CFLAGS) dataGen.cc

clean:
	\rm *.o *~ dataGen 

tar:
	tar -z --create --file day0.tgz dataGen.cc \
	TrackGenModule.cc TrackGenModule.hh HitStripGenModule.cc HitStripGenModule.hh DataOutPutModule.cc DataOutputModule.hh \
	DetectorGeometry.cc DetectorGeometry.hh \
	StripSet.cc StripSet.hh Track.cc Track.hh TrackSet.cc TrackSet.hh \
        Random.hh Random.cc Exception.hh \
        commands0.txt sensorgeometry.txt makefile doxygen.cfg \
        doc
