#include <Servo.h>

Servo myservo;
int pos = 0;
int abierto = 110;
int cerrado = 0;



volatile int ledPin = 13;      // the pin that the LED is attached to, needs to be a PWM pin.
int brightness = 0;


enum devices
{
	FLAT_MAN_L = 10,
	FLAT_MAN_XL = 15,
	FLAT_MAN = 19,
	FLIP_FLAT = 99
};

enum motorStatuses
{
	STOPPED = 0,
	RUNNING
};

enum lightStatuses
{
	OFF = 0,
	ON
};

enum shutterStatuses
{
	UNKNOWN = 0, // ie not open or closed...could be moving
	CLOSED,
	OPEN
};

int deviceId = FLAT_MAN;
int motorStatus = STOPPED;
int lightStatus = OFF;
int coverStatus = UNKNOWN;

void setup()
{
  // initialize the serial communication:
  Serial.begin(9600);
  // initialize the ledPin as an output:
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);


  myservo.attach(9);
}

void loop()
{
  handleSerial();
	Corrector();
}

void handleSerial()
{
  if( Serial.available() >= 6 )  // all incoming communications are fixed length at 6 bytes including the \n
  {
    char* cmd;
	char* data;
    char temp[10];

    int len = 0;

    char str[20];
    memset(str, 0, 20);

	// I don't personally like using the \n as a command character for reading.
	// but that's how the command set is.
    Serial.readBytesUntil('\n', str, 20);

	cmd = str + 1;
	data = str + 2;

	// useful for debugging to make sure your commands came through and are parsed correctly.
    if( false )
    {
      sprintf( temp, "cmd = >%c%s;", cmd, data);
      Serial.println(temp);
    }



    switch( *cmd )
    {
	  /*
		Ping device
			Request: >P000\n
			Return : *Pii000\n
				id = deviceId
	  */
      case 'P':
		  sprintf(temp, "*P%d000", deviceId);
		  Serial.println(temp);
		  break;

      /*
		Open shutter
			Request: >O000\n
			Return : *Oii000\n
				id = deviceId

			This command is only supported on the Flip-Flat!
	  */
      case 'O':
		  sprintf(temp, "*O%d000", deviceId);
		  SetShutter(OPEN);
		  Serial.println(temp);
		  break;


      /*
		Close shutter
			Request: >C000\n
			Return : *Cii000\n
				id = deviceId

			This command is only supported on the Flip-Flat!
	  */
      case 'C':
		  sprintf(temp, "*C%d000", deviceId);
		  SetShutter(CLOSED);
		  Serial.println(temp);
		  break;

	  /*
		Turn light on
			Request: >L000\n
			Return : *Lii000\n
				id = deviceId
	  */
      case 'L':
		  sprintf(temp, "*L%d000", deviceId);
		  Serial.println(temp);
		  lightStatus = ON;
		  analogWrite(ledPin, brightness);
		  break;

	  /*
		Turn light off
			Request: >D000\n
			Return : *Dii000\n
				id = deviceId
	  */
      case 'D':
		  sprintf(temp, "*D%d000", deviceId);
		  Serial.println(temp);
		  lightStatus = OFF;
		  analogWrite(ledPin, 0);
		  break;

	  /*
		Set brightness
			Request: >Bxxx\n
				xxx = brightness value from 000-255
			Return : *Biiyyy\n
				id = deviceId
				yyy = value that brightness was set from 000-255
	  */
      case 'B':
		  brightness = atoi(data);
		  if( lightStatus == ON )
			  analogWrite(ledPin, brightness);
		  sprintf( temp, "*B%d%03d", deviceId, brightness );
		  Serial.println(temp);
        break;

	  /*
		Get brightness
			Request: >J000\n
			Return : *Jiiyyy\n
				id = deviceId
				yyy = current brightness value from 000-255
	  */
      case 'J':
        sprintf( temp, "*J%d%03d", deviceId, brightness);
        Serial.println(temp);
        break;

	  /*
		Get device status:
			Request: >S000\n
			Return : *SidMLC\n
				id = deviceId
				M  = motor status( 0 stopped, 1 running)
				L  = light status( 0 off, 1 on)
				C  = Cover Status( 0 moving, 1 closed, 2 open)
	  */
      case 'S':
        sprintf( temp, "*S%d%d%d%d",deviceId, motorStatus, lightStatus, coverStatus);
        Serial.println(temp);
        break;

	  /*
		Get firmware version
			Request: >V000\n
			Return : *Vii001\n
				id = deviceId
	  */
      case 'V': // get firmware version
		  sprintf(temp, "*V%d001", deviceId);
		  Serial.println(temp);
		  break;
    }

	while( Serial.available() > 0 )
		Serial.read();

  }
}

void SetShutter(int val)
{
	if( val == OPEN && coverStatus != OPEN )
	{
		// TODO: Implement code to OPEN the shutter.
    myservo.write(abierto);
    coverStatus = OPEN;



	}
	else if( val == CLOSED && coverStatus != CLOSED )
	{
		// TODO: Implement code to CLOSE the shutter
    myservo.write(cerrado);
    coverStatus = CLOSED;

	}
	else
	{
		// TODO: Actually handle this case
		coverStatus = CLOSED;
    myservo.write(cerrado);

	}

}


void Corrector(myservo.read()){
	if (myservo.read != abierto && myservo.read != cerrado) {
		myservo.write(cerrado);
    coverStatus = CLOSED;
	}
}
