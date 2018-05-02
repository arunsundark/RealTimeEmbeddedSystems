INCLUDE_DIRS = 
LIB_DIRS = 
CC=g++

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -lrt
CPPLIBS= -L/usr/lib -L/usr/include/tesseract/ -L/usr/include/leptonica -lopencv_core -lopencv_flann -lopencv_video -ltesseract -ljpeg -ltiff -llept -lespeak

HFILES= 
CFILES= 
CPPFILES= profiler.cpp tesseract.cpp

SRCS= ${HFILES} ${CFILES}
CPPOBJS= ${CPPFILES:.cpp=.o}

all:	profiler 

clean:
	-rm -f *.o *.d
	-rm -f profiler histcomp espeak tesseract

distclean:
	-rm -f *.o *.d

profiler: profiler.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv` $(CPPLIBS)

histcomp: histcomp.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv` $(CPPLIBS)

espeak: espeak.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lespeak

tesseract: tesseract.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv` $(CPPLIBS)

#`pkg-config --libs opencv` $(CPPLIBS)




depend:

.c.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CC) $(CFLAGS) -c $<
