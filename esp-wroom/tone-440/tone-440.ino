int freq = 440;
int channel = 0;
int resolution = 8;
  
void setup() {
  
  Serial.begin(115200);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(25, channel);
  ledcWriteTone(channel, freq);
  
}
  

void loop() {
  // put your main code here, to run repeatedly:

}
