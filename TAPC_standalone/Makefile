SYSTEMC_HOME?=/home/george/systemc-2.3.3
TARGET_ARCH = linux64
FLAGS_COMMON = -g -Wall
FLAGS_STRICT = -pedantic -Wno-long-long
FLAGS_WERROR = -Werror


PROJECT = main
#INCDIR = -I.\
#	-I../usaTrackBusSimple
OBJS = $(PROJECT).o


## default values for additional setup variables
ifneq (,$(strip $(TARGET_ARCH)))
ARCH_SUFFIX      ?= -$(TARGET_ARCH)
endif
LDFLAG_RPATH     ?= -Wl,-rpath=

SYSTEMC_INC_DIR  ?= $(SYSTEMC_HOME)/include
SYSTEMC_LIB_DIR  ?= $(SYSTEMC_HOME)/lib$(ARCH_SUFFIX)

SYSTEMC_DEFINES  ?=
SYSTEMC_CXXFLAGS ?= $(FLAGS_COMMON) $(FLAGS_STRICT) $(FLAGS_WERROR)
SYSTEMC_LDFLAGS  ?= -L $(SYSTEMC_LIB_DIR) \
                    $(LDFLAG_RPATH)$(SYSTEMC_LIB_DIR)
SYSTEMC_LIBS     ?= -lsystemc -lm

FILTER ?= cat

INCDIR   += -I. -I.. -I$(SYSTEMC_INC_DIR)
LIBDIR   += -L. -L..

CXXFLAGS  += $(CFLAGS) $(SYSTEMC_CXXFLAGS) $(INCDIR) $(SYSTEMC_DEFINES)
LDFLAGS   += $(CFLAGS) $(SYSTEMC_CXXFLAGS) $(LIBDIR) $(SYSTEMC_LDFLAGS)
LIBS      += $(SYSTEMC_LIBS) $(EXTRA_LIBS)

# "real" Makefile needs to set PROJECT
ifeq (,$(strip $(PROJECT)))
$(error PROJECT not set. Cannot build.)
endif

# basic check for SystemC directory
ifeq (,$(wildcard $(SYSTEMC_HOME)/.))
$(error SYSTEMC_HOME [$(SYSTEMC_HOME)] is not present. \
        Please update Makefile.config)
endif
ifeq (,$(wildcard $(SYSTEMC_INC_DIR)/systemc.h))
$(error systemc.h [$(SYSTEMC_INC_DIR)] not found. \
        Please update Makefile.config)
endif
ifeq (,$(wildcard $(SYSTEMC_LIB_DIR)/libsystemc*))
$(error SystemC library [$(SYSTEMC_LIB_DIR)] not found. \
        Please update Makefile.config)
endif

## ***************************************************************************
## build rules

.SUFFIXES: .cc .cpp .o .x 

GOLDEN?=$(firstword $(wildcard ../results/expected.log golden.log))
EXEEXT?=.x
EXE   := $(PROJECT)$(EXEEXT)

all: announce build

announce:
	@if test x1 = x$(FLAG_BATCH) ; then \
		echo; echo "*** $(PROJECT):"; echo; \
	fi

check: announce all
	@if test -f "$(INPUT)" ; then INPUT="< $(INPUT)" ; fi ; \
		eval "$(VALGRIND) ./$(EXE) $(ARGS) $${INPUT} > run.log"
	@cat run.log | grep -v "stopped by user" | \
		$(FILTER) | awk '{if($$0!="") print $$0}' > run_trimmed.log
	@if test -f "$(GOLDEN)" ; then \
	  cat "$(GOLDEN)" | grep -v "stopped by user" | \
		awk '{if($$0!="") print $$0}' > ./expected_trimmed.log ; \
	  diff ./run_trimmed.log ./expected_trimmed.log > diff.log 2>&1 ; \
	  if [ -s diff.log ]; then \
	    echo "***ERROR:"; cat diff.log; \
	  else echo "OK"; fi \
	fi

run: announce all
	@if test -f "$(INPUT)" ; then INPUT="< $(INPUT)" ; fi ; \
		eval "./$(EXE) $(ARGS) $${INPUT}"

build: announce $(EXE)

$(EXE): $(OBJS) $(SYSTEMC_LIB_DIR)/libsystemc.a
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIBS) 2>&1 | c++filt
	@test -x $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:: announce
	rm -f $(OBJS) $(EXE) core $(EXTRA_CLEAN) \
		run.log run_trimmed.log expected_trimmed.log diff.log
