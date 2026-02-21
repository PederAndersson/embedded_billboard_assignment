#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include "clientManager.h"


void odd_even(uint32_t seconds, client *c, uint8_t maxLen);
void string_copy(char* strSrc, char* strDest, uint8_t maxLen);
void swedish_parser(char *text);

#endif