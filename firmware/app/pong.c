#include "pong.h"

#include "f446re.h"
#include "ili9341.h"


static pong_state_t g_pstate;
static pong_state_t g_cstate;
static int16_t g_screen_w;
static int16_t g_screen_h;
static int16_t g_pad_w;
static int16_t g_pad_h;
static int16_t g_ball_w;
static int16_t g_ball_h;

static void draw_initial_state(void);
static void draw_center_line(void);


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

void pong_init(void)
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

    ili9341_get_screen_size((uint16_t *)&g_screen_w, (uint16_t *)&g_screen_h);
    g_pad_w = PADDLE_W;
    g_pad_h = PADDLE_H;
    g_ball_w = BALL_SIZE;
    g_ball_h = BALL_SIZE;

    g_pstate.l_x = 3; // 3 pixels of padding
    g_pstate.l_y = (g_screen_h / 2) - (g_pad_h) / 2;

    g_pstate.r_x = (int16_t)(g_screen_w - g_pad_w - 3); // 3 pixels of padding
    g_pstate.r_y = g_pstate.l_y;

    g_pstate.b_x = (g_screen_w / 2) - (g_ball_w / 2);
    g_pstate.b_y = (g_screen_h / 2) - (g_ball_h / 2);

    g_cstate = g_pstate;

    ili9341_fill_screen(COLOR_BLACK);

    draw_initial_state();
    draw_center_line();
}

void draw_initial_state(void)
{
    // Ball
    ili9341_fill_rect((uint16_t)g_cstate.b_x,
                      (uint16_t)g_cstate.b_y,
                      (uint16_t)g_ball_w,
                      (uint16_t)g_ball_h,
                      COLOR_WHITE);
    // Left paddle
    ili9341_fill_rect((uint16_t)g_cstate.l_x,
                      (uint16_t)g_cstate.l_y,
                      (uint16_t)g_pad_w,
                      (uint16_t)g_pad_h,
                      COLOR_WHITE);

    // right paddle
    ili9341_fill_rect((uint16_t)g_cstate.r_x,
                      (uint16_t)g_cstate.r_y,
                      (uint16_t)g_pad_w,
                      (uint16_t)g_pad_h,
                      COLOR_WHITE);
}

static void restore_center_line_segment(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    const uint16_t dash_h   = 8;
    const uint16_t gap_h    = 4;
    const uint16_t line_x   = (uint16_t)(g_screen_w / 2 - 1);
    const uint16_t line_w   = 2;

    // Only do anything if overlap with center line
    if(x > line_x + line_w || x + w < line_x) return;

    // Redraw dashes overlapping the erased area
    uint16_t y_end = y + h;
    for(uint16_t yy = 0; yy < g_screen_h; yy += (dash_h + gap_h))
    {
        uint16_t dash_end = yy + dash_h;
        if(dash_end > y && yy < y_end)
        {
            uint16_t draw_y  = (yy > y) ? yy : y;
            uint16_t draw_h  = (dash_end < y_end) ? (dash_end - draw_y) : (y_end - draw_y);
            ili9341_fill_rect(line_x, draw_y, line_w, draw_h, COLOR_WHITE);
        }
    }
}

static void draw_left_paddle(void)
{
    // Erase old
    ili9341_fill_rect((uint16_t)g_pstate.l_x,
                      (uint16_t)g_pstate.l_y,
                      (uint16_t)g_pad_w,
                      (uint16_t)g_pad_h,
                      COLOR_BLACK);

    // Draw new
    ili9341_fill_rect((uint16_t)g_cstate.l_x,
                      (uint16_t)g_cstate.l_y,
                      (uint16_t)g_pad_w,
                      (uint16_t)g_pad_h,
                      COLOR_WHITE);
}

static void draw_right_paddle(void)
{
    // Erase old
    ili9341_fill_rect((uint16_t)g_pstate.r_x,
                      (uint16_t)g_pstate.r_y,
                      (uint16_t)g_pad_w,
                      (uint16_t)g_pad_h,
                      COLOR_BLACK);

    // Draw new
    ili9341_fill_rect((uint16_t)g_cstate.r_x,
                      (uint16_t)g_cstate.r_y,
                      (uint16_t)g_pad_w,
                      (uint16_t)g_pad_h,
                      COLOR_WHITE);
}

static void draw_ball(void)
{
    // Erase old
    ili9341_fill_rect((uint16_t)g_pstate.b_x,
                      (uint16_t)g_pstate.b_y,
                      (uint16_t)g_ball_w,
                      (uint16_t)g_ball_h,
                      COLOR_BLACK);

    restore_center_line_segment((uint16_t)g_pstate.b_x,
                                (uint16_t)g_pstate.b_y,
                                (uint16_t)g_ball_w,
                                (uint16_t)g_ball_h);

    // Draw new
    ili9341_fill_rect((uint16_t)g_cstate.b_x,
                      (uint16_t)g_cstate.b_y,
                      (uint16_t)g_ball_w,
                      (uint16_t)g_ball_h,
                      COLOR_WHITE);
}

static void draw_center_line(void)
{
    const uint16_t dash_h   = 8;   // height of each dash
    const uint16_t gap_h    = 4;   // gap between dashes
    const uint16_t line_x   = (uint16_t)(g_screen_w / 2 - 1); // 1-px line centered
    const uint16_t line_w   = 2;   // thickness

    for(uint16_t y = 0; y < g_screen_h; y += (dash_h + gap_h))
    {
        ili9341_fill_rect(line_x,
                          y,
                          line_w,
                          (y + dash_h <= g_screen_h) ? dash_h : (uint16_t)(g_screen_h - y),
                          COLOR_WHITE);
    }
}

void pong_play(void)
{
    dwt_delay_ms(100);

    // Ball velocity
    int16_t b_dx = 3;
    int16_t b_dy = 2;

    while(1)
    {
        g_pstate = g_cstate; // save old state

        // --- Move ball ---
        g_cstate.b_x += b_dx;
        g_cstate.b_y += b_dy;

        if(g_cstate.b_y <= 0)
        {
            g_cstate.b_y = 0;
            b_dy = -b_dy;
        }

        if(g_cstate.b_y + g_ball_h >= g_screen_h)
        {
            g_cstate.b_y = (int16_t)(g_screen_h - g_ball_h);
            b_dy = -b_dy;
        }

        // Ball bounce on left paddle
        if(g_cstate.b_x <= g_cstate.l_x + g_pad_w &&
            g_cstate.b_y + g_ball_h >= g_cstate.l_y &&
            g_cstate.b_y <= g_cstate.l_y + g_pad_h)
        {
            // place ball outside of paddle and reverse direction
            g_cstate.b_x = g_cstate.l_x + g_pad_w;
            b_dx = -b_dx;

            if(b_dx > 0 && b_dx < MAX_BALL_SPEED) b_dx++;
            if(b_dx < 0 && b_dx > -MAX_BALL_SPEED) b_dx--;
        }
        // Ball bounce on right paddle
        if(g_cstate.b_x + g_ball_w >= g_cstate.r_x &&
            g_cstate.b_y + g_ball_h >= g_cstate.r_y &&
            g_cstate.b_y <= g_cstate.r_y + g_pad_h)
        {
            // place ball outside of paddle and reverse direction
            g_cstate.b_x = g_cstate.r_x - g_ball_w;
            b_dx = -b_dx;

            if(b_dx > 0 && b_dx < MAX_BALL_SPEED) b_dx++;
            if(b_dx < 0 && b_dx > -MAX_BALL_SPEED) b_dx--;
        }

        // Reset if ball goes off screen
        if(g_cstate.b_x < 0 || g_cstate.b_x + g_ball_w > g_screen_w)
        {
            g_cstate.b_x = (g_screen_w / 2) - (g_ball_w / 2);
            g_cstate.b_y = (g_screen_h / 2) - (g_ball_h / 2);
            b_dx = (b_dx > 0) ? -3 : 3;
            b_dy = 2;
        }

        // Simple paddle follow ball
        if(g_cstate.l_y + g_pad_h / 2 < g_cstate.b_y) g_cstate.l_y += PADDLE_SPEED;
        else if(g_cstate.l_y + g_pad_h / 2 > g_cstate.b_y) g_cstate.l_y -= PADDLE_SPEED;

        if(g_cstate.r_y + g_pad_h / 2 < g_cstate.b_y) g_cstate.r_y += PADDLE_SPEED;
        else if(g_cstate.r_y + g_pad_h / 2 > g_cstate.b_y) g_cstate.r_y -= PADDLE_SPEED;

        // Clamp paddles
        if(g_cstate.l_y < 0) g_cstate.l_y = 0;
        if(g_cstate.l_y + g_pad_h > g_screen_h) g_cstate.l_y = g_screen_h - g_pad_h;
        if(g_cstate.r_y < 0) g_cstate.r_y = 0;
        if(g_cstate.r_y + g_pad_h > g_screen_h) g_cstate.r_y = g_screen_h - g_pad_h;

        // Draw
        draw_left_paddle();
        draw_right_paddle();
        draw_ball();

        dwt_delay_ms(16);
    }
}
