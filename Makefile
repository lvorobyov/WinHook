TARGET = WinHook.exe
LIB = WinHook.dll

ifeq (${CC},cc)
  CC = gcc
endif
RM = swiss rm

INCLUDES =
DEFINES = -D_UNICODE -DUNICODE
LIBS = -lshell32
CFLAGS = -std=c++14
CCFLAGS = -std=c99
CLFLAGS =
ifdef DEBUG
  CFLAGS += -g
  CCFLAGS += -g
else
  CFLAGS += -O2
  CCFLAGS += -O2
  CLFLAGS += -s -Wl,--gc-sections
endif

SRCS = $(filter-out crt.cpp,$(wildcard *.cpp))
OBJS = $(filter-out winhook.o,$(subst .cpp,.o,$(SRCS)))
HDRS = $(wildcard *.h)

ifdef TINY
  LIBS += -lmsvcrt -lkernel32 -luser32
  CLFLAGS += -nostdlib
  OBJS += crt.o
endif

RES = $(wildcard *.rc)
RESC = $(subst .rc,.res,$(RES))

all: ${LIB} ${TARGET}

${TARGET}: ${OBJS} ${RESC}
	${CXX} ${CLFLAGS} -mwindows -o $@ $^ ${LIBS} -L. -lwinhook

${LIB}: $(subst .c,.o,$(wildcard *.c))
	${CC} ${CLFLAGS} -mwindows -shared -Wl,--out-implib,winhook.lib -o $@ $^ ${LIBS}

winhook.o: winhook.c winhook.h
	${CC} ${DEFINES} -DBUILD_DLL ${CCFLAGS} ${INCLUDES} -c -o $@ $<

autoscroll.o: autoscroll.c autoscroll.h
	${CC} ${DEFINES} -DBUILD_DLL ${CCFLAGS} ${INCLUDES} -c -o $@ $<

%.o: %.cpp ${HDRS}
	${CXX} ${DEFINES} ${CFLAGS} ${INCLUDES} -c -o $@ $<

%.res: %.rc resource.h item.ico
	windres $< $@ -O coff

run: ${LIB} ${TARGET}
	${TARGET}

clean:
	${RM} -f ${OBJS} ${RESC}
