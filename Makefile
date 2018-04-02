TARGET = WinHook
LIB = WinHook.dll

INCLUDES =
DEFINES = -D_UNICODE -DUNICODE
LIBS = -lshell32 -lmsvcrt -lkernel32 -luser32
CFLAGS = -std=c++11
CCFLAGS = -std=c99
CLFLAGS = -nostdlib
ifdef DEBUG
  CFLAGS += -g
  CCFLAGS += -g
else
  CFLAGS += -O2
  CCFLAGS += -O2
  CLFLAGS += -s -Wl,--gc-sections
endif

SRCS = $(wildcard *.cpp)
OBJS = $(filter-out winhook.o,$(subst .cpp,.o,$(SRCS)))
HDRS = $(wildcard *.h)

RES = $(wildcard *.rc)
RESC = $(subst .rc,.res,$(RES))

all: ${LIB} ${TARGET}

${TARGET}: ${OBJS} ${RESC}
	g++ ${CLFLAGS} -mwindows -o $@ $^ ${LIBS} -L. -lwinhook

${LIB}: winhook.o
	gcc ${CLFLAGS} -mwindows -shared -Wl,--out-implib,winhook.lib -o $@ $^ ${LIBS}

winhook.o: winhook.c winhook.h
	gcc ${DEFINES} -DBUILD_DLL ${CCFLAGS} ${INCLUDES} -c -o $@ $<

%.o: %.cpp ${HDRS}
	g++ ${DEFINES} ${CFLAGS} ${INCLUDES} -c -o $@ $<

%.res: %.rc resource.h item.ico
	windres $< $@ -O coff

run: ${TARGET}
	${TARGET}.exe

clean:
	swiss rm -f ${OBJS} ${RESC}
