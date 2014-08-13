vpath %.cc = $(wildcard */src)
LIB_PATH = lib/
MLIB_PATH = mlib/
TEST_PATH = test/
OBJSRCS =  $(wildcard */src/*.cc)
OBJS = $(addprefix  $(LIB_PATH),$(notdir $(OBJSRCS:.cc=.o)))
MAINEXES = $(basename $(notdir  $(wildcard */test/*.cc)))
TESTTARGETS = $(addsuffix  .test,$(MAINEXES))

INCDIRS = -I.
CC = g++
#DEBUG = -g -O0
DEBUG = -g -O3 -fno-omit-frame-pointer -DNDEBUG
CFLAGS = -std=c++11 -Wall $(DEBUG) `root-config --cflags`
LFLAGS = -std=c++11 -Wall $(DEBUG) `root-config --glibs` 

.PRECIOUS:$(LIB_PATH)%.o

all: $(MAINEXES)

%: $(MLIB_PATH)%.o $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) ../../products/root/v5_34_18d/Darwin64bit+13-e5-prof/lib/libGeom.so ../../products/root/v5_34_18d/Darwin64bit+13-e5-prof/lib/libEve.so $< -o $@

-include  $(LIB_PATH)*.d

$(LIB_PATH)%.o: %.cc
	$(CC) $(CFLAGS) -MMD -c $(INCDIRS) $< -o $@

$(MLIB_PATH)%.o: */test/%.cc
	$(CC) $(CFLAGS) -MMD -c $(INCDIRS) $< -o $@

test: $(TESTTARGETS)

%.test: 
	./$* > testlog_$*
	diff -qs $(TEST_PATH)log_$* testlog_$*

clean:
	\rm *~ */*~ */*/*~ $(LIB_PATH)/*.o $(LIB_PATH)/*.d $(MLIB_PATH)/*.o $(MLIB_PATH)/*.d dataGen dataRead hitReco trackReco testlog*
