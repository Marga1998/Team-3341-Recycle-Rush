#include "ArcadeDrive.h"

ArcadeDrive::ArcadeDrive()
	:isReset(true)
{
	Requires(drive);
	Requires(gyro);
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(chassis);
}

// Called just before this Command runs the first time
void ArcadeDrive::Initialize()
{
	gyro->ResetGyro();
	anglePid = new NewPIDController(.05,0,0,0, false);
}

// Called repeatedly when this Command is scheduled to run
void ArcadeDrive::Execute()
{
	if(fabs(oi->getDriveStick()->GetZ()) >= 0.05)
	{
		isReset = false;
        double x = -oi->getDriveStick()->GetZ(); 
        double a = 1, b = 0.5;
        double control;
        if(x > 0)
            control = b + (1-b)*(a*power(x, 5) + (1-a)*x);
        else
            control = -(-b + (1-b)*(a*power(x, 5) + (1-a)*x));
		drive->arcadeDrive(-oi->getDriveStick()->GetY(), x * control);
	}
	else{
		if(!isReset){
			drive->arcadeDrive(0,0);
			Wait(.05);
			isReset = true;
			gyro->ResetGyro();
		}
		if(fabs(oi->getDriveStick()->GetY()) >= .05)
			drive->arcadeDrive(-oi->getDriveStick()->GetY(), anglePid->Tick(gyro->GetAngle()));
		else
		{
			drive->arcadeDrive(0,0);
		}
	}
}

// Make this return true when this Command no longer needs to run execute()
bool ArcadeDrive::IsFinished()
{
	return false;
}

// Called once after isFinished returns true
void ArcadeDrive::End()
{

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ArcadeDrive::Interrupted()
{

}
