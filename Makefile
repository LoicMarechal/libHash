

#-----------------------------------------------------------------------------#
#                                                                             #
#                          Hash table library   V1.40                         #
#                                                                             #
#-----------------------------------------------------------------------------#
#                                                                             #
#   Description:        various operations on hash tables                     #
#   Author:             Loic MARECHAL                                         #
#   Creation date:      sep 25 2015                                           #
#   Last modification:  jun 19 2019                                           #
#                                                                             #
#-----------------------------------------------------------------------------#


# Set compiler and flags
CC       =  gcc
CFLAGS   =  -O3 -Wunused-parameter -Wsign-compare -Wuninitialized -Wunused -Wall

ifeq ($(int), 64)
   CFLAGS   += -DINT64
endif


# Working directories
LIBDIR   = $(HOME)/lib/$(ARCHI)
INCDIR   = $(HOME)/include
SRCSDIR  = sources
OBJSDIR  = objects/$(ARCHI)
ARCHDIR  = archives
DIRS     = objects $(LIBDIR) $(OBJSDIR) $(ARCHDIR) $(INCDIR)
VPATH    = $(SRCSDIR)


# Files to be compiled
SRCS     = $(wildcard $(SRCSDIR)/*.c)
HDRS     = $(wildcard $(SRCSDIR)/*.h)
OBJS     = $(patsubst $(SRCSDIR)%, $(OBJSDIR)%, $(SRCS:.c=.o))
LIB      = libhash.a


# Definition of the compiling implicit rule
$(OBJSDIR)/%.o : $(SRCSDIR)/%.c
	$(CC) -c $(CFLAGS) -I$(SRCSDIR) $< -o $@


# Install the library
$(LIBDIR)/$(LIB): $(DIRS) $(OBJS)
	cp $(OBJSDIR)/libhash.o $@
	cp $(SRCSDIR)/libhash.h $(SRCSDIR)/libhash.ins $(INCDIR)


# Objects depends on headers
$(OBJS): $(HDRS)


# Build the working directories
$(DIRS):
	@[ -d $@ ] || mkdir $@


# Remove temporary files
clean:
	rm -f $(OBJS) $(LIBDIR)/$(LIB)


# Build a dated archive including sources, patterns and makefile
tar: $(DIRS)
	tar czf $(ARCHDIR)/libhash.`date +"%Y.%m.%d"`.tgz sources Makefile
