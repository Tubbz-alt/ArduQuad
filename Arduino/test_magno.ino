#include <Wire.h>
#define MAG_ADDR  0x0E
int axis[3], min[3], max[3];

//minX: -1792    maxX: -513    minY: -436    maxY: 716    minZ: -1360    maxZ: -306
//x = 1279 / 2 = 639.5
//y = 1152 / 2 = 576
//z = 1054 / 2 = 527

//x = -1152.5
//y = 140
//z = -833

void setup(){
  Wire.begin();
  Serial.begin(9600);
  initMag();
}

void loop(){
  updateAxes();
  setMaxes();
  printVals();
  //printMaxes();
}

void initMag(){
  writeByte(0x11, 0x80);  // cntrl register2 send 0x80, enable auto resets
  writeByte(0x10, 0x01);  // cntrl register1 send 0x01, active mode
  updateAxes();
  for(byte i = 0; i < 3; i++){
    min[i] = axis[i];
    max[i] = axis[i];
  }
  writeOffset(0X09, -1152);  //X offset
  writeOffset(0X0B, 140);  //Y offset
  writeOffset(0X0D, -833);  //Z offset
}

void setMaxes(){
  for(byte i = 0; i < 3; i++)
    if(axis[i] < min[i])
      min[i] = axis[i];
    else if(axis[i] > max[i])
      max[i] = axis[i];
}

void printVals(void){
  Serial.print("x ");
  Serial.print(180 * atan2(axis[1], axis[0]) / PI);
  Serial.print("    y ");
  Serial.print(axis[1]);
  Serial.print("    z ");
  Serial.println(axis[2]);
}

void printMaxes(){
  Serial.print("minX: ");
  Serial.print(min[0]);
  Serial.print("    minY: ");
  Serial.print(min[1]);
  Serial.print("    minZ: ");
  Serial.print(min[2]);
  Serial.print("    maxX: ");
  Serial.print(max[0]);
  Serial.print("    maxY: ");
  Serial.print(max[1]);
  Serial.print("    maxZ: ");
  Serial.println(max[2]);
}

int calcPercent(int val, int maxVal, int minVal){
  return (int)(100*(float)(val-minVal)/(maxVal-minVal));
}

void updateAxes(){
  axis[0] = readAxis(0x01);
  axis[1] = readAxis(0x03);
  axis[2] = readAxis(0x05);
}

int readAxis(byte MSB_REG){
  return (readByte(MSB_REG + 1)|(readByte(MSB_REG) << 8));  //concatenate the MSB and LSB to the total value
}

void writeOffset(byte MSB_REG, int val){
  writeByte(MSB_REG, highByte(val));     //MSB
  writeByte(MSB_REG + 1, lowByte(val));  //LSB
}

byte readByte(byte reg){
  byte bite;
  Wire.beginTransmission(MAG_ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.beginTransmission(MAG_ADDR);
  Wire.requestFrom(MAG_ADDR, 1);
  while(Wire.available())
    bite = Wire.read();
  return bite;
}

void writeByte(byte reg, byte val){
  Wire.beginTransmission(MAG_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}