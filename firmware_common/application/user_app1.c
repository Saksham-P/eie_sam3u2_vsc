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

  LedOn(BLUE0);

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
  static u16 blueLEDs[] = {BLUE0, BLUE1, BLUE2, BLUE3};
  static u16 furElise[] = {
    E4, D4S, E4, D4S, E4, B3, D4, C4, A3, C4, E4, A3, B3, E4, G4S, B3, C4, E4, 
    E4, D4S, E4, D4S, E4, B3, D4, C4, A3, C4, E4, A3, B3, E4, C4, B3, A3, B3, 
    C4, D4, E4, G4, F4, E4, D4, F4, E4, D4, C4, E4, D4, C4, B3, E4, D4S, E4, 
    D4S, E4, B3, D4, C4, A3, C4, E4, A3, B3, E4, G4S, B3, C4, E4, E4, D4S, 
    E4, D4S, E4, B3, D4, C4, A3, C4, E4, A3, B3, E4, C4, B3, A3
  };
  static u16 maryHadALittleLamb[] = {
    E4, D4, C4, D4, E4, E4, E4, 
    D4, D4, D4, E4, G4, G4, 
    E4, D4, C4, D4, E4, E4, E4, 
    E4, D4, D4, E4, D4, C4
  };
  static u16 twinkleTwinkle[] = {
    C4, C4, G4, G4, A4, A4, G4, 
    F4, F4, E4, E4, D4, D4, C4, 
    G4, G4, F4, F4, E4, E4, D4, 
    G4, G4, F4, F4, E4, E4, D4, 
    C4, C4, G4, G4, A4, A4, G4, 
    F4, F4, E4, E4, D4, D4, C4
  };
  static u16 happyBirthday[] = {
    C4, C4, D4, C4, F4, E4, 
    C4, C4, D4, C4, G4, F4, 
    C4, C4, C5, A4, F4, E4, D4, 
    B4, B4, A4, F4, G4, F4
  };

  static u16* songs[] = {furElise, maryHadALittleLamb, twinkleTwinkle, happyBirthday};
  static u16 ledIndex = 0;
  static bool songPlaying = FALSE;
  static u16 songIndex = 0;
  static u16 waitTime = 0;

  if (WasButtonPressed(BUTTON1)) {
    ButtonAcknowledge(BUTTON1);
    ledIndex++;

    if (ledIndex == (u8)(sizeof(blueLEDs) / sizeof(u16))) {
      ledIndex = 0;
    }

    for (u8 i = 0; i < 4; i++) {
      if (i == ledIndex) {
        LedOn(blueLEDs[i]);
      } else {
        LedOff(blueLEDs[i]);
      }
    }
  }

  if (WasButtonPressed(BUTTON0)) {
    songPlaying = TRUE;

    if (songPlaying) {
      if (waitTime == 0) {
        waitTime = 200;
        PWMAudioSetFrequency(BUZZER1, songs[ledIndex][songIndex]);
        PWMAudioOn(BUZZER1);

        if (ledIndex == 0) {
          if (songIndex == (u8)(sizeof(furElise) / sizeof(u16))) {
            songPlaying = 0;
            songPlaying = FALSE;
            PWMAudioOff(BUZZER1);
            ButtonAcknowledge(BUTTON0);
          } else {
            songIndex++;
          }
        } else if (ledIndex == 1) {
          if (songIndex == (u8)(sizeof(maryHadALittleLamb) / sizeof(u16))) {
            songPlaying = 0;
            songPlaying = FALSE;
            PWMAudioOff(BUZZER1);
            ButtonAcknowledge(BUTTON0);
          } else {
            songIndex++;
          }
        } else if (ledIndex == 2) {
          if (songIndex == (u8)(sizeof(twinkleTwinkle) / sizeof(u16))) {
            songPlaying = 0;
            songPlaying = FALSE;
            PWMAudioOff(BUZZER1);
            ButtonAcknowledge(BUTTON0);
          } else {
            songIndex++;
          }
        } else if (ledIndex == 3) {
          if (songIndex == (u8)(sizeof(happyBirthday) / sizeof(u16))) {
            songPlaying = 0;
            songPlaying = FALSE;
            PWMAudioOff(BUZZER1);
            ButtonAcknowledge(BUTTON0);
          } else {
            songIndex++;
          }
        }

        // if (songIndex == (u8)(sizeof(songs[ledIndex]) / sizeof(u16))) {
        //   songIndex = 0;
        //   songPlaying = FALSE;
        //   PWMAudioOff(BUZZER1);
        //   ButtonAcknowledge(BUTTON0);
        // } else {
        //   songIndex++;
        // }
      } else {
        waitTime--;
      }
    }
  }

  // if (IsButtonPressed(BUTTON0)) {
  //   PWMAudioOn(BUZZER1);
  // } else {
  //   PWMAudioOff(BUZZER1);
  // }
     
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
