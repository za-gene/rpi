VPATH=..
CXXFLAGS = -I..
BIN = levels

$(BIN) : main.cc mcp4921.cc
	g++ $(CXXFLAGS) $^ -o $@ -lbcm2835

clean :
	rm -f $(BIN)
