CPP=g++
CC=gcc
DLLWRAP=dllwrap
#CPP=i586-mingw32msvc-g++
#CC=i586-mingw32msvc-gcc
#DLLWRAP=i586-mingw32msvc-dllwrap

#OS=linux
#OS=windows
OS=mac

OBJ = server.o \
	bot_globals.o \
	bot_ai.o \
	bot_chat.o \
	meta_api.o \
	util.o \
	bot_navigate.o \
	client.o \
	console.o \
	debug.o \
	dllapi.o \
	engine_api.o \
	engine_util.o \
	entity.o \
	bot.o \
	h_export.o \
	ini.o \
	linkgame.o \
	namefunc.o \
	sdk_util.o \
	general.o \
	world.o \
	bot_ainet.o \
	bot_combat.o \
	bot_manager.o \
	bot_hlengine.o \
	gearbox/bot_gearbox.o \
	gearbox/server_gearbox.o \
	gearbox/server_gearbox_ctf.o \
	gearbox/bot_gearbox_ctf.o \
	cstrike/bot_cs.o \
	cstrike/botai_cs.o \
	cstrike/buy.o \
	cstrike/util_cs.o \
	cstrike/server_cs.o \
	ricochet/bot_ricochet.o \
	ricochet/server_ricochet.o \
	valve/bot_valve.o \
	valve/server_valve.o

GAMEDEF = -DVALVE_DLL=1
CCOPT = -march=i586 -O2 -DNDEBUG=1
CCDEBUG = -ggdb3 -D_DEBUG=1

INCLUDE = -I./hlsdk -I./metamod

CFLAGS = $(GAMEDEF) $(CCOPT)
#CFLAGS = $(GAMEDEF) $(CCDEBUG)

ifeq "$(OS)" "linux"

BASEFLAGS=-Dstricmp=strcasecmp -Dstrcmpi=strcasecmp
CPPFLAGS=${BASEFLAGS} ${CFLAGS} ${INCLUDE}

bot.so:	${OBJ}
	${CPP}  ${CFLAGS} -fPIC -shared -static -o $@ ${OBJ} -ldl -lm

clean:
	-rm -f *.o
	-rm -f cstrike/*.o
	-rm -f valve/*.o
	-rm -f gearbox/*.o
	-rm -f ricochet/*.o
	-rm -f bot.so

else ifeq "$(OS)" "mac"

BASEFLAGS=-Dstricmp=strcasecmp -Dstrcmpi=strcasecmp
CPPFLAGS=${BASEFLAGS} ${CFLAGS} ${INCLUDE} -m32

bot.dylib:	${OBJ}
	${CPP} -m32 -fPIC -shared -o $@ ${OBJ} -ldl -lm

clean:
	-rm -f *.o
	-rm -f cstrike/*.o
	-rm -f valve/*.o
	-rm -f gearbox/*.o
	-rm -f ricochet/*.o
	-rm -f bot.dylib

else

BASEFLAGS=
CPPFLAGS=${BASEFLAGS} ${CFLAGS} ${INCLUDE}

bot.dll:	${OBJ}
	${DLLWRAP} ${CFLAGS} -mwindows -Xlinker i386pe.merge --add-stdcall-alias ${OBJ} --driver-name ${CPP} -o bot.dll

clean:
	-rm -f *.o
	-rm -f cstrike/*.o
	-rm -f valve/*.o
	-rm -f gearbox/*.o
	-rm -f ricochet/*.o
	-rm -f bot.dll

endif

%.o:	%.cpp
	${CPP} ${CPPFLAGS} -c $< -o $@

%.o:	%.c
	${CC} ${CPPFLAGS} -c $< -o $@

