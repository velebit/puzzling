ifeq ($(shell uname -o),Cygwin)
EXE_EXT := .exe
else
CXX := g++44
endif

#CXXFLAGS_COMMON := -std=gnu++11
CXXFLAGS_COMMON := -std=gnu++0x
CXXFLAGS_OPT := -g -O3 $(CXXFLAGS_COMMON)
CXXFLAGS_DBG := -g -O0 -DDEBUG $(CXXFLAGS_COMMON)

SOURCES := main.cxx Fingers.cxx Hands.cxx Players.cxx

APP := chop
OPT := 
DBG := _dbg
APP_OPT := $(APP)$(OPT)$(EXE_EXT)
APP_DBG := $(APP)$(DBG)$(EXE_EXT)

.PHONY: default all gdb clean
default: $(APP_OPT)
all: $(APP_OPT) $(APP_DBG)
gdb: $(APP_DBG)
	gdb $(APP_DBG)
clean:
	rm -f $(APP_OPT) $(APP_DBG) *.d *.o *~

$(APP_OPT): $(SOURCES:%.cxx=%$(OPT).o)
	$(CXX) $(CXXFLAGS_OPT) -o $@ $^ $(LIBS)
# If you include %$(OPT).d as a target, Make will try to rebuild it after
# the source has changed, even if not relevant to the current build.
%$(OPT).o: %.cxx
	$(CXX) $(CXXFLAGS_OPT) -MMD -o $(@:%.d=%.o) -c $<

$(APP_DBG): $(SOURCES:%.cxx=%$(DBG).o)
	$(CXX) $(CXXFLAGS_DBG) -o $@ $^ $(LIBS)
# If you include %$(DBG).d as a target, Make will try to rebuild it after
# the source has changed, even if not relevant to the current build.
%$(DBG).o: %.cxx
	$(CXX) $(CXXFLAGS_DBG) -MMD -o $(@:%.d=%.o) -c $<

-include $(filter $(SOURCES:%.cxx=%$(OPT).d) $(SOURCES:%.cxx=%$(DBG).d), \
		$(wildcard *.d))

.PRECIOUS: %.o %.d
