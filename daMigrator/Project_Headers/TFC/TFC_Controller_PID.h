
#ifndef TFC_CONTROLLER_PID_H_
#define TFC_CONTROLLER_PID_H_


//Feedforward constants

#define LEFTWHEEL 0
#define RIGHTWHEEL 1

#define PI 3.14159

struct PIDwheel {
	
	float speedDesired;		
	float speedError;
	float speedErrorI;
	float speedErrorD;
	float prevSpeedError;
	float PWMtarget;
	float PWM;
	
};

void InitSpeedControlPID();
void SetWheelSpeedControlPID(struct PIDwheel *W, float wheelspeed);
void SetSpeedControlPID();

#endif /* TFC_CONTROLLER_PID_H_ */
