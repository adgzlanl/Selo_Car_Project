

#include "VirtualWire.h"
const int RX_Pin = 12;
const int StatusLed = 13;
int LedState = LOW;
unsigned long previousMillis = 0;
const int interval = 1000;
int buttonState = 0;
int buttonPushCounter = 0;        
int lastButtonState = 0;
int startPressed = 0;    
int endPressed = 0;     
int timeHold = 0;        
int timeReleased = 0;    
const int AIA = 2;
const int AIB = 3;
const int BIA = 4;
const int BIB = 5;
const int Play_E = 21;
const int Play_L = 22;
const int Record = 23;
unsigned int Analog_X, Analog_Y;
int Motor_X_Speed = 0;
int Motor_Y_Speed = 0;
int ButtonCounter = 0;
void setup()
{
	pinMode(StatusLed, OUTPUT);
	pinMode(Play_E, OUTPUT);
	pinMode(Play_L, OUTPUT);
	pinMode(Record, OUTPUT);
	pinMode(AIA, OUTPUT);
	pinMode(AIB, OUTPUT);
	pinMode(BIA, OUTPUT);
	pinMode(BIB, OUTPUT);
	Serial.begin(9600);
	vw_set_rx_pin(RX_Pin);
	vw_setup(2000);
	vw_rx_start();
}

/* Main program */
void loop()
{

	unsigned long currentMillis = millis();


	uint8_t Buffer_Size = 5;
	uint8_t RxBuffer[Buffer_Size];


	if (vw_get_message(RxBuffer, &Buffer_Size))
	{

		Analog_X = RxBuffer[0] << 8 | RxBuffer[1];
		Analog_Y = RxBuffer[2] << 8 | RxBuffer[3];
		buttonState = RxBuffer[4];

	}


	if (Analog_X > 600)
	{
		Motor_X_Speed = map(Analog_X, 562, 1023, 0, 255);
		forward(Motor_X_Speed);
	}
		

	if (Analog_X < 480)
	{
		Motor_X_Speed = map(Analog_X, 0, 500, 255, 0);
		backward(Motor_X_Speed);
	}

	
	if (Analog_Y > 600)
	{
		Motor_Y_Speed = map(Analog_Y, 550, 780, 0, 255);
		forward(Motor_Y_Speed);
	}


	if (Analog_Y < 420)
	{
		Motor_Y_Speed = map(Analog_Y, 210, 450, 255, 0);
		backward(Motor_Y_Speed);
	}


	if (buttonState != lastButtonState) {

		if (buttonState == HIGH) {

			startPressed = millis();
			//timeReleased = startPressed - endPressed;
			buttonPushCounter++;
			Serial.println(buttonPushCounter);
			
		}

		else {
			
			endPressed = millis();
			timeHold = endPressed - startPressed;

			if (timeHold > 25000)
			{
				digitalWrite(Play_L, HIGH);
			}
		}
	}


	if (currentMillis - previousMillis >= interval) {
		
		previousMillis = currentMillis;

		
		if (LedState == LOW) {
			LedState = HIGH;
			
		}
		else {

			LedState = LOW;

		}

	
		digitalWrite(StatusLed, LedState);
	}



	lastButtonState = buttonState;

}


void backward(int speed)
{
	analogWrite(AIA, 0);
	analogWrite(AIB, speed);
	analogWrite(BIA, 0);
	analogWrite(BIB, speed);
}
void forward(int speed)
{
	analogWrite(AIA, speed);
	analogWrite(AIB, 0);
	analogWrite(BIA,
		speed);
	analogWrite(BIB, 0);
}