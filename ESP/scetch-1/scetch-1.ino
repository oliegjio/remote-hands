#include <SPI.h>
#include <Wire.h>

#include "quaternionFilters.h"
#include "MPU9250.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define port       7247
//m#define I2C_PORT 0x71
#define I2C_PORT   0x73

WiFiClient client;



#define TO_DEG 57.29577951308232087679815481410517033f
#define KF     0.1

float angle_ax = 0;
float angle_ay = 0;
float angle_az = 0;
float angle_gx = 0;
float angle_gy = 0;
float angle_gz = 0;



const char *ssid  = "mint-pc";
const char *password = "fFK5ieoY";
const String id = "1";

MPU9250 myIMU;

float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;

// Forward functions
String getValues();
void calibration();
void connect(const char *ssid, const char *password);
void script_1();
float clamp(float value, float min_value, float max_value);
void script_2();
void script_3();
void script_4();

float correct(float value) {
    return (( -2 < value && value < 2 ) ? 0 : value);
}

void setup() {
    Wire.begin();
    Serial.begin(38400);
 
    calibration();

    // setup Wifi
    //connect(ssid, password);
    Serial.println('>' + id);
}

void loop() {
    //script_1();
    //script_2();
    script_3();
}

///////////////////////////// ///////////////////////////////////
/////             Desctiption functions                    /////
////////////////////////////////////////////////////////////////

String getValues() {
    String data = String(myIMU.ax) + ' ';
    data += String(myIMU.ay) + ' ';
    data += String(myIMU.az) + ' ';

    data += '|';

    data += String(correct(gx)) + ' ';
    data += String(correct(gy)) + ' ';
    data += String(correct(gz)) + ' ';

    data += '|';

    data += String(myIMU.mx) + ' ';
    data += String(myIMU.my) + ' ';
    data += String(myIMU.mz);

    return data;
}

void calibration() {
    byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
    Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX);
    Serial.print(" I should be "); Serial.println(I2C_PORT, HEX);

    if (c == I2C_PORT) {
        Serial.println("MPU9250 is online...");
    
        // Start by performing self test and reporting values
        myIMU.MPU9250SelfTest(myIMU.SelfTest);
        Serial.print("x-axis self test: acceleration trim within : ");
        Serial.print(myIMU.SelfTest[0],1); Serial.println("% of factory value");
        Serial.print("y-axis self test: acceleration trim within : ");
        Serial.print(myIMU.SelfTest[1],1); Serial.println("% of factory value");
        Serial.print("z-axis self test: acceleration trim within : ");
        Serial.print(myIMU.SelfTest[2],1); Serial.println("% of factory value");
        Serial.print("x-axis self test: gyration trim within : ");
        Serial.print(myIMU.SelfTest[3],1); Serial.println("% of factory value");
        Serial.print("y-axis self test: gyration trim within : ");
        Serial.print(myIMU.SelfTest[4],1); Serial.println("% of factory value");
        Serial.print("z-axis self test: gyration trim within : ");
        Serial.print(myIMU.SelfTest[5],1); Serial.println("% of factory value");
    
        // Calibrate gyro and accelerometers, load biases in bias registers
        myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);
    
        myIMU.initMPU9250();
        Serial.println("MPU9250 initialized for active data mode....");
    
        byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
        Serial.print("AK8963 "); Serial.print("I AM "); Serial.print(d, HEX);
        Serial.print(" I should be "); Serial.println(0x48, HEX);
    
        myIMU.initAK8963(myIMU.magCalibration);
        // Initialize device for active mode read of magnetometer
        Serial.println("AK8963 initialized for active data mode....");
    
    
        Serial.print("X-Axis sensitivity adjustment value ");
        Serial.println(myIMU.magCalibration[0], 2);
        Serial.print("Y-Axis sensitivity adjustment value ");
        Serial.println(myIMU.magCalibration[1], 2);
        Serial.print("Z-Axis sensitivity adjustment value ");
        Serial.println(myIMU.magCalibration[2], 2);
    } else {
        Serial.print("Could not connect to MPU9250: 0x");
        Serial.println(c, HEX);
        while(1) ; // Loop forever if communication doesn't happen
    }
}

void connect(const char *ssid, const char *password) {
    WiFi.mode(WIFI_OFF);
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(250);
    }
    Serial.println();
    Serial.println("Connected to the WiFi");
    Serial.println(ssid);
    Serial.println(WiFi.localIP());

    while (!client.connect(IPAddress(10,42,0,1), port)) {
        Serial.println("connection failed");
        delay(2000);
    }

    Serial.println();
    Serial.println("Connected to the Server");
    delay(3000);
}

void script_1() {
    unsigned long time_now = millis();
    ax = 0; ay = 0; az = 0;
    gx = 0; gy = 0; gz = 0;
    mx = 0; my = 0; mz = 0;
    while (millis() - time_now < 1000) {
        if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {
          
            myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values
            myIMU.getAres();
        
            // Now we'll calculate the accleration value into actual g's
            // This depends on scale being set
            myIMU.ax = (float)myIMU.accelCount[0]*myIMU.aRes; // - accelBias[0];
            myIMU.ay = (float)myIMU.accelCount[1]*myIMU.aRes; // - accelBias[1];
            myIMU.az = (float)myIMU.accelCount[2]*myIMU.aRes; // - accelBias[2];
            
            myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values
            myIMU.getGres();
        
            // Calculate the gyro value into actual degrees per second
            // This depends on scale being set
            myIMU.gx = (float)myIMU.gyroCount[0]*myIMU.gRes;
            myIMU.gy = (float)myIMU.gyroCount[1]*myIMU.gRes;
            myIMU.gz = (float)myIMU.gyroCount[2]*myIMU.gRes;

            myIMU.readMagData(myIMU.magCount);
            myIMU.getMres();

            myIMU.mx = (float)myIMU.magCount[0]*myIMU.mRes;
            myIMU.my = (float)myIMU.magCount[1]*myIMU.mRes;
            myIMU.mz = (float)myIMU.magCount[2]*myIMU.mRes;

            gx += correct(myIMU.gx); gy += correct(myIMU.gy); gz += correct(myIMU.gz);
        }
    }
   
    Serial.println(getValues());
    //client.println(getValues());
}



float clamp(float value, float min_value, float max_value) {
    value = ( value > max_value ) ? max_value : value;
    value = ( value < min_value ) ? min_value : value;
    return value;
}

void script_2() {
    unsigned long time_now = millis();
    while (millis() - time_now < 1000) {
        if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {
          
            myIMU.readAccelData(myIMU.accelCount);
            myIMU.getAres();

            myIMU.ax = (float)myIMU.accelCount[0]*myIMU.aRes; // - accelBias[0];
            myIMU.ay = (float)myIMU.accelCount[1]*myIMU.aRes; // - accelBias[1];
            myIMU.az = (float)myIMU.accelCount[2]*myIMU.aRes; // - accelBias[2];
            
            myIMU.readGyroData(myIMU.gyroCount);
            myIMU.getGres();
  
            myIMU.gx = (float)myIMU.gyroCount[0]*myIMU.gRes;
            myIMU.gy = (float)myIMU.gyroCount[1]*myIMU.gRes;
            myIMU.gz = (float)myIMU.gyroCount[2]*myIMU.gRes;

            gx = myIMU.gx / 16.4;
            ay = clamp(myIMU.ay, -1.0, 1.0);

            angle_ax = 90 - TO_DEG * acos(ay);
            angle_gx = angle_gx + gx * 25 / 1000.0;
            angle_gx = angle_gx * (1 - KF) + angle_ax * KF;

            gy = myIMU.gy / 16.4;
            ax = clamp(myIMU.ax, -1.0, 1.0);
            
            angle_ay = 90 - TO_DEG * acos(ax);
            angle_gy = angle_gy + gy * 25 / 1000.0;
            angle_gy = angle_gy * (1 - KF) + angle_ay * KF;

            gz = myIMU.gz / 16.4;
            az = clamp(myIMU.az, -1.0, 1.0);

            angle_az = 90 - TO_DEG * acos(az);
            angle_gz = angle_gz + gz * 25 / 1000.0;
            angle_gz = angle_gz * (1 - KF) + angle_az * KF;
        }
    }
    Serial.println(angle_gx); Serial.print(' ');
    /*Serial.print(angle_gy); Serial.print(' ');*/
    /*Serial.print(angle_gz); Serial.println(' ');*/
}

void script_3() {
    float* q = new float[4];
    unsigned long time_now = millis();
    while (millis() - time_now < 25) {
        if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {
          
            myIMU.readAccelData(myIMU.accelCount);
            myIMU.getAres();

            myIMU.ax = (float)myIMU.accelCount[0]*myIMU.aRes; // - accelBias[0];
            myIMU.ay = (float)myIMU.accelCount[1]*myIMU.aRes; // - accelBias[1];
            myIMU.az = (float)myIMU.accelCount[2]*myIMU.aRes; // - accelBias[2];
            
            myIMU.readGyroData(myIMU.gyroCount);
            myIMU.getGres();
  
            myIMU.gx = (float)myIMU.gyroCount[0]*myIMU.gRes;
            myIMU.gy = (float)myIMU.gyroCount[1]*myIMU.gRes;
            myIMU.gz = (float)myIMU.gyroCount[2]*myIMU.gRes;

            myIMU.readMagData(myIMU.magCount);
            myIMU.getMres();

            myIMU.mx = (float)myIMU.magCount[0]*myIMU.mRes;
            myIMU.my = (float)myIMU.magCount[1]*myIMU.mRes;
            myIMU.mz = (float)myIMU.magCount[2]*myIMU.mRes;

            MadgwickQuaternionUpdate(myIMU.ax, myIMU.ay, myIMU.az, 
                                     myIMU.gx, myIMU.gy, myIMU.gz,
                                     myIMU.mx, myIMU.my, myIMU.mz, 0.025);
            q = getQ();
        }
    }
    Serial.print(q[0]); Serial.print(' ');
    Serial.print(q[1]); Serial.print(' ');
    Serial.print(q[2]); Serial.print(' ');
    Serial.print(q[3]); Serial.println(' ');
}

void script_4() {
    float* q = new float[4];
    unsigned long time_now = millis();
    while (millis() - time_now < 25) {
        if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {
          
            myIMU.readAccelData(myIMU.accelCount);
            myIMU.getAres();

            myIMU.ax = (float)myIMU.accelCount[0]*myIMU.aRes; // - accelBias[0];
            myIMU.ay = (float)myIMU.accelCount[1]*myIMU.aRes; // - accelBias[1];
            myIMU.az = (float)myIMU.accelCount[2]*myIMU.aRes; // - accelBias[2];
            
            myIMU.readGyroData(myIMU.gyroCount);
            myIMU.getGres();
  
            myIMU.gx = (float)myIMU.gyroCount[0]*myIMU.gRes;
            myIMU.gy = (float)myIMU.gyroCount[1]*myIMU.gRes;
            myIMU.gz = (float)myIMU.gyroCount[2]*myIMU.gRes;

            myIMU.readMagData(myIMU.magCount);
            myIMU.getMres();

            myIMU.mx = (float)myIMU.magCount[0]*myIMU.mRes;
            myIMU.my = (float)myIMU.magCount[1]*myIMU.mRes;
            myIMU.mz = (float)myIMU.magCount[2]*myIMU.mRes;

            MahonyQuaternionUpdate(myIMU.ax, myIMU.ay, myIMU.az, 
                                     myIMU.gx, myIMU.gy, myIMU.gz,
                                     myIMU.mx, myIMU.my, myIMU.mz, 0.025);
            q = getQ();
        }
    }
    Serial.print(q[0]); Serial.print(' ');
    Serial.print(q[1]); Serial.print(' ');
    Serial.print(q[2]); Serial.print(' ');
    Serial.print(q[3]); Serial.println(' ');
}
