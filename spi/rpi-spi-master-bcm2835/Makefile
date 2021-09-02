BIN = app

$(BIN) : rpi-spi-master-bcm2835.c
	gcc $< -o $@ -lbcm2835

clean:
	rm -f $(BIN)
