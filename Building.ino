#include <Wire.h>
float ax,ay,az,gx1,gy1,gz1,yaw,pitch,roll,gx,gy,gz,temp;
float gyro_rate_x,gyro_rate_y,gyro_rate_z,gyroXangle,gyroYangle;
uint32_t timer;
void setup() {
  thietlapmpu();
  Serial.begin(12800);
    timer = micros();
  // put your setup code here, to run once:

}

void loop() {
  doc();
  roll=(180/3.141592)*atan2(ay,az);
  pitch=(180/3.141592)*atan(-ax/sqrt(ay*ay+az*az));
  gyroXangle = roll;
  gyroYangle = pitch;
  double dt = (double)(micros() -timer)/1000000;
  gyroXangle+=gyro_rate_x*dt;
  gyroYangle+=gyro_rate_y*dt;
  //Serial.print("roll : ");
  //Serial.print(roll);
  Serial.print("Gyro Angle : ");
  Serial.println(gyroXangle);
  //Serial.print(temp);
  //Serial.println(" *C");
  
  
  

}
void thietlapmpu()
{
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x08); //Setting the gyro to full scale +/- 500deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0x10); //Setting the accel to +/- 8g
  Wire.endTransmission(); 
}
void doc()
{
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,14); //Request Accel Registers (3B - 40)
  while(Wire.available() < 14);
  ax = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  ay = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  az = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  temp=Wire.read()<<8|Wire.read();
  gx1 = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gy1 = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gz1 = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  gx=gx1/65.5;
  gy=gy1/65.5;
  gz=gz1/65.5;
  temp=temp/340+36.53; //Chuyen sang do C
}

