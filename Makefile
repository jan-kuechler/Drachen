######
##  Default configuration
##
## Modify this configuration by creating a local.config file and resetting the
## variables there. See local.config.sample for an example and a description
## of all used variables.

CXX = g++
CPPFLAGS +=  -Iinclude -Ijson_spirit -std=c++11  -O2 
CPPFLAGS_DEBUG += -g
CPPFLAGS_RELEASE += -O2 -DNDEBUG

LD = g++
LDFLAGS = 
LDFLAGS_DEBUG +=
LDFLAGS_RELEASE += 
LIBS = -framework SFML -framework sfml-graphics -framework sfml-system -framework sfml-window 

DIST_NAME = $(PROG_NAME)-$(PROG_VERSION)
DIST_CONTENT = $(TARGET) $(DIST_FILES)



SRCS = $(shell find  . -maxdepth 1 -name '*.cpp')
SRCS += $(shell find json_spirit -maxdepth 1  -name '*.cpp')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

SRC_FILES = $(wildcard $(SRC)/*.cpp)
OBJ_FILES = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(SRC_FILES))
DEP_FILES = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.d,$(SRC_FILES))


BOOSTDIR = 
BOOSTLIBSDIR = 
BOOSTLIBS =  -lboost_system -lboost_filesystem 

######
##  Targets

Drachen: $(OBJS)
	$(LD) $(BOOSTLIBSDIR) $(BOOSTLIBS) $(LDFLAGS) $(LIBS)  -o $@ $^

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(BOOSTDIR) $(ROOTINC) $(ROOTCFLAGS) -c -o $@ $^

mkinfo:
	@echo $(SRCS)

clean:
	rm -f $(OBJS)
.PHONY: clean

