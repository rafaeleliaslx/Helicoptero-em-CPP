CC     = gcc
WINDRES= windres
RM     = rm -f
OBJS   = main.o \
         AppResource.res

LIBS   = -mwindows -static -lglut32 -lglu32 -lopengl32 -lwinmm -lgdi32
CFLAGS = -DGLUT_STATIC

.PHONY: Glut\ demo1.exe clean clean-after

all: Glut\ demo1.exe

clean:
	$(RM) $(OBJS) Glut\ demo1.exe

clean-after:
	$(RM) $(OBJS)

Glut\ demo1.exe: $(OBJS)
	$(CC) -Wall -s -o '$@' $(OBJS) $(LIBS)

main.o: main.c
	$(CC) -Wall -s -c $< -o $@ $(CFLAGS)

AppResource.res: AppResource.rc
	$(WINDRES) -i AppResource.rc -J rc -o AppResource.res -O coff

