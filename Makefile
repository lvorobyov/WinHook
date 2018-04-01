TARGET = WinHook

INCLUDES =
DEFINES = -D_UNICODE -DUNICODE
CFLAGS = -std=c++11
CLFLAGS =
ifdef DEBUG
  CFLAGS += -g
else
  CFLAGS += -O
  CLFLAGS += -s -Wl,--gc-sections
endif

SRCS = $(wildcard *.cpp)
OBJS = $(subst .cpp,.o,$(SRCS))
HDRS = $(wildcard *.h)

RES = $(wildcard *.rc)
RESC = $(subst .rc,.res,$(RES))

all: ${TARGET}

${TARGET}: ${OBJS} ${RESC}
	g++ ${CLFLAGS} -mwindows -o $@ $^ -lshell32

%.o: %.cpp ${HDRS}
	g++ ${DEFINES} ${CFLAGS} ${INCLUDES} -c -o $@ $<

%.res: %.rc resource.h item.ico
	windres $< $@ -O coff

run: ${TARGET}
	${TARGET}.exe

clean:
	swiss rm -f ${OBJS} ${RESC}
