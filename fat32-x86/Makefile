CFLAGS = -g -I../pico
CXXFLAGS = $(CFLAGS) -std=c++17
#CXXFLAGS += -fsanitize=address
#LDFLAGS = -lasan
VPATH = ../pico


BINS = part32 dir32 type32 can tree32
OBJS = part32.o dir32.o type32.o can.o

COMMON = fat32.o sdcard-x86.o

all : $(BINS)

tree32 : tree32.o $(COMMON)
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

can : can.o $(COMMON)
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

type32 : type32.o $(COMMON)
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

dir32 : dir32.o $(COMMON)
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

part32 : part32.o $(COMMON)
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o : %.cc
	g++ $(CXXFLAGS) -c $< -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -rf $(BINS) *.o
