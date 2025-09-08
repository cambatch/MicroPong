#include "ili9341.h"

// Driver state
typedef struct
{
    uint16_t width;
    uint16_t height;
    ili9341_rot_t rotation;
    uint8_t pixel_format;
    bool invert;
    uint8_t madctl;

} ili9341_context_t;

static ili9341_context_t g_context;

// Helpers
static inline void CS_LOW(void)   { gpio_write_pin(ILI9341_CONTROL_PORT, ILI9341_CS_PIN, 0);  }
static inline void CS_HIGH(void)  { gpio_write_pin(ILI9341_CONTROL_PORT, ILI9341_CS_PIN, 1);  }
static inline void DC_LOW(void)   { gpio_write_pin(ILI9341_CONTROL_PORT, ILI9341_DC_PIN, 0);  }
static inline void DC_HIGH(void)  { gpio_write_pin(ILI9341_CONTROL_PORT, ILI9341_DC_PIN, 1);  }
static inline void RST_LOW(void)  { gpio_write_pin(ILI9341_CONTROL_PORT, ILI9341_RST_PIN, 0); }
static inline void RST_HIGH(void) { gpio_write_pin(ILI9341_CONTROL_PORT, ILI9341_RST_PIN, 1); }
static inline void BARRIER(void)  { __asm volatile ("dsb"); }
static inline void SPI_WAIT_IDLE(void) { while(spi_flag_status(ILI9341_SPI_PERIPHERAL, SPI_FLAG_BUSY)); }


static void update_dims_from_rotation(void)
{
    if (g_context.rotation == ILI9341_ROT_0 || g_context.rotation == ILI9341_ROT_180)
    {
        g_context.width = ILI9341_TFTWIDTH;
        g_context.height = ILI9341_TFTHEIGHT;
    }
    else
    {
        g_context.width = ILI9341_TFTHEIGHT;
        g_context.height = ILI9341_TFTWIDTH;
    }
}

static void ili9341_send_cmd(uint8_t cmd)
{
    // Interpret as command
    DC_LOW(); BARRIER();

    // Start SPI communication
    CS_LOW(); BARRIER();

    spi_send(ILI9341_SPI_PERIPHERAL, &cmd, 1);

    SPI_WAIT_IDLE();

    // End SPI communication
    CS_HIGH(); BARRIER();
}

static void ili9341_send_cmd_data(uint8_t cmd, const uint8_t *data, uint32_t data_bytes)
{
    // Interpret as command
    DC_LOW(); BARRIER();

    // Start SPI communication
    CS_LOW(); BARRIER();

    spi_send(ILI9341_SPI_PERIPHERAL, &cmd, 1);

    SPI_WAIT_IDLE();

    // Interpret as parameters
    DC_HIGH(); BARRIER();

    while(data_bytes--)
    {
        spi_send(ILI9341_SPI_PERIPHERAL, data++, 1);
    }

    SPI_WAIT_IDLE();

    // End SPI communication
    CS_HIGH(); BARRIER();
}

static void ili9341_start_stream(void)
{
    DC_LOW(); BARRIER();
    CS_LOW(); BARRIER();

    uint8_t cmd = ILI9341_CMD_MEMORY_WRITE;
    spi_send(ILI9341_SPI_PERIPHERAL, &cmd, 1);

    SPI_WAIT_IDLE();

    DC_HIGH(); BARRIER();
}

static void ili9341_write_pixels(uint16_t *colors, uint32_t count)
{
    while(count--)
    {
        uint8_t hi = (uint8_t)((*colors) >> 8);
        uint8_t lo = (uint8_t)((*colors) & 0xFF);
        spi_send(ILI9341_SPI_PERIPHERAL, &hi, 1);
        spi_send(ILI9341_SPI_PERIPHERAL, &lo, 1);
        ++colors;
    }
    SPI_WAIT_IDLE();
}

static void ili9341_end_stream(void)
{
    SPI_WAIT_IDLE();
    CS_HIGH(); BARRIER();
}

static void ili9341_set_column(uint16_t x0, uint16_t x1)
{
    uint8_t p[4] = {
        (uint8_t)(x0 >> 8), (uint8_t)(x0 & 0xFF),
        (uint8_t)(x1 >> 8), (uint8_t)(x1 & 0xFF)
    };
    ili9341_send_cmd_data(ILI9341_CMD_COLUMN_ADDR, p, 4);
}

static void ili9341_set_page(uint16_t y0, uint16_t y1)
{
    uint8_t p[4] = {
        (uint8_t)(y0 >> 8), (uint8_t)(y0 & 0xFF),
        (uint8_t)(y1 >> 8), (uint8_t)(y1 & 0xFF)
    };
    ili9341_send_cmd_data(ILI9341_CMD_PAGE_ADDR, p, 4);
}

static uint8_t rotation_to_madctl(ili9341_rot_t r)
{
    switch(r)
    {
        default:
        case ILI9341_ROT_0: return MADCTL_MX | MADCTL_BGR;
        case ILI9341_ROT_90: return MADCTL_MV | MADCTL_BGR;
        case ILI9341_ROT_180: return MADCTL_MY | MADCTL_BGR;
        case ILI9341_ROT_270: return MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR;
    }
}

void ili9341_hardware_reset(bool worst_case)
{
    CS_HIGH();
    DC_HIGH();

    RST_LOW(); BARRIER();
    dwt_delay_us(15);

    RST_HIGH(); BARRIER();

    if(worst_case) dwt_delay_ms(120);
    else           dwt_delay_ms(10);
}

void ili9341_software_reset()
{
    ili9341_send_cmd(ILI9341_CMD_SOFTWARE_RESET);

    dwt_delay_ms(10U);
}

void ili9341_init(const ili9341_config_t *config)
{
    // config defaults
    g_context.rotation = ILI9341_ROT_0;
    g_context.pixel_format = ILI9341_PIXEL_FORMAT_RGB565;
    g_context.madctl = rotation_to_madctl(g_context.rotation);
    g_context.width = ILI9341_TFTWIDTH;
    g_context.height = ILI9341_TFTHEIGHT;
    g_context.invert = false;

    if(config)
    {
        g_context.rotation = config->rotation;
        g_context.invert = config->invert_on_init;
        g_context.pixel_format = config->pixel_format;
    }

    update_dims_from_rotation();

    ili9341_hardware_reset(true);

    ili9341_software_reset();

    ili9341_sleep_out();

    {
        uint8_t p = g_context.pixel_format;
        ili9341_send_cmd_data(ILI9341_CMD_PIXEL_FORMAT, &p, 1);
    }

    g_context.madctl = rotation_to_madctl(g_context.rotation);
    ili9341_send_cmd_data(ILI9341_CMD_MEMORY_ACCESS, &g_context.madctl, 1);

    if(g_context.invert) ili9341_send_cmd(ILI9341_CMD_DISPLAY_INV_ON);
    else                 ili9341_send_cmd(ILI9341_CMD_DISPLAY_INV_OFF);

    ili9341_display_on();
}

void ili9341_set_rotation(ili9341_rot_t rotation)
{
    g_context.rotation = rotation;
    update_dims_from_rotation();
    g_context.madctl = rotation_to_madctl(rotation);
    ili9341_send_cmd_data(ILI9341_CMD_MEMORY_ACCESS, &g_context.madctl, 1);
}

ili9341_rot_t ili9341_get_rotation(void)
{
    return g_context.rotation;
}

void ili9341_get_screen_size(uint16_t *width, uint16_t *height)
{
    *width = g_context.width;
    *height = g_context.height;
}

void ili9341_set_invert(bool enable)
{
    if(enable) ili9341_send_cmd(ILI9341_CMD_DISPLAY_INV_ON);
    else       ili9341_send_cmd(ILI9341_CMD_DISPLAY_INV_OFF);

    g_context.invert = enable;
}

void ili9341_display_on(void)
{
    ili9341_send_cmd(ILI9341_CMD_DISPLAY_ON);
    dwt_delay_ms(10);
}

void ili9341_display_off(void)
{
    ili9341_send_cmd(ILI9341_CMD_DISPLAY_OFF);
    dwt_delay_ms(10);
}

void ili9341_sleep_in(void)
{
    ili9341_send_cmd(ILI9341_CMD_SLEEP_IN);
    dwt_delay_ms(120);
}

void ili9341_sleep_out(void)
{
    ili9341_send_cmd(ILI9341_CMD_SLEEP_OUT);
    dwt_delay_ms(120);
}

void ili9341_set_addr_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint16_t x1 = (uint16_t)(x + w - 1U);
    uint16_t y1 = (uint16_t)(y + h - 1U);
    ili9341_set_column(x, x1);
    ili9341_set_page(y, y1);
}

void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    ili9341_set_addr_window(x, y, 1, 1);

    ili9341_start_stream();
    ili9341_write_pixels(&color, 1);
    ili9341_end_stream();
}

void ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    ili9341_set_addr_window(x, y, w, h);

    ili9341_start_stream();

    uint8_t hi = (uint8_t)(color >> 8);
    uint8_t lo = (uint8_t)color;
    const uint32_t total = (uint32_t)w * (uint32_t)h;
    const uint32_t BATCH_PIXELS = 64;
    uint8_t buffer[BATCH_PIXELS * 2];

    for(uint32_t i = 0; i < BATCH_PIXELS; ++i)
    {
        buffer[2*i + 0] = hi;
        buffer[2*i + 1] = lo;
    }

    uint32_t remaining = total;

    while(remaining)
    {
        uint32_t chunk = (remaining > BATCH_PIXELS) ? BATCH_PIXELS : remaining;
        spi_send(ILI9341_SPI_PERIPHERAL, buffer, chunk * 2U);
        remaining -= chunk;
    }

    SPI_WAIT_IDLE();
    ili9341_end_stream();
}

void ili9341_draw_hline(uint16_t x, uint16_t y, uint16_t w, uint16_t color)
{
    ili9341_set_addr_window(x, y, w, 1);

    ili9341_start_stream();

    for(uint32_t i = 0; i < w; ++i)
    {
        ili9341_write_pixels(&color, 1);
    }

    ili9341_end_stream();
}

void ili9341_draw_vline(uint16_t x, uint16_t y, uint16_t h, uint16_t color)
{
    ili9341_set_addr_window(x, y, 1, h);

    ili9341_start_stream();

    for(uint32_t i = 0; i < h; ++i)
    {
        ili9341_write_pixels(&color, 1);
    }

    ili9341_end_stream();
}

void ili9341_fill_screen(uint16_t color)
{
    ili9341_set_addr_window(0, 0, g_context.width, g_context.height);

    ili9341_start_stream();

    uint8_t hi = (uint8_t)(color >> 8);
    uint8_t lo = (uint8_t)color;
    const uint32_t total = ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT;
    const uint32_t BATCH_PIXELS = 64;
    uint8_t buffer[BATCH_PIXELS * 2];

    for(uint32_t i = 0; i < BATCH_PIXELS; ++i)
    {
        buffer[2*i + 0] = hi;
        buffer[2*i + 1] = lo;
    }

    uint32_t remaining = total;

    while(remaining)
    {
        uint32_t chunk = (remaining > BATCH_PIXELS) ? BATCH_PIXELS : remaining;
        spi_send(ILI9341_SPI_PERIPHERAL, buffer, chunk * 2U);
        remaining -= chunk;
    }

    SPI_WAIT_IDLE();

    CS_HIGH(); BARRIER();
}
