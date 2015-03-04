# ------------------------------------------------
# Generic Makefile for C++11
#
# Author: chivequiachon@gmail.com
# Date  : 2013-06-12
#
# Changelog :
#   2013-06-20 - first version
# ------------------------------------------------

# project name (generate executable with this name)
TARGET   = DBServer

CC       = g++
# compiling flags here
CFLAGS   = -std=c++11 -Wall -I. -I/usr/include/cppcon -fpermissive -g

LINKER   = g++ -o
# linking flags here
LFLAGS   = -std=c++11 -Wall -I. -lm -pthread

# change these to set the proper directories where each files shoould be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm       = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS) -L/usr/lib -lmysqlcppconn `pkg-config gtkmm-3.0 --cflags --libs`
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@ `pkg-config gtkmm-3.0 --cflags --libs`
	@echo "Compiled "$<" successfully!"

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
