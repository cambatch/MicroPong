#include "ili9341.h"

static spi_handle_t s_spi;


static inline void CS_LOW(void)   { gpio_write_pin(ILI9341_CONTROL_PORT, ILI9341_CS_PIN, 0);  }
static inline void CS_HIGH(void)  { gpio_write_pin(ILI9341_CONTROL_PORT, ILI9341_CS_PIN, 1);  }
static inline void DC_LOW(void)   { gpio_write_pin(ILI9341_CONTROL_PORT, ILI9341_DC_PIN, 0);  }
static inline void DC_HIGH(void)  { gpio_write_pin(ILI9341_CONTROL_PORT, ILI9341_DC_PIN, 1);  }
static inline void RST_LOW(void)  { gpio_write_pin(ILI9341_CONTROL_PORT, ILI9341_RST_PIN, 0); }
static inline void RST_HIGH(void) { gpio_write_pin(ILI9341_CONTROL_PORT, ILI9341_RST_PIN, 1); }

static inline void barrier() { __asm volatile ("dsb"); }

static void init_gpio(void)
{
    gpio_handle_t gh = { 0 };

    // SPI GPIO pins
    gh.gpiox = ILI9341_SPI_GPIO_PORT;
    gh.config.mode = GPIO_MODE_ALTFN;
    gh.config.altfn = 5;
    gh.config.otype = GPIO_OTYPE_PP;
    gh.config.pupd = GPIO_PUPD_DI;
    gh.config.speed = GPIO_SPEED_HIGH;

    gh.config.pin_num = ILI9341_SPI_SCL_PIN;  gpio_init(&gh); // SCL
    gh.config.pin_num = ILI9341_SPI_MISO_PIN; gpio_init(&gh); // MISO
    gh.config.pin_num = ILI9341_SPI_MOSI_PIN; gpio_init(&gh); // MOSI

    // Control pins
    gh.gpiox = ILI9341_CONTROL_PORT;
    gh.config.mode = GPIO_MODE_OUTPUT;
    gh.config.altfn = 0;

    gh.config.pin_num = ILI9341_CS_PIN;  gpio_init(&gh); // CS
    gh.config.pin_num = ILI9341_DC_PIN;  gpio_init(&gh); // DC
    gh.config.pin_num = ILI9341_RST_PIN; gpio_init(&gh); // RST

    RST_LOW();
    dwt_delay_ms(5U);

    CS_HIGH();
    DC_HIGH();
    RST_HIGH();

    dwt_delay_ms(5U);
}

static void init_spi(void)
{
    s_spi.spix = ILI9341_SPI_PERIPHERAL;
    s_spi.config.device_mode = SPI_MODE_MASTER;
    s_spi.config.bus_config = SPI_BUS_FULL_DUPLEX;
    s_spi.config.baud = SPI_BAUD_DIV2;
    s_spi.config.df = SPI_DF_8BIT;
    s_spi.config.ff = SPI_FF_MSB_FIRST;
    s_spi.config.cpol = SPI_CPOL_LOW;
    s_spi.config.cpha = SPI_CPHA_1EDGE;
    s_spi.config.ssm = SPI_SSM_SOFTWARE;

    spi_init(&s_spi);
}

void ili9341_send_cmd(uint8_t cmd)
{
    // Interpret as command
    DC_LOW();
    barrier();

    // Start SPI communication
    CS_LOW();
    barrier();

    spi_send(ILI9341_SPI_PERIPHERAL, &cmd, 1);

    while(spi_flag_status(ILI9341_SPI_PERIPHERAL, SPI_FLAG_BUSY));

    // End SPI communication
    CS_HIGH();
    barrier();
}

void ili9341_send_cmd_data(uint8_t cmd, const uint8_t *data, uint32_t data_bytes)
{
    // Interpret as command
    DC_LOW();
    barrier();

    // Start SPI communication
    CS_LOW();
    barrier();

    spi_send(ILI9341_SPI_PERIPHERAL, &cmd, 1);

    while(spi_flag_status(ILI9341_SPI_PERIPHERAL, SPI_FLAG_BUSY));

    // Interpret as parameters
    DC_HIGH();
    barrier();

    while(data_bytes--)
    {
        spi_send(ILI9341_SPI_PERIPHERAL, data++, 1);
    }

    while(spi_flag_status(ILI9341_SPI_PERIPHERAL, SPI_FLAG_BUSY));

    // End SPI communication
    CS_HIGH();
    barrier();
}

void ili9341_software_reset()
{
    ili9341_send_cmd(ILI9341_CMD_SOFTWARE_RESET);

    dwt_delay_ms(10U);
}

void ili9341_set_column(uint16_t x0, uint16_t x1)
{
    uint8_t p[4] = {
        (uint8_t)(x0 >> 8), (uint8_t)(x0 & 0xFF),
        (uint8_t)(x1 >> 8), (uint8_t)(x1 & 0xFF)
    };
    ili9341_send_cmd_data(ILI9341_CMD_COLUMN_ADDR, p, 4);
}

void ili9341_set_page(uint16_t y0, uint16_t y1)
{
    uint8_t p[4] = { 
        (uint8_t)(y0 >> 8), (uint8_t)(y0 & 0xFF),
        (uint8_t)(y1 >> 8), (uint8_t)(y1 & 0xFF)
    };
    ili9341_send_cmd_data(ILI9341_CMD_PAGE_ADDR, p, 4);
}

void ili9341_set_window(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1)
{
    ili9341_set_column(x0, x1);
    ili9341_set_page(y0, y1);
}

void ili9341_fill_screen(uint16_t color)
{
    ili9341_set_window(0, 239, 0, 319);

    DC_LOW();
    barrier();

    CS_LOW();
    barrier();

    uint8_t cmd = ILI9341_CMD_MEMORY_WRITE;
    spi_send(ILI9341_SPI_PERIPHERAL, &cmd, 1);

    while(spi_flag_status(ILI9341_SPI_PERIPHERAL, SPI_FLAG_BUSY));

    DC_HIGH();
    barrier();

    uint8_t hi = (uint8_t)(color >> 8);
    uint8_t lo = (uint8_t)(color & 0xFF);

    for(uint32_t i = 0; i < ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT; ++i)
    {
        spi_send(ILI9341_SPI_PERIPHERAL, &hi, 1);
        spi_send(ILI9341_SPI_PERIPHERAL, &lo, 1);
    }

    while(spi_flag_status(ILI9341_SPI_PERIPHERAL, SPI_FLAG_BUSY));

    CS_HIGH();
    barrier();
}

void ili9341_init(void)
{
    dwt_init();
    init_gpio();
    init_spi();
    spi_peripheral_control(ILI9341_SPI_PERIPHERAL, ENABLE);

    uint8_t parameter = 0x55; // pixel format RGB565

    ili9341_software_reset();

    ili9341_send_cmd(ILI9341_CMD_SLEEP_OUT);
    dwt_delay_ms(125U);

    ili9341_send_cmd_data(ILI9341_CMD_PIXEL_FORMAT, &parameter, 1);

    parameter = 0x48; // memory access control
    ili9341_send_cmd_data(ILI9341_CMD_MEMORY_ACCESS, &parameter, 1);

    ili9341_send_cmd(ILI9341_CMD_DISPLAY_ON);
    dwt_delay_ms(10);

    ili9341_fill_screen(0x0F0F);
}
