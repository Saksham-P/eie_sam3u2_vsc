/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  /* Turning of all LEDs and backlight to start*/
  for (u8 i = 0; i < U8_TOTAL_LEDS; i++) {
    LedOff((LedNameType)i);
  }

  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{

  // static bool redLedBlinking = FALSE;
  // LedRateType redBlinkinRates[] = {LED_1HZ, LED_2HZ, LED_4HZ, LED_8HZ};
  // static u8 currentBlinkingIndex = 0;

  // if (WasButtonPressed(BUTTON1)) {
  //   ButtonAcknowledge(BUTTON1);

  //   if (!redLedBlinking) {
  //     redLedBlinking = TRUE;
  //     LedBlink(RED1, redBlinkinRates[currentBlinkingIndex]);
      
  //   } else {

  //     LedOff(RED1);
  //     redLedBlinking = FALSE;
  //   }
  // }

  // if (redLedBlinking && WasButtonPressed(BUTTON0)) {

  //   ButtonAcknowledge(BUTTON0);
    
  //   currentBlinkingIndex++;
  //   if (currentBlinkingIndex == (sizeof(redBlinkinRates) / sizeof(LedRateType))) {
  //     currentBlinkingIndex = 0;
  //   }
  //   LedBlink(RED1, redBlinkinRates[currentBlinkingIndex]);
  // }

  ButtonNameType password[] = {BUTTON0, BUTTON1, BUTTON1, BUTTON0};
  static bool systemLocked = TRUE;
  static u8 currentPasswordProgress = 0;
  static bool wrongPasswordStatus = FALSE;
  static bool rightPasswordStatus = FALSE;
  static u16 threeSecondCounter = 3000;

  if (systemLocked && !wrongPasswordStatus && !rightPasswordStatus) {
    LedOn(RED3);
    LedOn(GREEN3);
  } else {
    LedOff(RED3);
    LedOff(GREEN3);
  }

  if (IsButtonPressed(BUTTON0)) {
    LedOn(BLUE0);
  } else {
    LedOff(BLUE0);
  }

  if (IsButtonPressed(BUTTON1)) {
    LedOn(BLUE1);
  } else {
    LedOff(BLUE1);
  }

  if (WasButtonPressed(BUTTON0)) {
    ButtonAcknowledge(BUTTON0);
    if (BUTTON0 == password[currentPasswordProgress]) {
      currentPasswordProgress++;
    } else {
      currentPasswordProgress = 0;
    }

    if (currentPasswordProgress > 4) {
      currentPasswordProgress = 0;
    }

  } else if (WasButtonPressed(BUTTON1)) {
    ButtonAcknowledge(BUTTON1);
    if (BUTTON1 == password[currentPasswordProgress]) {
      currentPasswordProgress++;
    } else {
      currentPasswordProgress = 0;
    }

    if (currentPasswordProgress > 4) {
      currentPasswordProgress = 0;
    }
  }

  if (IsButtonHeld(BUTTON0, 2000) && IsButtonHeld(BUTTON1, 2000)) {
    if (currentPasswordProgress == (sizeof(password) / sizeof(ButtonNameType))) {
      rightPasswordStatus = TRUE;
    } else {
      wrongPasswordStatus = TRUE;
    }
  }

  if (wrongPasswordStatus) {
    if (threeSecondCounter  == 3000) {
      LedOff(BLUE3);
      LedOff(GREEN3);
      LedBlink(RED3, LED_8HZ);
    }
    threeSecondCounter--;
    if (threeSecondCounter == 0) {
      threeSecondCounter = 3000;
      LedOn(RED3);
      LedOn(GREEN3);
      LedOff(BLUE3);
      wrongPasswordStatus = FALSE;
    }
  }

    if (rightPasswordStatus) {
    if (threeSecondCounter  == 3000) {
      LedOff(BLUE3);
      LedOff(RED3);
      LedBlink(GREEN3, LED_2HZ);
    }

    threeSecondCounter--;
    if (threeSecondCounter == 0) {
      threeSecondCounter = 3000;
      LedOff(RED3);
      LedOn(GREEN3);
      LedOff(BLUE3);
      rightPasswordStatus = FALSE;
    }
  }
     
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
