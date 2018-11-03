#include "RemoteHand.h"

#define _abs(value) ( (value > 0) ? value : -1 * value );

////////////////////// Private section ///////////////////////////

void RemoteHand::correct_angle(int & _angle) {
        _angle = (_angle < -1 * MAX_ANGLE) ? -1 * MAX_ANGLE : _angle;
        _angle = (_angle > MAX_ANGLE) ? MAX_ANGLE : _angle;
}

int RemoteHand::angle2pos(int _angle) {
         correct_angle(_angle);
         return (static_cast <int> (512 + _angle * 3.4));
}

////////////////////// Public section ////////////////////////////

void RemoteHand::move(unsigned char id, int _position) {
        _position = angle2pos(_position);
	long long int Position_H, Position_L;
  	Position_H = _position >> 8;
  	Position_L = _position;
  	long long int Speed_L = 150;
  	long long int Speed_H = Speed_L >> 8;

  	while (ax12GetRegister(id, AX_PRESENT_POSITION, 2) != Position_L) {
                ax12SetRegister2(id, AX_MOVING_SPEED_L, Speed_L);
  	        ax12SetRegister2(id, AX_MOVING_SPEED_H, Speed_H);
  
  	        ax12SetRegister2(id, AX_GOAL_POSITION_L, Position_L);
  	        ax12SetRegister2(id, AX_GOAL_POSITION_H, Position_H); 
        }
        delay(500);
}

void RemoteHand::moveSpeed(unsigned char id, int _speed, int _position) {
        _position = angle2pos(_position);
	long long int Position_H, Position_L, Speed_H, Speed_L;
  	Position_H = _position >> 8;
  	Position_L = _position;

  	Speed_H = _speed >> 8;
  	Speed_L = _speed;

        while (ax12GetRegister(id, AX_PRESENT_POSITION, 2) != Position_L) {
                ax12SetRegister2(id, AX_MOVING_SPEED_L, Speed_L);
  	        ax12SetRegister2(id, AX_MOVING_SPEED_H, Speed_H);
  
  	        ax12SetRegister2(id, AX_GOAL_POSITION_L, Position_L);
  	        ax12SetRegister2(id, AX_GOAL_POSITION_H, Position_H); 
        }
        delay(500);
}

void RemoteHand::startPosition(unsigned char id) {
	long long int Position_L = 512;
	long long int Position_H = Position_L >> 8;

        long long int Speed_L = 100;
  	long long int Speed_H = Speed_L >> 8;

        while (ax12GetRegister(id, AX_PRESENT_POSITION, 2) != Position_L) {
                ax12SetRegister2(id, AX_MOVING_SPEED_L, Speed_L);
  	        ax12SetRegister2(id, AX_MOVING_SPEED_H, Speed_H);
  
  	        ax12SetRegister2(id, AX_GOAL_POSITION_L, Position_L);
  	        ax12SetRegister2(id, AX_GOAL_POSITION_H, Position_H); 
        }	
  
        delay(500);
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

int RemoteHand::setRegister(unsigned char id, int _register, int value, int len_register) {
	if (len_register == 1 && _register < 50)
		ax12SetRegister(id, _register, value);
	else if (len_register == 2 && _register < 50)
		ax12SetRegister2(id, _register, value);
	else
		return -1;
	return 1;
}

int RemoteHand::getRegister(unsigned char id, int _register, int len_register) {
	return ax12GetRegister(id, _register, len_register);
}
