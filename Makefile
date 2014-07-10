# Installation directories.
PREFIX ?= $(DESTDIR)/usr
BINDIR ?= $(PREFIX)/bin

# Intermediates directories.
INTM = obj/

# Compiler and flag settings
CXX = clang++
CPPFLAGS ?= -std=c++11 -Wall -Werror
CPPFLAGS_DEBUG = -DDEBUG -gdwarf-4
CPPFLAGS_RELEASE = -O2
LINKFLAGS ?= 
LIBS += boost_system boost_filesystem

# Target name
TARGET=acc

# Source files
lex_sources += \
	lex/constants.cpp \
	lex/lexexception.cpp \
	lex/textpointer.cpp \
	lex/token.cpp \
	lex/tokenfsm.cpp \
	lex/tkstream.cpp
ana_sources += \
	analyzer/analyze_context.cpp \
	analyzer/func_base.cpp \
	analyzer/list_base.cpp \
	analyzer/non_terminal.cpp
main_sources += \
	main.cpp

#---------------------------------------------------------------------
# Working part

# Build object file list
SRC = \
	$(lex_sources) \
	$(ana_sources) \
	$(main_sources)
OBJS=$(addprefix $(INTM), $(patsubst %.cpp,%.o,$(SRC)))

# Define common compile command
define compile_cmd
	@echo [CXX] $< ...
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CPPFLAGS) -I./include -c
endef

# Make target definitions
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