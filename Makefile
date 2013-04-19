######
##  Default configuration
##
## Modify this configuration by creating a local.config file and resetting the
## variables there. See local.config.sample for an example and a description
## of all used variables.
CXX = g++
CPPFLAGS +=  -Iinclude -Ijson_spirit -std=c++11  -O2 

OBJCCC = clang++
OBJCFLAGS = -O2 

MAPCXX = clang++

LD = g++

MAPLD = clang++
LIBS = -framework SFML -framework sfml-graphics -framework sfml-system -framework sfml-window -lboost_system -lboost_filesystem -lboost_date_time

LDMAPFLAGS = -framework Cocoa

DIST_NAME = $(PROG_NAME)-$(PROG_VERSION)

-include ./local.config

BUILD = build
BIN = bin
SRC = . 
JSON = json_spirit
MAP = MapEditor

SRC_FILES = $(shell find $(SRC) -maxdepth 1 -name '*.cpp')
SRC_FILES += $(wildcard $(JSON)/*.cpp)

JSON_FILES = $(wildcard $(JSON)/*.cpp)

JSON_OBJS = $(JSON_FILES:.cpp=.o)

SRC_MAP_CXX = $(shell find $(MAP) -maxdepth 1 -name '*.cpp')
SRC_MAP_OBJC = $(shell find $(MAP) -maxdepth 1 -name '*.mm')

MAP_OBJS_CXX = $(SRC_MAP_CXX:.cpp=.o)
MAP_OBJS_OBJC = $(SRC_MAP_OBJC:.mm=.o)
#SRC_DEPS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.d,$(SRC_FILES))

#SRC_OBJS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(SRC_FILES))

MAP_DEP_CXX = $(SRC_MAP_CXX:.cpp=.d)
MAP_DEP_OBJC = $(SRC_MAP_OBJC:.mm=.d)

SRC_OBJS = $(SRC_FILES:.cpp=.o)
SRC_DEPS = $(SRC_FILES:.cpp=.d)
TARGETS = Drachen MapEdit

######
##  Targets

all: $(TARGETS)

Drachen: $(SRC_OBJS)
	$(LD) $(LDFLAGS) $(LIBS)  -o $@ $^

$(BUILD)/%.d: $(SRC)/%.cpp
	$(CPP) $(CPPFLAGS) -MM -MT $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$<) $< > $@

$(BUILD)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $^

$(BIN)/%: $(BUILD)/%.o
	@echo Linking $@
	$(LD) -o $@ $(LDFLAGS) $(LIBS) $<

MapEdit: $(MAP_OBJS_OBJC) $(MAP_OBJS_CXX) $(JSON_OBJS) Log.o
	@echo Making Map
	@echo $(MAP_OBJS_OBJC) $(MAP_OBJS_CXX) $(JSON_OBJS)
	$(MAPLD) -v  $(LDFLAGS) $(LDMAPFLAGS)    -o $@  $^ $(LIBS) $(LIBS) -lc++

$(MAP_DEP_OBJC): $(SRC_MAP_OBJC)
	@echo Dep OBJC
	$(OBJCCC) $(OBJCFLAGS) -MM -MT $(patsubst $(SRC_MAP_OBJC)/%.cpp,$(BUILD)/%.o,$<) $< > $@

$(MAP_DEP_CXX): $(SRC_MAP_CXX)
	@echo Dep CXX
	$(CXX) $(CPPFLAGS) -MM -MT $(patsubst $(SRC_MAP_CXX)/%.cpp,$(BUILD)/%.o,$<) $< > $@

$(MAP)/%.o: $(MAP)/%.mm
	$(OBJCCC) $(OBJCFLAGS) -c -o $@ $^

$(MAP)/%.o: $(MAP)/%.cpp
	$(CXX)  $(CPPFLAGS) -c -o $@ $^

$(JSON)/%.o: $(JSON)/%.cpp
	$(CXX)  $(CPPFLAGS)   -c -o $@ $^



-include $(SRC_DEPS)
-include $(MAP_DEP_OBJC)
-include $(MAP_DEP_CXX)

mkinfo:
	@echo $(SRC_OBJS)

clean:
	rm -f $(OBJS) $(SRC_OBJS) $(MAP_OBJS_OBJC) $(MAP_OBJS_CXX) $(TARGETS)

.PHONY: clean mkinfo

