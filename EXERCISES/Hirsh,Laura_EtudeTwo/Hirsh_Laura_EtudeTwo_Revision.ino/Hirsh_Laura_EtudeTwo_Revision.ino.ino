/**********************ETUDE 2 CART 360 2017*******************************
 * WELCOME! 
 * THE PURPOSE OF THIS EXERCISE IS TO DESIGN A VERY SIMPLE KEYBOARD (5 KEYS)
 * WHICH ALLOWS YOU TO PLAY LIVE, RECORD, AND PLAYBACK. 
 * THERE WILL BE A BUTTON WHICH WHEN PRESSED WILL TAKE THE USER TO THE NEXT MODE:
 * THERE ARE 5 POSSIBLE MODES
 * 0 ==  reset    ==   led off == also resets
 * 1 ==  live     ==   led BLUE
 * 2 ==  record   ==   led RED
 * 3 ==  play     ==   led GREEN
 * 4 ==  loopMODE ==   led PURPLE
 * 
 * ADDITIONALLY - WHEN THE USER CHANGES MODE, 
 * THE RGB LED WILL CHANGE COLOR (ACCORDING TO THE SPECIFICATIONS)

 * PLEASE FOLLOW THE INSTRUCTIONS IN THE COMMENTS:
 * DO NOT ADD ANY MORE FUNCTION DEFINITIONS 
 * ONLY IMPLEMENT THE FUNCTION DEFINITIONS SUPPLIED
 * THERE IS NO NEED TO ADD ANY NEW VARIABLES OR CONSTANTS
 * PROVIDE COMMENTS FOR ANY OF THE CODE IMPLEMENTED
 * GOOD LUCK!
 */
/**** CONSTANTS ********************************************************/

#define BUTTON_MODE_PIN 2 // Button to change the mode

// constants for RGB LED
#define LED_PIN_R 5 // R PIN
#define LED_PIN_G 9 // G PIN
#define LED_PIN_B 6 // B PIN

// constant for note in (button-resistor ladder on breadboard)
# define NOTE_IN_PIN A0

//constant for max number of notes in array
#define MAX_NOTES 16

// a constant for duration
const int duration =200;

// constant for pin to output for buzzer 
#define BUZZER_PIN 3 // PWM

/******** VARIABLES *****************************************************/
// counter for how many notes we have
int countNotes =0;
// array to hold the notes played (for record/play mode)
int notes [MAX_NOTES]; 
int mode =0; // start at off

/*************************************************************************/


/**********************SETUP() DO NOT CHANGE*******************************/
// Declare pin mode for the single digital input
void setup()
{
  pinMode(BUTTON_MODE_PIN, INPUT); //to have the button be the input control
  Serial.begin(9600); //to read serial monitor
}

/**********************LOOP() DO NOT CHANGE *******************************
 * INSTRUCTIONS: 
 * There is NO NEED to change the loop - it establishes the flow of the program
 * We call here 3 functions repeatedly:
 * 1: chooseMode(): this function will determine the mode that your program is in 
 * based on if the button (linked to the BUTTON_MODE_PIN) was pressed
 * 2: setRGB(): will set the color of the RGB LED based on the value of the mode variable
 * 3: selectMode(): will determine which function to call based on the value of the mode variable

**************************************************************************/
void loop()
{
  chooseMode();
  setRGB();
  selectMode();
}
/******************CHOOSEMODE(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * Read the value from the Button (linked to the BUTTON_MODE_PIN) and 
 * if is being pressed then change the mode variable.
 * REMEMBER:
 * mode == 0 is reset 
 * mode == 1 is live
 * mode == 2 is record
 * mode == 3 is play
 * mode == 4 is loopMode
 * Every time the user presses the button, the program will go to the next mode,
 * once it reaches 4, it should go back to mode == 0. 
 * (i.e. if mode ==2 and we press, then mode ==3) ...
**************************************************************************/
void chooseMode(){
  // IMPLEMENT

   if (digitalRead(BUTTON_MODE_PIN) == HIGH) //if button is being pressed
   
   {
   if (mode == 4) //mode can equal up to four
       mode = 0;  //mode starts at 0
     else
       mode++;    //mode inceases by one (until reaching four)
   }
     delay(200); //add time to register button press 
}


/******************SETRGB(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * Depending on the value of the mode variable:
 * if the mode is 0 - RGB LED IS OFF
 * if the mode is 1 - RGB LED IS BLUE
 * if mode is 2 - RGB LED IS RED
 * if mode is 3 - RGB LED IS GREEN
 * if mode is 4 - RGB LED iS PURPLE
 * YOU MUST USE A SWITCH CASE CONSTRUCT (NOT A SERIES OF IF / ELSE STATEMENTS
**************************************************************************/
void setRGB() //controls led colors being activated at designated mode
{
  //IMPLEMENT
 
  
  if(mode ==0)  //if mode is at 0
  {
     analogWrite(LED_PIN_R, 0);    //tell red led pin to be off (analog = 0-255)
     analogWrite(LED_PIN_G, 0);    //tell green led pin to be off
     analogWrite(LED_PIN_B, 0);    //tell blue led pin to be off
   }
    
  if (mode ==1)  //if mode is at 1
  {
     analogWrite(LED_PIN_R, 0);    //tell red led pin to be off
     analogWrite(LED_PIN_G, 0);    //tell green led pin to be off
     analogWrite(LED_PIN_B, 255);  //tell blue led pin to be on
  }

  if (mode ==2)  //if mode is at 2
  {
     analogWrite(LED_PIN_R, 255);  //tell red led pin to be on
     analogWrite(LED_PIN_G, 0);    //tell green led pin to be off
     analogWrite(LED_PIN_B, 0);    //tell blue led pin to be off
  }

  if (mode ==3)  //if mode is at 3
  {
     analogWrite(LED_PIN_R, 0);    //tell red led pin to be off
     analogWrite(LED_PIN_G, 255);  //tell green led pin to be on
     analogWrite(LED_PIN_B, 0);    //tell blue led pin to be off
  }

   if (mode ==4)  //if mode is at 4
  {
     analogWrite(LED_PIN_R, 100);  //tell red led pin to be on
     analogWrite(LED_PIN_G, 0);    //tell green led pin to be off
     analogWrite(LED_PIN_B, 200);  //tell blue led pin to be on
  } 
}
/**********************SELECTMODE() DO NOT CHANGE *******************************
 * INSTRUCTIONS: 
 * There is NO NEED to change this function - it selects WHICH function should run based on the mode variable
 * There are 4 possibilities
 * 1: reset(): this function will reset any arrays etc, and will do nothing else
 * 2: live(): this function will play the corresponding notes 
 * to the user pressing the respective buttons. 
 * NOTE:: the notes played are NOT stored
 * 3: record(): this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * AND will STORE up to 16 consecutive notes in an array.
 * 4: play(): this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * 5: loopMode(): this function will playback any notes stored in the array that were recorded, 
 * BUT unlike the previous mode, you can choose in which sequence the notes are played.
 * REQUIRED: only play notes from the array (no live stuff)

******************************************************************************/
void selectMode()
{
  if(mode ==0) { 
    reset();
  }
  else if(mode ==1) {
    live();
  }
  else if(mode ==2) {
    record();
  }
  
  else if(mode ==3) {
    play();
  }
   
   else if(mode ==4) {
    loopMode();
  }
}
/******************RESET(): IMPLEMENT **************************************
 * INSTRUCTIONS:
 * this function should ensure that any notes recorded are no longer available
**************************************************************************/
void reset()
{
  // IMPLEMENT
   for( int i = 0; i < (MAX_NOTES); i++) //when the notes in the array (i) begin at 0 (the mode 0) and i is less than 16 and incremented by one note until 16
   {
   notes[i] = 0; //the notes in the array (i) are equal to zero 
   }
   countNotes = 0; //and the number of notes counted is zero (resets counter) 
}
/******************LIVE(): IMPLEMENT **************************************
 * INSTRUCTIONS:
 * this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * NOTE:: the notes played are NOT stored
 * SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
 * THEN - output the note to the buzzer using the tone() function
**************************************************************************/
void live()
{
    //IMPLEMENT

      tone(BUZZER_PIN, analogRead(NOTE_IN_PIN), duration); //buzzer pin=pin to which the tone is generated, the note in is the 
      //frequency with which the tone in played and duration is the time of which the the is played for
      
      delay(duration); //keep active for long enough to see the change in state
}
/******************RECORD(): IMPLEMENT **********************************
 * INSTRUCTIONS:
 * this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * AND will STORE up to 16 consecutive notes in an array.
 * SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
 * AND - output the note to the buzzer using the tone() function
 * THEN store that note in the array  - BE CAREFUL - you can only allow for up to MAX_NOTES to be stored
**************************************************************************/
void record()
{
  // IMPLEMENT

      if (analogRead(NOTE_IN_PIN)>5) //if constant frequency for the note in is greater than 5
      {
      if(countNotes < MAX_NOTES)     //and if the notes counted are less than 16
      {
      notes[countNotes++] = analogRead(NOTE_IN_PIN); //the note array (which is comprised of increasing the note count by one each time) will equal the constant for the note in
      }
     tone(BUZZER_PIN, analogRead(NOTE_IN_PIN), duration); //and the tone will play at that frequency
     
     delay(100); //and be active for long enough to see the change in state
      }
      
}
/******************PLAY(): IMPLEMENT ************************************
 * INSTRUCTIONS:
 * this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
 * READ each value IN ORDER
 * AND output each note to the buzzer using the tone() function
 * ALSO: as long as we are in this mode, the notes are played over and over again
 * BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void play()
{
  // IMPLEMENT

   for (int i = 0; i < MAX_NOTES; i++) //when the notes in the array (i) begin at 0 (the mode 0) and i is less than 16 and incremented by one note until 16
   {
    tone(BUZZER_PIN, notes[i], duration); //play the tone stored in the array
    delay(duration); //and long enough to hear it before changing states/the note

    if (digitalRead(BUTTON_MODE_PIN) == HIGH) //and if button is pressed again
    {
      break; //stop the loop
    }
  }
  
}
/******************LOOPMODE(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
 * READ values IN ANY ORDERING (You MUST use the array - but you can determine your own sequence)
 * AND output each note to the buzzer using the tone() function
 * ALSO: as long as we are in this mode, the notes are played over and over again
 * BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void loopMode()
{
  //IMPLEMENT
   for (int i = 0; i < MAX_NOTES; i = i+8)  //when the notes in the array (i) begin at 0 (the mode 0) and i is less than 16, i is then equal to i plus 8
   
   {
    tone(BUZZER_PIN, notes[i], duration); //play the tone stored in the array
    if (countNotes==2)                    //and if the two notes are counted in the array
    {
      delay(duration*2);                 //the state/note doesn't change for two times the designated amount of time
    } 
    else                                 //otherwise
    {
      delay(duration);                   //the state/note doesn't change for a designated amount of time
    }
        if (digitalRead(BUTTON_MODE_PIN) == HIGH)  //and if button is pressed again
    {
      break;                                       //stop the loop
    }
   }
}

/**************************************************************************/




