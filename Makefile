LIBS = libs
SRC = src
BUILD = build
BIN = bin

VPATH = $(LIBS) $(SRC)
INCLUDES = -I. $(addprefix -I, $(VPATH))

UTILS = Utils
EXCEPTS = LaytExceptions
INST = Instruction
PARSE = LayoutParser
SRC_PARSE = SourceParser
WRITER = LAYTWriter

FLEX_SRC = lex.yy
FLEX_FLAGS = -lfl

MAIN = layt-main
MAIN_DEPS = $(addprefix $(BUILD)/, $(INST).o $(PARSE).o $(MAIN).o $(EXCEPTS).o $(SRC_PARSE).o $(UTILS).o $(WRITER).o)

PREPROC = layt-preproc
PPWRAPPER = layt-wrapper

CCPP = g++
FLEX = flex
CC = gcc
CPPFLAGS = -Wall $(INCLUDES) -std=c++14
CFLAGS = -Wall $(INCLUDES)

all: dirs main pplexer ppwrapper
	@echo "Compilación terminada."

main: $(MAIN_DEPS) | $(BUILD) $(BIN)
	$(CCPP) $(MAIN_DEPS) -o $(BIN)/$(MAIN) $(CPPFLAGS)

pplexer: $(SRC)/$(PREPROC).l | $(BUILD) $(BIN)
	$(FLEX) $(SRC)/$(PREPROC).l
	$(CC) $(FLEX_FLAGS) $(FLEX_SRC).c -o $(BIN)/$(PREPROC)

ppwrapper: $(SRC)/$(PPWRAPPER).c | $(BUILD) $(BIN)
	$(CC) $(CFLAGS) $(SRC)/$(PPWRAPPER).c -o $(BIN)/$(PPWRAPPER)
	
$(BUILD)/%.o: %.cpp | $(BUILD)
	$(CCPP) $(CPPFLAGS) -c $< -o $@

# additional required headers
$(BUILD)/$(EXCEPTS).o: $(LIBS)/$(EXCEPTS).hpp $(LIBS)/$(INST).hpp
$(BUILD)/$(PARSE).o: $(LIBS)/$(PARSE).hpp $(LIBS)/$(EXCEPTS).hpp $(LIBS)/$(INST).hpp $(LIBS)/$(UTILS).hpp
$(BUILD)/$(SRC_PARSE).o: $(LIBS)/$(SRC_PARSE).hpp $(LIBS)/$(EXCEPTS).hpp $(LIBS)/$(INST).hpp $(LIBS)/$(UTILS).hpp
$(BUILD)/$(INST).o: $(LIBS)/$(EXCEPTS).hpp $(LIBS)/$(INST).hpp
$(BUILD)/$(UTILS).o: $(LIBS)/$(UTILS).hpp
$(BUILD)/$(WRITER).o: $(LIBS)/$(WRITER).hpp $(LIBS)/$(EXCEPTS).hpp

dirs:
	mkdir -p bin build

clean:
	rm -rvf bin build 