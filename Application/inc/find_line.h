//
// Created by Taseny on 24-12-23.
//

#ifndef FIND_LINE_H
#define FIND_LINE_H

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE 128
#define DEFAULT_CENTER 64
#define DEFAULT_STEP 3
#define DEFAULT_THRESHOLD 20

typedef struct {
    int x[ARRAY_SIZE];
    int line_left;
    int line_right;
    int center_line;
    int step;
    int threshold;
    int center_pointer;
    int center_last;
    int right_last;
    int left_last;
    int lost_left_flag;
    int lost_right_flag;
    int error_patch;
} Line;

typedef struct {
    int Car_Mode;
} Flag;

void zebra_idntify(Line *ccd_inst, Flag *flag_inst);
void get_line(Line *ccd_inst);
void init_line(Line *line);
void init_instance_flag(Flag *flag_inst);

extern Line ccd_inst;
extern Flag flag_inst;

#endif //FIND_LINE_H
