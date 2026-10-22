CC ?= gcc
CFLAGS ?= -O2 -Wall
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

PKG_CONFIG ?= pkg-config
DEPS = gtk+-3.0 spice-client-gtk-3.0

CFLAGS += $(shell $(PKG_CONFIG) --cflags $(DEPS))
LIBS = $(shell $(PKG_CONFIG) --libs $(DEPS))

TARGET = clean-spice
SRCS = clean-spice.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

install: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all install uninstall clean
