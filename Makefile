#!/bin/make
#
#	---> Quick Guide <---
#	$@ : name of the target
#	$< : name of first dependency
#	$ˆ : list of dependencies
#	$? : list of dependencies most recent than the target
#	$* : name of a file without suffixe

	# General

VERSION := v9-4
TARGET = cts_$(VERSION)	
RM = rm -f
RMDIR = rm -rf
# THREAD: the recommended compile's option for thread safety in GCC.
#	The "-pthread" define option is equivalent to "-lpthread -D_REENTRANT"
#	The flags do the following:
#		(*) -lpthread instructs the linker to use the appropriate library
#		versions for thread compatibility.
#		(*) -D_REENTRANT tells the compiler to use the declarations
#		(functions, types, ...) necessary for thread usage.
THREADS = -pthread

	# Flags and Prefix

CFLAGS := -Wall
LDFLAGS := $(THREADS)

	# Project's structure

SRCDIR = src
HEADDIR = inc
OBJDIR = lib
BINDIR = bin
ETCDIR = etc
# Associated with the System V IPC keys created during program execution.
PSWDIR = $(ETCDIR)/password

	# Source, Binaries and Dependencies

SRC := $(shell find $(SRCDIR) -type f -name '*.c')
OBJ := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SRC:.c=.o))
DEP := $(OBJ:.o=.d)
BIN := $(BINDIR)/$(TARGET)
-include $(DEP)

	# Build Rules

.PHONY: all remake setup install clean uninstall
.DEFAULT_GOAL := all

all: setup $(BIN)

remake: clean all

setup:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@mkdir -p $(PSWDIR)

$(BIN): $(OBJ)
	gcc $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	gcc $(CFLAGS) -c -MMD -MP -o $@ $<

install: $(BIN)
	install -d $(HOME)
	install -m 755 $(BIN) $(HOME)

clean:
	$(RM) $(OBJ) $(DEP) $(BIN)
	$(RMDIR) $(OBJDIR) $(ETCDIR) 2> /dev/null; true

uninstall: $(BIN)
	$(RM) $(BIN) $(HOME)
