#include "SparkFunMPU9250-DMP.h"

MPU9250_DMP imu;

void setup() 
{
  Serial.begin(9500);
  Serial3.begin(115200);

  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250.");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }
  
  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 10);
}

void loop() 
{
  if (imu.fifoAvailable())
  {
    if (imu.dmpUpdateFifo() == INV_SUCCESS)
    {
      imu.computeEulerAngles();
      printIMUData();
    }
  }
}

void printIMUData(void)
{  
  float q0 = imu.calcQuat(imu.qw);
  float q1 = imu.calcQuat(imu.qx);
  float q2 = imu.calcQuat(imu.qy);
  float q3 = imu.calcQuat(imu.qz);

  Serial.println("Q: " + String(q0, 4) + ", " + String(q1, 4) + ", " + String(q2, 4) + ", " + String(q3, 4));
}
