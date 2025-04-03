//
// Created by Taseny on 24-12-23.
//

#include "find_line.h"

Line ccd_inst;
Flag flag_inst;

void zebra_idntify(Line *ccd_inst, Flag *flag_inst)
{
    static uint8_t zebra_leftborden = 30;
    static uint8_t zebra_rightborden = 90;
    uint8_t up_point = 0;
    uint8_t down_point = 0;
    for (uint8_t i = zebra_leftborden; i <= zebra_rightborden; i+=ccd_inst->step)
    {
        if (abs((ccd_inst->x[i] - ccd_inst->x[i - ccd_inst->step]) * 100 / (ccd_inst->x[i] + ccd_inst->x[i - ccd_inst->step])) > ccd_inst->threshold) down_point += 1;
        if (abs((ccd_inst->x[i] - ccd_inst->x[i - ccd_inst->step]) * 100 / (ccd_inst->x[i] + ccd_inst->x[i - ccd_inst->step])) > ccd_inst->threshold) up_point += 1;
    }
    if (up_point >= 6) flag_inst->Car_Mode = 1;
    else flag_inst->Car_Mode = 0;
}

void get_line(Line *ccd_inst)
{
    uint8_t i;
    uint8_t left_flag = 0;
    uint8_t right_flag = 0;
    int temp_l, temp_r;
    ccd_inst->lost_left_flag = 0;
    ccd_inst->lost_right_flag = 0;

    for (i = ccd_inst->left_last + 10; i >= ccd_inst->step; i -= ccd_inst->step) {
        if ((ccd_inst->x[i] - ccd_inst->x[i - ccd_inst->step]) * 100 / (ccd_inst->x[i] + ccd_inst->x[i - ccd_inst->step]) > ccd_inst->threshold) {
            ccd_inst->line_left = i;
            if (i < DEFAULT_CENTER)
                ccd_inst->left_last = i;
            left_flag = 1;
            break;
        }
    }
    if (left_flag == 0) {
        temp_l = (ccd_inst->center_last < 100) ? (15 + ccd_inst->center_last) : ccd_inst->center_last;
        for (i = temp_l; i >= ccd_inst->step; i -= ccd_inst->step) {
            if ((ccd_inst->x[i] - ccd_inst->x[i - ccd_inst->step]) * 100 / (ccd_inst->x[i] + ccd_inst->x[i - ccd_inst->step]) > ccd_inst->threshold) {
                ccd_inst->line_left = i;
                if (i < DEFAULT_CENTER)
                    ccd_inst->left_last = i;
                left_flag = 1;
                break;
            }
        }
    }
    if (left_flag == 0) {
        ccd_inst->line_left = ccd_inst->left_last;
        ccd_inst->lost_left_flag = 1;
    }

    for (i = ccd_inst->right_last - 10; i < ARRAY_SIZE - ccd_inst->step; i += ccd_inst->step) {
        if ((ccd_inst->x[i] - ccd_inst->x[i + ccd_inst->step]) * 100 / (ccd_inst->x[i] + ccd_inst->x[i + ccd_inst->step]) > ccd_inst->threshold) {
            ccd_inst->line_right = i;
            if (i > DEFAULT_CENTER)
                ccd_inst->right_last = i;
            right_flag = 1;
            break;
        }
    }
    if (right_flag == 0) {
        temp_r = (ccd_inst->center_last > 27) ? (ccd_inst->center_last - 15) : ccd_inst->center_last;
        for (i = temp_r; i < ARRAY_SIZE - ccd_inst->step; i += ccd_inst->step) {
            if ((ccd_inst->x[i] - ccd_inst->x[i + ccd_inst->step]) * 100 / (ccd_inst->x[i] + ccd_inst->x[i + ccd_inst->step]) > ccd_inst->threshold) {
                ccd_inst->line_right = i;
                if (i > DEFAULT_CENTER)
                    ccd_inst->right_last = i;
                right_flag = 1;
                break;
            }
        }
    }
    if (right_flag == 0) {
        ccd_inst->line_right = ccd_inst->right_last;
        ccd_inst->lost_right_flag = 1;
    }

    // Calculate center_line
    ccd_inst->center_line = ccd_inst->line_left + ccd_inst->line_right - ARRAY_SIZE;
    ccd_inst->center_last = (ccd_inst->line_right + ccd_inst->line_left) / 2;
}

void init_line(Line *line) {
    memset(line->x, 0, sizeof(line->x));
    line->line_left = 0;
    line->line_right = ARRAY_SIZE;
    line->center_line = 0;
    line->step = DEFAULT_STEP;
    line->threshold = DEFAULT_THRESHOLD;
    line->center_pointer = 0;
    line->center_last = DEFAULT_CENTER;
    line->right_last = DEFAULT_CENTER + 10;
    line->left_last = DEFAULT_CENTER - 10;
    line->lost_left_flag = 0;
    line->lost_right_flag = 0;
    line->error_patch = 0;
}

void init_instance_flag(Flag *flag_inst) {
    memset(flag_inst, 0, sizeof(Flag));
}
