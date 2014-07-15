vpath %.cc = $(wildcard */src)
LIB_PATH = lib/
MLIB_PATH = mlib/
OBJSRCS =  $(wildcard */src/*.cc)
OBJS = $(addprefix  $(LIB_PATH),$(notdir $(OBJSRCS:.cc=.o)))
MAINSRCS = $(wildcard */test/*.cc)
MAINEXES = $(basename $(notdir $(MAINSRCS)))
INCDIRS =   $(foreach DIR,$(wildcard */include/),-I$(DIR))
CC = g++
DEBUG = -g -O0
CFLAGS = -std=c++11 -Wall $(DEBUG) `root-config --cflags`
LFLAGS = -std=c++11 -Wall $(DEBUG) `root-config --glibs`

all: $(MAINEXES)

%: $(MLIB_PATH)%.o $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) $< -o $@

-include  $(LIB_PATH)*.d

.PRECIOUS: lib/%.o mlib/%.o

$(LIB_PATH)%.o: %.cc
	$(CC) $(CFLAGS) -MMD -c $(INCDIRS) $< -o $@

$(MLIB_PATH)%.o: */test/%.cc
	$(CC) $(CFLAGS) -MMD -c $(INCDIRS) $< -o $@

clean:
	\rm *~ */*~ */*/*~ lib/*.o lib/*.d mlib/*.d mlib/*.o dataGen dataRead hitReco trackReco
