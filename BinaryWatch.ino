// Section 1 is hours/minutes depending on the mode
const int section1Pins = {0,1,2,3,4,5,6};
// Section 2 is minutes/seconds depending on the mode
const int[] section2Pins = {7,8,9,10,11};

// Pizeo pin for buzzer
const int piezoPin = A4;

// Using analog to free up digital pins
const int toggleButtonPin = A0; // Button to start/stop progress
const int section1ButtonPin = A1; // Button to modify section 1 value
const int section2ButtonPin = A2; // Button to modify section 2 value

// Potentiometer to control mode
const int modeSelectorPin = A3;

// Number of ms in a second
const int MS_IN_SECOND = 1000;
// Number of ms in a minute
const int MS_IN_MINUTE = 60000;
// Length of binary data representations (number of bits)
int arrayLen = 6;

// Variables for clock mode
int ckHours = 0; // Hours on clock
int ckMinutes = 0; // Minutes on clock
unsigned long ckLastUpdatedTime = 0;

// Variables for timer mode
int tmMinutes = 0; // Minutes left
int tmSeconds = 0; // Seconds left
unsigned long tmLastUpdatedTime = 0; // Time when "time" was last updated
bool tmStopped = true;

// Variables for stopwatch mode
int swMinutes = 0; // Minutes on stopwatc
int swSeconds = 0; // Seconds on stopwatch
unsigned long swLastUpdatedTime = 0; // Time when stopwatch display was last updated
bool swStopped = true;

// Enumeration to keep track of watch mode/state
enum State : int {
  timeClock = 0, // Clock mode
  stopwatch = 1, // Stopwatch mode
  timer = 2 // Timer mode
}

// Track current state - starts off on clock mode
State currentState = timeClock;

// Set up pins
void setup() {
  // Set all the LED pins as outputs to enable 5V output
  for (int i = 0; i < arrayLen; i++) {
    pinMode(section1Pins[i], OUTPUT);
    pinMode(section2Pins[i], OUTPUT);
  }
  // Set the piezo pin as an output
  pinMode(piezoPin, OUTPUT);
}

// Recurring commands
void loop() {

  // Get current mode reading and update if necessary
  int modeSelectorRawData = analogRead(modeSelectorPin); 
  // Map the data to a range of 0-2 to allow selecting between 3 modes
  int modeSelectorMapped = map(modeSelectorRawData,0,1024,0,2); 

  // Check if the mode has been changed
  if (modeSelectorMapped != currentState) {
    changeState(modeSelectorMapped); // Change the mode
  }

  // Get button readings
  bool sec1ButtonPressed = digitalRead(section1ButtonPin);
  bool sec2ButtonPressed = digitalRead(section2ButtonPin);

  // Edit the section corresponding to the button press
  if (sec1ButtonPressed) {editSection1();}
  if (sec2ButtonPressed) {editSection2();}
  
  // Update values and displays based on mode
  timeStepRouter();
}

// Calls the correct updation function for the current mode
void timeStepRouter() {
   // Determine which function to call based on the state
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

// Updation function for clock mode
void ckUpdate() {
  // If it's been more than a minute since the last update
  if (millis() - ckLastUpdatedTime > MS_IN_MINUTE) {

    // Increment hours and minutes
    ckHours++;
    ckMinutes++;

    // Get binary representation for each value
    int[] hoursBinaryRep = binaryRepresentation(ckSeconds);
    int[] minsBinaryRep = binaryRepresentation(ckMinutes);

    // Write the obtained binary representation for each value to the display
    writeBinaryData(section1Pins, hoursBinaryRep);
    writeBinaryData(section2Pins, minsBinaryRep);
    
    // Update the last updated time
    ckLastUpdatedTime = millis();
  }
  
}

void swUpdate() {
  // If it's been more than a second since the last update and the stopwatch is not stopped
  if (millis() - swLastUpdatedTime > MS_IN_SECOND && !swStopped) {
    
    // Increment minutes and seconds
    swMinutes++;
    swSeconds++;

    // Get binary representation for each value
    int[] minsBinaryRep = binaryRepresentation(swMinutes);
    int[] secsBinaryRep = binaryRepresentation(swSeconds);

    // Write the obtained binary representation for each value to the display
    writeBinaryData(section1Pins, minsBinaryRep);
    writeBinaryData(section2Pins, secsBinaryRep);

    // Update the last updated time
    swLastUpdatedTime = millis();
  }
}

// Update display for the timer function
void tmUpdate() {
  // If it's been more than a second since the last update and the timer is not stopped
  if (millis() - tmLastUpdatedTime > MS_IN_SECOND && !tmStopped) {

    // Decrement minutes and seconds
    tmMinutes--;
    tmSeconds--;
    
    // Get binary representation for each value
    int[] minsBinaryRep = binaryRepresentation(tmMinutes);
    int[] secsBinaryRep = binaryRepresentation(tmSeconds);

    // Get binary representation for each value
    writeBinaryData(section1Pins, minsBinaryRep);
    writeBinaryData(section2Pins, secsBinaryRep);

    // If the timer has expired, buzz the piezo
    if (tmMinutes == 0 && tmSeconds == 0) {
      digitalWrite(piezoPin, HIGH);
      delay(1000); // Wait 1 second before turning off buzzer
      digitalWrite(pizeoPin, LOW);
      tmStopped = true; // Mark the timer as stopped
    }

    // Update the last updated time
    tmLastUpdatedTime = millis();
  }
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
