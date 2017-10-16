CC      = gcc
INCDIR  = $(HOME)/include
HDR     = $(INCDIR)/libmeshb7.h $(INCDIR)/libhash.h
LIBDIR  = $(HOME)/lib/$(ARCHI)
LIB     = $(LIBDIR)/libmeshb7.a $(LIBDIR)/libhash.a
EXE     = extract_boundary_faces
CFLAGS  = -O3 -I $(INCDIR) -Wuninitialized -Wunused -Wall
LDFLAGS = -L $(LIBDIR)

examples: $(EXE)

extract_boundary_faces: extract_boundary_faces.c $(HDR) $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -lm -lmeshb7 -lhash extract_boundary_faces.c -o $@

clean:
	rm $(EXE)
