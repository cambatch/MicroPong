#ifndef DRIVER_ILI9341_H
#define DRIVER_ILI9341_H

#include <stdint.h>

#include "f446re.h"

//  ==================== Must configure these ====================
// SPI2 pins: PB13=SCK, PB14=MISO, PB15=MOSI
#define ILI9341_SPI_PERIPHERAL  SPI2
#define ILI9341_SPI_GPIO_PORT   GPIOB
#define ILI9341_SPI_SCL_PIN     GPIO_PIN_13
#define ILI9341_SPI_MISO_PIN    GPIO_PIN_14
#define ILI9341_SPI_MOSI_PIN    GPIO_PIN_15

// Control pins:
#define ILI9341_CONTROL_PORT    GPIOB
#define ILI9341_CS_PIN          GPIO_PIN_5
#define ILI9341_DC_PIN          GPIO_PIN_6
#define ILI9341_RST_PIN         GPIO_PIN_7
// ===============================================================

#define ILI9341_TFTWIDTH   240
#define ILI9341_TFTHEIGHT  320

typedef enum ili9341_rot_t
{
    ILI9341_ROT_0 = 0, // portrait
    ILI9341_ROT_90,    // landscape
    ILI9341_ROT_180,   // portrait
    ILI9341_ROT_270    // landscape
} ili9341_rot_t;

typedef struct
{
    uint8_t pixel_format;
    ili9341_rot_t rotation;
    bool invert_on_init;
} ili9341_config_t;

// Commands
#define ILI9341_CMD_NOP              0x00
#define ILI9341_CMD_SOFTWARE_RESET   0x01
#define ILI9341_CMD_READ_ID          0x04
#define ILI9341_CMD_READ_STATUS      0x09
#define ILI9341_CMD_READ_POWER_MODE  0x0A
#define ILI9341_CMD_READ_MADCTL      0x0B
#define ILI9341_CMD_READ_PIXEL_FMT   0x0C
#define ILI9341_CMD_READ_DISPLAY_ON  0x0D
#define ILI9341_CMD_READ_ID1         0xDA
#define ILI9341_CMD_READ_ID2         0xDB
#define ILI9341_CMD_READ_ID3         0xDC

#define ILI9341_CMD_SLEEP_IN         0x10
#define ILI9341_CMD_SLEEP_OUT        0x11
#define ILI9341_CMD_DISPLAY_INV_OFF  0x20
#define ILI9341_CMD_DISPLAY_INV_ON   0x21
#define ILI9341_CMD_DISPLAY_OFF      0x28
#define ILI9341_CMD_DISPLAY_ON       0x29
#define ILI9341_CMD_COLUMN_ADDR      0x2A
#define ILI9341_CMD_PAGE_ADDR        0x2B
#define ILI9341_CMD_MEMORY_WRITE     0x2C
#define ILI9341_CMD_MEMORY_READ      0x2E

#define ILI9341_CMD_PARTIAL_AREA     0x30
#define ILI9341_CMD_VERT_SCROLL_DEF  0x33
#define ILI9341_CMD_TEARING_OFF      0x34
#define ILI9341_CMD_TEARING_ON       0x35
#define ILI9341_CMD_MEMORY_ACCESS    0x36
#define ILI9341_CMD_PIXEL_FORMAT     0x3A

#define ILI9341_CMD_FRAME_RATE_CTRL1 0xB1
#define ILI9341_CMD_FRAME_RATE_CTRL2 0xB2
#define ILI9341_CMD_FRAME_RATE_CTRL3 0xB3
#define ILI9341_CMD_DISPLAY_INVERT   0xB4
#define ILI9341_CMD_DISPLAY_FUNCTION 0xB6
#define ILI9341_CMD_ENTRY_MODE       0xB7

#define ILI9341_CMD_POWER_CTRL1      0xC0
#define ILI9341_CMD_POWER_CTRL2      0xC1
#define ILI9341_CMD_VCOM_CTRL1       0xC5
#define ILI9341_CMD_VCOM_CTRL2       0xC7

#define ILI9341_CMD_POWER_CTRL_A     0xCB
#define ILI9341_CMD_POWER_CTRL_B     0xCF
#define ILI9341_CMD_DRIVER_TIMING_A  0xE8
#define ILI9341_CMD_DRIVER_TIMING_B  0xEA
#define ILI9341_CMD_POWER_ON_SEQ     0xED
#define ILI9341_CMD_ENABLE_3G        0xF2
#define ILI9341_CMD_PUMP_RATIO_CTRL  0xF7

#define ILI9341_CMD_GAMMA_SET        0x26
#define ILI9341_CMD_POS_GAMMA        0xE0
#define ILI9341_CMD_NEG_GAMMA        0xE1

// MADCTL values
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_BGR 0x08
#define MADCTL_MH 0x04

#define ILI9341_PIXEL_FORMAT_RGB565 0x55

// Colors
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF
#define COLOR_RED   0xF800
#define COLOR_BLUE  0x001F

#define COLOR_YELLOW 0xFFE0
#define COLOR_CYAN   0x07FF
#define COLOR_MAGENTA 0xF81F

#define COLOR_GRAY    0x8410
#define COLOR_ORANGE  0xFD20
#define COLOR_PINK    0xF81F
#define COLOR_PURPLE  0x780F

/**
 * @brief Initializes the ILI9341 display with the given configuration.
 * 
 * Performs hardware and software rests, configures rotation, pixel format,
 * inversion, and powers on the display. If no configuration is provided,
 * defaults are used.
 * 
 * @param config Ponter to configuration structure, or NULL to use defaults.
 */
void ili9341_init(const ili9341_config_t *config);

/**
 * @brief Performs a hardware reset of the ILI9341 display.
 *
 * Toggles the reset pin and delays according to the specified reset mode.
 *
 * @param worst_case If true, applies the full reset delay (120 ms).
 *                   If false, uses a shorter delay (10 ms).
 */
void ili9341_hardware_reset(bool worst_case);

/**
 * @brief Issues a software reset command to the display.
 *
 * Sends the software reset command and delays to allow the panel
 * to reinitialize internally.
 */
void ili9341_software_reset(void);

/**
 * @brief Sets the display rotation.
 *
 * Updates internal state, recalculates dimensions, and writes
 * the corresponding MADCTL register value.
 *
 * @param rotation Desired rotation (0, 90, 180, 270 degrees).
 */
void ili9341_set_rotation(ili9341_rot_t rotation);

/**
 * @brief Gets the current display rotation.
 *
 * @return The current rotation setting.
 */
ili9341_rot_t ili9341_get_rotation(void);

void ili9341_get_screen_size(uint16_t *width, uint16_t *height);

/**
 * @brief Enables or disables display color inversion.
 *
 * @param enable True to enable inversion, false to disable.
 */
void ili9341_set_invert(bool enable);

/**
 * @brief Turns the display on.
 *
 * Sends the display-on command and waits for stabilization.
 */
void ili9341_display_on(void);

/**
 * @brief Turns the display off.
 *
 * Sends the display-off command and waits for stabilization.
 */
void ili9341_display_off(void);

/**
 * @brief Puts the display into sleep mode.
 *
 * Sends the sleep-in command and delays to allow the panel
 * to enter low-power mode.
 */
void ili9341_sleep_in(void);

/**
 * @brief Wakes the display from sleep mode.
 *
 * Sends the sleep-out command and delays for stabilization.
 */
void ili9341_sleep_out(void);

/**
 * @brief Defines the active drawing window.
 *
 * Sets the address range for subsequent pixel data transfers.
 *
 * @param x X-coordinate of the top-left corner.
 * @param y Y-coordinate of the top-left corner.
 * @param w Width of the window in pixels.
 * @param h Height of the window in pixels.
 */
void ili9341_set_addr_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h);


/**
 * @brief Draws a single pixel on the display.
 *
 * @param x X-coordinate of the pixel.
 * @param y Y-coordinate of the pixel.
 * @param color 16-bit RGB565 color value.
 */
void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief Fills a rectangular area with a solid color.
 *
 * @param x X-coordinate of the top-left corner.
 * @param y Y-coordinate of the top-left corner.
 * @param w Width of the rectangle in pixels.
 * @param h Height of the rectangle in pixels.
 * @param color 16-bit RGB565 color value.
 */
void ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/**
 * @brief Draws a horizontal line.
 *
 * @param x Starting X-coordinate.
 * @param y Y-coordinate of the line.
 * @param w Width of the line in pixels.
 * @param color 16-bit RGB565 color value.
 */
void ili9341_draw_hline(uint16_t x, uint16_t y, uint16_t w, uint16_t color);

/**
 * @brief Draws a vertical line.
 *
 * @param x X-coordinate of the line.
 * @param y Starting Y-coordinate.
 * @param h Height of the line in pixels.
 * @param color 16-bit RGB565 color value.
 */
void ili9341_draw_vline(uint16_t x, uint16_t y, uint16_t h, uint16_t color);

/**
 * @brief Fills the entire display with a solid color.
 *
 * @param color 16-bit RGB565 color value.
 */
void ili9341_fill_screen(uint16_t color);

#endif