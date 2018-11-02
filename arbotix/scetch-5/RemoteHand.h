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
	unsigned   _count;
	unsigned * _id;
	void correct_speed(int id, int speed);
public:
	RemoteHand(int count);
	~RemoteHand();

	void setID(unsigned char id, unsigned char new_id);
	void move(unsigned char id, int position);
	void moveSpeed(unsigned char id, int speed, int position);
	void startPosition(unsigned char id);
	int getPosition(unsigned char id);
	int getSpeed(unsigned char id);
	void lockMotor(unsigned char id);

	int setRegister(unsigned char id, int register, int value, int len_register);
	int getRegister(unsigned char id, int register, int len_register);
};

#endif /* REMOTEHAND_H */