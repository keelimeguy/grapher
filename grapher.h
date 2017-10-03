#ifndef GRAPHER_H
#define GRAPHER_H

#include <stdio.h>

#define IS_CONSOLE_GRAPH 1

/* Comment out if not used */
// #define DEFAULT_MIN 0.0
// #define DEFAULT_MAX 10.0

#define DANGER_HIGH 20.0
#define WARNING_HIGH 10.0
#define WARNING_LOW -7.0
#define DANGER_LOW -14.0

#if IS_CONSOLE_GRAPH

    #define GRAPH_WIDTH                 80 // pixels
    #define BAR_WIDTH                   2.0 // pixels
    #define MIN_BAR_HEIGHT              1 // pixels
    #define MAX_BAR_HEIGHT              28 // pixels
    #define BAR_PADDING_LOW             1
    #define BAR_PADDING_HIGH            1

    #define GRAPH_PIXEL_ON(i, j)        printf("%c", 219)
    #define GRAPH_PIXEL_WARNING(i, j)   printf("%c", 177)
    #define GRAPH_PIXEL_DANGER(i, j)    printf("%c", 176)
    #define GRAPH_PIXEL_OFF(i, j)       printf("-")
    #define GRAPH_NEXT_ROW()            printf("\n")
    #define GRAPH_RESET()               0

#else

    #include "display_driver.h"

    #define GRAPH_WIDTH         DISPLAY_GRAPH_WIDTH // pixels
    #define BAR_WIDTH           DISPLAY_GRAPH_BAR_WIDTH // pixels
    #define MIN_BAR_HEIGHT      DISPLAY_GRAPH_MIN_BAR_HEIGHT // pixels
    #define MAX_BAR_HEIGHT      DISPLAY_GRAPH_MAX_BAR_HEIGHT // pixels
    #define BAR_PADDING_LOW     DISPLAY_GRAPH_BAR_PADDING_LOW
    #define BAR_PADDING_HIGH    DISPLAY_GRAPH_BAR_PADDING_HIGH

    #define GRAPH_PIXEL_ON(i, j)        display_set_graph_pixel(i, j, PIXEL_WHITE)
    #define GRAPH_PIXEL_WARNING(i, j)   display_set_graph_pixel(i, j, PIXEL_ORANGE)
    #define GRAPH_PIXEL_DANGER(i, j)    display_set_graph_pixel(i, j, PIXEL_RED)
    #define GRAPH_PIXEL_OFF(i, j)       display_set_graph_pixel(i, j, PIXEL_OFF)
    #define GRAPH_NEXT_ROW()            0
    #define GRAPH_RESET()               display_reset_graph()

#endif

void graph(double* data, int size, int start, int bar_width);

#endif
