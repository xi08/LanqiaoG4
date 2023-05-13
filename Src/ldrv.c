#include "ldrv.h"
#include "lfont.h"

uint16_t lcdBackColor = 0x0000, lcdFrontColor = 0xffff;

#define lcdDelay() (__nop(), __nop(), __nop(), __nop(), __nop(), __nop(), __nop())
#define lcdDelayL(t) (HAL_Delay(t))

/* Bus Control Function */
/**
 * @brief Set bus to input mode
 *
 */
void lcdBusI(void)
{
    LL_GPIO_SetPinMode(LC0_GPIO_Port, LC0_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC1_GPIO_Port, LC1_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC2_GPIO_Port, LC2_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC3_GPIO_Port, LC3_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC4_GPIO_Port, LC4_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC5_GPIO_Port, LC5_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC6_GPIO_Port, LC6_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC7_GPIO_Port, LC7_Pin, LL_GPIO_MODE_INPUT);

    LL_GPIO_SetPinMode(LD0_GPIO_Port, LD0_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD1_GPIO_Port, LD1_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD2_GPIO_Port, LD2_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD3_GPIO_Port, LD3_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD4_GPIO_Port, LD4_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD5_GPIO_Port, LD5_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD6_GPIO_Port, LD6_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD7_GPIO_Port, LD7_Pin, LL_GPIO_MODE_INPUT);
}

/**
 * @brief Set bus to output mode
 *
 */
void lcdBusO(void)
{
    LL_GPIO_SetPinMode(LC0_GPIO_Port, LC0_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC1_GPIO_Port, LC1_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC2_GPIO_Port, LC2_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC3_GPIO_Port, LC3_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC4_GPIO_Port, LC4_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC5_GPIO_Port, LC5_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC6_GPIO_Port, LC6_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC7_GPIO_Port, LC7_Pin, LL_GPIO_MODE_OUTPUT);

    LL_GPIO_SetPinMode(LD0_GPIO_Port, LD0_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD1_GPIO_Port, LD1_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD2_GPIO_Port, LD2_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD3_GPIO_Port, LD3_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD4_GPIO_Port, LD4_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD5_GPIO_Port, LD5_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD6_GPIO_Port, LD6_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD7_GPIO_Port, LD7_Pin, LL_GPIO_MODE_OUTPUT);
}

/* LCD Base Control Function */
/**
 * @brief Write the register
 *
 * @param regAddr Register address
 * @param regVal Register value
 */
void lcdWriteReg(uint8_t regAddr, uint16_t regVal)
{
    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin);

    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, regAddr);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);

    LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, regVal);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);

    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin);
}

/**
 * @brief Read the register
 *
 * @param regAddr Register address
 * @return uint16_t Register value
 */
uint16_t lcdReadReg(uint8_t regAddr)
{
    uint16_t dat = 0;

    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin);

    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, regAddr);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);

    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);
    lcdBusI();
    LL_GPIO_ResetOutputPin(nRD_GPIO_Port, nRD_Pin);
    lcdDelay();
    dat = LL_GPIO_ReadInputPort(LC0_GPIO_Port);
    LL_GPIO_SetOutputPin(nRD_GPIO_Port, nRD_Pin);
    lcdBusO();

    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin);

    return dat;
}

/**
 * @brief Write to GRAM
 *
 * @param RGBcode RGB565 code array
 * @param dataSize Array sizr
 */
void lcdWriteRAM(uint16_t *RGBcode, uint16_t dataSize)
{
    uint16_t i = 0;

    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin);

    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, 0x22);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);

    for (i = 0; i < dataSize; i++)
    {
        LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);
        LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
        LL_GPIO_WriteOutputPort(LC0_GPIO_Port, RGBcode[i]);
        LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
        lcdDelay();
        LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    }

    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin);
}

/**
 * @brief Prepare for write to GRAM
 *
 */
void lcdWriteRAM0(void)
{
    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin);

    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, 0x22);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
}

/**
 * @brief Write to GRAM
 *
 * @param RGBcode RGB565 code
 */
void lcdWriteRAM1(uint16_t RGBcode)
{
    LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, RGBcode);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
}

/**
 * @brief End write to GRAM
 *
 */
void lcdWriteRAM2(void)
{
    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin);
}

/**
 * @brief Read GRAM
 *
 * @param RGBcode RGB565 code buffer
 * @param dataSize read size
 */
void lcdReadRAM(uint16_t *RGBcode, uint16_t dataSize)
{
    uint16_t i = 0;

    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin);

    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, 0x22);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);

    lcdBusI();
    LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_ResetOutputPin(nRD_GPIO_Port, nRD_Pin);
    lcdDelay();
    LL_GPIO_ReadInputPort(LC0_GPIO_Port);
    LL_GPIO_SetOutputPin(nRD_GPIO_Port, nRD_Pin);

    for (i = 0; i < dataSize; i++)
    {
        LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);
        LL_GPIO_ResetOutputPin(nRD_GPIO_Port, nRD_Pin);
        lcdDelay();
        RGBcode[i] = LL_GPIO_ReadInputPort(LC0_GPIO_Port);
        LL_GPIO_SetOutputPin(nRD_GPIO_Port, nRD_Pin);
    }
    lcdBusO();

    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin);
}

/* LCD Display Control Function */
/**
 * @brief LCD init. for ili9325/ili9328
 *
 * @param lfr LCD framerate
 */
void lcdInit_932x(lcdDispFR_t lfr)
{
    lcdWriteReg(0xe3, 0x3008); // Set internal timing
    lcdWriteReg(0xe7, 0x0012); // Set internal timing
    lcdWriteReg(0xef, 0x1231); // Set internal timing

    lcdWriteReg(0x10, 0x0000); // Set power control
    lcdWriteReg(0x11, 0x0007); // Set power control
    lcdWriteReg(0x12, 0x0000); // Set power control
    lcdWriteReg(0x13, 0x0000); // Set power control
    lcdDelayL(200);            // Set power control
    lcdWriteReg(0x10, 0x1690); // Set power control
    lcdWriteReg(0x11, 0x0227); // Set power control
    lcdDelayL(50);             // Set power control
    lcdWriteReg(0x12, 0x001d); // Set power control
    lcdDelayL(50);             // Set power control
    lcdWriteReg(0x13, 0x0800); // Set power control
    lcdWriteReg(0x29, 0x0014); // Set power control
    lcdDelayL(50);             // Set power control

    lcdWriteReg(0x30, 0x0007); // Adjust the gamma curve
    lcdWriteReg(0x31, 0x0707); // Adjust the gamma curve
    lcdWriteReg(0x32, 0x0006); // Adjust the gamma curve
    lcdWriteReg(0x35, 0x0704); // Adjust the gamma curve
    lcdWriteReg(0x36, 0x1f04); // Adjust the gamma curve
    lcdWriteReg(0x37, 0x0004); // Adjust the gamma curve
    lcdWriteReg(0x38, 0x0000); // Adjust the gamma curve
    lcdWriteReg(0x39, 0x0706); // Adjust the gamma curve
    lcdWriteReg(0x3c, 0x0701); // Adjust the gamma curve
    lcdWriteReg(0x3d, 0x000f); // Adjust the gamma curve

    lcdWriteReg(0x90, 0x0010); // Set panel interface
    lcdWriteReg(0x92, 0x0000); // Set panel interface
    lcdWriteReg(0x95, 0x0110); // Set panel interface
    lcdWriteReg(0x97, 0x0000); // Set panel interface

    lcdWriteReg(0x61, 0x0001); // NDL, VLE, REV
    lcdWriteReg(0x6a, 0x0000); // Set scrolling line
    lcdWriteReg(0x02, 0x0700); // Set line inversion
    lcdWriteReg(0x04, 0x0000); // Set resize register
    lcdWriteReg(0x07, 0x0173); // Display Control
    lcdWriteReg(0x08, 0x0207); // Set back & front porch
    lcdWriteReg(0x09, 0x0000); // Set non-disp. area mode
    lcdWriteReg(0x50, 0x0000); // Horizontal GRAM Start Address
    lcdWriteReg(0x51, 0x00ef); // Horizontal GRAM End Address
    lcdWriteReg(0x52, 0x0000); // Vertical GRAM Start Address
    lcdWriteReg(0x53, 0x013f); // Vertical GRAM Start Address
    lcdWriteReg(0x20, 0x0000); // GRAM horizontal Address
    lcdWriteReg(0x21, 0x0000); // GRAM Vertical Address

    lcdWriteReg(0x60, 0x2700); // Gate Scan Line & GS
    lcdWriteReg(0x01, 0x0000); // SS & SM
    lcdWriteReg(0x03, 0x1018); // Set entry mode
    lcdWriteReg(0x2b, lfr);    // Set framerate

    lcdWriteReg(0x07, 0x0173); // 262K color and display ON
}

/**
 * @brief LCD init. for uc8230
 *
 */
void lcdInit_8230(void)
{
    lcdWriteReg(0x00, 0x0001); // Start auto power-up
    lcdDelayL(40);

    lcdWriteReg(0x10, 0x1790); // Set power control
    lcdWriteReg(0x12, 0x80fe); // Set power control
    lcdWriteReg(0x13, 0x8010); // Set power control

    lcdWriteReg(0x30, 0x0303); // Adjust the gamma curve
    lcdWriteReg(0x31, 0x0303); // Adjust the gamma curve
    lcdWriteReg(0x32, 0x0303); // Adjust the gamma curve
    lcdWriteReg(0x33, 0x0300); // Adjust the gamma curve
    lcdWriteReg(0x34, 0x0003); // Adjust the gamma curve
    lcdWriteReg(0x35, 0x0303); // Adjust the gamma curve
    lcdWriteReg(0x36, 0x0014); // Adjust the gamma curve
    lcdWriteReg(0x37, 0x0303); // Adjust the gamma curve
    lcdWriteReg(0x38, 0x0303); // Adjust the gamma curve
    lcdWriteReg(0x39, 0x0303); // Adjust the gamma curve
    lcdWriteReg(0x3a, 0x0300); // Adjust the gamma curve
    lcdWriteReg(0x3b, 0x0003); // Adjust the gamma curve
    lcdWriteReg(0x3c, 0x0303); // Adjust the gamma curve
    lcdWriteReg(0x3d, 0x1400); // Adjust the gamma curve

    lcdWriteReg(0x90, 0x080d); // Set panel interface
    lcdWriteReg(0x92, 0x0200); // Set panel interface
    lcdWriteReg(0x93, 0x0303); // Set panel interface

    lcdWriteReg(0x61, 0x0001); // NDL, VLE, REV
    lcdWriteReg(0x02, 0x0500); // Set line inversion
    lcdWriteReg(0x04, 0x0000); // Set resize register
    lcdWriteReg(0x07, 0x0173); // Display Control
    lcdWriteReg(0x50, 0x0000); // Horizontal GRAM Start Address
    lcdWriteReg(0x51, 0x00ef); // Horizontal GRAM End Address
    lcdWriteReg(0x52, 0x0000); // Vertical GRAM Start Address
    lcdWriteReg(0x53, 0x013f); // Vertical GRAM Start Address
    lcdWriteReg(0x20, 0x0000); // GRAM horizontal Address
    lcdWriteReg(0x21, 0x0000); // GRAM Vertical Address

    lcdWriteReg(0x60, 0x2700); // Gate Scan Line & GS
    lcdWriteReg(0x01, 0x0000); // SS & SM
    lcdWriteReg(0x03, 0x1018); // Set entry mode
}

/**
 * @brief LCD display character
 *
 * @param Xst Start position in X
 * @param Yst Start position in Y
 * @param fn Character font
 */
void lcdDispFont(uint16_t Xst, uint16_t Yst, const uint16_t *fn)
{
    uint16_t Xaddr = Xst;
    uint8_t fontX, fontY;

    lcdSetCursor(Xaddr, Yst);
    for (fontX = 0; fontX < 24; fontX++)
    {
        lcdWriteRAM0();
        for (fontY = 0; fontY < 16; fontY++)
        {
            if (fn[fontX] & (1 << fontY))
                lcdWriteRAM1(lcdFrontColor);
            else
                lcdWriteRAM1(lcdBackColor);
        }
        lcdWriteRAM2();
        lcdSetCursor(++Xaddr, Yst);
    }
}

/* LCD High-layer Function */
/**
 * @brief LCD Init.
 *
 * @param lfr LCD framerate
 */
void lcdInit(lcdDispFR_t lfr)
{
    uint16_t lcdID = lcdReadReg(0x00);
    if (lcdID == 0x8230)
        lcdInit_8230();
    else
        lcdInit_932x(lfr);
}

/**
 * @brief Set back color
 *
 * @param lc RGB565 code
 */
void lcdSetBackColor(uint16_t lc)
{
    lcdBackColor = lc;
}

/**
 * @brief Set front color
 *
 * @param lc RGB565 code
 */
void lcdSetFrontColor(uint16_t lc)
{
    lcdFrontColor = lc;
}

/**
 * @brief Set the refresh cursor
 *
 * @param Xpos Position in X
 * @param Ypos Position in Y
 */
void lcdSetCursor(uint16_t Xpos, uint16_t Ypos)
{
    lcdWriteReg(0x20, Xpos);
    lcdWriteReg(0x21, Ypos);
}

/**
 * @brief Clear the screen & fill with back color
 *
 */
void lcdClearScreen(void)
{
    uint32_t i = (320 * 240);
    lcdSetCursor(0, 0);
    lcdWriteRAM0();
    while (i--)
        lcdWriteRAM1(lcdBackColor);
    lcdWriteRAM2();
}

void lcdDisplayChar(uint16_t Xpos, uint16_t Ypos, char ch)
{
    lcdDispFont(Xpos, Ypos, ascii_map[ch - 32]);
}

void lcdDisplayString(uint16_t Line, char *str)
{
    uint8_t i = 20;
    uint16_t colPos = 319;

    while (*str && i--)
    {
        lcdDisplayChar(Line, colPos, *str++);
        colPos -= 16;
    }
}

void lcdClearLine(uint16_t Line)
{
    uint8_t i = 20;
    uint16_t colPos = 319;

    while (i--)
    {
        lcdDisplayChar(Line, colPos, ' ');
        colPos -= 16;
    }
}
