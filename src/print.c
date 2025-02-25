#include <stdint.h>
#include <stdio.h>

#include "s21_decimal.h"

int get_scale(s21_decimal value) {
    int pow_mask = 0xFF;
    int pow = (value.bits[3] >> 16) & pow_mask;
    return pow;
}

int get_sign(s21_decimal value) {
    int sign = (value.bits[3] & 0x80000000) ? -1 : 1;
    return sign;
}

void print_96bit_decimal(s21_decimal decimal, void *str) {
    const uint64_t BASE = 1000000000ULL;
    uint64_t parts[3] = {(uint32_t)decimal.bits[0], (uint32_t)decimal.bits[1],
                         (uint32_t)decimal.bits[2]};
    char result[40] = {0};
    int pos = sizeof(result) - 1;
    int scale = get_scale(decimal);
    int sign = get_sign(decimal);

    if (sign == -1) {
        if (str) {
            sprintf(str, "-");
            str++;
        } else {
            printf("-");
        }
    }

    if (!parts[0] && !parts[1] && !parts[2]) {
        if (str) {
            sprintf(str, "0");
        } else {
            printf("0\n");
        }
        return;
    } else {
        while (parts[0] || parts[1] || parts[2]) {
            uint64_t remainder = 0;

            for (int i = 2; i >= 0; i--) {
                uint64_t current = (remainder << 32) | parts[i];
                parts[i] = current / BASE;
                remainder = current % BASE;
            }

            for (int i = 0; i < 9; i++) {
                result[--pos] = '0' + (remainder % 10);
                remainder /= 10;
            }
        }

        while (result[pos] == '0') {
            pos++;
        }

        int result_len = sizeof(result) - 1 - pos;

        // Если нужно добавить запятую
        if (scale > 0 && result_len > scale) {
            int comma_pos = sizeof(result) - scale - 1; // Позиция запятой
            for (int i = sizeof(result) - 1; i > comma_pos; i--) {
                result[i] = result[i - 1];
            }
            result[comma_pos] = '.';
        } else if (scale > 0 && result_len <= scale) {
            // Если степень больше длины результата, нужно дополнить нулями
            if (str) {
                sprintf(str, "0.");
                str += 2;
            } else {
                printf("0.");
            }
            for (int i = 0; i < scale - result_len; i++) {
                if (str) {
                    sprintf(str, "0");
                    str++;
                } else {
                    printf("0");
                }
            }
            if (str) {
                sprintf(str, "%s", (result + pos));
            } else {
                printf("%s\n", (result + pos));
            }
            return;
        }
        if (str) {
            sprintf(str, "%s", (result + pos));
        } else {
            printf("%s\n", (result + pos));
        }
    }
}