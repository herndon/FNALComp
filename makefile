SRCS = Geometry/src/*.cc Services/src/*.cc DataObjects/src/*.cc Algorithms/src/*.cc Modules/src/*.cc Modules/test/*.cc
OBJS = Random.o Config.o DetectorGeometry.o StripSet.o StripSetIO.o Hit.o HitSet.o HitSetIO.o Helix.o Track.o TrackFit.o TrackSet.o TrackGenModule.o HitStripGenModule.o DataOutputModule.o DataInputModule.o HitRecoModule.o HitCompareModule.o TrackRecoModule.o
NOTES = notes/
CC = g++
DEBUG = -g -O0
CFLAGS = -std=c++11 -Wall -c $(DEBUG) `root-config --cflags` -MD
INCDIRS = -IGeometry/include/ -IServices/include/ -IDataObjects/include/ -I Algorithms/include/ -IModules/include/
LFLAGS = -std=c++11 -Wall $(DEBUG) `root-config --glibs`

all : trackReco hitReco dataRead dataGen

trackReco : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) trackReco.o -o trackReco

hitReco : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) hitReco.o -o hitReco

dataRead : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) dataRead.o -o dataRead

dataGen : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) dataGen.o -o dataGen


%.o: $(SRCS)
	$(CC) $(CFLAGS) $(INCDIRS) $(SRCS)

clean:
	\rm *.o *.d dataGen dataRead hitReco trackReco
