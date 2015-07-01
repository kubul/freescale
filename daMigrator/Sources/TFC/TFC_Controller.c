
#include "TFC\TFC.h"
#include "math.h"

//Steering
//Variables
//float delta; 			//Total steering delta
//float Ux; 				//x axis speed
float deltapsi;			//vehicle heading error
float Rs;				//radius of the curvature at the apex
float r;				//yaw rate
float e;				//path error

//Calculated constants
float Wf;				//front axle normal load
float Wr;				//rear axle normal load
float Kug;				//understeer gradient

//Feedforward
float deltaff;			//delta feed forward

//Control
float deltacontrol;		//delta control

//Damping
float deltadamping;		//delta damping


//Speed control
struct wheel LW;
struct wheel RW;
float Fffw;
float Ffb;
float Fspeedfb;

//Drag
float Fturning;
float Fdrag;

//Feedback
float alpha;
float deltaalpha;

float deltakappa;
float deltakappamin;

void InitSteeringControl() {
	/*
	Ux = 0;
	deltapsi = 0;
	Rs = 1;
	r = 0;
	e = 0;
	*/
	
	Wf = (b*mass*g)/L;
	Wr = (a*mass*g)/L;
	Kug = Wf/Cf - Wr/Cr;

}

void SetSteeringControl() {
	
	//Feedforward
	deltaff = (L + (Kug*daCar.speed*daCar.speed)/g)/Rs;
	
	//Control
	deltacontrol = -2*kp*(e+xla*sinf(deltapsi))/Cf;
	
	//Damping
	deltadamping = -kdeltapsidot*(r - (daCar.speed/Rs)*(cosf(deltapsi) - tanf(beta)*sinf(deltapsi)));
	
	daCar.steeringAngle = deltaff + deltacontrol + deltadamping;
	
	TFC_SetServo(rad2pot1*daCar.steeringAngle + rad2pot2*daCar.steeringAngle*daCar.steeringAngle);

}

void InitSpeedControl() {
	alpha = 0;
}


void SetSpeedControl() {
	float sdiff;
	
	//Drag
	Fturning = (mass * b/(a+b) * abs(daCar.speed*daCar.speed / Rs * tan(daCar.steeringAngle)));
	//Fgrade = -mass*g*sin(thetagrade);
	Fdrag = Fturning;// + Faero + Frolling + Fgrade;
	

	//differential for desire
	sdiff = tanf(daCar.steeringAngle) * CAR_WHEELBASE / (2*CAR_TRACK_WIDTH);	
	LW.Uxs = daCar.speedDesired * (1 + sdiff); 
	RW.Uxs = daCar.speedDesired * (1 - sdiff); 
	
	//differential for current
	LW.Ux = daCar.speed * (1 + sdiff);
	RW.Ux = daCar.speed * (1 - sdiff);
	
	//gimme kappa - gotcha
	LW.kappa = (daCar.wheelspeed_left  - LW.Ux) / LW.Ux; //TODO: normalize 
	RW.kappa = (daCar.wheelspeed_right - RW.Ux) / RW.Ux; //TODO: normalize	
	
	
	//gimme asx
	
	SetWheelSpeedControl(&LW);
	SetWheelSpeedControl(&RW);
	
	TFC_SetMotorPWM(F2PWM(RW.Fx), F2PWM(LW.Fx));
}

void SetWheelSpeedControl(struct wheel *W) {
	
		//Feedforward
		Fffw = mass * W->axs;
		
		//Feedback
		if ((W->kappa*W->kappa + alpha*alpha)>=1) { //The rear wheel is saturated
			
			if (abs(alpha) <= 1) {
				deltakappa = abs(W->kappa) - sqrt(1-alpha*alpha);
				deltaalpha = 0;
			} else {
				deltakappa = abs(W->kappa);
				deltaalpha = abs(alpha) - 1;
			}
			
			if (W->kappa <= 0) {
				Ffb = kkappa*deltakappa + kalpha*deltaalpha;
			} else {
				Ffb = -kkappa*deltakappa - kalpha*deltaalpha;
			}
			
			Fspeedfb = 0;
			
		} else { //Not saturated
			deltakappamin = sqrt(1-alpha*alpha)-abs(W->kappa);
			Ffb = knoslip*deltakappamin;
			
			//Speed feedback
			Fspeedfb = kspeed*(W->Uxs - W->Ux);
		}

		W->Fx = Fffw + Fdrag + Ffb + Fspeedfb;

}

float F2PWM(float force) {
	return force;
}

