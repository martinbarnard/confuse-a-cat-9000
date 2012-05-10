#include <Servo.h> 
 
Servo s1;  // create servo object to control a servo 
           // a maximum of eight servo objects can be created 
Servo s2;

int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  s1.attach(9);  // attaches the servo on pin 9 to the servo object 
  s2.attach(10);
} 
 
 
void loop() 
{ 
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    s1.write(pos);              // tell servo to go to position in variable 'pos' 
    s2.write(180 - pos);
    delay(10);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    s1.write(pos);              // tell servo to go to position in variable 'pos' 
    s2.write(180 - pos);
    delay(10);                       // waits 15ms for the servo to reach the position 
  } 
} 
