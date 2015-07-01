
#ifndef TFC_CONTROLLER_H_
#define TFC_CONTROLLER_H_

#define mu  0.4

//Feedforward constants
#define L	CAR_WHEELBASE	//vehicle length
#define Cf 	0//huy			//frontal cornering stiffness
#define Cr 	0//huy			//rear cornering stiffness

//Control constants	
#define kp 	0//huy			//lanekeeping potential field gain
#define xla 1				//lookahead distance

//Damping constants
#define kdeltapsidot 0//huy	//yaw damping gain
#define beta 0//huy			//vehicle sideslip

//Feedforward constants
#define mass	CAR_MASS	//vehicle mass

//Feedback constants
#define knoslip 0//huy

//Speed feedback constants
#define kkappa 0//huy
#define kalpha 0//huy
#define kspeed 0//huy

#define LEFTWHEEL 0
#define RIGHTWHEEL 1

#define PI 3.14159
#define g 9.81
#define rad2pot1 2.024095
#define rad2pot2 0.002325

volatile struct wheel {
	
	float Fx;
	float Ux;

	//Feedforward
	float axs;

	//Feedback
	float kappa;

	//Speed feedback
	float Uxs;
	
};

void InitSteeringControl();
void SetSteeringControl();

void InitSpeedControl();
void SetSpeedControl();
void SetWheelSpeedControl(struct wheel *W);

float F2PWM(float force);

#endif /* TFC_CONTROLLER_H_ */
