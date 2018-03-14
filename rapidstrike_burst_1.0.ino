////////////////////////////////////////////
// RapidStrike Burst Engine
// single shot, 3 round burst, Full auto supported
///////////////////////////////////////////

#include <Bounce2.h>

//Single fire werkt, en opmerkelijk genoeg: bij vast blijven houden gaat ie door in FA. Mogelijk handig?
//Mogelijk relaxter om een xp180 als pusher te zetten en de speed van de pusher te kunnen beinvloeden?
// Serial.write makes the program significantly laggy, system doesn't correctly hold back the pusher anymore. But, good for debugging. So comment the lines out once done.

// set constants
const int triggerPin = 2;
const int relayPin = 3;    
const int pusherPin = 4;
const int fireModePin = 5;

// set variables
int sequence = 0;
int pushCount = 0;
int fireMode = 3 ; // Single, Burst, FA (1,2,3)
int burstCount = 10; // Burst fire Count
int pusherPressedDown = 0;

Bounce pusher = Bounce();
Bounce trigger = Bounce();

void setup() {
  pinMode(triggerPin, INPUT_PULLUP);
  trigger.attach(triggerPin);
  trigger.interval(5);

  pinMode(pusherPin,INPUT_PULLUP);
  pusher.attach(pusherPin);
  pusher.interval(5);
  
  //pinMode(fireModePin, INPUT_PULLUP);
  digitalWrite(relayPin, HIGH); 
  pinMode(relayPin, OUTPUT);
  //Serial.begin(9600);

}

void loop() {
  
  // Read inputs at same time for the loop
  trigger.update();
  pusher.update();

  // Determine firemode. Only when not in a sequence.
  if(sequence == 0) {
      //fireMode = digitalRead(fireModePin);
      if(fireMode == 1) {
        burstCount = 1;
      } else if(fireMode == 2) {
        burstCount = 3;
      } 
      //determine whether trigger pull happened. Ignore trigger pull within sequence.
      if(trigger.fell()) {
        //Serial.println(" trigger push" );
        sequence = 1;
        digitalWrite(relayPin, LOW); // Start pushing.
      }
  }

  // determine sequence behavior, based on fireMode. <3 == single or burst behavior.  3 == Full Auto
  if(fireMode < 3) {
      if(sequence == 1) {
          //Pusher retracted
          if(pusher.fell()) {
            pushCount++;
          } 

          //Check whether burst should end
            if(pushCount >= burstCount) {
              digitalWrite(relayPin, HIGH);
              sequence = 0;
              pushCount = 0;
            }
      }
  } else if(fireMode == 3) {
    if(sequence == 1) {
       if(pusher.fell() && trigger.read() == 1) {
          digitalWrite(relayPin, HIGH);
          sequence = 0;
          pushCount = 0;
       }
    }
  }
}
