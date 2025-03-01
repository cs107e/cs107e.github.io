#include "gl.h"
#include "math_float.h"
#include "printf.h"
#include "stdbool.h"
#include "timer.h"
#include "uart.h"

#define abs(a) ((a) < 0 ? -(a) : (a))

#define CSR_SET_BIT(csr, v)                                                    \
  __asm__ __volatile__("csrs " #csr " , %0"                                    \
                       : /* no outputs */                                      \
                       : "rK"(v)                                               \
                       : /* clobbers: none */);

// https://cs107e.github.io/readings/XuanTie-OpenC906-UserManual.pdf
// page 628
static inline void enable_fpu(void) { CSR_SET_BIT(mstatus, 3 << 13); }

// Copilot & ChatGPT helped write this
void plot_axes(int minRange, int maxRange) {
  const int centerX = gl_get_width() / 2;
  const int centerY = gl_get_height() / 2;
  const int x_axis_length = gl_get_width() / 3 * 2;
  const int y_axis_length = gl_get_height() / 3 * 2;

  //   Draw axes
  gl_draw_line(centerX - x_axis_length / 2, centerY,
               centerX + x_axis_length / 2, centerY, GL_BLACK);
  gl_draw_line(centerX, centerY - y_axis_length / 2, centerX,
               centerY + y_axis_length / 2, GL_BLACK);

  int pixelsPerUnitX = x_axis_length / (maxRange - minRange);
  int pixelsPerUnitY = y_axis_length / (maxRange - minRange);

  // Draw ticks
  for (int i = minRange; i <= maxRange; i++) {
    char label[10];
    snprintf(label, sizeof(label), "%d", i);
    if (i != 0) {
      gl_draw_string(centerX + i * pixelsPerUnitX - 4, centerY + 5, label,
                     GL_BLACK);
    }
    if (i != 0) {
      gl_draw_string(centerX + 5, centerY - i * pixelsPerUnitY - 10, label,
                     GL_BLACK);
    }
  }
}

typedef float (*math_func_t)(float);

// warning: this code does not know about the axes we draw nor aligns with them
void plot_function(float (*func)(float), color_t color, float minX,
                   float maxX) {
  int centerX = gl_get_width() / 2;
  int centerY = gl_get_height() / 2;
  int x_axis_length = gl_get_width() * 2 / 3;
  int y_axis_length = gl_get_height() * 2 / 3;

  int pixelsPerUnitX = x_axis_length / (maxX - minX);
  int pixelsPerUnitY = y_axis_length / (maxX - minX);

  for (float x = minX; x <= maxX; x += 0.01) {
    float y = func(x);

    int pixelX = centerX + (int)(x * pixelsPerUnitX);
    int pixelY = centerY - (int)(y * pixelsPerUnitY);

    if (y == y) { // check y is not NaN
      gl_draw_pixel(pixelX, pixelY, color);
    }
  }
}

// from the pseudocode at https://www.a1k0n.net/2011/07/20/donut-math.html
void render_frame(float A, float B, int screen_width, int screen_height,
                  float theta_spacing, float phi_spacing, float R1, float R2,
                  float K2) {
  float cosA = cos(A), sinA = sin(A);
  float cosB = cos(B), sinB = sin(B);

  float K1 = screen_width * K2 * 3 / (8 * (R1 + R2)) / 2;

  float zbuffer[screen_width][screen_height];
  for (int i = 0; i < screen_width; ++i) {
    for (int j = 0; j < screen_height; ++j) {
      zbuffer[i][j] = NEG_INF;
    }
  }

  gl_clear(GL_BLACK);

  for (float theta = 0; theta < 2 * PI; theta += theta_spacing) {
    float costheta = cos(theta), sintheta = sin(theta);

    for (float phi = 0; phi < 2 * PI; phi += phi_spacing) {
      float cosphi = cos(phi), sinphi = sin(phi);

      float circlex = R2 + R1 * costheta;
      float circley = R1 * sintheta;

      float x = circlex * (cosB * cosphi + sinA * sinB * sinphi) -
                circley * cosA * sinB;
      float y = circlex * (sinB * cosphi - sinA * cosB * sinphi) +
                circley * cosA * cosB;
      float z = 5 + cosA * circlex * sinphi + circley * sinA;
      float ooz = 1 / z;

      int xp = (int)(screen_width / 2 + K1 * ooz * x);
      int yp = (int)(screen_height / 2 - K1 * ooz * y);

      float L = cosphi * costheta * sinB - cosA * costheta * sinphi -
                sinA * sintheta +
                cosB * (cosA * sintheta - costheta * sinA * sinphi);
      if (L > 0) {
        if (ooz > zbuffer[xp][yp]) {
          zbuffer[xp][yp] = ooz;
          int luminance_index = L * 8;
          color_t color = gl_color(luminance_index * 23, luminance_index * 23,
                                   luminance_index * 23);
          gl_draw_pixel(xp, yp, color);
        }
      }
    }
  }

  gl_swap_buffer();
}

void animate_donut(int screen_width, int screen_height, float theta_spacing,
                   float phi_spacing, float R1, float R2, float K2) {
  float A = 1, B = 1;
  const float rotation_speed_A = 0.07 / 3;
  const float rotation_speed_B = 0.03 / 3;

  while (true) {
    render_frame(A, B, screen_width, screen_height, theta_spacing, phi_spacing,
                 R1, R2, K2);
    A += rotation_speed_A;
    B += rotation_speed_B;

    if (A > 2 * PI)
      A -= 2 * PI;
    if (B > 2 * PI)
      B -= 2 * PI;
  }
}

void main(void) {
  uart_init();
  printf("Starting main() in %s\n", __FILE__);
  enable_fpu();
  const int WIDTH = 480;
  const int HEIGHT = 480;
  const float LEFT = -5;
  const float RIGHT = 5;
  gl_init(WIDTH, HEIGHT, GL_DOUBLEBUFFER);
  gl_clear(GL_WHITE);
  gl_draw_string(20, HEIGHT / 2 - 10, "Plots of select math functions", GL_RED);
  gl_swap_buffer();
  timer_delay(3);
  // trig functions
  gl_clear(GL_WHITE);
  plot_axes(LEFT, RIGHT);
  gl_draw_string(10, 10, "sin(x)", GL_RED);
  gl_draw_string(10, 30, "cos(x)", GL_BLUE);
  gl_draw_string(10, 50, "tan(x)", GL_GREEN);
  plot_function(sin, GL_RED, LEFT, RIGHT);
  plot_function(cos, GL_BLUE, LEFT, RIGHT);
  plot_function(tan, GL_GREEN, LEFT, RIGHT);
  gl_swap_buffer();
  timer_delay(5);
  // inverse trig functions
  gl_clear(GL_WHITE);
  plot_axes(LEFT, RIGHT);
  gl_draw_string(10, 10, "asin(x)", GL_RED);
  gl_draw_string(10, 30, "acos(x)", GL_BLUE);
  gl_draw_string(10, 50, "atan(x)", GL_GREEN);
  plot_function(asin, GL_RED, LEFT, RIGHT);
  plot_function(acos, GL_BLUE, LEFT, RIGHT);
  plot_function(atan, GL_GREEN, LEFT, RIGHT);
  gl_swap_buffer();
  timer_delay(5);
  // hyperbolic trig functions
  gl_clear(GL_WHITE);
  plot_axes(LEFT, RIGHT);
  gl_draw_string(10, 10, "sinh(x)", GL_RED);
  gl_draw_string(10, 30, "cosh(x)", GL_BLUE);
  gl_draw_string(10, 50, "tanh(x)", GL_GREEN);
  plot_function(sinh, GL_RED, LEFT, RIGHT);
  plot_function(cosh, GL_BLUE, LEFT, RIGHT);
  plot_function(tanh, GL_GREEN, LEFT, RIGHT);
  gl_swap_buffer();
  timer_delay(5);
  // exp, log, sqrt
  gl_clear(GL_WHITE);
  plot_axes(LEFT, RIGHT);
  gl_draw_string(10, 10, "exp(x)", GL_RED);
  gl_draw_string(10, 30, "ln(x)", GL_BLUE);
  gl_draw_string(10, 50, "sqrt(x)", GL_ORANGE);
  gl_draw_string(10, HEIGHT - 30, "Use only on defined domain!", GL_ORANGE);
  plot_function(exp, GL_RED, LEFT, RIGHT);
  plot_function(log, GL_BLUE, LEFT, RIGHT);
  plot_function(sqrt, GL_ORANGE, LEFT, RIGHT);
  gl_swap_buffer();
  timer_delay(5);
  // donut.c
  // https://www.a1k0n.net/2011/07/20/donut-math.html
  gl_clear(GL_BLACK);
  gl_draw_string(20, HEIGHT / 2 - 40, "SLOWLY SPINNING TORUS", GL_WHITE);
  gl_draw_string(20, HEIGHT / 2, "www.a1k0n.net/2011/07/20/donut-math.html",
                 GL_AMBER);
  gl_swap_buffer();
  timer_delay(3);
  animate_donut(WIDTH, HEIGHT, 0.105, 0.03, 1, 2, 5);
  // my thoughts: there is enough performance to transform a fairly large number
  // of points around at an acceptable framerate with just loops & no SIMD /
  // vector support, though higher framerate is needed for faster moving objects

  // a wireframe 3D game could be possible; real-time lighting probably not
}