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
#include "lcd_NHD-C12864LZ.h"
#include "lcd_bitmaps.h"

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

static u8 UserApp1_au8Name[] = "Saksham";
const u8 aau8CustomLogo[U8_LCD_IMAGE_ROW_SIZE_25PX][U8_LCD_IMAGE_COL_BYTES_25PX] = {
{0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00},
{0xF0, 0xFF, 0x1F, 0x00},
{0xFC, 0xFF, 0x7F, 0x00},
{0xFE, 0xFF, 0xFF, 0x00},
{0x0F, 0x00, 0xE0, 0x00},
{0x07, 0xC0, 0xC0, 0x01},
{0x07, 0xC0, 0xC0, 0x01},
{0x07, 0xC0, 0xC0, 0x01},
{0x1F, 0xC0, 0xC0, 0x01},
{0xFE, 0xC1, 0xE0, 0x00},
{0xFC, 0xC7, 0xFF, 0x00},
{0xF0, 0xCF, 0x7F, 0x00},
{0x00, 0xCF, 0x00, 0x00},
{0x00, 0xCE, 0x00, 0x00},
{0x00, 0xCE, 0x00, 0x00},
{0x00, 0xCE, 0x00, 0x00},
{0x03, 0xCF, 0x00, 0x00},
{0xFF, 0xCF, 0x00, 0x00},
{0xFF, 0xC7, 0x00, 0x00},
{0xF8, 0xC0, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00}
};


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

  LcdClearScreen();
  // PixelAddressType sTargetPixel = {32, 64};
  // LcdSetPixel(&sTargetPixel);

  // PixelAddressType sTestStringLocation = {U16_LCD_TOP_MOST_ROW, U16_LCD_LEFT_MOST_COLUMN};
  // LcdLoadString(UserApp1_au8Name, LCD_FONT_SMALL, &sTestStringLocation);

  PixelBlockType sTestImage;
  sTestImage.u16RowStart = 0;
  sTestImage.u16ColumnStart = 0;
  sTestImage.u16RowSize = 25;
  sTestImage.u16ColumnSize = 25;
  LcdLoadBitmap(&aau8CustomLogo[0][0], &sTestImage);

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
  static u8 u8CurrentRow = 0;

  if (WasButtonPressed(BUTTON0)) {
    ButtonAcknowledge(BUTTON0);
    if (u8CurrentRow + U8_LCD_SMALL_FONT_ROWS < U16_LCD_ROWS - U8_LCD_SMALL_FONT_ROWS) {
      u8CurrentRow += U8_LCD_SMALL_FONT_ROWS;

      LcdClearScreen();
      PixelAddressType sTestStringLocation = {u8CurrentRow, U16_LCD_LEFT_MOST_COLUMN};
      LcdLoadString(UserApp1_au8Name, LCD_FONT_SMALL, &sTestStringLocation);
    }
  }

  if (WasButtonPressed(BUTTON1)) {
    ButtonAcknowledge(BUTTON1);
    if (u8CurrentRow > 0) {
      u8CurrentRow -= U8_LCD_SMALL_FONT_ROWS;

      LcdClearScreen();
      PixelAddressType sTestStringLocation = {u8CurrentRow, U16_LCD_LEFT_MOST_COLUMN};
      LcdLoadString(UserApp1_au8Name, LCD_FONT_SMALL, &sTestStringLocation);
    }
  }
  // PixelAddressType sTestStringLocation = {U8_LCD_SMALL_FONT_LINE4, U16_LCD_LEFT_MOST_COLUMN};
  // u8 au8TestString[] = "Saksham";
  // LcdLoadString(au8TestString, LCD_FONT_SMALL, &sTestStringLocation);
     
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
