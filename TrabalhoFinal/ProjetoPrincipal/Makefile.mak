CC     = gcc
WINDRES= windres
RM     = rm -f
OBJS   = Helicoptro.o \
         AppResource.res

LIBS   = -mwindows -static -lglut32 -lglu32 -lopengl32 -lwinmm -lgdi32 -std=c99
CFLAGS = -DGLUT_STATIC

.PHONY: Helicoptero.exe clean clean-after

all: Helicoptero.exe

clean:
	$(RM) $(OBJS) Helicoptero.exe

clean-after:
	$(RM) $(OBJS)

Helicoptero.exe: $(OBJS)
	$(CC) -Wall -s -o $@ $(OBJS) $(LIBS)

Helicoptro.o: Helicoptro.c image.c
	$(CC) -Wall -s -c $< -o $@ $(CFLAGS)

AppResource.res: AppResource.rc
	$(WINDRES) -i AppResource.rc -J rc -o AppResource.res -O coff

