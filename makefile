vpath %.cc = $(wildcard */src)
LIB_PATH = lib/
MLIB_PATH = mlib/
TEST_PATH = test/
OBJSRCS =  $(wildcard */src/*.cc)
OBJS = $(addprefix  $(LIB_PATH),$(notdir $(OBJSRCS:.cc=.o)))
MAINEXES = $(basename $(notdir  $(wildcard */test/*.cc)))
TESTTARGETS = $(addsuffix  .test,$(MAINEXES))

INCDIRS =   $(foreach DIR,$(wildcard */include/),-I$(DIR))
CC = g++
DEBUG = -g -O0
CFLAGS = -std=c++11 -Wall $(DEBUG) `root-config --cflags`
LFLAGS = -std=c++11 -Wall $(DEBUG) `root-config --glibs`

all: $(MAINEXES)

%: $(MLIB_PATH)%.o $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) $< -o $@

-include  $(LIB_PATH)*.d

.PRECIOUS: $(LIB_PATH)/%.o $(MLIB_PATH)/%.o

$(LIB_PATH)%.o: %.cc
	$(CC) $(CFLAGS) -MMD -c $(INCDIRS) $< -o $@

$(MLIB_PATH)%.o: */test/%.cc
	$(CC) $(CFLAGS) -MMD -c $(INCDIRS) $< -o $@

test: $(TESTTARGETS)

%.test: 
	./$* > testlog_$*
	diff $(TEST_PATH)log_$* testlog_$*
	diff genoutputeventdatafile.bin test/genoutputeventdatafile.bin

clean:
	\rm *~ */*~ */*/*~ $(LIB_PATH)/*.o $(LIB_PATH)/*.d $(MLIB_PATH)/*.o $(MLIB_PATH)/*.d dataGen dataRead hitReco trackReco testlog*
