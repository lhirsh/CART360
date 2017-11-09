/*----------------------------------------------------------------------------*/
/*------------------------------ SHIFT REGISTER ------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------      IMPLEMENT THE SHIFT REGISTER FUNCTIONALITY        ----------*/
/*----------          ******** ATTRIBUTE YOUR CODE ********         ----------*/
/*----------------------------------------------------------------------------*/

#ifndef SHIFTREGISTER_H_
#define SHIFTREGISTER_H_

#include "Constants.h"

/* SHIFT REGISTER IC PIN OUTS
        __
  Q1 -|    |- VCC
  Q2 -|    |- Q0
  Q3 -|    |- DS
  Q4 -|    |- OE
  Q5 -|    |- ST_CP
  Q6 -|    |- SH_CP
  Q7 -|    |- MR
 GND -| __ |- Q'

  Key:
  Q0 - Q7: Parallel Out Pins
  Q': Cascade Pin
  DS: Serial Data In Pin
  OE: Output Enable (GND)
  ST_CP: Latch Pin
  SH_CP: Clock Pin
  MR: Master Reset (VCC)
*/

/* PINS FOR SHIFT REG */
// ST_CP of 74HC595
#define LATCH_PIN 6
// SH_CP of 74HC595
#define CLOCK_PIN 8
// DS of 74HC595
#define DATA_PIN 7

/* CONSTANT FOR EMPTY*/
#define EMPTY B00000001

/* DEFINE AND INITIALIZE THE ARRAY WITH THE NECESSARY VALUES */
// Hex Character from 0 - F
byte hexArray[16] =
{
  // you have to implement the necessary values for each element
  
  //display 0,1,2,3,4,5,6,7,8,9,A,b,C,d,E,F refrence https://forum.arduino.cc/index.php?topic=354835.0

  252, 96, 218, 242, 102, 182, 190, 224, 254, 246, 238, 62, 156, 122, 158, 142
  
};


void setupShiftRegister() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  }
  
/******************sendToShiftRegister *******************************
TODO:: IMPLEMENT THE FUNCTIONALITY TO SEND THE CORRECT DATA TO 
SHIFT REG - BASED ON THE POT VAL
********************************************************************/
void sendToShiftRegister(int pot)
{
    digitalWrite(LATCH_PIN,LOW); //ground latchPin and hold low for as long as you are transmitting
    shiftOut(DATA_PIN,CLOCK_PIN,MSBFIRST,hexArray[pot]^255);
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(LATCH_PIN,HIGH); //pull the latchPin to save the data
  
}

/******************READ FROM POT*********************************
TO DO:: GET VALUE FROM POT AND ENSURE THE VALUE RETURNED IS A VALID VALUE 
********************************************************************/
int getAnalog() {

  //IMPLEMENT

  int val = analogRead(POT_PIN);

  if (val > 0 && val <= 64){
    return(0);
  }
  if (val > 64 && val <= 128){
    return(1);
  }
  if (val > 128 && val <= 192){
    return(2);
  }
  if (val > 192 && val <= 256){
    return(3);
  }
  if (val > 256 && val <= 320){
    return(4);
  }
  if (val > 320 && val <= 384){
    return(5);
  }
  if (val > 384 && val <= 448){
    return(6);
  }
  if (val > 448 && val <= 512){
    return(7);
  }
  if (val > 512 && val <= 576){
    return(8);
  }
  if (val > 576 && val <= 640){
    return(9);
  }
  if (val > 640 && val <= 704){
    return(10);
  }
  if (val > 704 && val <= 768){
    return(11);
  }
  if (val > 768 && val <= 832){
    return(12);
  }
  if (val > 832 && val <= 896){
    return(13);
  }
  if (val > 896 && val <= 960){
    return(14);
  }
  if (val > 960 && val <= 1024){
    return(15);
  }
}

#endif /* SHIFTREGISTER_H_ */
