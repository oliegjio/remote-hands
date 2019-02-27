
#ifndef MX106_h
#define MX106_h

// EEPROM AREA  ///////////////////////////////////////////////////////////
#define MX_MODEL_NUMBER_L           0
#define MX_MODEL_NUMBER_H           1
#define MX_VERSION                  2
#define MX_ID                       3
#define MX_BAUD_RATE                4
#define MX_RETURN_DELAY_TIME        5
#define MX_CW_ANGLE_LIMIT_L         6
#define MX_CW_ANGLE_LIMIT_H         7
#define MX_CCW_ANGLE_LIMIT_L        8
#define MX_CCW_ANGLE_LIMIT_H        9
#define MX_SYSTEM_DATA2             10
#define MX_LIMIT_TEMPERATURE        11
#define MX_DOWN_LIMIT_VOLTAGE       12
#define MX_UP_LIMIT_VOLTAGE         13
#define MX_MAX_TORQUE_L             14
#define MX_MAX_TORQUE_H             15
#define MX_RETURN_LEVEL             16
#define MX_ALARM_LED                17
#define MX_ALARM_SHUTDOWN           18
#define MX_MULTI_TURN_L             20
#define MX_MULTI_TURN_H             21
#define MX_RESOLUTION_DRIVER        22

// RAM AREA  //////////////////////////////////////////////////////////////
#define MX_TORQUE_ENABLE            24
#define MX_LED                      25
#define MX_D_GAIN                   26
#define MX_I_GAIN                   27
#define MX_P_GAIN                   28
#define MX_GOAL_POSITION_L          30
#define MX_GOAL_POSITION_H          31
#define MX_GOAL_SPEED_L             32
#define MX_GOAL_SPEED_H             33
#define MX_TORQUE_LIMIT_L           34
#define MX_TORQUE_LIMIT_H           35
#define MX_PRESENT_POSITION_L       36
#define MX_PRESENT_POSITION_H       37
#define MX_PRESENT_SPEED_L          38
#define MX_PRESENT_SPEED_H          39
#define MX_PRESENT_LOAD_L           40
#define MX_PRESENT_LOAD_H           41
#define MX_PRESENT_VOLTAGE          42
#define MX_PRESENT_TEMPERATURE      43
#define MX_REGISTERED_INSTRUCTION   44
#define MX_PAUSE_TIME               45
#define MX_MOVING                   46
#define MX_LOCK                     47
#define MX_PUNCH_L                  48
#define MX_PUNCH_H                  49
#define MX_CURRENT_L                68
#define MX_CURRENT_H                69
#define MX_TORQUE_CONTROL_MODE      70
#define MX_GOAL_TORQUE_L            71
#define MX_GOAL_TORQUE_H            72
#define MX_GOAL_ACCELERATION        73

// Status Return Levels ///////////////////////////////////////////////////
#define MX_RETURN_NONE              0
#define MX_RETURN_READ              1
#define MX_RETURN_ALL               2

// Instruction Set ////////////////////////////////////////////////////////
#define MX_PING                     1
#define MX_READ_DATA                2
#define MX_WRITE_DATA               3
#define MX_REG_WRITE                4
#define MX_ACTION                   5
#define MX_RESET                    6
#define MX_SYNC_WRITE               131

// Specials ///////////////////////////////////////////////////////////////
#define OFF                         0
#define ON                          1
#define LEFT						            0
#define RIGHT                       1
#define MX_BYTE_READ                1
#define MX_BYTE_READ_POS            2
#define MX_RESET_LENGTH				      2
#define MX_ACTION_LENGTH			      2
#define MX_ID_LENGTH                4
#define MX_LR_LENGTH                4
#define MX_SRL_LENGTH               4
#define MX_RDT_LENGTH               4
#define MX_LEDALARM_LENGTH          4
#define MX_SALARM_LENGTH            4
#define MX_TL_LENGTH                4
#define MX_VL_LENGTH                6
#define MX_CM_LENGTH                6
#define MX_CS_LENGTH                6
#define MX_CCW_CW_LENGTH            8
#define MX_BD_LENGTH                4
#define MX_TEM_LENGTH               4
#define MX_MOVING_LENGTH            4
#define MX_RWS_LENGTH               4
#define MX_VOLT_LENGTH              4
#define MX_LED_LENGTH               4
#define MX_TORQUE_LENGTH            4
#define MX_POS_LENGTH               4
#define MX_GOAL_LENGTH              5
#define MX_MT_LENGTH                5
#define MX_PUNCH_LENGTH             5
#define MX_SPEED_LENGTH             5
#define MX_GOAL_SP_LENGTH           7
#define MX_ACTION_CHECKSUM			250
#define BROADCAST_ID                254
#define MX_START                    255
#define MX_CCW_AL_L                 255
#define MX_CCW_AL_H                 3
#define TIME_OUT                    10
#define TX_MODE                     1
#define RX_MODE                     0
#define LOCK                        1

#include <inttypes.h>

class MX106 {
  private:

    unsigned char Checksum;
    unsigned char Direction_Pin;
    unsigned char Time_Counter;
    unsigned char Incoming_Byte;
    unsigned char Position_High_Byte;
    unsigned char Position_Low_Byte;
    unsigned char Speed_High_Byte;
    unsigned char Speed_Low_Byte;
    unsigned char Load_High_Byte;
    unsigned char Load_Low_Byte;

    int Moving_Byte;
    int RWS_Byte;
    int Speed_Long_Byte;
    int Load_Long_Byte;
    int Position_Long_Byte;
    int Temperature_Byte;
    int Error_Byte;

    int returned_Byte;

    int read_error(void);

    HardwareSerial *varSerial;

  public:
    void begin(long baud, unsigned char directionPin, HardwareSerial *srl);
    void end(void);

    int ping(unsigned char ID);

    int setID(unsigned char ID, unsigned char newID);
    int setBD(unsigned char ID, long baud);

    int move(unsigned char ID, int Position);
    int moveSpeed(unsigned char ID, int Position, int Speed);
    int setEndless(unsigned char ID, bool Status);
    int turn(unsigned char ID, bool SIDE, int Speed);
    int moveRW(unsigned char ID, int Position);
    int moveSpeedRW(unsigned char ID, int Position, int Speed);

    void action(void);
    int setPunch(unsigned char ID, int Punch);

    int moving(unsigned char ID);
    int lockRegister(unsigned char ID);
    int RWStatus(unsigned char ID);

    int readTemperature(unsigned char ID);
    int readPosition(unsigned char ID);

    int sendMXPacket(unsigned char *packet, unsigned int length);
    void sendMXPacketNoError(unsigned char *packet, unsigned int length);

    int readRegister(unsigned char ID, unsigned char reg, unsigned char reg_len);
};

extern MX106 mx106;

#endif /* MX106 */
