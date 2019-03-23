#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "MX106R.h"

// Macros /////////////////////////////////////////////////////////////////////

#define sendData(packet, length)  	(varSerial->write(packet, length))    	// Write Over Serial
#define flush()						(varSerial->flush())					// Wait until buffer empty
#define availableData() 			(varSerial->available())    			// Check Serial Data Available
#define readData()      			(varSerial->read())         			// Read Serial Data
#define peekData()      			(varSerial->peek())         			// Peek Serial Data
#define beginCom(args)  			(varSerial->begin(args))   				// Begin Serial Comunication
#define endCom()        			(varSerial->end())          			// End Serial Comunication

#define setDPin(DirPin,Mode)   		(pinMode(DirPin,Mode))       			// Select the Switch to TX/RX Mode Pin
#define switchCom(DirPin,Mode) 		(digitalWrite(DirPin,Mode))  			// Switch to TX/RX Mode

#define delayus(args) 				(delayMicroseconds(args))  				// Delay Microseconds

// Private Methods ////////////////////////////////////////////////////////////

int MX106::read_error(void)
{
  Time_Counter = 0;
  while ((availableData() < 5) & (Time_Counter < TIME_OUT)) // Wait for Data
  {
    Time_Counter++;
    delayus(1000);
  }

  while (availableData() > 0)
  {
    Incoming_Byte = readData();
    if ( (Incoming_Byte == 255) & (peekData() == 255) )
    {
      readData();                                    // Start Bytes
      readData();                                    // MX-12 ID
      readData();                                    // Length
      Error_Byte = readData();                       // Error
      return (Error_Byte);
    }
  }
  return (-1);											 // No MX Response
}

// Public Methods /////////////////////////////////////////////////////////////

void MX106::begin(long baud, unsigned char directionPin, HardwareSerial *srl, unsigned int count_servo, int *angl)
{
  varSerial = srl;
  Direction_Pin = directionPin;
  setDPin(Direction_Pin, OUTPUT);
  beginCom(baud);
  this->count_servo = count_servo;
  for (unsigned i = 0; i < count_servo; ++i)
    zero_positions[i] = angl[i];
}

void MX106::end()
{
  endCom();
}

int MX106::ping(unsigned char ID)
{
  const unsigned int length = 6;
  unsigned char packet[length];

  Checksum = (~(ID + MX_READ_DATA + MX_PING)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = 2;
  packet[3] = 2;
  packet[4] = 1;
  packet[5] = Checksum;

  return (sendMXPacket(packet, length));
}

int MX106::torqueStatus(unsigned char ID, bool Status) {
  const unsigned int length = 8;
  unsigned char packet[length];

  Checksum = (~(ID + MX_TORQUE_LENGTH + MX_WRITE_DATA + MX_TORQUE_ENABLE + Status)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = MX_TORQUE_LENGTH;
  packet[4] = MX_WRITE_DATA;
  packet[5] = MX_TORQUE_ENABLE;
  packet[6] = Status;
  packet[7] = Checksum;
    
  return (sendMXPacket(packet, length));
}

int MX106::setID(unsigned char ID, unsigned char newID)
{
  const unsigned int length = 8;
  unsigned char packet[length];

  Checksum = (~(ID + MX_ID_LENGTH + MX_WRITE_DATA + MX_ID + newID)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = MX_ID_LENGTH;
  packet[4] = MX_WRITE_DATA;
  packet[5] = MX_ID;
  packet[6] = newID;
  packet[7] = Checksum;

  return (sendMXPacket(packet, length));
}

int MX106::setBD(unsigned char ID, long baud)
{
  const unsigned int length = 8;
  unsigned char packet[length];

  unsigned char Baud_Rate = (2000000 / baud) - 1;
  Checksum = (~(ID + MX_BD_LENGTH + MX_WRITE_DATA + MX_BAUD_RATE + Baud_Rate)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = MX_BD_LENGTH;
  packet[4] = MX_WRITE_DATA;
  packet[5] = MX_BAUD_RATE;
  packet[6] = Baud_Rate;
  packet[7] = Checksum;

  return (sendMXPacket(packet, length));
}

int MX106::setMaxTorque(unsigned char ID, int MaxTorque)
{
    char MaxTorque_H,MaxTorque_L;
    MaxTorque_H = MaxTorque >> 8;           // 16 bits - 2 x 8 bits variables
    MaxTorque_L = MaxTorque;

	const unsigned int length = 9;
	unsigned char packet[length];

	Checksum = (~(ID + MX_MT_LENGTH + MX_WRITE_DATA + MX_MAX_TORQUE_L + MaxTorque_L + MaxTorque_H)) & 0xFF;

	packet[0] = MX_START;
	packet[1] = MX_START;
	packet[2] = ID;
	packet[3] = MX_MT_LENGTH;
	packet[4] = MX_WRITE_DATA;
	packet[5] = MX_MAX_TORQUE_L;
	packet[6] = MaxTorque_L;
	packet[7] = MaxTorque_H;
	packet[8] = Checksum;

	return (sendMXPacket(packet, length));
}

int MX106::move(unsigned char ID, int Position)
{
  char Position_H, Position_L;
  Position_H = Position >> 8;           // 16 bits - 2 x 8 bits variables
  Position_L = Position;

  const unsigned int length = 9;
  unsigned char packet[length];

  Checksum = (~(ID + MX_GOAL_LENGTH + MX_WRITE_DATA + MX_GOAL_POSITION_L + Position_L + Position_H)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = MX_GOAL_LENGTH;
  packet[4] = MX_WRITE_DATA;
  packet[5] = MX_GOAL_POSITION_L;
  packet[6] = Position_L;
  packet[7] = Position_H;
  packet[8] = Checksum;

  return (sendMXPacket(packet, length));
}

int MX106::moveSpeed(unsigned char ID, int Position, int Speed)
{
  char Position_H, Position_L, Speed_H, Speed_L;
  Position_H = Position >> 8;
  Position_L = Position;                // 16 bits - 2 x 8 bits variables
  Speed_H = Speed >> 8;
  Speed_L = Speed;                      // 16 bits - 2 x 8 bits variables

  const unsigned int length = 11;
  unsigned char packet[length];

  Checksum = (~(ID + MX_GOAL_SP_LENGTH + MX_WRITE_DATA + MX_GOAL_POSITION_L + Position_L + Position_H + Speed_L + Speed_H)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = MX_GOAL_SP_LENGTH;
  packet[4] = MX_WRITE_DATA;
  packet[5] = MX_GOAL_POSITION_L;
  packet[6] = Position_L;
  packet[7] = Position_H;
  packet[8] = Speed_L;
  packet[9] = Speed_H;
  packet[10] = Checksum;

  return (sendMXPacket(packet, length));
}

int MX106::setEndless(unsigned char ID, bool Status)
{
  if ( Status )
  {
    const unsigned int length = 9;
    unsigned char packet[length];

    Checksum = (~(ID + MX_GOAL_LENGTH + MX_WRITE_DATA + MX_CCW_ANGLE_LIMIT_L)) & 0xFF;

    packet[0] = MX_START;
    packet[1] = MX_START;
    packet[2] = ID;
    packet[3] = MX_GOAL_LENGTH;
    packet[4] = MX_WRITE_DATA;
    packet[5] = MX_CCW_ANGLE_LIMIT_L;
    packet[6] = 0; 						// full rotation
    packet[7] = 0;						// full rotation
    packet[8] = Checksum;

    return (sendMXPacket(packet, length));
  }
  else
  {
    turn(ID, 0, 0);

    const unsigned int length = 9;
    unsigned char packet[length];

    Checksum = (~(ID + MX_GOAL_LENGTH + MX_WRITE_DATA + MX_CCW_ANGLE_LIMIT_L + MX_CCW_AL_L + MX_CCW_AL_H)) & 0xFF;

    packet[0] = MX_START;
    packet[1] = MX_START;
    packet[2] = ID;
    packet[3] = MX_GOAL_LENGTH;
    packet[4] = MX_WRITE_DATA;
    packet[5] = MX_CCW_ANGLE_LIMIT_L;
    packet[6] = MX_CCW_AL_L;
    packet[7] = MX_CCW_AL_H;
    packet[8] = Checksum;

    return (sendMXPacket(packet, length));
  }
}

int MX106::turn(unsigned char ID, bool SIDE, int Speed)
{
  if (SIDE == LEFT)
  {
    char Speed_H, Speed_L;
    Speed_H = Speed >> 8;
    Speed_L = Speed;                     // 16 bits - 2 x 8 bits variables

    const unsigned int length = 9;
    unsigned char packet[length];

    Checksum = (~(ID + MX_SPEED_LENGTH + MX_WRITE_DATA + MX_GOAL_SPEED_L + Speed_L + Speed_H)) & 0xFF;

    packet[0] = MX_START;
    packet[1] = MX_START;
    packet[2] = ID;
    packet[3] = MX_SPEED_LENGTH;
    packet[4] = MX_WRITE_DATA;
    packet[5] = MX_GOAL_SPEED_L;
    packet[6] = Speed_L;
    packet[7] = Speed_H;
    packet[8] = Checksum;

    return (sendMXPacket(packet, length));
  }

  else
  {
    char Speed_H, Speed_L;
    Speed_H = (Speed >> 8) + 4;
    Speed_L = Speed;                     // 16 bits - 2 x 8 bits variables

    const unsigned int length = 9;
    unsigned char packet[length];

    Checksum = (~(ID + MX_SPEED_LENGTH + MX_WRITE_DATA + MX_GOAL_SPEED_L + Speed_L + Speed_H)) & 0xFF;

    packet[0] = MX_START;
    packet[1] = MX_START;
    packet[2] = ID;
    packet[3] = MX_SPEED_LENGTH;
    packet[4] = MX_WRITE_DATA;
    packet[5] = MX_GOAL_SPEED_L;
    packet[6] = Speed_L;
    packet[7] = Speed_H;
    packet[8] = Checksum;

    return (sendMXPacket(packet, length));
  }
}

int MX106::moveRW(unsigned char ID, int Position)
{
  char Position_H, Position_L;
  Position_H = Position >> 8;           // 16 bits - 2 x 8 bits variables
  Position_L = Position;

  const unsigned int length = 9;
  unsigned char packet[length];

  Checksum = (~(ID + MX_GOAL_LENGTH + MX_REG_WRITE + MX_GOAL_POSITION_L + Position_L + Position_H)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = MX_GOAL_LENGTH;
  packet[4] = MX_REG_WRITE;
  packet[5] = MX_GOAL_POSITION_L;
  packet[6] = Position_L;
  packet[7] = Position_H;
  packet[8] = Checksum;

  return (sendMXPacket(packet, length));
}

int MX106::moveSpeedRW(unsigned char ID, int Position, int Speed)
{
  char Position_H, Position_L, Speed_H, Speed_L;
  Position_H = Position >> 8;
  Position_L = Position;                // 16 bits - 2 x 8 bits variables
  Speed_H = Speed >> 8;
  Speed_L = Speed;                      // 16 bits - 2 x 8 bits variables

  const unsigned int length = 11;
  unsigned char packet[length];

  Checksum = (~(ID + MX_GOAL_SP_LENGTH + MX_REG_WRITE + MX_GOAL_POSITION_L + Position_L + Position_H + Speed_L + Speed_H)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = MX_GOAL_SP_LENGTH;
  packet[4] = MX_REG_WRITE;
  packet[5] = MX_GOAL_POSITION_L;
  packet[6] = Position_L;
  packet[7] = Position_H;
  packet[8] = Speed_L;
  packet[9] = Speed_H;
  packet[10] = Checksum;

  return (sendMXPacket(packet, length));
}

void MX106::turnAngle(unsigned char ID, double Angle) {
  Angle = (Angle > 90) ? 90 : Angle;
  Angle = (Angle < -90) ? -90 : Angle;
  Angle = map(Angle, -90, 90, zero_positions[ID] - 1020, zero_positions[ID] + 1020);
  move(ID, Angle);
}

void MX106::turnAngleSpeed(unsigned char ID, double Angle, int Speed) {
  Angle = (Angle > 90) ? 90 : Angle;
  Angle = (Angle < -90) ? -90 : Angle;
  Angle = map(Angle, -90, 90, zero_positions[ID] - 1020, zero_positions[ID] + 1020);
  moveSpeed(ID, Angle, Speed);
}

void MX106::action()
{
  const unsigned int length = 6;
  unsigned char packet[length];

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = BROADCAST_ID;
  packet[3] = MX_ACTION_LENGTH;
  packet[4] = MX_ACTION;
  packet[5] = MX_ACTION_CHECKSUM;

  sendMXPacket(packet, length);
}

int MX106::readTemperature(unsigned char ID)
{
  const unsigned int length = 8;
  unsigned char packet[length];

  Checksum = (~(ID + MX_TEM_LENGTH + MX_READ_DATA + MX_PRESENT_TEMPERATURE + MX_BYTE_READ)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = MX_TEM_LENGTH;
  packet[4] = MX_READ_DATA;
  packet[5] = MX_PRESENT_TEMPERATURE;
  packet[6] = MX_BYTE_READ;
  packet[7] = Checksum;

  sendMXPacketNoError(packet, length);

  Temperature_Byte = -1;
  Time_Counter = 0;
  while ((availableData() < 6) & (Time_Counter < TIME_OUT))
  {
    Time_Counter++;
    delayus(1000);
  }

  while (availableData() > 0)
  {
    Incoming_Byte = readData();
    if ( (Incoming_Byte == 255) & (peekData() == 255) )
    {
      readData();                            // Start Bytes
      readData();                            // MX-12 ID
      readData();                            // Length
      if ( (Error_Byte = readData()) != 0 )  // Error
        return (Error_Byte * (-1));
      Temperature_Byte = readData();         // Temperature
    }
  }
  return (Temperature_Byte);               // Returns the read temperature
}

int MX106::readPosition(unsigned char ID)
{
  const unsigned int length = 8;
  unsigned char packet[length];

  Checksum = (~(ID + MX_POS_LENGTH + MX_READ_DATA + MX_PRESENT_POSITION_L + MX_BYTE_READ_POS)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = MX_POS_LENGTH;
  packet[4] = MX_READ_DATA;
  packet[5] = MX_PRESENT_POSITION_L;
  packet[6] = MX_BYTE_READ_POS;
  packet[7] = Checksum;

  sendMXPacketNoError(packet, length);

  Position_Long_Byte = -1;
  Time_Counter = 0;
  while ((availableData() < 7) & (Time_Counter < TIME_OUT))
  {
    Time_Counter++;
    delayus(1000);
  }

  while (availableData() > 0)
  {
    Incoming_Byte = readData();
    if ( (Incoming_Byte == 255) & (peekData() == 255) )
    {
      readData();                            // Start Bytes
      readData();                            // MX-12 ID
      readData();                            // Length
      if ( (Error_Byte = readData()) != 0 )  // Error
        return (Error_Byte * (-1));

      Position_Low_Byte = readData();            // Position Bytes
      Position_High_Byte = readData();
      Position_Long_Byte = Position_High_Byte << 8;
      Position_Long_Byte = Position_Long_Byte + Position_Low_Byte;
    }
  }
  return (Position_Long_Byte);     // Returns the read position
}

int MX106::setPunch(unsigned char ID, int Punch)
{
  char Punch_H, Punch_L;
  Punch_H = Punch >> 8;           // 16 bits - 2 x 8 bits variables
  Punch_L = Punch;

  const unsigned int length = 9;
  unsigned char packet[length];

  Checksum = (~(ID + MX_PUNCH_LENGTH + MX_WRITE_DATA + MX_PUNCH_L + Punch_L + Punch_H)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = MX_PUNCH_LENGTH;
  packet[4] = MX_WRITE_DATA;
  packet[5] = MX_PUNCH_L;
  packet[6] = Punch_L;
  packet[7] = Punch_H;
  packet[8] = Checksum;

  return (sendMXPacket(packet, length));
}

int MX106::moving(unsigned char ID)
{
  const unsigned int length = 8;
  unsigned char packet[length];

  Checksum = (~(ID + MX_MOVING_LENGTH + MX_READ_DATA + MX_MOVING + MX_BYTE_READ)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = MX_MOVING_LENGTH;
  packet[4] = MX_READ_DATA;
  packet[5] = MX_MOVING;
  packet[6] = MX_BYTE_READ;
  packet[7] = Checksum;

  sendMXPacketNoError(packet, length);

  Moving_Byte = -1;
  Time_Counter = 0;
  while ((availableData() < 6) & (Time_Counter < TIME_OUT))
  {
    Time_Counter++;
    delayus(1000);
  }

  while (availableData() > 0)
  {
    Incoming_Byte = readData();
    if ( (Incoming_Byte == 255) & (peekData() == 255) )
    {
      readData();                           	// Start Bytes
      readData();                           	// MX-12 ID
      readData();                           	// Length
      if ( (Error_Byte = readData()) != 0 )   	// Error
        return (Error_Byte * (-1));
      Moving_Byte = readData();         		// Moving
    }
  }
  return (Moving_Byte);              				// Returns the read Moving
}

int MX106::lockRegister(unsigned char ID)
{
  const unsigned int length = 8;
  unsigned char packet[length];

  Checksum = (~(ID + MX_LR_LENGTH + MX_WRITE_DATA + MX_LOCK + LOCK)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = MX_LR_LENGTH;
  packet[4] = MX_WRITE_DATA;
  packet[5] = MX_LOCK;
  packet[6] = LOCK;
  packet[7] = Checksum;

  return (sendMXPacket(packet, length));
}

int MX106::RWStatus(unsigned char ID)
{
  const unsigned int length = 8;
  unsigned char packet[length];

  Checksum = (~(ID + MX_RWS_LENGTH + MX_READ_DATA + MX_REGISTERED_INSTRUCTION + MX_BYTE_READ)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = MX_RWS_LENGTH;
  packet[4] = MX_READ_DATA;
  packet[5] = MX_REGISTERED_INSTRUCTION;
  packet[6] = MX_BYTE_READ;
  packet[7] = Checksum;

  sendMXPacketNoError(packet, length);

  RWS_Byte = -1;
  Time_Counter = 0;
  while ((availableData() < 6) & (Time_Counter < TIME_OUT))
  {
    Time_Counter++;
    delayus(1000);
  }

  while (availableData() > 0)
  {
    Incoming_Byte = readData();
    if ( (Incoming_Byte == 255) & (peekData() == 255) )
    {
      readData();                            	// Start Bytes
      readData();                            	// MX-12 ID
      readData();                            	// Length
      if ( (Error_Byte = readData()) != 0 )   	// Error
        return (Error_Byte * (-1));
      RWS_Byte = readData();         			// RWStatus
    }
  }
  return (RWS_Byte);               				// Returns the read RWStatus
}

int MX106::sendMXPacket(unsigned char * packet, unsigned int length)
{
  switchCom(Direction_Pin, TX_MODE); 	// Switch to Transmission  Mode

  sendData(packet, length);			// Send data through sending buffer
  flush(); 							// Wait until buffer is empty

  switchCom(Direction_Pin, RX_MODE); 	// Switch back to Reception Mode

  return (read_error());              // Return the read error
}

void MX106::sendMXPacketNoError(unsigned char * packet, unsigned int length)
{
  switchCom(Direction_Pin, TX_MODE); 	// Switch to Transmission  Mode

  sendData(packet, length);			// Send data through sending buffer
  flush(); 							// Wait until buffer is empty

  switchCom(Direction_Pin, RX_MODE); 	// Switch back to Reception Mode
}

int MX106::readRegister(unsigned char ID, unsigned char reg, unsigned char reg_len)
{
  const unsigned int length = 8;
  unsigned char packet[length];

  Checksum = (~(ID + 4 + MX_READ_DATA + reg + reg_len)) & 0xFF;

  packet[0] = MX_START;
  packet[1] = MX_START;
  packet[2] = ID;
  packet[3] = 4;
  packet[4] = MX_READ_DATA;
  packet[5] = reg;
  packet[6] = reg_len;
  packet[7] = Checksum;

  sendMXPacketNoError(packet, length);

  returned_Byte = -1;
  Time_Counter = 0;
  while ((availableData() < 7) & (Time_Counter < TIME_OUT))
  {
    Time_Counter++;
    delayus(1000);
  }

  while (availableData() > 0)
  {
    Incoming_Byte = readData();
    if ( (Incoming_Byte == 255) & (peekData() == 255) )
    {
      readData();                            // Start Bytes
      readData();                            // MX-12 ID
      readData();                            // Length
      if ( (Error_Byte = readData()) != 0 )  // Error
        return (Error_Byte * (-1));

      switch (reg_len)
      {
        case 1:
          returned_Byte = readData();
          break;
        case 2:
          returned_Byte = readData();
          returned_Byte += readData() << 8;
          break;
      }
    }
  }
  return (returned_Byte);     // Returns the read position
}

MX106 MX106;
