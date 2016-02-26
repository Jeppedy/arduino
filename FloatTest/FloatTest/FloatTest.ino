
// printFloat prints out the float 'value' rounded to 'places' places after the decimal point
void printFloat(float value, int places) {
  // this is used to cast digits 
  int digit;
  float tens = 0.1;
  int tenscount = 0;
  int i;
  float tempfloat = value;

    // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
  // if this rounding step isn't here, the value  54.321 prints as 54.3209

  // calculate rounding term d:   0.5/pow(10,places)  
  float d = 0.5;
  if (value < 0)
    d *= -1.0;
  // divide by ten for each decimal place
  for (i = 0; i < places; i++)
    d/= 10.0;    
  // this small addition, combined with truncation will round our values properly 
  tempfloat +=  d;

  // first get value tens to be the large power of ten less than value
  // tenscount isn't necessary but it would be useful if you wanted to know after this how many chars the number will take

  if (value < 0)
    tempfloat *= -1.0;
  while ((tens * 10.0) <= tempfloat) {
    tens *= 10.0;
    tenscount += 1;
  }


  // write out the negative if needed
  if (value < 0)
    Serial.print('-');

  if (tenscount == 0)
    Serial.print(0, DEC);

  for (i=0; i< tenscount; i++) {
    digit = (int) (tempfloat/tens);
    Serial.print(digit, DEC);
    tempfloat = tempfloat - ((float)digit * tens);
    tens /= 10.0;
  }

  // if no places after decimal, stop now and return
  if (places <= 0)
    return;

  // otherwise, write the point and continue on
  Serial.print('.');  

  // now write out each decimal place by shifting digits one by one into the ones place and writing the truncated value
  for (i = 0; i < places; i++) {
    tempfloat *= 10.0; 
    digit = (int) tempfloat;
    Serial.print(digit,DEC);  
    // once written, subtract off that digit
    tempfloat = tempfloat - (float) digit; 
  }
}




void setup() {
   Serial.begin(57600); 
}

void loop() {

  // ==========================================
  // now run a series on the printFloat function

    // looking at the precision of the float
  Serial.println("printFloat(1000000.321 , 5);");
  printFloat(1000000.321, 5); 
  Serial.println();
  Serial.println("printFloat(100000.321 , 5);");
  printFloat(100000.321, 5); 
  Serial.println();
  Serial.println("printFloat(10000.321 , 5);");
  printFloat(10000.321, 5); 
  Serial.println();  
  Serial.println("printFloat(1000.321 , 5);");
  printFloat(1000.321, 5); 
  Serial.println();
  Serial.println("printFloat(100.321 , 5);");
  printFloat(100.321, 5); 
  Serial.println();

  // ==========================================
  // looking at effect of changing precision
  Serial.println("printFloat(100000.321 , 6);");
  printFloat(100000.321, 6);
  Serial.println();
  Serial.println("printFloat(100000.321 , 7);");
  printFloat(100000.321, 7);
  Serial.println();
  Serial.println("printFloat(100000.321 , 8);");
  printFloat(100000.321, 8);
  Serial.println();
  Serial.println("printFloat(100000.321 , 9);");
  printFloat(100000.321, 9);
  Serial.println();


  // ==========================================

  // check negatives and rounding, and some edge cases
  Serial.println("printFloat(-5004.321 , 5);");
  printFloat(-5004.321 , 5); 
  Serial.println();

  Serial.println("printFloat(99.999 , 3); ");
  printFloat(99.999 , 3); 
  Serial.println();

  Serial.println("printFloat(100, 1);");
  printFloat(100, 1); 
  Serial.println();

  Serial.println("printFloat(-100.999 , 1);");
  printFloat(-100.999 , 1); 
  Serial.println();

  Serial.println("printFloat(-54.321 , 0);");
  printFloat(-54.321 , 0); 
  Serial.println();

  Serial.println("printFloat(0.321 , 5);");
  printFloat(0.321 , 5);    
  Serial.println();

  Serial.println("printFloat(-1.0001 , 5);");
  printFloat(-1.0001 , 5);   
  Serial.println();

  Serial.println("printFloat(-0.000001 , 5);");
  printFloat(-0.000001 , 5); 
  Serial.println();

  Serial.println("printFloat(0.000001 , 5);");
  printFloat(0.000001 , 5); 
  Serial.println();

  Serial.println("printFloat(-0.00001 , 5);");
  printFloat(-0.00001 , 5); 
  Serial.println();

  Serial.println("printFloat(0.000099 , 5);");
  printFloat(0.000099 , 5); 
  Serial.println();

  // alternative technique that depends on <stdio.h>
  //Serial.println("Serial.println(dtostrf(-1.0001, 2, 5, s));");
  //char s[32];
 // Serial.println(dtostrf(-1.0001, 2, 5, s));

  delay(1000);
  Serial.println();

}
