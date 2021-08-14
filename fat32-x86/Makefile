CXXFLAGS = -g -std=c++17
#CXXFLAGS += -fsanitize=address
#LDFLAGS = -lasan

BINS = part32 dir32 type32 can tree32
OBJS = fat32.o part32.o dir32.o type32.o sdcard.o can.o

all : $(BINS)

tree32 : tree32.o fat32.o sdcard.o
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

can : can.o fat32.o sdcard.o
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

type32 : type32.o fat32.o sdcard.o
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

dir32 : dir32.o fat32.o sdcard.o
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

part32 : part32.o fat32.o sdcard.o
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o : %.cc
	g++ $(CXXFLAGS) -c $< -o $@

clean :
	rm -rf $(BINS) $(OBJS)
