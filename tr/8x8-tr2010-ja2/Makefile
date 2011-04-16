ARCH_NAME   := $(shell uname -m)+$(shell uname -s)+$(shell uname -r)

ifneq (,$(filter i686+CYGWIN_%,$(ARCH_NAME)))
#	CXX		:= g++
#	ARCHFLAGS	:= -march=nocona -msse2
	CXX		:= g++-4
	ARCHCCFLAGS	:= -march=core2 -msse4
	ARCHLDFLAGS	:= -Wl,--enable-auto-import
	BINEXT		:= .exe
	ARCHNAME	:= win32
else
  ifneq (,$(filter i686+Linux+%,$(ARCH_NAME)))
	CXX		:= g++
	ARCHCCFLAGS	:= -march=athlon
	ARCHLDFLAGS	:=
	BINEXT		:=
	ARCHNAME	:= linux
  else
    $(error      Unknown architecture --  $(ARCH_NAME))
    # fallback in case $(error ...) isn't defined by this version of make:
    $(shell echo Unknown architecture -- "${ARCH_NAME}" >&2)
    error        Unknown architecture --  $(ARCH_NAME)
  endif
endif

DEBUGFLAGS  := -O3 -DNDEBUG -Wall
#DEBUGFLAGS  := -g -Wall
#DEBUGFLAGS  := -g -pg -Wall
CCFLAGS     := $(ARCHCCFLAGS) $(DEBUGFLAGS)
LDFLAGS     := $(ARCHLDFLAGS) $(DEBUGFLAGS) -lm

BIN_NAMES   := solve_grid test_set test_grid test_mutex
BINARIES    := $(BIN_NAMES:%=%$(BINEXT))

OBJDIR      := objs/$(ARCHNAME)

default: $(OBJDIR).dir deps.dir $(firstword $(BINARIES))
all:     $(OBJDIR).dir deps.dir $(BINARIES)
clean:
	rm -f *.csv $(OBJDIR)/* *.stackdump core.* $(BINARIES)
veryclean: clean
	rm -f *~ .*~ *.png *.pdf deps/* objs/*/* \
		$(BIN_NAMES:%=%.exe) $(BIN_NAMES)
	find deps objs -depth | xargs rm -rf
%.dir:
	@if [ ! -e $* ]; then echo mkdir -p $*; mkdir -p $*; fi
.PHONY: default all clean veryclean %.dir

.SECONDEXPANSION:
## The second expansion makes it easy to specify extra objects for each binary:
OBJS_solve_grid			:= $(patsubst %,$(OBJDIR)/%.o, \
	Grid LocationSequence )
OBJS_test_grid			:= $(patsubst %,$(OBJDIR)/%.o, \
	Grid )

include $(wildcard deps/*.d)

%$(BINEXT) : $(OBJDIR)/%.o $$(OBJS_$$*)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o deps/%.d : %.cxx
	$(CXX) -c $*.cxx -o $(OBJDIR)/$*.o -MMD -MF deps/$*.d $(CCFLAGS)

.PRECIOUS: $(OBJDIR)/%.o deps/%.d
