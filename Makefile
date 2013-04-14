######
##  Default configuration
##
## Modify this configuration by creating a local.config file and resetting the
## variables there. See local.config.sample for an example and a description
## of all used variables.
CXX = g++
CPPFLAGS +=  -Iinclude -Ijson_spirit -std=c++11  -O2 -D_DEBUG


LD = g++
LIBS = -framework SFML -framework sfml-graphics -framework sfml-system -framework sfml-window -lboost_system -lboost_filesystem 

DIST_NAME = $(PROG_NAME)-$(PROG_VERSION)

-include ./local.config

BUILD = build
BIN = bin
SRC = . 
JSON = json_spirit


SRC_FILES = $(shell find $(SRC) -maxdepth 1 -name '*.cpp')
SRC_FILES += $(wildcard $(JSON)/*.cpp)

#SRC_DEPS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.d,$(SRC_FILES))

#SRC_OBJS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(SRC_FILES))

SRC_OBJS = $(SRC_FILES:.cpp=.o)
SRC_DEPS = $(SRC_FILES:.cpp=.d)
TARGETS = $(BIN)/Drachen

######
##  Targets

all: $(TARGETS)

$(BIN)/Drachen: $(SRC_OBJS)
	$(LD) $(LDFLAGS) $(LIBS)  -o $@ $^

$(BUILD)/%.d: $(SRC)/%.cpp
	$(CPP) $(CPPFLAGS) -MM -MT $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$<) $< > $@

$(BUILD)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $^

$(BIN)/%: $(BUILD)/%.o
	@echo Linking $@
	$(LD) -o $@ $(LDFLAGS) $(LIBS) $<

-include $(SRC_DEPS)

mkinfo:
	@echo $(SRC_OBJS)

clean:
	rm -f $(OBJS) $(SRC_OBJS)

.PHONY: clean mkinfo

