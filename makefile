OBJS = Random.o Config.o DetectorGeometry.o StripSet.o StripSetIO.o Hit.o HitSet.o HitSetIO.o Helix.o Track.o TrackFit.o TrackSet.o TrackGenModule.o HitStripGenModule.o DataOutputModule.o dataGen.o
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

Config.o : Exception.hh Config.cc Config.hh
	$(CC) $(CFLAGS) Config.cc

DetectorGeometry.o : Exception.hh DetectorGeometry.cc DetectorGeometry.hh
	$(CC) $(CFLAGS) DetectorGeometry.cc

StripSet.o : Exception.hh DetectorGeometry.hh StripSet.cc StripSet.hh
	$(CC) $(CFLAGS) StripSet.cc

StripSetIO.o : Exception.hh DetectorGeometry.hh StripSet.hh StripSetIO.cc StripSetIO.hh
	$(CC) $(CFLAGS) StripSetIO.cc

Hit.o : Hit.cc Hit.hh
	$(CC) $(CFLAGS) Hit.cc

HitSet.o : Exception.hh DetectorGeometry.hh Hit.hh HitSet.cc HitSet.hh
	$(CC) $(CFLAGS) HitSet.cc

HitSetIO.o : Exception.hh DetectorGeometry.hh Hit.hh HitSet.hh HitSetIO.cc HitSetIO.hh
	$(CC) $(CFLAGS) HitSetIO.cc

Helix.o : Helix.cc Helix.hh
	$(CC) $(CFLAGS) Helix.cc

Track.o : Exception.hh DetectorGeometry.hh Hit.hh Helix.hh Track.cc Track.hh
	$(CC) $(CFLAGS) Track.cc

TrackFit.o : Exception.hh DetectorGeometry.hh Hit.hh Helix.hh Track.hh TrackFit.cc TrackFit.hh
	$(CC) $(CFLAGS) TrackFit.cc

TrackSet.o : Exception.hh DetectorGeometry.hh Hit.hh Helix.hh Track.hh TrackSet.cc TrackSet.hh
	$(CC) $(CFLAGS) TrackSet.cc

TrackGenModule.o : Random.hh  Exception.hh DetectorGeometry.hh Hit.hh Helix.hh Track.hh TrackSet.hh TrackGenModule.cc TrackGenModule.hh
	$(CC) $(CFLAGS) TrackGenModule.cc

HitStripGenModule.o :  Random.hh  Exception.hh DetectorGeometry.hh StripSet.hh Hit.hh HitSet.hh Helix.hh Track.hh TrackFit.hh TrackSet.hh HitStripGenModule.cc HitStripGenModule.hh
	$(CC) $(CFLAGS) HitStripGenModule.cc

DataOutputModule.o : Exception.hh DetectorGeometry.hh StripSet.hh StripSetIO.hh Hit.hh HitSet.hh HitSetIO.hh Track.hh TrackSet.hh DataOutputModule.cc DataOutputModule.hh
	$(CC) $(CFLAGS) DataOutputModule.cc

dataGen.o : Exception.hh Config.hh DetectorGeometry.hh StripSet.hh StripSetIO.hh Hit.hh HitSet.hh HitSetIO.hh Helix.hh Track.hh TrackFit.hh TrackSet.hh DataOutputModule.hh dataGen.cc
	$(CC) $(CFLAGS) dataGen.cc

clean:
	\rm *.o *~ dataGen 

tar:
	tar -z --create --file day0.tgz dataGen.cc \
	TrackGenModule.cc TrackGenModule.hh HitStripGenModule.cc HitStripGenModule.hh DataOutputModule.cc DataOutputModule.hh \
	DetectorGeometry.cc DetectorGeometry.hh \
	StripSet.cc StripSet.hh StripSetIO.cc StripSetIO.hh\
	Hit.cc Hit.hh HitSet.cc HitSet.hh \
	Track.cc Track.hh TrackSet.cc TrackSet.hh \
        Config.cc Config.hh Random.hh Random.cc Exception.hh \
        commands0.txt sensorgeometry.txt makefile doxygen.cfg \
        doc
