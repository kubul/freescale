
#include "TFC\TFC.h"
#include "math.h"


/* differential
ratio_L = 1+tan(d)*W/(2L)
ratio_R = 1-tan(d)*W/(2L)
 */


/*  deg to ±1.0   =    0.035327126*deg + 4.05856E-05*deg^2   */

/*  ±1.0 to deg  =  0.10545 + 28.3 x - 0.92055 x^2   */


/* Integrating these values in time requires knowing how old
a given data packet is, the amount of time from packet
creation by the GPS until packet use by the High Level
Controller. The resulting
time difference between packet creation and use, Tdiff , is
used in a simple forward euler integration to update the most
recent data to an approximation of that data at the time of
the controller’s execution:
x^ = xin + d/dt*xin *Tdiff ; (1)
where x represents a vehicle state such as position or
velocity. If no new data is received from one time step to the
next, then the previous time step’s data is used a second time
with a different integration time. In addition to thus handling
missed data on a given time step, this approximation also
helps smooth out discontinuities due to inconsistent packet
transmission from the GPS.
*/

static const float KD = 0.01;
static const float KI = 0.05;//4*KD;
static const float KP = 0.05;
static const float KP_THRESH = 0.2;
static const float REVERSE_THRESH = 0.2;

//static const float dt = 1/45.5; //TODO
static const float dt = 1.0/50; //TODO

struct PIDwheel LW;
struct PIDwheel RW;

float steerPot;


void InitSpeedControlPID() {
	
	LW.speedDesired = 0;		
	LW.speedError  = 0;
	LW.speedErrorI = 0;
	LW.speedErrorD = 0;
	LW.prevSpeedError = 0;
	LW.PWMtarget = 0;
	LW.PWM = 0;
	
	RW.speedDesired = 0;		
	RW.speedError  = 0;
	RW.speedErrorI = 0;
	RW.speedErrorD = 0;
	RW.prevSpeedError = 0;
	RW.PWMtarget = 0;
	RW.PWM = 0;
}

void SetWheelSpeedControlPID(struct PIDwheel *W, float wheelspeed) {
	W->speedError  = W->speedDesired - wheelspeed; 
	W->speedErrorI = W->speedErrorI + W->speedError*dt;
	if (W->speedErrorI>KP_THRESH) {
		W->speedErrorI = KP_THRESH;
	} else if (W->speedErrorI < -KP_THRESH) {
		W->speedErrorI = -KP_THRESH;
	}
	W->speedErrorD = (W->speedError-W->prevSpeedError)/dt;
	W->prevSpeedError = W->speedError;
	
	W->PWMtarget = W->PWMtarget + KP*W->speedError + KI*W->speedErrorI + KD*W->speedErrorD;
	//TODO huy	
	
	if (W->PWMtarget > 1.0)  W->PWMtarget = 1.0;
	if (W->PWMtarget < -1.0)  W->PWMtarget = -1.0;
	
	W->PWM = W->PWMtarget; 
	
	if (daCar.speed < daCar.speedDesired && wheelspeed > 1.2*daCar.speed) {
		W->PWM = W->PWM/2;
	}	
	
	if (daCar.speed > daCar.speedDesired && 1.2*wheelspeed < daCar.speed) {
		W->PWM = 0; //W->PWM/3;
	}
	
	/*
	
	if (daCar.speedDesired == 0) {
		W->PWM = 0;
	}	
	*/
	
	if (W->PWMtarget < 0 && wheelspeed < REVERSE_THRESH)  {
		W->PWM = 0;
	}
	
}


void SetSpeedControlPID() {
	float sdiff;
	
	daCar.speed = daCar.wheelspeed_front; //TODO coeff	
	steerPot = TFC_ReadPot(0);	
	daCar.steeringAngle = 0.10545 + 28.3*steerPot - 0.92055*steerPot*steerPot; 
		
	sdiff = tanf(daCar.steeringAngle * (PI / 180)) * CAR_WHEELBASE / (2*CAR_TRACK_WIDTH);
	
	//differential
	//LW.Ux = daCar.speed * (1 + sdiff);
	//RW.Ux = daCar.speed * (1 - sdiff);		
	
	LW.speedDesired = daCar.speedDesired * (1 + sdiff); 
	RW.speedDesired = daCar.speedDesired * (1 - sdiff);
	
	SetWheelSpeedControlPID(&LW, daCar.wheelspeed_left);
	SetWheelSpeedControlPID(&RW, daCar.wheelspeed_right);
	
	/* Set new PWM */ 	
	TFC_SetMotorPWM(RW.PWM, LW.PWM);
}


