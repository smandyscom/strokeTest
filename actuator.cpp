#include<actuator.h>
//#include<stdlib.h>		//for dummy testing
#include<string.h>
#if ARDUINO_PROJECT == 1
#include<time.h>
#endif
#define DUMMY_SENSE 129

static void goHome(actuator* __actuator){
	*__actuator->OutputPort = __actuator->homeStep;
	__actuator->currentPosition--;
	if (__actuator->currentPosition == 0)
		__actuator->currentPosition = 1;		// zero should be established by sensor
}

static void goLimit(actuator* __actuator){
	*__actuator->OutputPort = __actuator->limitStep;
	__actuator->currentPosition++;
}

static void shutDown(actuator* __actuator)
{
	*__actuator->OutputPort = 0;		// shut output
	((deviceBase*)__actuator)->Enable((deviceBase*)__actuator, false);	// stop driven
}

static void calculateCheckPoints(actuator* __actuator){
	//----------------------
	//	Calculate parameters
	//----------------------
	__actuator->CheckPoints[CHECKPOINT_PRE_LIMIT] = __actuator->limitPosition * 0.95;
	__actuator->CheckPoints[CHECKPOINT_PRE_HOME] = __actuator->limitPosition * 0.05;

	if (__actuator->CheckPoints[CHECKPOINT_PRE_HOME] == 0)
		__actuator->CheckPoints[CHECKPOINT_PRE_HOME] = 1;		// at least for 1
}
static int workingProcedure(actuator* __actuator)
{
	//------------------------------
	//	Listen and Execute requests
	//------------------------------
	switch (__actuator->Command)
	{
	case REQUEST_TO_LIMIT:
		__actuator->status &= ~HOME_END_REACHED;		//reset home end status
		//------------------
		//	Request to Limit End
		//------------------
		if (__actuator->currentPosition < __actuator->CheckPoints[CHECKPOINT_PRE_LIMIT]){
			if (__actuator->CurrentSense() >= __actuator->Thresholds[THRESHOLD_ALARM]){
				//----------------
				//	Error , out of expection the obstacle reached
				//shut acturator , and shutdown whole procedure
				//----------------
				shutDown(__actuator);
			}
			else{
				//-------------------
				//	Normal , go limit
				//-------------------
				goLimit(__actuator);
			}
		}
		else if (__actuator->currentPosition >= __actuator->CheckPoints[CHECKPOINT_PRE_LIMIT]
			&& !(__actuator->status & LIMIT_END_REACHED))
		{
			//-----------------------
			//	Between LIMIT and PRE_LIMIT , sensing the threshold
			//------------------------
			if (__actuator->CurrentSense() >= __actuator->Thresholds[THRESHOLD_LIMIT]){
				//stop motor
				__actuator->limitPosition = __actuator->currentPosition;	// re-establish limit position
				calculateCheckPoints(__actuator);	// re-calculating check points
				*__actuator->OutputPort = 0;
				__actuator->status |= LIMIT_END_REACHED;
			}
			else
			{
				//-------------------
				//	Normal , go limit
				//-------------------
				goLimit(__actuator);
			}
		}
		else if (__actuator->status & LIMIT_END_REACHED)
		{
			//--------------------------
			//	End reached , do nothing
			//--------------------------
			*__actuator->OutputPort = 0;
		}
		break;
	case REQUEST_TO_HOME:
		__actuator->status &= ~LIMIT_END_REACHED;
		//------------------
		//	Request to Home End
		//------------------
		if (__actuator->currentPosition > __actuator->CheckPoints[CHECKPOINT_PRE_HOME])
		{
			if (__actuator->CurrentSense() >= __actuator->Thresholds[THRESHOLD_ALARM]){
				//----------------
				//	Error , obstacle reached
				//----------------
				shutDown(__actuator);
			}
			else{
				//-------------------
				//	Normal , go home
				//-------------------
				goHome(__actuator);
			}
		}
		else if (__actuator->currentPosition <= __actuator->CheckPoints[CHECKPOINT_PRE_HOME] &&
			!(__actuator->status & HOME_END_REACHED))
		{
			//-----------------------
			//	Between HOME and PRE_HOME , sensing the threshold
			//------------------------
			if (__actuator->CurrentSense() >= __actuator->Thresholds[THRESHOLD_HOME]){
				*__actuator->OutputPort = 0;//stop motor
				__actuator->currentPosition = 0;	// re-establish home position
				__actuator->status |= HOME_END_REACHED;
			}
			else
			{
				//-------------------
				//	Normal , go home
				//-------------------
				goHome(__actuator);
			}
		}
		else if (__actuator->status & HOME_END_REACHED)
		{
			//--------------------------
			//	End reached , do nothing
			//--------------------------
			*__actuator->OutputPort = 0;
		}
		break;
	case REQUEST_STOP:
		//stop motor
		*__actuator->OutputPort = 0;
		break;
	default:
		break;
	}

	return 0;
}
static int homeProcedure(actuator* __actuator)
{
	unsigned short* state = &__actuator->base.state;
	switch (*state)
	{
	case 0 :
		if (true)
		{	
			//*__actuator->OutputPort = __actuator->Speed * __actuator->Direction;
			goHome(__actuator);
			*state = 10;
		}
		else
		{
			//-------------------
			//	Wait start signal
			//-------------------
		}
		break;
	case 10:
		if (__actuator->CurrentSense() >= __actuator->Thresholds[THRESHOLD_HOME]){
			//*__actuator->OutputPort = __actuator->Speed * __actuator->Direction * -1;		// reverse
			__actuator->currentPosition = 0;		// reset
			//goLimit(__actuator);
			*state = 20;
		}
		else
		{
			//-------------------------------
			//	Wait until limit-home reached
			//-------------------------------
		}
		break;
	case 20:
		if (__actuator->CurrentSense() >= __actuator->Thresholds[THRESHOLD_LIMIT]){
			*__actuator->OutputPort = 0;		//stop
			__actuator->limitPosition = __actuator->currentPosition;	//record current position
			//----------------------
			//	Calculate parameters
			//----------------------
			calculateCheckPoints(__actuator);

			*state = 0;	//rewind
			//Serial.println("Home finished");
			__actuator->base.process = (int(*)(deviceBase*))workingProcedure;	// switch control procedure
		}
		else
		{
			//----------------------------
			//	Not reached limit yet,  keep go forward to limit-end
			//----------------------------
			goLimit(__actuator);
		}
	default:
		break;
	}

	return 0;
}



static unsigned int dummyCurrentSense()
{
	return 0;
	//return rand()%(DUMMY_SENSE);
}


int actuatorInitialize(actuator* __acturator)
{
#if ARDUINO_PROJECT  == 1
	srand(time(NULL));		// dummy test used
#endif

	memset(__acturator, 0, sizeof(actuator));
	deviceInitialize((deviceBase*)__acturator);
	((deviceBase*)__acturator)->process = (int(*)(deviceBase*))homeProcedure;	// specify initial state
	// given default setting
	__acturator->CurrentSense = dummyCurrentSense;
	for (int i = 0; i < TOTAL_THRESHOLDS; i++)
	{
		__acturator->Thresholds[i] = 128;
	}


	return 0;
}

//static int speed = 1;		// shared temp variable
void actuatorSetDirectionAndSpeed(actuator* __actuator, char direction,int speed)
{
	//--------------------------------------------------
	//	Setup directions
	//--------------------------------------------------
	__actuator->homeStep = speed * direction;
	__actuator->limitStep = __actuator->homeStep * -1;
}

float actuatorCurrentDegree(actuator* __actuator)
{
	// report current degreed = currentPosition/limitPosit
	return (float)__actuator->currentPosition / (float)__actuator->limitPosition;
}	

