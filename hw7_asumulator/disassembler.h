#pragma once
#include "mips.h"
#include <stdio.h>
#include <stdlib.h>

void Rtype(FILE *, unsigned int ins);
void Itype(FILE *, unsigned int ins);
void Jtype(FILE *, unsigned int ins);
void Ctype(FILE *, unsigned int ins);