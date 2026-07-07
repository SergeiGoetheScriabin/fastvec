#pragma once

extern "C" {
#include "postgres.h"
}

struct FastVec
{
    int32 vl_len_;
    int32 dim;
    float values[];
};
