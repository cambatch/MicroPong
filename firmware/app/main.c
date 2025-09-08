#include "f446re.h"
#include "ili9341.h"

 void init_gpio(void)
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
}

 void init_spi(void)
{
    spi_handle_t sh = { 0 };
    sh.spix = ILI9341_SPI_PERIPHERAL;
    sh.config.device_mode = SPI_MODE_MASTER;
    sh.config.bus_config = SPI_BUS_FULL_DUPLEX;
    sh.config.baud = SPI_BAUD_DIV2;
    sh.config.df = SPI_DF_8BIT;
    sh.config.ff = SPI_FF_MSB_FIRST;
    sh.config.cpol = SPI_CPOL_LOW;
    sh.config.cpha = SPI_CPHA_1EDGE;
    sh.config.ssm = SPI_SSM_SOFTWARE;

    spi_init(&sh);
}

int main(void)
{
    ili9341_config_t ili_config = {
        .invert_on_init = false,
        .pixel_format = ILI9341_PIXEL_FORMAT_RGB565,
        .rotation = ILI9341_ROT_90
    };

    dwt_init();
    init_gpio();
    init_spi();
    spi_peripheral_control(ILI9341_SPI_PERIPHERAL, ENABLE);

    ili9341_init(&ili_config);

    ili9341_fill_screen(COLOR_ORANGE);

    ili9341_fill_rect(20, 10, 100, 40, COLOR_BLACK);

    ili9341_draw_hline(0, 10, 240, COLOR_WHITE);

    while (1) { }
}
