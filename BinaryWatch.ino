// Section 1 is hours/minutes depending on the mode
const int section1Pins = {0,1,2,3,4,5,6};
// Section 2 is minutes/seconds depending on the mode
const int[] section2Pins = {7,8,9,10,11};
// Length of binary data representations (number of bits)
int arrayLen = 6;

// Number of ms in a second
const int MS_IN_SECOND = 1000;
// Number of ms in a minute
const int MS_IN_MINUTE = 60000;

// Using analog to free up digital pins
const int toggleButtonPin = A0; // Button to start/stop progress
const int section1ButtonPin = A1; // Button to modify section 1 value
const int section2ButtonPin = A2; // Button to modify section 2 value
// Potentiometer to control mode
const int modeSelectorPin = A1;

// Variables for clock mode
int ckHours = 0;
int ckMinutes = 0;
int ckLastUpdatedTime = 0;

// Variables for timer mode
int tmMinutes = 0;
int tmSeconds = 0;
int tmLastUpdatedTime = 0;
bool tmStopped = true;

// Variables for stopwatch mode
int swMinutes = 0;
int swSeconds = 0;
int swLastUpdatedTime = 0;
bool swStopped = true;

// Enumeration to keep track of watch mode/state
enum State : int {
  timeClock = 0,
  stopwatch = 1,
  timer = 2
}

// Track current state - starts off on clock mode
State currentState = timeClock;

// Set up pins
void setup() {
  // Set the LED pins as outputs to enable 5V output
  for (int i = 0; i < arrayLen; i++) {
    pinMode(section1Pins[i], OUTPUT);
    pinMode(section2Pins[i], OUTPUT);
  }
}

void loop() {

  // Get current mode reading and update if necessary
  int modeSelectorRawData = analogRead(modeSelectorPin);
  int modeSelectorMapped = map(modeSelectorRawData,0,1024,0,2);

  if (modeSelectorMapped != currentState) {
    changeState(modeSelectorMapped);
  }

  // Get button readings and update if necessary
  bool sec1ButtonPressed = digitalRead(section1ButtonPin);
  bool sec2ButtonPressed = digitalRead(section2ButtonPin);

  if (sec1ButtonPressed) {editSection1();}
  if (sec2ButtonPressed) {editSection2();}
  
  // Update values and displays based on mode
  timeStepRouter();

}

// Calls the correct updation function for the current mode
void timeStepRouter() {
   switch (currentState) {
    case timeClock:
      ckUpdate();
      break;
    case stopwatch:
      swUpdate();
      break;
    case timer:
      tmUpdate();
      break; 
  }
}

void ckUpdate() {
  // If it's been more than a minute since the last update
}

void swUpdate() {
  // If it's been more than a second since the last update
}

void tmUpdate() {
  // If it's been more than a second since the last update
}

// Edit button for section 1 - reset button for the stopwatch
void editSection1() {
  if (currentState = timeClock) {
    ckHours++;
    if (ckHours >= 24) {
      ckHours = ckHours-24;    
    }
  } else if (currentState == stopwatch) {
    swMinutes = 0;
    swSeconds = 0;
    swLastUpdatedTime = 0;
    swStopped = true;
  } else if (currentState == timer) {
    tmMinutes++;
    if (tmMinutes >= 60) {
      tmMinutes = tmMinutes-60;    
    }
  }
}

// Edit button for section 2 - another reset button for the stopwatch
void editSection2() {
  if (currentState = timeClock) {
    ckMinutes++;
    if (ckMinutes >= 60) {
      ckHours = ckHours-60;    
    }
  } else if (currentState == stopwatch) {
    swMinutes = 0;
    swSeconds = 0;
    swLastUpdatedTime = 0;
    swStopped = true;
  } else if (currentState == timer) {
    tmSeconds++;
    if (tmSeconds >= 60) {
      tmSeconds = tmSeconds-60;    
    }
  }
}

// Stop button - doesn't do anything in clock mode
void stopButton() {
  switch (currentState) {
    case timeClock:
      break;
    case stopwatch:
      swStopped = !swStopped;
      break;
    case timer:
      tmStopped = !tmStopped;
      break; 
  }
}

// Sets a new state and clears out old values
void changeState(State newState) {
  // Clear out all values
  ckHours = 0;
  kMinutes = 0;
  ckLastUpdatedTime = 0;
  tmMinutes = 0;
  tmSeconds = 0;
  tmLastUpdatedTime = 0;
  tmStopped = true;
  swMinutes = 0;
  swSeconds = 0;
  swLastUpdatedTime = 0;
  swStopped = true;

  // Wipe all pin data
  for (int i = 0; i < arrayLen; i++) {
    digitalWrite(section1Pins[i], LOW);
    digitalWrite(section2Pins[i], LOW);
  }
  
  // Update the state
  currentState = newState;
}


// Converts a number to binary in arraay
int[] binaryRepresentation(int num) {
  int binary[arrayLen] = {0}; // 6 bits of data needed
  int quotient = num;
  while (quotient != 0) {
    binary[--arrayLen] = quotient % 2;
    quotient = quotient/2; // Calculate next quotient
  }
  return arrayLen;
}

// Writes binary data to specified output pins
void writeBinaryData(int[] pins, int[] data) {
  for (int i = 0; i < arrayLen; i++) {
    if (data[i] == 1) {
      digitalWrite(pins[i], HIGH);
    } else {
      digitalWrite(pins[i], LOW);
    }
  }
}

