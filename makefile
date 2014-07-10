vpath %.cc = Geometry/src:Services/src:DataObjects/src:Algorithms/src:Modules/src:Modules/test:Framework/src
DIRS = Geometry Services DataObjects Algorithms Modules
LIB_PATH = lib/
SRCS =  $(foreach DIR,$(DIRS),$(DIR)/src/*.cc) Modules/test/*.cc
OBJSRCS =  $(foreach DIR,$(DIRS),$(wildcard $(DIR)/src/*.cc))
OBJS = $(addprefix  $(LIB_PATH),$(notdir $(OBJSRCS:.cc=.o)))
DEPS =  $(patsubst %.o,%.d,$(OBJS))
NOTES = notes/
CC = g++
DEBUG = -g -O0
CFLAGS = -std=c++11 -Wall $(DEBUG) `root-config --cflags`
INCDIRS =  $(foreach DIR,$(DIRS),-I$(DIR)/include/)
LFLAGS = -std=c++11 -Wall $(DEBUG) `root-config --glibs`

trackReco : $(OBJS) $(LIB_PATH)trackReco.o
	$(CC) $(LFLAGS) $(OBJS) $(LIB_PATH)trackReco.o -o trackReco

-include  $(LIB_PATH)*.d

hitReco : $(OBJS) $(LIB_PATH)hitReco.o
	$(CC) $(LFLAGS) $(OBJS)  $(LIB_PATH)hitReco.o -o hitReco

dataRead : $(OBJS) $(LIB_PATH)dataRead.o 
	$(CC) $(LFLAGS) $(OBJS)  $(LIB_PATH)dataRead.o -o dataRead

dataGen : $(OBJS) $(LIB_PATH)dataGen.o
	$(CC) $(LFLAGS) $(OBJS)  $(LIB_PATH)dataGen.o -o dataGen

$(LIB_PATH)%.o: %.cc
	$(CC) $(CFLAGS) -MMD -c $(INCDIRS) $< -o $@

clean:
	\rm *~ */*~ */*/*~ lib/*.o lib/*.d dataGen dataRead hitReco trackReco
