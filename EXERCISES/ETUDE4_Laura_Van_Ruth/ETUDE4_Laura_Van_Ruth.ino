/**********************ETUDE 4 CART 360 2017*******************************
   1/ As in Etude 3, in Etude 4 you will still Play & Record Notes and capture the associated Note Duration. 
   Additionally, in the record() , Pauses between Notes are also captured - you need to implement. 
   2/In Playback Mode, you will NOW have the option to Store the Pattern of Notes you just recorded by pressing the Store Button - 
   3/ You will be able to store up to 16 Patterns (Hardcoded Limit). 
   4/ Loop Mode has been Deprecated and has been replaced by the new Retrieve Mode ( retrieveMode() ). 
   In Retrieve Mode, you will be able to Select a Stored Pattern and Retrieve it 
   - the Stored Pattern Number will be displayed on the 7-Segment Display. 
   To Play the Stored Pattern, you will Press the Store Button (Confirm Selection). 
   The Potentiometer, Shift Register and 7-Segment Display work in concert to support your intention:
   In Retrieve Mode, the Potentiometer selects Pattern ID -> Sends Pattern ID to Shift Register -> Shift Register to 7-Segment Display. 

The Work of Van Lee, Laura Hirsh and Ruth Johnstone
*/

#include "Constants.h"
#include "InterruptHandler.h"
#include "ShiftRegister.h"
#include "TestPattern.h"
#include "Timer.h"

/******** VARIABLES *****************************************************/
// counter for how many notes we have
int countNotes = 0;
// array to hold the notes played (for record/play mode)
int notes [MAX_NOTES];
int mode = 0; //

/******** VARIABLES FOR ETUDE 3 *****************************************************/
// NEW FOR ETUDE 3: array to hold durations
// will be a parallel array to the notes array -
// meaning that at index 0 of notes[] is stored the 1st frequency, and at index 0 of
// the durations array  is stored the 1st duration for THAT note....
int durations [MAX_NOTES];
int durations_In_Between[MAX_NOTES];

// boolean to determine if a note button was being pressed:
boolean activeNoteButton = false;

// keep track of the active frequency
int activeFrequency = 0;

// var to hold the note pressed
int testNote;

// var to hold a reference to the timer object used for finding the note durations
Timer t1;

// ** NEW :: this timer object YOU will need to use  for finding pauses BETWEEN notes
Timer t_between;

/**********************SETUP() DO NOT CHANGE*******************************/
// Declare pin mode for the single digital input
void setup()
{
  Serial.begin(9600);

  // INTERRUPT HANDLER
  setupInterruptHandler();

  // SHIFT REGISTER
  setupShiftRegister();
}


/**********************LOOP() *******************************
   INSTRUCTIONS:
   The loop establishes the flow of the program:
   If the mode Button has been pressed (modeChangeInterruptRequest boolean == true) then:
   1: chooseMode():  &&&
   2. YOU need to ensure that the  7-SEGMENT DISPLAY is off.
   Then  - regardless of if the button is pressed:
   3: setRGB(): will set the color of the RGB LED based on the value of the mode variable
   4: selectMode(): will determine which function to call based on the value of the mode variable

**************************************************************************/
void loop()
{
  //
  if (modeChangeInterruptRequest)
  {
    chooseMode();
    delay(DURATION * 2);

    modeChangeInterruptRequest = ISFALSE;

  }
    /* TO DO ::: TURN OFF THE 7-SEGMENT DISPLAY */ 

  setRGB();
  selectMode();

  digitalWrite(LATCH_PIN, LOW);
  
 shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, hexArray[32]^255); // clear the display (help with converting binary number from Martin of IMCA)
       digitalWrite(LATCH_PIN,HIGH); //pull the latchPin to save the data  
  
}



/******************CHOOSEMODE() -NOTHING TO CHANGE *********************************
   INSTRUCTIONS:
   Read the value from the Button (linked to the BUTTON_MODE_PIN) and
   if is being pressed then change the mode variable.
   REMEMBER:
   mode == 0 is reset
   mode == 1 is live
   mode == 2 is record
   mode == 3 is play
   mode == 4 is retrieve mode
   Every time the user presses the button, the program will go to the next mode,
   once it reaches 4, it should go back to mode == 0.
   (i.e. if mode ==2 and we press, then mode ==3) ...
**************************************************************************/
void chooseMode() {
  if (mode < 4) mode++;
  else mode = 0;
}


/**********************SELECTMODE() - NOTHING TO CHANGE *******************************
   INSTRUCTIONS:
   There is NO NEED to change this function - it selects WHICH function should run based on the mode variable
   There are 4 possibilities
   1: reset(): this function will reset any arrays etc, and will do nothing else
   2: live(): this function will play the corresponding notes
   to the user pressing the respective buttons.
   NOTE:: the notes played are NOT stored
   3: recordWithDuration(): this function will play the corresponding notes
   to the user pressing the respective buttons
   AND will STORE up to 16 consecutive notes in an array.
   4: playWithDuration(): this function will playback any notes stored in the array that were recorded
   in the previous mode AND allow the user to store the pattern.
   5: retrieve():

******************************************************************************/
void selectMode()
{
  if (mode == 0) {
    reset();
  }
  else if (mode == 1) {
    live();
  }
  else if (mode == 2) {
    recordWithDuration();
  }
  else if (mode == 3) {
    playWithDuration();
  }
  else if (mode == 4) {
    retreiveMode();
  }
}
/******************RESET() **************************************
   INSTRUCTIONS:
   this function should ensure that any notes recorded are no longer available
   ALSO: to IMPLEMENT: turn off the SEVEN SEGMENT DISPLAY
**************************************************************************/
void reset() {
  countNotes = 0;
  noTone(BUZZER_PIN);
  storeOnce = ISFALSE;
  /* TO DO:: TURN OFF THE 7-SEGMENT DISPLAY */ 

 //memset(hexArray, 0, (sizeof(hexArray)/sizeof(hexArray[0])));

 digitalWrite(LATCH_PIN, LOW);
 shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, hexArray[32]^255); // clear the display (help with converting binary number from Martin of IMCA)
     digitalWrite(LATCH_PIN,HIGH); //pull the latchPin to save the data

 //hexArray[0] = 0;
 
}
/******************LIVE()- NOTHING TO DO **************************************
   INSTRUCTIONS:
   this function will play the corresponding notes
   to the user pressing the respective buttons
   NOTE:: the notes played are NOT stored
   SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
   ALSO - get the value of the photo cell and combine it with the note frequency
   THEN - output the note to the buzzer using the tone() function, as 3rd argument
   make the duration value == to 100
**************************************************************************/
void live() {
  if (analogRead(NOTE_IN_PIN) > 8) {
    tone(BUZZER_PIN, analogRead(NOTE_IN_PIN), 100);
  }
   // make sure that in record mode we cannot save a pattern
  if(storePatternInterruptRequest == ISTRUE)
  {
    storePatternInterruptRequest=ISFALSE;
  }
}



/******************RECORDWITHDURATION(): IMPLEMENT NEW ETUDE 4 ************
   INSTRUCTIONS:
   YOU need to implement the new functionality for capturing the pauses between notes ... 
   HINT: you will need to use the second timer and at some point call the  addToArrayInBetween() function which will
   update the necessary data structures. 
   THERE IS NO INDICATION WHERE OR HOW YOU SHOULD DO THIS IN THIS FUNCTION
   
**************************************************************************/
void recordWithDuration()
{
  int theAdjuster = 8;
  // check we have not stored more than 16 notes
  if (countNotes < MAX_NOTES) {
    // read in the value from button press
    testNote = analogRead(NOTE_IN_PIN);
    /*** STATE A::: *******************************************
        IF the testNote is > min AND the timer has not reached 5 secs:
        This means we are pressing on a note button
    */
    if (testNote > theAdjuster && (t1.getTimePassed() < MAX_PRESS_TIME)) {
      /*** STATE AA::: *******************************************
          IF the boolean is false it means we have just started to press the button
          YOU now need to implement the function below which will set up all things
          required to have a new timer
      */
      if (activeNoteButton == ISFALSE) {

        t1.startUpTimer();
        activeNoteButton = ISTRUE;
        addToArrayInBetween();
      }
      /*** STATE AA::: *******************************************
           IF the boolean is true it means we are continuing to press the button
      */
      else { 
        // update the timer (for durations)
        t1.updateTimer();
        activeFrequency = testNote;
        playCurrentNote();
      }
    }
    /*** STATE B::: *******************************************
       IF the testNote is > min AND the timer HAS reached 5 secs:
       then turn OFF the buzzer.
    */
    else if (testNote > theAdjuster && (t1.getTimePassed() > MAX_PRESS_TIME)) {

      noTone(BUZZER_PIN);
    }
    /*** STATE C::: *******************************************
       IF the testNote is < min AND the boolean is true
       this means we have STOPPED pressing the button, update the arrays 
    */
    else if ((testNote <= theAdjuster && activeNoteButton == ISTRUE) ) {
      /*** FUNCTION TO IMPLEMENT ***/
      updateArraysWithNoteAndTimings();
    }
  }
   // make sure that in record mode we cannot save a pattern
  if(storePatternInterruptRequest == ISTRUE)
  {
    storePatternInterruptRequest=ISFALSE;
  }
}


/******************PLAYCURRENTNOTE(): *********************************
   INSTRUCTIONS:
   this function will play the current note being pressed in RecordDuration mode
**************************************************************************/
void playCurrentNote() {
  tone(BUZZER_PIN, activeFrequency, 100);

}
/******************UPDATEARRAYSWITHNOTEANDTIMINGS():SMALL CHANGE ************
   INSTRUCTIONS:
   this function handles the case where the note is no longer being pressed:
   you need to update the arrays
   reset the necessary booleans
   reset any counters
   //TO DO :: perhaps we want to start up our other timer ????
**************************************************************************/
void updateArraysWithNoteAndTimings() {
  notes[countNotes] = activeFrequency;
  durations[countNotes] = t1.getTimePassed();
  countNotes++;
  activeNoteButton = ISFALSE;
  t1.setTimePassed(0);
  t_between.startUpTimer();

}
/******************addToArrayInBetween(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function will handle the case that adds the current pause into the necessary array
   You may also need to reset some variables....
  /*************************************************************************/
void addToArrayInBetween() {
  // IMPLEMENT
  t_between.updateTimer();
  durations_In_Between[countNotes] = t_between.getTimePassed();
  t_between.setTimePassed(0);
}

/******************PLAYWITHDURATION(): ADAPTED FOR ETUDE 4 *******************
   INSTRUCTIONS:
   this function goes through the notes array and play back the note and
   its corresponding duration value.
   TO IMPLEMENT:: add the ability to store the current sequence playing within the if statement
**************************************************************************/
void playWithDuration() {
  int i = 0;

  while (storePatternInterruptRequest == ISFALSE && modeChangeInterruptRequest == ISFALSE ) {
    if (countNotes > 0) {
      tone(BUZZER_PIN, notes[i], durations[i]);
      delay(durations[i]);
      // ensure that you have values in this array ...
      delay(durations_In_Between[i]);
      
      if (i < countNotes - 1) i++;
      else {
        i = 0;
        delay(DURATION);
      }
    }

  }

  if (countNotes > 0 && storePatternInterruptRequest == ISTRUE && storeOnce == ISFALSE)
  {
    // TODO:: add pattern to our list
    //HINT use the TestPattern files..... there is a specific function that one can do to do this...
void addDataToPatternObj(const int * cNotes, const int * cDurations, const int * cPauses,struct patternObj* p);
    void addPattern(const int * cNotes, const int * cDurations, const int * cPauses, int numberNotes);
    
    //keep
    storeOnce = ISTRUE;
    
   /* TO DO::: FLASH THE LED ON AND OFF for DURATION (once) */

  digitalWrite(LED_PIN_G,HIGH); //Turn red LED on
  delay(DURATION);             //Leave on for duration
  digitalWrite(LED_PIN_G,LOW);  //Turn red LED off
    
  }
  delay(DURATION * 2);
  storePatternInterruptReset();
}
/******************HELPER STATE 0 - PARTIALLY IMPLEMENT *******************
      INSTRUCTIONS: while neither button is pressed then:
      1/ get pot value 
      2/if we have do NOT have a valid index (i.e. we selected "3" but there are only 3 patterns not 4) then
      make sure the seven segment display blinks.
      3/ otherwise, make sure that the shift register sends the correct number to the seven segment display
      
       
**************************************************************************/
void selectThePatternWithPot_Stage_0_(int & shiftRef)
{
  while (storePatternInterruptRequest == ISFALSE && modeChangeInterruptRequest == ISFALSE) {
    //1::YOU NEED TO IMPLEMENT THIS FUNCTION IN SHIFTREGISTER.h
    shiftRef = getAnalog();
    
    if (isValidIndex(shiftRef) == ISFALSE) {
      //2:: IMPLEMENT (again you will use functions that you implemented in SHIFTREGISTER.h)
      sendToShiftRegister(shiftRef);
      retreiveMode();
    }
    // is valid index
    else {
      //3::IMPLEMENT (again you will use functions that you implemented in SHIFTREGISTER.h)
      sendToShiftRegister(shiftRef);
      retreiveMode();
    }
  }
}
/******************HELPER STATE 1 -PARTIALLY IMPLEMENT *********************************
      INSTRUCTIONS:
  While the modebutton is not pressed: if we have a valid index 
  then:
  1/ use the TestPattern functions to retrieve the notes, 
  durations and pauses for that pattern.
 2/  Then play the current pattern
**************************************************************************/
void retrieveThePatternAccordingToSelection_Stage_1(int & shiftRef)
{
  while (modeChangeInterruptRequest == ISFALSE) {
    if (isValidIndex(shiftRef) == ISTRUE) {
      //1: TO DO :::RETRIEVE THE NOTES, PAUSES AND DURATIONS
      //HINT::: you will need to return a reference from the appropriate testPattern function
      // the FIRST one is given to you - the others will follow the same idea
      const int * noteArray = retrievePatternNotes(shiftRef);
      const int * durationsArray = retrievePatternDurations(shiftRef); 
      const int * pausesArray =  retrievePatternPauses(shiftRef);
      int i;6hy
      //2: TODO: play the current pattern:
      for(i=0; i<getNumberNotesOfPattern(shiftRef); i++){
        tone(BUZZER_PIN, noteArray[i], durationsArray[i]);
        delay(pausesArray[i]);
      }
  }
}
}
/******************RETRIEVEMODE(): IMPLEMENT *********************************
   INSTRUCTIONS:
   1/ while there are no patterns and the mode button is not pressed then blink the
   seven segment display for DURATION
   2/ otherwise select the pattern and prehaps retrieve it
   

**************************************************************************/
void retreiveMode()
{
  noTone(BUZZER_PIN);
  static int shift = -1;

  while (patternListIsEmpty() == ISTRUE && modeChangeInterruptRequest == ISFALSE) {
    //1:: Implement - again you will use the functions you implemented in shiftRegister.h
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, hexArray[32]^255); // clear the display (help with converting binary number from Martin of IMCA)
    digitalWrite(LATCH_PIN,HIGH); //pull the latchPin to save the data  
    byte incoming = shiftIn(D, clockPin, bitOrder)

 }

  // not empty case::
  //(PLEASE IMPLMENT THIS FUNCTION)
  selectThePatternWithPot_Stage_0_(shift);
  // we will come here because the store button was pressed  -> so we go to next stage - (store that pattern)
  delay(DURATION * 2);
  storePatternInterruptReset();
  // store and play the pattern until the mode button is pressed (PLEASE IMPLMENT THIS FUNCTION)
  retrieveThePatternAccordingToSelection_Stage_1(shift);
}

/******************SETRGB() - NOTHING TO DO*********************************
   INSTRUCTIONS:
   Depending on the value of the mode variable:
   if the mode is 0 - RGB LED IS OFF
   if the mode is 1 - RGB LED IS BLUE
   if mode is 2 - RGB LED IS RED
   if mode is 3 - RGB LED IS GREEN
   if mode is 4 - RGB LED iS PURPLE
   YOU MUST USE A SWITCH CASE CONSTRUCT (NOT A SERIES OF IF / ELSE STATEMENTS
**************************************************************************/
void setRGB()
{
  switch (mode)
  {
    case 0:
      {
        analogWrite(LED_PIN_R, 0);   // Turn off the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn off the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn off the LED -B
        break;
      }
    case 1:
      {
        analogWrite(LED_PIN_R, 0);   // Turn off the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn off the LED -G
        analogWrite(LED_PIN_B, 255);   // Turn on the LED -B
        break;
      }
    case 2:
      {
        analogWrite(LED_PIN_R, 255);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn off the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn off the LED -B
        break;
      }
    case 3:
      {
        analogWrite(LED_PIN_R, 0);   // Turn off the LED -R
        analogWrite(LED_PIN_G, 255);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn off the LED -B
        break;
      }

    case 4:
      {
        analogWrite(LED_PIN_R, 125);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 255);   // Turn on the LED -B
        break;
      }
  }
}

/**************************************************************************
Anwser the  questions IN THIS SPACE....

1.Please explain in your own words, what and how a Shift Register works
-is this how it is used the context of this Etude?

ANSWER
 A shift register works by communicating with the microcontroller by  
 either converting parallel to serial communication or vice versa. 
 Serial communication works by streaming a bit of data at a time where as 
 parallel transmits large chunks of bits at once. Although parallel 
 results in faster transmission and could be easier to use, serial frees
 up a lot of the pins on a microcontroller by sending information over 
 a single wire rather than a multitude. Depending on which type of shift
 register, SIPO, Serial-In-Parallel-Out or PISO, Parallel-In-Serial-Out,
 the microcontroller can output or input data to or from 8-16+ sensors
 over one pin connection. The one used in the etude is SIPO model 74HC595
 and was connected to a seven segment display that required 8 analog pins
 if connected through the microcontroller. The SIPO 74HC595 converted the
 serial data received from the microcontroller and relayed the information
 to the individual analog pins connected to it, creating the corresponding
 numbers/letters. By using the shift register, the number of analog pins
 needed was reduced and freed up more pin space for the other sensors 
 within the schematic. 

2.
The outcome for this Etude allows the user in Mode 3 to Store a Pattern and then in Mode
4 to select a Pattern by using the Potentiometer, then they click on the Store Button and
that Pattern is Retrieved and Played. At this point, if the user wants to select another
Stored Pattern they need to use the Mode Button to cycle through mode 1-> mode 4 in
order to do so.
Please explain how you would implement the functionality to allow the user to Select and
Retrieve Patterns WITHIN the same Mode (without having to re-cycle through).

ANSWER

Have the patterns stored in a single mode where each the pattern fills a duration of time
and a duration of time correlates with a specific analog int range. Selecting something 
within the analog int range would in respose provide a pattern if stored in range. 


3.
In this Etude, we do not use the Photocell to modulate the Note Frequency while it is 
being pressed.Please explain briefly how one could STORE a modulated Note over time 
(i.e. how wouldone store the changes in frequency over a specific time interval)? 
NB: you need not consider the specific code implementation provided for this Etude

ANSWER




4.Please provide 2 ideas ( 150 words each) for how you would suggest to extend this
instrument we have been developing throughout the semester


ANSWER
Idea 1: 
A second arduino could be implemented and information can be relayed back and forth
between the two using asynchronous serial (UART) communication over a radio connection. 
The second arduino can receive the information and play it with the option to save, 
store etc. It would also be interesting if the second arduino could produce sounds and 
both arduinos could collaborate on a song together live. To collaborate live, both 
arduinos would have to be on the same mode. An RGB LED can be used to signal when both 
are on this mode, red being no and green being yes and you could start recording. This 
song could then be stored and replayed on both arduinos. It would be interesting to 
continue along this idea of collaboration, possibly creating a network of mini musical
instruments that can create collaborative songs with a separate collaboration mode on 
each device.

Idea 2:
The continuation would have the ability to edit old songs through the implementation 
of a separate edit mode where a particular song could be selected and you could resume
editing by adding more notes or deleting notes. There could be a mode where two songs 
selected could be combined or cut to be combined. The modulation of the notes could also
be represented in the modulation in colour through a RGB LED or translated into a form
of movement either through motors or a kinetic structure.. Continuing along the idea of 
editing stored songs, we could implement a MIDI output therefor being able to upload 
the songs onto a computer to edit on applications like max msp.





/**************************************************************************/


