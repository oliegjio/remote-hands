#ifndef REMOTEHAND_H
#define REMOTEHAND_H

#include "ax12.h"

#define AX_GOAL_POSITION_L   30
#define AX_GOAL_POSITION_H   31
#define AX_MOVING_SPEED_L    32
#define AX_MOVING_SPEED_H    33
#define AX_PRESENT_POSITION  36
#define AX_PRESENT_SPEED     38
#define AX_LOCK              47

class RemoteHand {
private:
	void correct_speed(int id, int _speed, int _position);
        int wait_time(int _speed, int _position);
public:
	void setID(unsigned char id, unsigned char new_id);
	void move(unsigned char id, int _position);
	void moveSpeed(unsigned char id, int _speed, int _position);
	void startPosition(unsigned char id);
	int getPosition(unsigned char id);
	int getSpeed(unsigned char id);
	void lockMotor(unsigned char id);

	int setRegister(unsigned char id, int _register, int value, int len_register);
	int getRegister(unsigned char id, int _register, int len_register);
};

#endif /* REMOTEHAND_H */
