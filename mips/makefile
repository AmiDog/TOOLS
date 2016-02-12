#
# Makefile
#

CROSS=

CC=$(CROSS)gcc
LD=$(CROSS)gcc
CFLAGS=-g -O1 -fomit-frame-pointer -fno-strict-aliasing -march=pentium -I. \
-Wall -Werror -Wextra -Wundef -Wsign-compare -Wmissing-prototypes -Wmissing-declarations -Wwrite-strings
LDFLAGS=

MKDIR=mkdir -p
CP=cp --remove-destination -p
CPR=$(CP) -r
RM=rm -rf

# object
OBJFILE=$(addprefix obj/, lwl_lwr.o swl_swr.o main.o)

# binary
BINFILE=bin/mips.exe

PATHS=obj bin

# link libs (Win32)
LIBS=-lm -lgdi32 -lddraw -lwinmm

# make rules
all: default

default: $(PATHS) $(BINFILE)

clean:
	$(RM) obj/*
	$(RM) bin/*

$(BINFILE): $(OBJFILE)
	$(LD) $(LDFLAGS) -o $(BINFILE) $(OBJFILE) $(LIBS)

obj/%.o : %.c
	$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@

define DIR_template
$(1):
	$(MKDIR) $(1)
endef

$(foreach path,$(PATHS), \
	$(eval $(call DIR_template,$(path))) \
)

$(OBJFILE): $(wildcard *.h)
