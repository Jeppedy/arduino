void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  pinMode(12, OUTPUT);
  digitalWrite(13, LOW);

  // for debugging and calibration review
  Serial.begin(57600);
  Serial.println("");
  Serial.println("Initializing...");


  toggleLaser() ;

}

void loop() {
  static int prevReading = -1;
  int photocellReading ;

  photocellReading = analogRead(1);  
//    Serial.println( photocellReading );    
  if( prevReading == -1 ) {
    prevReading = photocellReading ;
  } else {
    if( photocellReading < prevReading - 200 ) {
      Serial.print( "Beam BROKEN:" );
      Serial.print( prevReading ) ;
      Serial.print(" ") ;
      Serial.println( photocellReading ) ;
    } else if( photocellReading > prevReading + 200 ) {
      Serial.println( "Beam Restored:" );
      Serial.print( prevReading ) ;
      Serial.print(" ") ;
      Serial.println( photocellReading ) ;
    }
  }
  prevReading = photocellReading ;

//  for( int i = 0; i<=10; i++ ) {
//    photocellReading = analogRead(1)/10;  
//    Serial.println( photocellReading );    
//    delay(100);
//  }

//  for (long i = 0; i < 2048 * 1; i++ ) 
//  {
//     // 1 / 2048Hz = 488uS, or 244uS high and 244uS low to create 50% duty cycle
//     digitalWrite(12, HIGH);
//     delayMicroseconds(144);
//     digitalWrite(12, LOW);
//     delayMicroseconds(344);
//  }

  delay(50);
}

void toggleLaser( void ) {
  static int state = HIGH ;
  digitalWrite(13, state);
  state = (state == LOW ? HIGH : LOW ) ;
  
}

