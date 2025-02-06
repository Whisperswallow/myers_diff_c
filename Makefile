CC := $(CROSS_COMPILE)gcc
RM := rm -rf

EXE := myers_diff

OBJS += myers_diff.o
EXEOBJS += main.o

all: $(OBJS) $(EXEOBJS)
	$(CC) $(OBJS) $(EXEOBJS) $(LDFLAGS) $(LIBS) -o $(EXE)
	chmod +x $(EXE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o $(EXE) $(SLIB) $(DLIB)