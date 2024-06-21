# config

include .env

TARGET  ?= $(shell uname -s)
CONFIG  ?= Debug
PROJECT ?= game

# paths

SRC = src
BIN = bin
LIB = libs
AST = assets

OUT = $(BIN)/$(PROJECT)
PKG = $(PROJECT) $(AST)

ifeq ($(TARGET), Web)
	OUT = $(BIN)/$(PROJECT).html
	PKG = $(PROJECT).data $(PROJECT).html $(PROJECT).js $(PROJECT).wasm
endif

# compiler

CC  ?= 
CXX ?= 

C_STD   = c11
CXX_STD = c++17

CFLAGS   = 
CXXFLAGS = 
CPPFLAGS =
LDFLAGS  =

CFLAGS 	 += -std=$(C_STD)
CXXFLAGS += -std=$(CXX_STD)

CFLAGS   += $(CPPFLAGS)
CXXFLAGS += $(CPPFLAGS)

ifeq ($(CONFIG), Debug)
	CPPFLAGS += -Wall
	CPPFLAGS += -g
	CPPFLAGS += -O0

	CFLAGS += -DDEBUG
endif

ifeq ($(CONFIG), Release)
	CPPFLAGS += -O2
endif

ifeq ($(TARGET), Windows)
ifeq ($(shell uname -s), Linux)
	CFLAGS   += -static-libgcc
	CXXFLAGS += -static-libstdc++
endif
endif

ifeq ($(TARGET), Web)
	CC  = emcc
	CXX = em++
endif

# linker

ifeq ($(TARGET), Web)
	LDFLAGS += -s USE_SDL=2
	LDFLAGS += -s USE_WEBGL2=1
	LDFLAGS += -s ALLOW_MEMORY_GROWTH
	LDFLAGS += -s ABORT_ON_WASM_EXCEPTIONS
	LDFLAGS += --preload-file $(AST)
	LDFLAGS += --shell-file $(AST)/emscripten/shell.html
endif

ifeq ($(CONFIG), Release)
ifeq ($(TARGET), Windows)
	LDFLAGS += -mwindows
endif
endif

# libs
 
DLL_SRC =

SDL_DIR	= $(LIB)/sdl2
STB_DIR = $(LIB)/stb

SDL_CPPFLAGS =
STB_CPPFLAGS =

SDL_LDFLAGS =
STB_LDFLAGS =

CPPFLAGS += $(SDL_CPPFLAGS)
CPPFLAGS += $(STB_CPPFLAGS)

LDFLAGS += $(SDL_LDFLAGS)
LDFLAGS += $(STB_LDFLAGS)

ifeq ($(TARGET), Windows)
	DLL_SRC += $(shell find $(LIB) -type f -name '*.dll')
	LDFLAGS += -lmingw32

	SDL_LDFLAGS += -L$(SDL_DIR)/lib -lSDL2main -lSDL2
endif

ifeq ($(TARGET), Darwin)
	SDL_CPPFLAGS += $(shell sdl2-config --cflags)
	SDL_LDFLAGS  += $(shell sdl2-config --libs)
endif

ifeq ($(TARGET), Web)
	SDL_LDFLAGS :=
endif

# includes

INC =

INC += $(realpath $(shell find $(SRC) -type d))
INC += $(realpath $(shell find $(LIB) -type d))

INCLUDES = $(addprefix -I, $(INC))

# sources

C_SRC 	= $(shell find $(SRC) -name "*.c")
CXX_SRC = $(shell find $(SRC) -name "*.cc")

C_OBJ 	= $(C_SRC:%.c=$(BIN)/%.o)
CXX_OBJ = $(CXX_SRC:%.cc=$(BIN)/%.o)

OBJ = $(C_OBJ) $(CXX_OBJ)
DEP = $(OBJ:%.o=%.d)

# rules

.PHONY: all config bin dirs assets libraries build run package commands clean

all: config bin assets libraries time-build commands run

config:
	@echo ""
	@echo "⚡ Build Settings ______________________"
	@echo ""
	@echo "    SYSTEM_PROCESSOR                     $(shell uname -p)"
	@echo "    HOST_SYSTEM_NAME                     $(shell uname -s)"
	@echo ""
	@echo "    BUILD_TYPE                           ${CONFIG}"
	@echo ""
	@echo "    C_STANDARD                           $(C_STD)"
	@echo "    C_COMPILER_VERSION                   $(shell $(CC) --version | head -n 1)"
	@echo "    C_COMPILER                           $(CC)"
	@echo ""
	@echo "    CXX_STANDARD                         $(CXX_STD)"
	@echo "    CXX_COMPILER_VERSION                 $(shell $(CXX) --version | head -n 1)"
	@echo "    CXX_COMPILER                         $(CXX)"

bin:
	@mkdir -p $(BIN)

assets: | bin
	@echo "\n📦 Assets ______________________________"
	@rsync -a --include '*/' "$(AST)" "$(BIN)"
	@cd $(AST) && tree --noreport | grep -v '^\.'

libraries: $(DLL_SRC) | bin
	@echo "\n📗 Libraries ___________________________"
	@rsync -a --include '*/' --exclude '*' "$(LIB)" "$(BIN)"
	@$(foreach dll, $^, cp $(dll) $(BIN)/;)
	@$(foreach dll, $^, echo "    $(dll)";)

dirs: | bin
	@rsync -a --include '*/' --exclude '*' "$(SRC)" "$(BIN)"

build: dirs $(OBJ)
	@$(CXX) -o $(OUT) $(CXXFLAGS) $(OBJ) $(LDFLAGS) && \
	echo "\n⭐ Build success!" || \
	echo "\n❌ Build failed!"

$(C_OBJ): $(BIN)/%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDES) -MMD -MP

$(CXX_OBJ): $(BIN)/%.o: %.cc
	@$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDES) -MMD -MP

run: 
	@echo "\n🎮 Starting the game...\n"
	@if [ "$(TARGET)" = "Web" ]; then \
		emrun $(PROJECT).html --no_browser --port 8080; \
	elif [ "$(TARGET)" = "Windows" ]; then \
		cmd.exe /C start $(OUT); \
	else \
		$(OUT); \
	fi 

package: build
	@cd $(BIN) && zip -r $(PROJECT).zip $(PKG)
	@echo "\n📬 Game packaged & ready-to-ship!"

commands:
	@rm -rf compile_commands.json
	@make --no-print-directory --always-make --dry-run CC=clang CXX=clang++ \
	 | grep -wE 'clang|clang\+\+' \
	 | grep -w '\-c' \
	 | jq -nR --arg dir "$$(pwd)" '[inputs | {directory: ($$dir + "/$(BIN)"), command:., file: ($$dir + "/" + (match(" ([^ ]*\\.(cc|c))").string[1:])), output: ($$dir + "/$(BIN)/" + (match(" ([^ ]*\\.(cc|c))").string[1:] | sub("\\.(c|cc)$$"; ".o")))}]' \
	 > compile_commands.json
	@echo "\n💿 Refreshed compile commands!"

time-%:
	@START_TIME=$$(date +%s%3N); \
	$(MAKE) --no-print-directory $*; \
	END_TIME=$$(date +%s%3N); \
	ELAPSED_TIME=$$(echo "$$END_TIME $$START_TIME" | awk '{print ($$1 - $$2)}'); \
	echo "⏰ Completed in $$ELAPSED_TIME ms"

clean:
	@rm -rf $(BIN)
	@echo "\n🔥 Purged files!"

-include $(DEP)

