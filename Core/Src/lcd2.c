
#include "lcd2.h"
#include "font2.h"

volatile static lcdColor_t LCD_FrontColor = 0x0000, LCD_BackColor = 0xffff;
volatile uint16_t dummy;

#define nop_3() (__nop(), __nop(), __nop())

void LCD_Delay(uint16_t t)
{
}

/* legacy support */

/**
 * @brief Configures the Parallel interface for LCD
 *
 */
void LCD_BusIn(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = LL_GPIO_PIN_ALL;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
 * @brief Configures the Parallel interface for LCD
 *
 */
void LCD_BusOut(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = LL_GPIO_PIN_ALL;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/* Bus Control */

/**
 * @brief Change the parallel interface to output mode
 *
 */
void LCD_busModeOut(void)
{
    uint16_t pinMsk = LL_GPIO_PIN_15;
    while (pinMsk >>= 1)
        LL_GPIO_SetPinMode(GPIOC, pinMsk, LL_GPIO_MODE_OUTPUT);
}

/**
 * @brief Change the parallel interface to input mode
 *
 */
void LCD_busModeIn(void)
{
    uint16_t pinMsk = LL_GPIO_PIN_15;
    while (pinMsk >>= 1)
        LL_GPIO_SetPinMode(GPIOC, pinMsk, LL_GPIO_MODE_INPUT);
}

/**
 * @brief Configures the parallel interface
 *
 */
void LCD_busInit(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = LL_GPIO_PIN_ALL;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
 * @brief Reset the parallel interface to initial state
 *
 */
void LCD_busReset(void)
{
    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin); // nCS inactive (PB9=1)
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin); // nWR inactive (PB5=1)
    LL_GPIO_SetOutputPin(nRD_GPIO_Port, nRD_Pin); // nRD inactive (PA8=1)
    LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);   // RS reset(PB8=1, data)
}

/* Register IO */

/**
 * @brief Read the selected LCD register.
 *
 * @param regIdx Register Index
 * @return uint16_t Register Value
 */
uint16_t LCD_getRegVal(uint8_t regIdx)
{
    uint16_t _dat;

    /* Write index register */
    LCD_busModeOut();                               // switch to output mode
    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin); // nCS active (PB9=0)
    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);   // index reg active (PB8=0)
    LL_GPIO_WriteOutputPort(GPIOC, regIdx);         // write reg
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin); // nWR active (PB5=0)
    nop_3(), nop_3(), nop_3();                      // keep the timing (9WS@170MHz, 52ns)
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);   // nWR inactive (PB5=1)
    LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);     // data reg active (PB8=1)

    /* Read data register */
    LCD_busModeIn();                                // switch to input mode
    LL_GPIO_ResetOutputPin(nRD_GPIO_Port, nRD_Pin); // nRD active (PA8=0)
    nop_3(), nop_3(), nop_3();                      // keep the timing (9WS@170MHz, 52ns)
    _dat = LL_GPIO_ReadInputPort(GPIOC);            // read reg
    LL_GPIO_SetOutputPin(nRD_GPIO_Port, nRD_Pin);   // nRD inactive (PA8=1)
    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin);   // nCS inactive (PB9=1)

    return _dat;
}

/**
 * @brief Write to the selected LCD register
 *
 * @param regIdx Register Index
 * @param regVal Register Value
 */
void LCD_setRegVal(uint8_t regIdx, uint16_t regVal)
{
    /* Write index register */
    LCD_busModeOut();                               // switch to output mode
    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin); // nCS active (PB9=0)
    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);   // index reg active (PB8=0)
    LL_GPIO_WriteOutputPort(GPIOC, regIdx);         // write reg
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin); // nWR active (PB5=0)
    nop_3(), nop_3(), nop_3();                      // keep the timing (9WS@170MHz, 52ns)
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);   // nWR inactive (PB5=1)
    LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);     // data reg active (PB8=1)

    /* Write data register */
    LL_GPIO_WriteOutputPort(GPIOC, regIdx);         // write reg
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin); // nWR active (PB5=0)
    nop_3(), nop_3(), nop_3();                      // keep the timing (9WS@170MHz, 52ns)
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);   // nWR inactive (PB5=1)
    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin);   // nCS inactive (PB9=1)
}

/* GRAM IO */

/**
 * @brief Prepare to read the GRAM of LCD continuously
 *
 */
void LCD_startReadGRAM(void)
{
    /* Write index register */
    LCD_busModeOut();                               // switch to output mode
    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin); // nCS active (PB9=0)
    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);   // index reg active (PB8=0)
    LL_GPIO_WriteOutputPort(GPIOC, 0x22);           // write GRAM IO flag reg
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin); // nWR active (PB5=0)
    nop_3(), nop_3(), nop_3();                      // keep the timing (9WS@170MHz, 52ns)
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);   // nWR inactive (PB5=1)
    LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);     // data reg active (PB8=1)

    /* Dummy read */
    LCD_busModeIn();                                // switch to input mode
    LL_GPIO_ResetOutputPin(nRD_GPIO_Port, nRD_Pin); // nRD active (PA8=0)
    nop_3(), nop_3(), nop_3();                      // keep the timing (9WS@170MHz, 52ns)
    LL_GPIO_ReadInputPort(GPIOC);                   // dummy read
    LL_GPIO_SetOutputPin(nRD_GPIO_Port, nRD_Pin);   // nRD inactive (PA8=1)
}

/**
 * @brief Read the GRAM of LCD
 *
 */
uint16_t LCD_readGRAM(void)
{
    uint16_t _dat;
    LL_GPIO_ResetOutputPin(nRD_GPIO_Port, nRD_Pin); // nRD active (PA8=0)
    nop_3(), nop_3(), nop_3();                      // keep the timing (9WS@170MHz, 52ns)
    LL_GPIO_ReadInputPort(GPIOC);                   // dummy read
    LL_GPIO_SetOutputPin(nRD_GPIO_Port, nRD_Pin);   // nRD inactive (PA8=1)
    return _dat;
}

/**
 * @brief Prepare to write to the GRAM of LCD continuously
 *
 */
void LCD_startWriteGRAM(void)
{
    LCD_busModeOut();                               // switch to output mode
    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin); // nCS active (PB9=0)
    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);   // index reg active (PB8=0)
    LL_GPIO_WriteOutputPort(GPIOC, 0x22);           // write GRAM IO flag reg
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin); // nWR active (PB5=0)
    nop_3(), nop_3(), nop_3();                      // keep the timing (9WS@170MHz, 52ns)
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);   // nWR inactive (PB5=1)
    LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);     // data reg active (PB8=1)
}

/**
 * @brief Write to the GRAM of LCD
 *
 * @param RGBcode the color in RGB565 mode
 */
void LCD_writeGRAM(uint16_t RGBcode)
{
    LL_GPIO_WriteOutputPort(GPIOC, RGBcode);        // write RGB code to GRAM
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin); // nWR active (PB5=0)
    nop_3(), nop_3(), nop_3();                      // keep the timing (9WS@170MHz, 52ns)
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);   // nWR inactive (PB5=1)
}

/**
 * @brief Stop read/write operations to the GRAM of LCD
 *
 */
void LCD_stopGRAM(void)
{
    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin); // nCS inactive (PB9=1)
}

/* Display Control */

/**
 * @brief Enable display
 *
 */
void LCD_displayOn(void)
{
    LCD_setRegVal(0x07, 0x0173);
}

/**
 * @brief Disable display
 *
 */
void LCD_displayOff(void)
{
    LCD_setRegVal(0x07, 0x0);
}

/**
 * @brief Power on the LCD
 *
 */
void LCD_powerOn(void)
{
    LCD_setRegVal(0x10, 0x0000);
    LCD_setRegVal(0x11, 0x0000);
    LCD_setRegVal(0x12, 0x0000);
    LCD_setRegVal(0x13, 0x0000);
    LCD_Delay(20);
    LCD_setRegVal(0x10, 0x17b0);
    LCD_setRegVal(0x11, 0x0137);
    LCD_Delay(5);
    LCD_setRegVal(0x12, 0x0139);
    LCD_Delay(5);
    LCD_setRegVal(0x13, 0x1d00);
    LCD_setRegVal(0x29, 0x0013);
    LCD_Delay(5);
    LCD_setRegVal(0x07, 0x0173);
}

/* LCD init. operations */

/**
 * @brief Configures the register of uc8230
 *
 */
void LCD_init_8230(void)
{
    LCD_setRegVal(0x00, 0x0001);
    LCD_Delay(1000);
    LCD_setRegVal(0x01, 0x0000);
    LCD_setRegVal(0x10, 0x1790);
    LCD_setRegVal(0x60, 0x2700);
    LCD_setRegVal(0x61, 0x0001);
    LCD_setRegVal(0x46, 0x0002);
    LCD_setRegVal(0x13, 0x8010);
    LCD_setRegVal(0x12, 0x80fe);
    LCD_setRegVal(0x02, 0x0500);
    LCD_setRegVal(0x03, 0x1030);

    LCD_setRegVal(0x30, 0x0303);
    LCD_setRegVal(0x31, 0x0303);
    LCD_setRegVal(0x32, 0x0303);
    LCD_setRegVal(0x33, 0x0300);
    LCD_setRegVal(0x34, 0x0003);
    LCD_setRegVal(0x35, 0x0303);
    LCD_setRegVal(0x36, 0x0014);
    LCD_setRegVal(0x37, 0x0303);
    LCD_setRegVal(0x38, 0x0303);
    LCD_setRegVal(0x39, 0x0303);
    LCD_setRegVal(0x3a, 0x0300);
    LCD_setRegVal(0x3b, 0x0003);
    LCD_setRegVal(0x3c, 0x0303);
    LCD_setRegVal(0x3d, 0x1400);

    LCD_setRegVal(0x92, 0x0200);
    LCD_setRegVal(0x93, 0x0303);
    LCD_setRegVal(0x90, 0x080d);
    LCD_setRegVal(0x03, 0x1018);
    LCD_setRegVal(0x07, 0x0173);
}

/**
 * @brief Configures the register of ILI932x
 *
 */
void LCD_init_932x(void)
{
    LCD_setRegVal(0xe3, 0x3008); // Set internal timing
    LCD_setRegVal(0xe7, 0x0012); // Set internal timing
    LCD_setRegVal(0xef, 0x1231); // Set internal timing
    LCD_setRegVal(0x01, 0x0000); // set SS and SM bit, 0x0100
    LCD_setRegVal(0x02, 0x0700); // set 1 line inversion
    LCD_setRegVal(0x03, 0x1030); // set GRAM write direction and BGR=1.
    LCD_setRegVal(0x04, 0x0000); // Resize register
    LCD_setRegVal(0x08, 0x0207); // set the back porch and front porch
    LCD_setRegVal(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
    LCD_setRegVal(0x0a, 0x0000); // FMARK function
    LCD_setRegVal(0x0c, 0x0000); // RGB interface setting
    LCD_setRegVal(0x0d, 0x0000); // Frame marker Position
    LCD_setRegVal(0x0f, 0x0000); // RGB interface polarity

    /**************Power On sequence ****************/
    LCD_setRegVal(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_setRegVal(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
    LCD_setRegVal(0x12, 0x0000); // VREG1OUT voltage
    LCD_setRegVal(0x13, 0x0000); // VDV[4:0] for VCOM amplitude
    msDelay(200);                // Delay 200 MS , Dis-charge capacitor power voltage
    LCD_setRegVal(0x10, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_setRegVal(0x11, 0x0227); // R11H=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]
    msDelay(50);                 // Delay 50ms
    LCD_setRegVal(0x12, 0x001D); // External reference voltage= Vci;
    msDelay(50);                 // Delay 50ms
    LCD_setRegVal(0x13, 0x0800); // R13H=1D00 when R12H=009D;VDV[4:0] for VCOM amplitude
    LCD_setRegVal(0x29, 0x0014); // R29H=0013 when R12H=009D;VCM[5:0] for VCOMH
    LCD_setRegVal(0x2b, 0x000B); // Frame Rate = 96Hz
    msDelay(50);                 // Delay 50ms
    LCD_setRegVal(0x20, 0x0000); // GRAM horizontal Address
    LCD_setRegVal(0x21, 0x0000); // GRAM Vertical Address

    /* ----------- Adjust the Gamma Curve ---------- */
    LCD_setRegVal(0x30, 0x0007);
    LCD_setRegVal(0x31, 0x0707);
    LCD_setRegVal(0x32, 0x0006);
    LCD_setRegVal(0x35, 0x0704);
    LCD_setRegVal(0x36, 0x1F04);
    LCD_setRegVal(0x37, 0x0004);
    LCD_setRegVal(0x38, 0x0000);
    LCD_setRegVal(0x39, 0x0706);
    LCD_setRegVal(0x3c, 0x0701);
    LCD_setRegVal(0x3d, 0x000F);

    /* ------------------ Set GRAM area --------------- */
    LCD_setRegVal(0x50, 0x0000); // Horizontal GRAM Start Address
    LCD_setRegVal(0x51, 0x00EF); // Horizontal GRAM End Address
    LCD_setRegVal(0x52, 0x0000); // Vertical GRAM Start Address
    LCD_setRegVal(0x53, 0x013F); // Vertical GRAM Start Address
    LCD_setRegVal(0x60, 0x2700); // Gate Scan Line, 0xA700
    LCD_setRegVal(0x61, 0x0001); // NDL,VLE, REV
    LCD_setRegVal(0x6a, 0x0000); // set scrolling line

    /* -------------- Partial Display Control --------- */
    LCD_setRegVal(0x80, 0x0000);
    LCD_setRegVal(0x81, 0x0000);
    LCD_setRegVal(0x82, 0x0000);
    LCD_setRegVal(0x83, 0x0000);
    LCD_setRegVal(0x84, 0x0000);
    LCD_setRegVal(0x85, 0x0000);

    /* -------------- Panel Control ------------------- */
    LCD_setRegVal(0x90, 0x0010);
    LCD_setRegVal(0x92, 0x0000);
    LCD_setRegVal(0x93, 0x0003);
    LCD_setRegVal(0x95, 0x0110);
    LCD_setRegVal(0x97, 0x0000);
    LCD_setRegVal(0x98, 0x0000);

    /* Set GRAM write direction and BGR = 1 */
    /* I/D=01 (Horizontal : increment, Vertical : decrement) */
    /* AM=1 (address is updated in vertical writing direction) */
    LCD_setRegVal(0x03, 0x1018); // 0x1018
    LCD_setRegVal(0x07, 0x0173); // 262K color and display ON
}

/* Public Function */

/**
 * @brief Initializes the LCD
 *
 */
void LCD_init(void)
{
    LCD_busInit();

    dummy = LCD_getRegVal(0x0);
    if (dummy == 0x8230)
        REG_8230_Init();
    else
        REG_932X_Init();
    dummy = LCD_getRegVal(0x0);
}

/**
 * @brief Set the front color
 *
 * @param color Front color code in RGB565
 */
void LCD_setFrontColor(lcdColor_t color)
{
    LCD_FrontColor = color;
}

/**
 * @brief Set the background color
 *
 * @param color Background color code in RGB565
 */
void LCD_setBackColor(lcdColor_t color)
{
    LCD_BackColor = color;
}

/**
 * @brief Clear the whole LCD with the selected color
 *
 * @param color Color code in RGB565
 */
void LCD_clear(lcdColor_t color)
{
    uint32_t pixIdx = 76800;
    LCD_setCursor(0, 0);
    LCD_startWriteGRAM();
    while (pixIdx--)
        LCD_writeGRAM(color);
    LCD_stopGRAM();
}

/**
 * @brief Sets the cursor position.
 *
 * @param Xpos
 * @param Ypos
 */
void LCD_setCursor(uint8_t Xpos, uint16_t Ypos)
{
    LCD_setRegVal(0x20, Xpos);
    LCD_setRegVal(0x21, Ypos);
}

void LCD_dispChar(uint8_t Xpos,uint16_t Ypos,const uint16_t *c)
{
    
}