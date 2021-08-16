#include <SPIFFS.h> // ESP file system


#define FALSE  0
#define TRUE  1


prim P_format() { Push = SPIFFS.format(); }

prim P_fallot()
{
	hptr += sizeof(File);
}

prim P_fopen() 
{ 
	Sl(3); 
	Hpc(S1);
	Hpc(S2);
	File* f = (File*) S0;

	//char* filename = S1;
	//char* mode = S0;
	Serial.println("About the open");
	*f = SPIFFS.open((char*) S2, (char*) S1); 
	Serial.println("Phew");
	Pop;
	Pop;
	S0 = (stackitem) &f;
}

prim P_favail()
{
	Sl(1);
	File* f = (File*) S0;
	S0 = f->available();
}

prim P_freadc()
{
	Sl(1);
	//spiffs_t* sp;
	//spiffs_check(sp);

	File* f = (File*) S0;
	S0 = f->read();
}

prim P_fwritec()
{
	Sl(2);
	File* f = (File*) S0;
	Pop;
	S0 = f->write(S1);
}

prim P_fclose() 
{ 
	Sl(1); 
	File* f = (File*) S0; 
	f->close();
	Pop;
	//Push = f.close(); 
}

void writeln(char *str) {
	Serial.print(str);
	P_cr();
}

char t[132];

int readln()
{
	t[0] = 0;
	int n = 0;
	for (;;) {
		if (Serial.available() > 0) {
			int b = Serial.read();
			if(b == '\r') {Serial.print('\r'); b = '\n'; }
			t[n++] = b;
			if (b == '\n') goto eoi;
		}
	}
eoi:
	t[n] = 0;
	return n - 1;
}


void setup() {
	Serial.begin(115200);
	//Serial.println("atlast started 2");
	int fname = FALSE, defmode = FALSE;
	//FILE *ifp;

	Serial.print("Mounting SPIFFS:");
	if(SPIFFS.begin())
		Serial.print("success\n");
	else {
		Serial.print("failure\n");
		Serial.println("Have you formatted it?");
	}

	while (TRUE) {
		if (readln() != 0) {
			V atl_eval(t);      
		}
		V writeln("\n  ok");
	}
}


void loop() {

}

void some_words()
{
	{"0FALLOT", P_fallot},
	{"0FORMAT", P_format},
	{"0FOPEN", P_fopen},
	{"0FCLOSE", P_fclose},
	{"0FWRITEC", P_fwritec},
	{"0FREADC", P_freadc},
	{"0FAVAIL", P_favail},

	// mcarter added for Arduino
	{"0DACW", P_dacw},
	{"0PINM", P_pinm},
	{"0DIGW", P_digw},
	{"0DIGR", P_digr},
	{"0DELAY-MS", P_delay_ms},
	{"0KEY?", P_keyq},
	{"0TESTFS", P_testfs},
	{"0USECS", P_usecs},
	{"0MSECS", P_msecs},

}

// https://github.com/espressif/arduino-esp32/blob/master/libraries/SPIFFS/examples/SPIFFS_Test/SPIFFS_Test.ino
prim P_testfs() 
{
	writeln("Testing the file system");
	int res;

	writeln("Formatting. Make take awhile");
	res = SPIFFS.format();
	if(!res) {
		writeln("Error formatting SPIFFS");
	}

	res = SPIFFS.begin();
	if(!res) {
		writeln("Error mounting SPIFFS");
		return;
	}

	File file = SPIFFS.open("/hello.txt", "w");
	if(!file) {
		writeln("Error opening file for writing");
		return;
	}

	int nbytes = file.print("hello world");
	if(nbytes == 0) {
		writeln("Error writing to file");
		return;
	}

	file.close();

	file = SPIFFS.open("/hello.txt", "r");
	if(!file) {
		writeln("Error opening file for reading");
		return;
	}

	writeln("Contents of file are:");
	while(file.available()) {
		Serial.write(file.read());
	}

	file.close();

	writeln("\nFinished testfs");

}
prim P_pinm() {
	Sl(2);
	pinMode(S1, S0);
	Pop2;
}
prim P_digw() {
	Sl(2);
	digitalWrite(S1, S0);
	Pop2;
}
prim P_digr() {
	Sl(1);
	stackitem s = digitalRead(S0);
	S0 = s;
}
prim P_delay_ms() {
	Sl(1);
	delay(S0);
	Pop;
}

prim P_keyq() {
	Push = Serial.available();
}


prim P_usecs() {
	Push = micros();
}

prim P_msecs() {
	Push = millis();
}
prim P_dacw()
{
	Sl(2);
	dacWrite(S0, S1);
	Pop;
	Pop;
}

