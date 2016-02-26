/*
 * Laser trip wire
 * v 1.3
 * by: Keith Kay
 * 1/31/2013
 * CC by-sa v3.0 - http://creativecommons.org/licenses/by-sa/3.0/
 * http://keithkay.com
 */

#include <StackArray.h>
 
// minimum difference needed between ambient light level and laser calibration
const int min_diff = 400;    

// Larger values make it LESS sensitive
const int LASER_SENSITIVITY = 30;  

const int AMBIENT_SAMPLE_DELAYSTART = 400;
const int NUM_AMBIENT_SAMPLES = 10;
const int TOTAL_LASER_SAMPLES = 15;
const int GOOD_LASER_SAMPLES = 5;

// first define the constants used for sensor / emitter pins
const int triggeredLED = 7;  // pin for the warning light LED
const int RedLED = 3;        // pin for the 'armed' state indicator
const int GreenLED = 4;      // pin for the 'un-armed' state indicator
const int laserLED = 13;
const int inputPin = A1;     // pin for analog input
const int speakerPin = 12;   // pin for the speaker output
const int armButton = 11;    // pin for the arming button

// define variables used for readings and programatic control
boolean isArmed = false;      // variable for the armed state
boolean isTriggered = false; // has the wire been tripped
int threshold = 0;           // variable set by the calibration process

// constants used for the siren
const int lowrange = 2000;   // the lowest frequency value to use
const int highrange = 4000;  //  the highest...

void setup(){
  // configure LEDs for output
  pinMode(triggeredLED, OUTPUT);
  pinMode(RedLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);
  pinMode(laserLED, OUTPUT);

  //configure the button for input
  pinMode(armButton, INPUT);

  // set initial state
  setLaser( false ) ;
  setTriggeredLED( false ) ;
  setRedLED( false ) ;
  setGreenLED( false ) ;

  // for debugging and calibration review
  Serial.begin(57600);
  Serial.println("");
  Serial.println("Initializing...");

  // intial 'test' to be sure all LEDs and the speaker are working
  setTriggeredLED( true ) ;  delay(500); setTriggeredLED( false) ;
  setRedLED( true ) ;  delay(500); setRedLED( false) ;
  setGreenLED( true ) ;  delay(500);  setGreenLED( false) ;
  setLaser( true ) ;  delay(500);  setLaser( false ) ;  
  tone(speakerPin, 220, 125);

  // start unarmed
  setArmedState(false);  

  // calibrate the laser light level in the current environment
  calibrate();
}

void loop(){
  // check to see if the button is pressed
  int buttonVal = digitalRead(armButton);  
  if ( buttonVal == HIGH && !isArmed ) {
    Serial.println("Now arming!");
    setArmedState( true );
    delay(500);
  }

  if( !isArmed )  // If not armed, just end the loop
    return ;
    
  // read the LDR sensor
  int sample = analogRead(inputPin);
  //Serial.print("Sample = ");  Serial.println(sample);
  
  // check to see if the laser beam is interrupted based on the threshold
  if ( sample < threshold ) {
    Serial.print("Triggering Sample: " ) ; Serial.println( sample ) ;
    isTriggered = true;
  }
    
  if (isTriggered) {
    // Play one sweep of the "Siren"
    for (int i = lowrange; i <= highrange; i++) {
      tone (speakerPin, i, 250);  // increasing tone
    }
    for (int i = highrange; i >= lowrange; i--) {
      tone (speakerPin, i, 250);  // decreasing tone
    }
    
    // flash LED
    setTriggeredLED( true ) ; delay(10); setTriggeredLED( false ) ;
  }
  
  // short delay - if you are debugging a modification you may want to increase this to slow down the serial readout
  delay(20);
}

// function to flip the armed state of the trip wire
void setArmedState( bool setArmed ){
  setGreenLED( !setArmed ) ;
  setRedLED( setArmed ) ;
  setLaser( setArmed ) ;
  isTriggered = false ;
  isArmed = setArmed ;

  if (!setArmed){
    Serial.println("Disarmed") ;
  } else {
    tone(speakerPin, 220, 125);
    delay(200);
    tone(speakerPin, 196, 250);
    Serial.println("Armed") ;
  } 
}

void calibrate(){
  StackArray<int> stack;
  
  int sample = 0;              
  unsigned int sampleTotal = 0;
  int baseline = 0;            
  
  setLaser( false ) ;     // ensure Laser is off
  
  // Take the average of N readings for our ambient light baseline
  sampleTotal = 0;
  delay(AMBIENT_SAMPLE_DELAYSTART) ;  // Considerable bounce when first reading
  Serial.println("Sampling ambient light levels") ;
  for (int i=0; i<NUM_AMBIENT_SAMPLES; i++){
    sample = analogRead(inputPin); 
    Serial.print("Ambient Sample: " ) ;  Serial.println( sample ) ;

    sampleTotal += sample ; // take reading and add it to the sample

    setGreenLED( true ) ;
    delay (50); // delay to blink the LED and space readings
    setGreenLED( false ) ;
    delay (50); // delay to blink the LED and space readings
  }
  baseline = sampleTotal / NUM_AMBIENT_SAMPLES;
  Serial.print("Ambient baseline = ");  Serial.println(baseline);  


  Serial.println("Sampling Laser-captured light levels.  Focus now...") ;
  setLaser( true ) ;  // Turn on laser
  sampleTotal = 0;
  do
  {
    sample = analogRead(inputPin);      
//    Serial.print("Laser-On Sample: " ) ;  Serial.println( sample ) ;
    
    if (sample > baseline + min_diff){
//      Serial.print("In-Tolerance Sample: " ) ;  Serial.println( sample ) ;
      stack.push(sample) ;
      
      setGreenLED( true ) ;
      delay (200);                     // delay to blink the LED and space readings
      setGreenLED( false ) ;
      delay (200);                     // delay to blink the LED and space readings
    } else {
      if( !stack.isEmpty() ) {
        tone(speakerPin, 190, 125);
        Serial.print( "Reset...  Sample=" ) ; Serial.println( sample ) ;
        while( !stack.isEmpty() ) stack.pop() ;  // Clear out the stack when reading drops
      }
    }
  } while (stack.count() < TOTAL_LASER_SAMPLES);  // Need 'N' in-tolerance samples in a row

  for( int n=0; n < GOOD_LASER_SAMPLES; n++ )
        sampleTotal += stack.pop() ;

  //lastly we need to correctly set the threshold as it now hold the sum of 3 samples
  threshold = (sampleTotal/GOOD_LASER_SAMPLES) - LASER_SENSITIVITY;
  
  // play the arming tone and in reverse and print the threshold to confirm threshold set
  tone(speakerPin, 196, 250);
  delay(200);
  tone(speakerPin, 220, 125);

  Serial.print("Trigger threshold= ");  Serial.println(threshold);

  setLaser( false ) ;
}    

void setLaser( bool setOn ) {
  if( setOn ) digitalWrite(laserLED, HIGH);  
  else        digitalWrite(laserLED, LOW);    
}
void setGreenLED( bool setOn ) {
  if( setOn ) digitalWrite(GreenLED, HIGH);  
  else        digitalWrite(GreenLED, LOW);    
}
void setRedLED( bool setOn ) {
  if( setOn ) digitalWrite(RedLED, HIGH);  
  else        digitalWrite(RedLED, LOW);    
}
void setTriggeredLED( bool setOn ) {
  if( setOn ) digitalWrite(triggeredLED, HIGH);  
  else        digitalWrite(triggeredLED, LOW);    
}

