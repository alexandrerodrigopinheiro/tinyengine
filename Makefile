####### Platform specifics

PLATFORM_OS		:= $(shell uname)
PLATFORM_ARCH	:= $(shell getconf LONG_BIT)

####### Compiler, tools and options

DEL_FILE		= rm -f
CHKDIR			= test -d
MKDIR			= mkdir -p
COPY			= cp -f
COPY_DIR		= cp -f -R
DEL_DIR			= rmdir
MOVE			= mv -f
STRIP			= strip

####### Flags

CC				= gcc-4.8
GXX			    = g++-4.8
CFLAGS			= -pipe -Wall -Wextra -pedantic -fmessage-length=0 -std=c99 $(DEFINES) -fstack-protector-all
CXXFLAGS		= -pipe -Wall -Wextra -pedantic -fmessage-length=0 -std=c++11 $(DEFINES) -fstack-protector-all
DEFINES			= -D_BSD_SOURCE -D_GNU_SOURCE

####### Files, Folders and Paths to app

DIR_OBJ			= build
DIR_SRC			= src

APP_NAME		= Duck Tales
APP_FILE		= app
APP_EXEC		= ducktales

DIR_APP			= src/App

SRC_APP			= $(wildcard $(DIR_SRC)/*/*.cpp)
INC_APP			= $(wildcard $(DIR_SRC)/*/*.h)
OBJ_APP			= $(addsuffix .o, $(basename $(subst $(DIR_APP)/, $(DIR_OBJ)/$(DIR_APP)/, $(SRC_APP))))

####### Files, Folders and Paths to keygen

KGN_NAME		= Key Gen
KGN_FILE		= keygen
KGN_EXEC		= keygen

DIR_KGN			= src/Keygen

SRC_KGN			= $(wildcard $(DIR_SRC)/*/*.cpp)
INC_KGN			= $(wildcard $(DIR_SRC)/*/*.h)
OBJ_KGN			= $(addsuffix .o, $(basename $(subst $(DIR_KGN)/, $(DIR_OBJ)/$(DIR_KGN)/, $(SRC_KGN))))

####### Includes and Libraries

ifeq ($(PLATFORM_ARCH), 32)
INCS			= -Ilib/TinyEngine/include -Isrc -Isrc/FrameWork -I./src/SceneGame -I/usr/include/freetype2 -I/usr/include/GL -I/usr/include/SDL -fabi-version=2 -fno-omit-frame-pointer -I/usr/include/mysql
LIBS			= -L/usr/lib -L/usr/lib/i386-linux-gnu -lcrypto -lssl -lblkid -lSDL -lSDL_gfx -lSDL_image -lSDL_mixer -lSDL_net -lSDL_ttf -lX11 -lXrandr -lmysqlclient -lsqlite3 -lpthread -lz -m32
else ifeq ($(PLATFORM_ARCH), 64)
INCS			= -Ilib/TinyEngine/include -Isrc -Isrc/FrameWork -I./src/SceneGame -I/usr/include/freetype2 -I/usr/include/GL -I/usr/include/SDL -fabi-version=2 -fno-omit-frame-pointer -I/usr/include/mysql
LIBS			= -L/usr/lib/ -L/usr/lib/x86_64-linux-gnu/ -lcrypto -lssl -lblkid -lSDL -lSDL_gfx -lSDL_image -lSDL_mixer -lSDL_net -lSDL_ttf -lX11 -lXrandr -lmysqlclient -lsqlite3 -lpthread -lz -m64
endif

####### Build rules

.PHONY: all light hard app gdb upload download
.SECONDARY: build-pre build-post build-debug build-app build-game build-key build-keygen all exec-pre exec-post exec-gdb exec-run run gdb git-pre git-post git-upload git-download upload download

####### Compiling all

all: build-debug
light: build-light
hard: build-hard

print: 
	@echo $(OBJ_APP)

####### Compiling objects

build-game: build-pre $(DIR_OBJ)/$(APP_FILE)
build-keygen: build-pre $(DIR_OBJ)/$(KGN_FILE)

$(DIR_OBJ)/$(APP_FILE): $(OBJ_APP)
	@$(GXX) $(CXXFLAGS) $(DIR_APP).cpp -o $(DIR_OBJ)/$(APP_EXEC) $(DEFINES) $(OBJ_APP) $(INCS) $(LIBS)

$(DIR_OBJ)/$(KGN_FILE): $(OBJ_KGN)
	@$(GXX) $(CXXFLAGS) $(DIR_KGN).cpp -o $(DIR_OBJ)/$(KGN_EXEC) $(DEFINES) $(OBJ_KGN) $(INCS) $(LIBS)

####### Compiling apps

build-app: build-pre $(OBJ_APP)
build-key: build-pre $(OBJ_KGN)

$(DIR_OBJ)/$(DIR_APP)/%.o: $(DIR_APP)/%.cpp $(DIR_APP)/%.h
	@$(CHKDIR) $(DIR_OBJ) | $(MKDIR) $(DIR_OBJ)
	@$(CHKDIR) $(dir $@) | $(MKDIR) $(dir $@)
	@$(GXX) $(CXXFLAGS) -c $< -o $@ $(DEFINES) $(INCS) $(LIBS)
	@echo Done $< in $@

$(DIR_OBJ)/$(DIR_KGN)/%.o: $(DIR_KGN)/%.cpp $(DIR_KGN)/%.h
	@$(CHKDIR) $(DIR_OBJ) | $(MKDIR) $(DIR_OBJ)
	@$(CHKDIR) $(dir $@) | $(MKDIR) $(dir $@)
	@$(GXX) $(CXXFLAGS) -c $< -o $@ $(DEFINES) $(INCS) $(LIBS)
	@echo Done $< in $@

####### Build

build-pre:
	@echo Building ... 

build-post:
	@echo Done.

build-light: CFLAGS += -g0
build-light: DEFINES += -DLIGHT
build-light: build-app build-key build-game build-keygen
	@$(MAKE) --no-print-directory build-post

build-hard: CFLAGS += -g0
build-hard: DEFINES += -DHARD
build-hard: build-app build-key build-game build-keygen
	@$(MAKE) --no-print-directory build-post

build-debug: DEFINES += -DDEBUG -DLIGHT
build-debug: CFLAGS += -O0 -g3 -g -ggdb -O0 -Wall -W
build-debug: CXXFLAGS += -O0 -g3 -g -ggdb -O0 -Wall 	-W
build-debug: build-app build-key build-game build-keygen
	@$(MAKE) --no-print-directory build-post

####### Clearing

clean-pre:
	@echo Cleaning ...

clean-post:
	@echo Cleaned.

clean-key:
	@$(DEL_FILE) $(DIR_OBJ)/$(DIR_KGN)/*/*

clean-app:
	@$(DEL_FILE) $(DIR_OBJ)/$(APP_FILE)/*/*
	@$(DEL_FILE) $(DIR_OBJ)/$(APP_EXEC)

clean: clean-pre clean-key clean-app clean-post

####### Execute

exec-pre:
	@echo Running ...

exec-post:
	@echo Done.

exec-dbg:
	@gdb ./$(DIR_OBJ)/$(APP_EXEC)

exec-run:
	./$(DIR_OBJ)/$(APP_EXEC)

run: exec-pre exec-run exec-post
dbg: exec-pre exec-dbg exec-post

####### Update and Download

git-pre:
	@echo GitHub ...

git-post:
	@echo Done.

git-upload:
	@git add conf src resources Makefile INFO INSTALL; git commit -m "Compile and update autmatic"; git push origin master;

git-download:
	@git pull --recurse-submodules
	@git submodule update --remote --recursive
	@git submodule foreach git pull origin master

upload: git-pre git-upload git-post
download: git-pre git-download git-post