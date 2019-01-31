/* Gyan Prayaga. Independent Study Project Spring 2017 */

/* Motor */
int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;     //M1 Direction Control
int M2 = 7;     //M1 Direction Control

/* Ultrasound */
int trigPin = 11;    //Trig - green Jumper
int echoPin = 12;    //Echo - yellow Jumper
long duration, cm, inches;

int pulse_delay = 100;

// steps table --> this lets us create a procession of navigational tasks
// unfortunately mixed arrays are not supported in C :/

/*
   Motion types:
   1: Forward motion
   2: Turn left

   Important note --> stepes should always be multiples of 250


*/

int steps[3][2] = {
  {1, 10000},   /*  motion type, microseconds */
  {2, 2100},
  {1, 10000}
};

// start us off on the first index (0)
int current_step = -1;

// currently in motion (check this in our nav function)
int in_motion = 0;

// counter starts at 0, which will cause navigate() to move us into step 0 when called in setup()
int counter = 0;

/* Motor directive functions */
void stop(void)                    //Stop
{
  digitalWrite(E1, 0);
  digitalWrite(M1, LOW);
  digitalWrite(E2, 0);
  digitalWrite(M2, LOW);

  // we have stopped
  in_motion = 0;
}
void advance(char a, char b)         //Move forward
{
  analogWrite (E1, a);     //PWM Speed Control
  digitalWrite(M1, HIGH); // High/Low are binaries (HIGH means positive direction)
  analogWrite (E2, b);
  digitalWrite(M2, HIGH);

  // we are moving
  in_motion = 1;
}
void back_off (char a, char b)         //Move backward
{
  analogWrite (E1, a);
  digitalWrite(M1, LOW);
  analogWrite (E2, b);
  digitalWrite(M2, LOW);

  // we are moving
  in_motion = 1;
}
void turn_L (char a, char b)            //Turn Left
{
  analogWrite (E1, a);
  digitalWrite(M1, LOW);
  analogWrite (E2, b);
  digitalWrite(M2, HIGH);

  // we are moving
  in_motion = 1;
}
void turn_R (char a, char b)            //Turn Right
{
  analogWrite (E1, a);
  digitalWrite(M1, HIGH);
  analogWrite (E2, b);
  digitalWrite(M2, LOW);
}

// handles navigation memory
void navigate() {

  Serial.print("Counter: " + String(counter));

  if (!counter) {
    // WE GOT TO ZERO! Time to move to next task :)
    Serial.print("Step countdown expired. Moving to next task...\n");

    // we are not moving
    in_motion = 0;

    // countdown = 0, so move onto next task
    current_step = current_step + 1;

    // get the countdown for this step
    int countdown = steps[current_step][1];

    // set the relative counter based on the time limit for the step
    counter = countdown;
  }
  else {
    // LETS DRIVE!

    if (in_motion) {
      // do noting, don't want to re-call advance() function
      Serial.print("navigate() called during motion. Not re-calling any motion functions.\n");
    }
    else {

      // run the motor directive for our current step
      int motor_directive = steps[current_step][0];

      switch (motor_directive) {
        case 1:
          // Overview: drive forward for 10 seconds
          Serial.print("Starting or resuming task: Driving forward for 10 secs...\n");
          stop();
          back_off(254, 255);
          break;
        case 2:
          // Overview: make a U-turn
          Serial.print("Starting or resuming task: Making a U-turn...\n");
          stop();
          turn_L(150, 150);
          break;
        default:
          Serial.print("Invalid directive called -- does not work...\n");
          // invalid directive, do nothing
          break;
      }

    }

  }
}



/* Main thread */
void setup() {
  //Serial Port begin --> this should be bluetooth for wireless comm between smartphone & tank
  // this port should be "115200"
  Serial.begin (9600);

  // Define ultrasounic inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // lets go!
  navigate();
}

void loop()
{


  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  cm = (duration / 2) / 29.1;
  inches = (duration / 2) / 74;

  Serial.print("distance" + String(cm));

  if (cm < 20) {
    // detected obstacle
    Serial.print("Object detected! Stopping now...\n");
    stop();

    // dont increment counter, because nav step has ceased
  }
  else {

    // (!ISSUE although ultrasound works, the advance() function keep getting restarted.
    // we need to build in memory so it doesn't get distracted from its current task
    // and presets...

    // lets nav at our current relative time
    navigate();
    //    advance(254,255);

    //    navigate(counter)

    //      if (!navigational_step) {
    //        Serial.print("Begin navigation...");
    //
    //        // what are we doing here? lay out a path
    //        navigate(navigational_step);
    //      }
    //      else {
    //        Serial.print("Resume navigation...");
    //        navigate(0);
    //      }

    // increment counter
    counter = counter - pulse_delay;

  }

  delay(pulse_delay);
}
