TARGET = WinHook

INCLUDES =
DEFINES = -D_UNICODE -DUNICODE
LIBS = -lshell32 -lmsvcrt -lkernel32 -luser32
CFLAGS = -std=c++11
CLFLAGS = -nostdlib
ifdef DEBUG
  CFLAGS += -g
else
  CFLAGS += -O2
  CLFLAGS += -s -Wl,--gc-sections
endif

SRCS = $(wildcard *.cpp)
OBJS = $(subst .cpp,.o,$(SRCS))
HDRS = $(wildcard *.h)

RES = $(wildcard *.rc)
RESC = $(subst .rc,.res,$(RES))

all: ${TARGET}

${TARGET}: ${OBJS} ${RESC}
	g++ ${CLFLAGS} -mwindows -o $@ $^ ${LIBS}

%.o: %.cpp ${HDRS}
	g++ ${DEFINES} ${CFLAGS} ${INCLUDES} -c -o $@ $<

%.res: %.rc resource.h item.ico
	windres $< $@ -O coff

run: ${TARGET}
	${TARGET}.exe

clean:
	swiss rm -f ${OBJS} ${RESC}
