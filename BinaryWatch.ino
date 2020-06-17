// Section 1 is hours/minutes depending on the mode
int section1Pins = {0,1,2,3,4,5,6};
// Section 2 is minutes/seconds depending on the mode
int[] section2Pins = {7,8,9,10,11};
// Using analog to free up digital pins
int toggleButtonPin = A0; // Button to start/stop progress
int section1ButtonPin = A1; // Button to modify section 1 value
int section2ButtonPin = A2; // Button to modify section 2 value
// Potentiometer to control mode
int modeSelectorPin = A1;

// Enumeration to keep track of watch mode/state
enum State {
  timeClock = 0,
  stopwatch = 1,
  timer = 2
}

void setup() {
  // Check the potentiometer reading and set the mode

}

void loop() {
  // put your main code here, to run repeatedly:

}

// Update required values, clear out old data, clean 
void changeWatchMode(State newState) {
  
}

// Converts a number to binary in arraay
int[] binaryRepresentation(int num) {
  int arrayLen = 6;
  int binary[arrayLen]; // 6 bits of data needed
  int quotient = num;
  while (quptient != 0) {
    binary[--arrayLen] = quotient % 2;
    quotient = quotient/2; // Calculate next quotient
  }
  return arrayLen;
}
