# Installation directories.
PREFIX ?= $(DESTDIR)/usr
BINDIR ?= $(PREFIX)/bin

# Intermediates directories.
INTM = obj/

CXX = clang++
CPPFLAGS ?= -std=c++11 -Wall -Werror
CPPFLAGS_DEBUG = -DDEBUG -gdwarf-4
CPPFLAGS_RELEASE = -O2
LINKFLAGS ?= 
LIBS += boost_system boost_filesystem

TARGET=acc

lex_sources += \
	lex/constants.cpp \
	lex/lexexception.cpp \
	lex/textpointer.cpp \
	lex/token.cpp \
	lex/tokenfsm.cpp \
	lex/tokenizer.cpp
main_sources += \
	main.cpp

SRC = $(lex_sources) $(main_sources)

OBJS=$(addprefix $(INTM), $(patsubst %.cpp,%.o,$(SRC)))

define compile_cmd
	@echo [CC] $< ...
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CPPFLAGS) -I./include -c
endef

$(TARGET): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(TARGET) $(addprefix -l, $(LIBS)) $(OBJS)

obj/lex/%.o: src/lex/%.cpp
	$(call compile_cmd) -o $@ $<

obj/%.o: src/%.cpp
	$(call compile_cmd) -o $@ $<

install: all
	mkdir -p $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)

clean:
	rm -rf $(INTM)
distclean: clean
	rm -rf $(TARGET) $(TARGETD)