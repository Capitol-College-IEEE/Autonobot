#include <AFMotor.h>
#include <XBee.h>

AF_DCMotor left(1, MOTOR12_1KHZ);
AF_DCMotor right(2, MOTOR12_1KHZ);

unsigned char ltb[] = {0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,4,4,4,4,5,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,11,11,11,11,12,12,12,13,13,13,14,14,14,15,15,15,16,16,16,17,17,17,18,18,18,19,19,20,20,20,21,21,22,22,23,23,23,24,24,25,25,26,26,27,27,28,28,29,29,30,30,31,31,32,32,33,33,34,34,35,36,36,37,37,38,39,39,40,40,41,42,42,43,44,44,45,46,46,47,48,49,49,50,51,52,52,53,54,55,56,56,57,58,59,60,61,62,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,82,83,84,85,86,87,89,90,91,92,93,95,96,97,99,100,101,103,104,105,107,108,110,111,113,114,116,117,119,120,122,123,125,127,128,130,132,134,135,137,139,141,142,144,146,148,150,152,154,156,158,160,162,164,166,168,170,173,175,177,179,182,184,186,189,191,193,196,198,201,203,206,209,211,214,216,219,222,225,228,230,233,236,239,242,245,248,251,255};

XBee xbee = XBee();

Rx16IoSampleResponse io16 = Rx16IoSampleResponse();
Rx64IoSampleResponse io64 = Rx64IoSampleResponse();

void setup() {
  left.setSpeed(255);
  right.setSpeed(255);
  
  left.run(RELEASE);
  right.run(RELEASE);
  
  Serial.begin(9600);
  
  xbee.setSerial(Serial);
}

void loop() {
  XBeeResponse response;
  
  xbee.readPacket();
  
  response = xbee.getResponse();
  if (response.isAvailable()) {
    if (response.getApiId() == RX_16_IO_RESPONSE) {
      response.getRx16IoSampleResponse(io16);
      setMotors(1024 - io16.getAnalog(0,0), io16.getAnalog(2,0));
    } else if (response.getApiId() == RX_64_IO_RESPONSE) {
      response.getRx64IoSampleResponse(io64);
      setMotors(1024 - io64.getAnalog(0,0), io64.getAnalog(2,0));
    } else {
//      Serial.print("API Packet: ");
//      Serial.println(response.getApiId(), HEX);
    }
  }
}

void setMotors(int a, int b) {
  int r = a - MID;
  int l = b - MID;
  
  r /= 2;
  l /= 2;
  
  int rd = r < 0;
  int ld = l < 0;
  
  if (rd)
    r *= -1;
  
  if (ld)
    l *= -1;
  
  right.setSpeed(r);
  left.setSpeed(l);
  
  if (r < 10)
    right.run(RELEASE);
  else if (rd)
    right.run(BACKWARD);
  else
    right.run(FORWARD);
  
  if (l < 10)
    left.run(RELEASE);
  else if (ld)
    left.run(BACKWARD);
  else
    left.run(FORWARD);
}
