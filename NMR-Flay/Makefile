# Project name
PROJECT = nmr_ana

# Directories
OBJDIR = obj
SODIR  = lib
SRCDIR = src
INCDIR = include

# Compiler
CC       = g++
CPPFLAGS = -Wall -I$(INCDIR) -O2 -fPIC -g  \
       -Wmissing-declarations -Wreturn-type -Wunused \
       -Wcomment -Wformat

# shared object 
LDFLAGS = -shared

# Libraries
LIBS       = -lm
LIBRARY    = NMRANA
TARGET_LIB = $(SODIR)/lib$(LIBRARY).so
 
# sources 
SRCDIRS = $(shell find $(SRCDIR) -type d | sed 's/$(SRCDIR)/./g' )
SRCS    = $(SRCDIR)/NMRMath.cpp             \
          $(SRCDIR)/NMRUtility.cpp          \
          $(SRCDIR)/NMRFourierTransform.cpp 

#$(SRCDIR)/NMRAnalysis.cpp         \
          $(SRCDIR)/NMRRun.cpp              \
          $(SRCDIR)/NMRPulse.cpp            \
          $(SRCDIR)/NMRPulseAnalyzed.cpp    \
          $(SRCDIR)/NMRFileManager.cpp      \
          $(SRCDIR)/NMRInputManager.cpp     
#$(SRCDIR)/NMRZeroCrossing.cpp     \
# objects 
OBJS    = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

all: $(TARGET_LIB)
#$(PROJECT)  

# build a shared library 
$(TARGET_LIB): buildrepo $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $@ $(LDFLAGS) 
# make objects
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@
# build the main program 
$(PROJECT): 
	$(CC) $(CPPFLAGS) -o $(PROJECT) $(SRCDIR)/main.cpp $(LIBS) -L$(SODIR) -l$(LIBRARY)

.PHONY: clean
clean:
	rm -Rf $(PROJECT) $(PROJECT).dSYM $(TARGET_LIB) $(OBJDIR) $(SRCDIR)/*~  *~ 

buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	for dir in $(SRCDIRS); \
	do \
	mkdir -p $(OBJDIR)/$$dir; \
	done
endef

