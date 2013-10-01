#define ACCEL_ADDR  0x68
int x = 0;
int y = 0;

void initAccel(){
  writeByte(ACCEL_ADDR, 0x6B, 0x80);  // PWR_MGT_1 send 0x80, reset device
  delay(5);
  writeByte(ACCEL_ADDR, 0x6B, 0x00);  // PWR_MGT_1 send 0x00, active mode
  delay(5);
}

void Accelloop(){
  printAccelVals();
  delay(5);
}

void printAccelVals(){
  Serial.println(readByte(ACCEL_ADDR, 0x75));
}