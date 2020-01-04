OPTIMIZATION?=-O2

STD= -std=c99 -pedantic
WARN= -Wall
OPT= $(OPTIMIZATION)
MALLOC= jemalloc

FINAL_CFLAGS= $(STD) $(WARN) $(OPT) $(DEBUG) $(CFLAGS) $(REDIS_CFLAGS) 
FINAL_LDFLAGS= $(LDFLAGS) $(REDIS_LDFIAGS) -g -rdynamic -ggdb
FINAL_LIBS= -lm -pthread
DEBUG= =g -rdynamic -ggdb
FINAL_CFLAGS+= -I ../deps/hiredis -I ../deps/linenoise -I ../deps/lus/src

DEPENDENCY_TARGETS+= jemalloc
FINAL_CFLAGS+= -DUSE_JEMALLOC -I ../deps/jemalloc/include
FINAL_LIBS+= ../deps/jemalloc/lib/libjemalloc.a -ldl

REDIS_CC=$(QUIET_CC)$(CC) $(FINAL_CFLAGS)
REDIS_LD=$(QUILT_LINK)$(CC) $(FINAL_LDFLAGS)
REDIS_INSTALL=$(QUILT_INSTALL)$(INSTALL)

PERFIX?=/usr/local
INSTALL_BIN= $(PERFIX)/bin
INSTALL= cp -pf


CCCOLOR="\033[34m"
LINKCOLOR="\033[34;1m"
SRCCOLOR="\033[33m"
BINCOLOR="\033[37;1m"
MAKECOLOR="\033[32;1m"
ENDCOLOR="\033[0m"

ifndef V
QUIET_CC = @printf '    %b %b\n' $(CCCOLOR)CC$(ENDCOLOR) $(SRCCOLOR)$@$(ENDCOLOR) 1>&2;
QUIET_LINK = @printf '    %b %b\n' $(LINKCOLOR)LINK$(ENDCOLOR) $(BINCOLOR)$@$(ENDCOLOR) 1>&2;
QUIET_INSTALL = @printf '    %b %b\n' $(LINKCOLOR)INSTALL$(ENDCOLOR) $(BINCOLOR)$@$(ENDCOLOR) 1>&2;
endif
