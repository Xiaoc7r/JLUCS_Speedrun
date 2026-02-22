#CC=arm-none-linux-gnueabi-gcc
CC=gcc
#LDFLAGS+=static
EXTRA_LIBS+=
EXTRA_LIBS += -lpthread

EXEC=./pthread
OBJS=pthread.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)  $(EXTRA_LIBS)
clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

