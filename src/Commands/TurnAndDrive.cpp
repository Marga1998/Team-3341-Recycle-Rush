#include <Commands/TurnAndDrive.h>
#include "../NetworkTablesInterface.h"

TurnAndDrive::TurnAndDrive(double inDistance, double inAngle)
	: distance(inDistance), angle(inAngle)
{
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(chassis);
	Requires(drive);
}

// Called just before this Command runs the first time
void TurnAndDrive::Initialize()
{
	drive->ResetEncoders();
	distancePid = new NewPIDController(1.3125, 0, 0, distance);
	anglePid = new NewPIDController(.05, 1e-4, 0, angle);
}

// Called repeatedly when this Command is scheduled to run
void TurnAndDrive::Execute()
{
	double current_distance = drive->GetDistance();
	double pwm_val = distancePid->Tick(current_distance);
	double current_angle = gyro->GetAngle();
	double rotateVal = anglePid->Tick(current_angle);
	drive->arcadeDrive(Drive::Limit(pwm_val,.5), -Drive::Limit(rotateVal, 1.0));
	//printf("Setpoint: %f\nCurrent Distance: %f\nError: %f\nPWM Value: %f\n\n", distancePid->GetSetPoint(), current_distance, distancePid->GetError(), pwm_val);
	//printf("Setpoint: %f\nCurrent Angle: %f\nError: %f\nPWM Value: %f\n\n", anglePid->GetSetPoint(), current_angle, anglePid->GetError(), rotateVal);
	//printf("%f", current_distance);
	//	if (distance >= 0){
//		drive->arcadeDrive(.5,0);
//	}
//	else {
//		drive -> arcadeDrive (-.5,0);
//	}
}

// Make this return true when this Command no longer needs to run execute()
bool TurnAndDrive::IsFinished()
{
	return (fabs(distancePid->GetError()) < 0.005) && (fabs(anglePid->GetError()) < 0.7);
}

// Called once after isFinished returns true
void TurnAndDrive::End()
{
	drive->arcadeDrive(0,0);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void TurnAndDrive::Interrupted()
{

}
