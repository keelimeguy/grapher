#include "grapher.h"
#include "util.h"

static double get_min(int size);
static double get_max(int size);

#ifdef DEFAULT_MIN
    static double data_min = DEFAULT_MIN;
#else
    static double data_min = 0;
#endif
static int min_index = 0;

#ifdef DEFAULT_MAX
    static double data_max = DEFAULT_MAX;
#else
    static double data_max = 0;
#endif
static int max_index = 0;

const int data_size = GRAPH_WIDTH/BAR_WIDTH;
static int data_index = 0, data_length = 0, data_start = 0;
static double data[data_size];
static int bar_cache[data_size];
static char is_changed = 0;

unsigned char graph_changed() {
    return is_changed;
}

int graph_length(){
    return data_length;
}

double graph_min() {
    return data_min;
}

double graph_max() {
    return data_max;
}

static double get_min(int size) {
    if (size<=0) return 0;

    #ifdef DEFAULT_MIN
        double min = DEFAULT_MIN;
        min_index = -1;
        int i=0;
    #else
        double min = data[0];
        min_index = 0;
        int i=1;
    #endif

    for (; i<size; i++)
        if (data[i]<min) {
            min = data[i];
            min_index = i;
        }
    return min;
}

static double get_max(int size) {
    if (size<=0) return 0;

    #ifdef DEFAULT_MAX
        double max = DEFAULT_MAX;
        max_index = -1;
        int i=0;
    #else
        double max = data[0];
        max_index = 0;
        int i=1;
    #endif

    for (; i<size; i++)
        if (data[i]>max) {
            max = data[i];
            max_index = i;
        }
    return max;
}

void reset_graph() {
    is_changed = 1;
    data_index = 0;
    data_length = 0;
    data_start = 0;

    #ifdef DEFAULT_MIN
        data_min = DEFAULT_MIN;
    #else
        data_min = 0;
    #endif

    #ifdef DEFAULT_MAX
        data_max = DEFAULT_MAX;
    #else
        data_max = 0;
    #endif

    min_index = 0;
    max_index = 0;
}

extern "C" int add_to_graph(int val) {
    int refresh_axis = 0;
    is_changed = 1;
    data[data_index] = val;
    if (data_length == 0 || val<data_min) {
        data_min = val;
        min_index = data_index;
        refresh_axis = 1;
    } else if (min_index == data_index) {
        data_min = get_min(data_length);
        refresh_axis = 1;
    }

    if (data_length == 0 || val>data_max) {
        data_max = val;
        max_index = data_index;
        refresh_axis = 1;
    } else if (max_index == data_index) {
        data_max = get_max(data_length);
        refresh_axis = 1;
    }

    double min = data_min - BAR_PADDING_LOW;
    double max = data_max + BAR_PADDING_HIGH;

    #define CONVERT(xx) ((int)(((xx-min)/(max-min))*(double)(MAX_BAR_HEIGHT-MIN_BAR_HEIGHT)) + MIN_BAR_HEIGHT)
    if (refresh_axis) {
        for (int i = 0; i <= data_length; i++)
            bar_cache[i] = CONVERT(data[i]);

    } else {
        bar_cache[data_index] = CONVERT(val);
    }

    data_index++;
    if (data_index >= data_size) data_index = 0;
    if (data_length < data_size) {
        data_length++;
        data_start = 0;
    } else
        data_start = data_index;

    return data_index;
}

extern "C" void graph(int clear) {
    int i, x, y, index;
    int bar[data_length];

    if (data_start>data_length) return;
    for (y=0, i=data_start; i<data_length; i++, y++)
        bar[y] = bar_cache[i];
    for (y=data_length-data_start, i=0; i<data_start; i++, y++)
        bar[y] = bar_cache[i];

    if (clear)
        GRAPH_RESET();
    for (y=MAX_BAR_HEIGHT; y>0; y--) {
        for (x=0; x<GRAPH_WIDTH-(GRAPH_WIDTH%BAR_WIDTH)-data_length*BAR_WIDTH; x++) {
            GRAPH_PIXEL_OFF(x, y);
        }
        for (x=GRAPH_WIDTH-(GRAPH_WIDTH%BAR_WIDTH)-data_length*BAR_WIDTH, i=0; i<GRAPH_WIDTH-(GRAPH_WIDTH%BAR_WIDTH); i++, x++) {
            if ((i/BAR_WIDTH)>=data_length) break;
            index = (data_start+i/BAR_WIDTH)%data_length;
            if (bar[i/BAR_WIDTH]>=y)
                if (data[index]>=DANGER_HIGH)
                    GRAPH_PIXEL_DANGER(x, y);
                else if (data[index]>=WARNING_HIGH)
                    GRAPH_PIXEL_WARNING(x, y);
                else if (data[index]<=DANGER_LOW)
                    GRAPH_PIXEL_DANGER(x, y);
                else if (data[index]<=WARNING_LOW)
                    GRAPH_PIXEL_WARNING(x, y);
                else
                    GRAPH_PIXEL_ON(x, y);
            else
                GRAPH_PIXEL_OFF(x, y);
        }
        GRAPH_NEXT_ROW();
    }
    is_changed = 0;
}
