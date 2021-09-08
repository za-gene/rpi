VPATH=..
CXXFLAGS = -I..
BIN = app

$(BIN) : main.cc mcp4921.cc
	g++ $(CXXFLAGS) $^ -o $@ -lbcm2835 -lm

clean :
	rm -f $(BIN)
