/*
 * Bender:
 * Bender starts asleep 
 * 
 * If it is dark Bender goes to sleep
 * otherwise if someone is close then he goes cross-eyed
 * otherwise randomly display shifty, surprised, happy, sad, crying or mad eyes.
 */
 
#include "LedControlMS.h" // https://github.com/shaai/Arduino_LED_matrix_sketch
#include "LedEyes.h" 
#include <stdlib.h>
#include "Streaming.h" // http://arduiniana.org/libraries/streaming/

// Note: 
// On a Vista machine the USB 2 drivers for the Arduino clone were not found
// I needed to download http://www.wch.cn/download/CH341SER_ZIP.html and manually point to the drivers
// This was not needed on Windows 7 or when using a Duinotech or a Freetronics board.

/*
 * LED Matrix:
 * Use pins 12, 11 and 10 on the Arduino for the SPI interface
 * Pin 12 is connected to the DATA IN-pin of the first MAX7221
 * Pin 11 is connected to the CLK-pin of the first MAX7221
 * Pin 10 is connected to the LOAD(/CS)-pin of the first MAX7221
 */

 /* Ultrasonic sensor:
  * VCC connection of the sensor attached to +5V
  * GND connection of the sensor attached to ground
  * TRIG connection of the sensor attached to digital pin 3
  * ECHO connection of the sensor attached to digital pin 4
  */

/* Photocell sensor:
  * VCC connection of the sensor attached to +5V
  * GND connection of the sensor attached to ground
  * SIGNAL connection of the sensor attached to analog pin A0
  */

  

LedControl lc = LedControl(12, 11, 10, NumDevices ); // Note this will set lc1.getDeviceCount() = 2
bool isSleeping; // True if it is dark and Bender is sleeping

void setup()
{
  // initialize serial communication:
  // Used for output only
  Serial.begin (38400);

  // Init Ultrasonic sensor
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  // Init the LED Matrix
  for (int i = 0; i < NumDevices ; i++)
  {
    // wake up the LED matrix from power-saving mode
    lc.shutdown(i, false);
    /* and clear the display */
    lc.clearDisplay(i);
  }

  // Bender wakes from sleeping
  DisplaySleepEyes();
  isSleeping = true;
}

/*
 * If it is dark Bender goes to sleep
 * otherwise if someone is close thenhe goes cross-eyed
 * otherwise randomly display shifty, surprised, happy, sad, crying or mad eyes.
 */
void loop()
{
  Serial << "Start of loop" << endl;
  float distance = GetDistance();
  int photocellReading = ReadLightSensor();
    
  if (photocellReading < SleepThreshold)
  {
    if (!isSleeping)
    {
      ChangeIntensityFromTo(WakeLedIntensity, SleepLedIntensity);
      isSleeping = true;
    }

    Serial << "Go to Sleep" << endl;    
    DisplaySleepEyes();
  }
  else
  {
    if (isSleeping)
    {
      Serial << "Waking up" << endl;    
      ChangeIntensityFromTo(SleepLedIntensity, WakeLedIntensity);
      isSleeping = false;
    }

    if (distance < CrossEyedDistance)
    {
      Serial << "Cross Eyed" << endl;
      DisplayCrossEyedWhileSomeoneClose();
    }
    else if (distance < MidCrossEyedDistance)
    {
      Serial << "Med Cross Eyed" << endl;
      DisplayMedCrossEyedWhileSomeoneClose();
    }
    else if (distance < SemiCrossEyedDistance)
    {
      Serial << "Semi Cross Eyed" << endl;
      DisplaySemiCrossEyedWhileSomeoneClose();
    }
    else
    {
      int randomNumber = GetRandomPercentage();
      Serial << "Choose eye based on the random Number: " << randomNumber << endl << endl;

      if (randomNumber < 10)
      {
        Serial << "Surprised Eyed" << endl;
        DisplaySurprisedEyesIfNoOneClose();
      }
      else if (randomNumber < 20)
      {
        Serial << "Happy Eyed" << endl;
        DisplayHappyEyesIfNoOneClose();
      }
      else if (randomNumber < 30)
      {
        Serial << "Sad Eyed" << endl;
        DisplaySadEyesIfNoOneClose();
      }
      else if (randomNumber < 50)
      {
        Serial << "Crying Eyes" << endl;
        DisplayCryEyesIfNoOneClose();
      }
      else if (randomNumber < 60)
      {
        Serial << "Mad Eyes" << endl;
        DisplayMadEyesIfNoOneClose();
      }
      else
      {
        Serial << "Shifty Eyed" << endl;
        DisplayShiftyEyesLookingLeftAndRightIfNoOneClose();
      }
    }
  }

  Serial << endl << endl;
}


/*********************
 * Sleep Eyes    *
 *********************/
void DisplaySleepEyes()
{
  /* Set the brightness to its lowest values */
  SetIntensity(1);

  DisplayEyes( shiftyPartialClosedEye, shiftyPartialClosedEye, 200);
  DisplayEyes( shiftyClosedEye, shiftyClosedEye, 1000);
}


/*********************
 * Shifty Eyes    *
 *********************/
void DisplayShiftyEyesLookingLeftAndRightIfNoOneClose()
{
  float distance = GetDistance();
  int photocellReading = ReadLightSensor();
  
  for (int i = 0; i < NumEyesShown && distance >= SemiCrossEyedDistance && photocellReading >= SleepThreshold; i++)
  {
    int randomNumber = GetRandomPercentage();

    // Choose the shifty eye variant
    if (randomNumber < 10)
    {
      DisplayShiftyEyesLookingLeftAndRightWhileBlinkingLeft();
    }
    else if (randomNumber < 20)
    {
      DisplayShiftyEyesLookingLeftAndRightWhileBlinkingRight();
    }
    else if (randomNumber < 25)
    {
      DisplayShiftyEyesLookingLeftAndRightWhileBlinkingBoth();
    }
    else if (randomNumber < 35)
    {
      DisplayShiftyEyesLookingInACircle();
    }
    else
    {
      DisplayShiftyEyesLookingLeftAndRight();
    }

    distance = GetDistance();
    photocellReading = ReadLightSensor();
  }
}

void DisplayCrossEyedWhileSomeoneClose()
{
  DisplayEyes( shiftyPupilEye4, shiftyPupilEye1, 100);
}

void DisplayMedCrossEyedWhileSomeoneClose()
{
  DisplayEyes( shiftyPupilEye3, shiftyPupilEye2, 100);
}

void DisplaySemiCrossEyedWhileSomeoneClose()
{
  DisplayEyes( shiftyPupilEye2, shiftyPupilEye3, 100);
}

void DisplayShiftyEyesLookingLeftAndRight()
{
  DisplayEyes( shiftyPupilEye1, shiftyPupilEye1, 100);
  DisplayEyes( shiftyPupilEye2, shiftyPupilEye2, 100);
  DisplayEyes( shiftyPupilEye3, shiftyPupilEye3, 100);
  DisplayEyes( shiftyPupilEye4, shiftyPupilEye4, 100);
  DisplayEyes( shiftyPupilEye3, shiftyPupilEye3, 100);
  DisplayEyes( shiftyPupilEye2, shiftyPupilEye2, 100);
}

void DisplayShiftyEyesLookingInACircle()
{
  DisplayEyes( shiftyPupilEye1, shiftyPupilEye1, 100);
  DisplayEyes( shiftyPupilEye2, shiftyPupilEye2, 100);
  DisplayEyes( shiftyPupilEye3, shiftyPupilEye3, 100);
  DisplayEyes( shiftyPupilEye4, shiftyPupilEye4, 100);
  DisplayEyes( shiftyPupilEye5, shiftyPupilEye5, 100);
  DisplayEyes( shiftyPupilEye6, shiftyPupilEye6, 100);
  DisplayEyes( shiftyPupilEye7, shiftyPupilEye7, 100);
  DisplayEyes( shiftyPupilEye8, shiftyPupilEye8, 100);
}

void DisplayShiftyEyesLookingLeftAndRightWhileBlinkingRight()
{
  DisplayEyes( shiftyPupilEye1,        shiftyPupilEye1, 100);
  DisplayEyes( shiftyPartialClosedEye, shiftyPupilEye1, 100);
  DisplayEyes( shiftyClosedEye,        shiftyPupilEye2, 100);
  DisplayEyes( shiftyPartialClosedEye, shiftyPupilEye3, 100);
  DisplayEyes( shiftyPupilEye4,        shiftyPupilEye4, 100);
  DisplayEyes( shiftyPupilEye3,        shiftyPupilEye3, 100);
  DisplayEyes( shiftyPupilEye2,        shiftyPupilEye2, 100);
}

void DisplayShiftyEyesLookingLeftAndRightWhileBlinkingLeft()
{
  DisplayEyes( shiftyPupilEye1, shiftyPupilEye1,        100);
  DisplayEyes( shiftyPupilEye1, shiftyPartialClosedEye, 100);
  DisplayEyes( shiftyPupilEye2, shiftyClosedEye,        100);
  DisplayEyes( shiftyPupilEye3, shiftyPartialClosedEye, 100);
  DisplayEyes( shiftyPupilEye4, shiftyPupilEye4,        100);
  DisplayEyes( shiftyPupilEye3, shiftyPupilEye3,        100);
  DisplayEyes( shiftyPupilEye2, shiftyPupilEye2,        100);
}

void DisplayShiftyEyesLookingLeftAndRightWhileBlinkingBoth()
{
  DisplayEyes( shiftyPupilEye1,        shiftyPupilEye1,        100);
  DisplayEyes( shiftyPartialClosedEye, shiftyPartialClosedEye, 100);
  DisplayEyes( shiftyClosedEye,        shiftyClosedEye,        100);
  DisplayEyes( shiftyPartialClosedEye, shiftyPartialClosedEye, 100);
  DisplayEyes( shiftyPupilEye4,        shiftyPupilEye4,        100);
  DisplayEyes( shiftyPupilEye3,        shiftyPupilEye3,        100);
  DisplayEyes( shiftyPupilEye2,        shiftyPupilEye2,        100);
}

/*********************
 * Surprised Eyes    *
 *********************/
void DisplaySurprisedEyesIfNoOneClose()
{
  DisplayShiftyToSurprisedEyes();

  float distance = GetDistance();
  int photocellReading = ReadLightSensor();
  int randomNumber = GetRandomPercentage();

  for (int i = 0; i < NumEyesShown && distance >= SemiCrossEyedDistance && photocellReading >= SleepThreshold && randomNumber < ShowEyeDurationRandomiser; i++)
  {
    DisplaySurprisedEyes();
    distance = GetDistance();
    photocellReading = ReadLightSensor();
  }

  DisplaySurprisedToShiftyEyes();
}

void DisplaySurprisedEyes()
{
  DisplayEyes( surprisedEye, surprisedEye, 100);
}

void DisplayShiftyToSurprisedEyes()
{
  DisplayEyes( shiftyEye,         shiftyEye,         100);
  DisplayEyes( shiftyToSurprised, shiftyToSurprised, 100);
  DisplayEyes( surprisedEye,      surprisedEye,      100);
}

void DisplaySurprisedToShiftyEyes()
{
  DisplayEyes( surprisedEye,      surprisedEye,      100);
  DisplayEyes( shiftyToSurprised, shiftyToSurprised, 100);
  DisplayEyes( shiftyEye,         shiftyEye,         100);
}

/*********************
 * Happy Eyes        *
 *********************/
void DisplayHappyEyesIfNoOneClose()
{
  DisplayShiftyToHappyEyes();

  float distance = GetDistance();
  int photocellReading = ReadLightSensor();
  int randomNumber = GetRandomPercentage();

  for (int i = 0; i < NumEyesShown && distance >= SemiCrossEyedDistance && photocellReading >= SleepThreshold && randomNumber < ShowEyeDurationRandomiser; i++)
  {
    DisplayHappyEyes();
    distance = GetDistance();
    photocellReading = ReadLightSensor();
  }

  DisplayHappyToShiftyEyes();
}

void DisplayHappyEyes()
{
  DisplayEyes( happyEye, happyEye, 100);
}

void DisplayShiftyToHappyEyes()
{
  DisplayEyes( shiftyEye,     shiftyEye,     100);
  DisplayEyes( shiftyToHappy, shiftyToHappy, 100);
  DisplayEyes( happyEye,      happyEye,      100);
}

void DisplayHappyToShiftyEyes()
{
  DisplayEyes( surprisedEye,   surprisedEye,  100);
  DisplayEyes( shiftyToHappy,  shiftyToHappy, 100);
  DisplayEyes( shiftyEye,      shiftyEye,     100);
}


/*********************
 * Sad Eyes        *
 *********************/
void DisplaySadEyesIfNoOneClose()
{
  DisplayShiftyToSadEyes();

  float distance = GetDistance();
  int photocellReading = ReadLightSensor();
  int randomNumber = GetRandomPercentage();

  for (int i = 0; i < NumEyesShown && distance >= SemiCrossEyedDistance && photocellReading >= SleepThreshold && randomNumber < ShowEyeDurationRandomiser; i++)
  {
    DisplaySadEyes();
    distance = GetDistance();
    photocellReading = ReadLightSensor();
  }

  DisplaySadToShiftyEyes();
}

void DisplaySadEyes()
{
  DisplayEyes( sadEye, sadEye, 100);
}

void DisplayShiftyToSadEyes()
{
  DisplayEyes( shiftyEye,   shiftyEye,   100);
  DisplayEyes( shiftyToSad, shiftyToSad, 100);
  DisplayEyes( sadEye,      sadEye,      100);
}

void DisplaySadToShiftyEyes()
{
  DisplayEyes( sadEye,       sadEye,      100);
  DisplayEyes( shiftyToSad,  shiftyToSad, 100);
  DisplayEyes( shiftyEye,    shiftyEye,   100);
}

/*********************
 * Cry Eyes          *
 *********************/
void DisplayCryEyesIfNoOneClose()
{
  DisplayShiftyToCryEyes();

  float distance = GetDistance();
  int photocellReading = ReadLightSensor();
  int randomNumber = GetRandomPercentage();

  for (int i = 0; i < NumEyesShown && distance >= SemiCrossEyedDistance && photocellReading >= SleepThreshold && randomNumber < ShowEyeDurationRandomiser; i++)
  {
    DisplayCryEyes();
    distance = GetDistance();
    photocellReading = ReadLightSensor();
  }

  DisplayCryToShiftyEyes();
}

void DisplayCryEyes()
{
  DisplayEyes( cryEye,      cryEye,      100);
  DisplayEyes( shiftyToCry, shiftyToCry, 100);
  DisplayEyes( shiftyEye,   shiftyEye,   100);
  DisplayEyes( shiftyToCry, shiftyToCry, 100);
  DisplayEyes( cryEye,      cryEye,      100);
}

void DisplayShiftyToCryEyes()
{
  DisplayEyes( shiftyEye,   shiftyEye,   100);
  DisplayEyes( shiftyToCry, shiftyToCry, 100);
  DisplayEyes( cryEye,      cryEye,      100);
}

void DisplayCryToShiftyEyes()
{
  DisplayEyes( cryEye,       cryEye,      100);
  DisplayEyes( shiftyToCry,  shiftyToCry, 100);
  DisplayEyes( shiftyEye,    shiftyEye,   100);
}


/*********************
 * Mad Eyes          *
 *********************/
void DisplayMadEyesIfNoOneClose()
{
  DisplayShiftyToMadEyes();

  float distance = GetDistance();
  int photocellReading = ReadLightSensor();
  int randomNumber = GetRandomPercentage();

  for (int i = 0; i < NumEyesShown && distance >= SemiCrossEyedDistance && photocellReading >= SleepThreshold && randomNumber < ShowEyeDurationRandomiser; i++)
  {
    DisplayMadEyes();
    distance = GetDistance();
    photocellReading = ReadLightSensor();
  }

  DisplayMadToShiftyEyes();
}

void DisplayMadEyes()
{
  DisplayEyes( madEyeLeft, madEyeRight, 100);
}

void DisplayShiftyToMadEyes()
{
  DisplayEyes( shiftyEye,       shiftyEye,        100);
  DisplayEyes( madLeftToShifty, madRightToShifty, 100);
  DisplayEyes( madEyeLeft,      madEyeRight,      100);
}

void DisplayMadToShiftyEyes()
{
  DisplayEyes( madEyeLeft,      madEyeRight,      100);
  DisplayEyes( madLeftToShifty, madRightToShifty, 100);
  DisplayEyes( shiftyEye,       shiftyEye,        100);
}


/*********************
 * Common            *
 *********************/
void DisplayEyes( EyeDataType leftEyeData, EyeDataType rightEyeData, int delayMs )
{
  {
    DisplayEye(LeftEyePos, leftEyeData);
    DisplayEye(RightEyePos, rightEyeData);

    delay(delayMs);
  }
}

void DisplayEye( int eyePos, EyeDataType eyeArrary )
{
  for (int j = 0; j < LedCols; j++)
  {
    // Note: setRow(addr,row,value)
    lc.setRow(eyePos, j, eyeArrary.EyeData[j]);
  }
}

// Return a number between 0 and 99;
int GetRandomPercentage()
{
  int result = rand() % 100;
  return result;
}

/*********************
 * Distance          *
 *********************/

float GetDistance()
{
  const float TooClose = 2.00;
  const float OutOfRange = 450.00;

  unsigned long duration;
  float result;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(EchoPin, HIGH);
  result = MicrosecondsToCentimeters(duration);

  Serial << "Distance: " << result << "cm";
  
  if (result <= TooClose)
  {
    Serial << " - too close";
  }
  else if (result >= OutOfRange)
  {
    Serial << " - out of range";
  }

  Serial << endl;
  
  return result;
}

// The speed of sound is 340.29 m/s or 29.3866996 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
float MicrosecondsToCentimeters(long microseconds)
{
  const float speedOfSoundMicroSecondsperCentimeter = 29.3866996;

  return microseconds / speedOfSoundMicroSecondsperCentimeter / 2;  // Divide by two as the sound has travelled to the object and back again.
}


/*********************
 * Light Sensor      *
 *********************/
int ReadLightSensor()
{
  int result = analogRead(PhotocellPin);
  Serial << "Light Sensor reading: = " << result << endl;     // the raw analog reading

  return result;
}

void ChangeIntensityFromTo(int curIntensity, int newIntensity)
{
  if (curIntensity < newIntensity)
  {
    for (int i = curIntensity; i <= newIntensity ; i++)
    {
      SetIntensity(i);
      delay(100);
    }
  }
  else
  {
    for (int i = curIntensity; i >= newIntensity ; i--)
    {
      SetIntensity(i);
      delay(100);
    }
  }

}

void SetIntensity(int intensity)
{
  for (int i = 0; i < NumDevices ; i++)
  {
    /* Set the brightness to a medium values */
    lc.setIntensity(i, intensity);
  }
}


/**********************
 * Not used currently *
 **********************/
void DisplayTransition( TwoEyeTransistionType twoEyeTransition )
{
  DisplaytwoEyes(twoEyeTransition.TwoEyeData1, 0);
  delay(twoEyeTransition.Delay1);

  DisplaytwoEyes(twoEyeTransition.TwoEyeData2, 0);
  delay(twoEyeTransition.Delay2);

  DisplaytwoEyes(twoEyeTransition.TwoEyeData3, 0);
  delay(twoEyeTransition.Delay3);
}

void DisplaytwoEyes( TwoEyeDataType twoEyeData, int delayMs )
{
  {
    DisplayEye(LeftEyePos, twoEyeData.LeftEye);
    DisplayEye(RightEyePos, twoEyeData.RightEye);

    delay(delayMs);
  }
}

