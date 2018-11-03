#include "RemoteHand.h"

#define _abs(value) ( (value > 0) ? value : -1 * value );

RemoteHand::RemoteHand(int count) {
	_count = count;
	_id = new int[_count];
	for (unsigned i = 0; i < count; ++i)
		_id[i] = i + 1;
}

RemoteHand::~RemoteHand() {
	delete [] _id;
}

void RemoteHand::correct_speed(int id, int speed) {
	double kp = 1;

}

void RemoteHand::setID(unsigned char id, unsigned char new_id) {
	_id[id] = new_id;
}

void RemoteHand::move(unsigned char id, int position) {
	long long int Position_H, Position_L;
  	Position_H = position >> 8;
  	Position_L = position;
  	long long int Speed_L = 150;
  	long long int Speed_H = Speed_L >> 8;
  	
  	ax12SetRegister2(id, AX_MOVING_SPEED_L, Speed_L);
  	ax12SetRegister2(id, AX_MOVING_SPEED_H, Speed_H);

  	ax12SetRegister2(id, AX_GOAL_POSITION_L, Position_L);
  	ax12SetRegister2(id, AX_GOAL_POSITION_H, Position_H);
}

void RemoteHand::moveSpeed(unsigned char id, int speed, int position) {
	long long int Position_H, Position_L, Speed_H, Speed_L;
  	Position_H = position >> 8;
  	Position_L = position;

  	Speed_H = speed >> 8;
  	Speed_L = speed;

  	ax12SetRegister2(id, AX_MOVING_SPEED_L, Speed_L);
  	ax12SetRegister2(id, AX_MOVING_SPEED_H, Speed_H);

  	ax12SetRegister2(id, AX_GOAL_POSITION_L, Position_L);
  	ax12SetRegister2(id, AX_GOAL_POSITION_H, Position_H);
}

void RemoteHand::startPosition(unsigned char id) {
	long long int Position_L = 512;
	long long int Position_H = Position_L >> 8;
  	ax12SetRegister2(id, AX_GOAL_POSITION_L, Position_L);
  	ax12SetRegister2(id, AX_GOAL_POSITION_H, Position_H);
}

int RemoteHand::getPosition(unsigned char id) {
	return ax12GetRegister(id, AX_PRESENT_POSITION, 2);
}

int RemoteHand::getSpeed(unsigned char id) {
	return ax12GetRegister(id, AX_PRESENT_SPEED, 2);
}

void RemoteHand::lockMotor(unsigned char id) {
	ax12SetRegister(id, AX_LOCK, 1);
}

int RemoteHand::setRegister(unsigned char id, int register, int value, int len_register) {
	if (len_register == 1 && register < 50)
		ax12SetRegister(id, register, value);
	else if (len_register == 2 && register < 50)
		ax12SetRegister2(id, register, value);
	else
		return -1;
	return 1;
}

int RemoteHand::getRegister(unsigned char id, int register, int len_register) {
	return ax12GetRegister(id, register, len_register);
}
