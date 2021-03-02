VPATH=..
CXXFLAGS = -I..
BIN = sine440

$(BIN) : main.cc mcp4921.cc
	g++ $(CXXFLAGS) $^ -o $@ -lbcm2835

clean :
	rm -f $(BIN)
