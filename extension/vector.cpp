#include <cstdlib>
#include <cmath>
extern "C" {

#include "postgres.h"

#include "fmgr.h"
#include "varatt.h"
#include "utils/builtins.h"
#include "utils/varlena.h"
#include "lib/stringinfo.h"

}

#include "vector.hpp"


extern "C" {


PG_FUNCTION_INFO_V1(fastvec_in);


Datum fastvec_in(PG_FUNCTION_ARGS)
{
    char *input = PG_GETARG_CSTRING(0);

    // Count dimensions
    int dim = 0;

    for(char *p = input; *p; p++)
        if(*p == ',')
            dim++;

    dim++;


    // Allocate PostgreSQL varlena object
    Size size =
        offsetof(FastVec, values) +
        sizeof(float) * dim;


    FastVec *vec =
        (FastVec*) palloc(size);


    SET_VARSIZE(vec, size);   // fixed: was raw "vec->vl_len_ = size;"
    vec->dim = dim;


    // Parse values
    char *ptr = input + 1;


    for(int i = 0; i < dim; i++)
    {
        char *end;

        vec->values[i] = strtof(ptr, &end);

        ptr = end;

        while(*ptr == ',' || *ptr == ' ')
            ptr++;
    }

    // Debug check - uncomment if [0,0,0] still shows up after this fix
    // elog(INFO, "dim=%d v0=%f v1=%f v2=%f", dim, vec->values[0], vec->values[1], vec->values[2]);


    PG_RETURN_POINTER(vec);
}

PG_FUNCTION_INFO_V1(fastvec_l2_distance);

Datum fastvec_l2_distance(PG_FUNCTION_ARGS)
{
    FastVec *a = (FastVec*) PG_GETARG_POINTER(0);
    FastVec *b = (FastVec*) PG_GETARG_POINTER(1);

    if (a->dim != b->dim)
        ereport(ERROR,
            (errcode(ERRCODE_DATA_EXCEPTION),
             errmsg("fastvec dimension mismatch: %d vs %d", a->dim, b->dim)));

    float sum = 0.0f;
    for (int i = 0; i < a->dim; i++)
    {
        float diff = a->values[i] - b->values[i];
        sum += diff * diff;
    }

    PG_RETURN_FLOAT8(sqrtf(sum));
}
PG_FUNCTION_INFO_V1(fastvec_cosine_distance);

Datum fastvec_cosine_distance(PG_FUNCTION_ARGS)
{
    FastVec *a = (FastVec*) PG_GETARG_POINTER(0);
    FastVec *b = (FastVec*) PG_GETARG_POINTER(1);

    if (a->dim != b->dim)
        ereport(ERROR,
            (errcode(ERRCODE_DATA_EXCEPTION),
             errmsg("fastvec dimension mismatch: %d vs %d", a->dim, b->dim)));

    float dot = 0.0f, norm_a = 0.0f, norm_b = 0.0f;
    for (int i = 0; i < a->dim; i++)
    {
        dot += a->values[i] * b->values[i];
        norm_a += a->values[i] * a->values[i];
        norm_b += b->values[i] * b->values[i];
    }

    if (norm_a == 0.0f || norm_b == 0.0f)
        ereport(ERROR,
            (errcode(ERRCODE_DATA_EXCEPTION),
             errmsg("fastvec cosine distance undefined for zero vector")));

    float similarity = dot / (sqrtf(norm_a) * sqrtf(norm_b));
    PG_RETURN_FLOAT8(1.0 - similarity);
}

PG_FUNCTION_INFO_V1(fastvec_inner_product);

Datum fastvec_inner_product(PG_FUNCTION_ARGS)
{
    FastVec *a = (FastVec*) PG_GETARG_POINTER(0);
    FastVec *b = (FastVec*) PG_GETARG_POINTER(1);

    if (a->dim != b->dim)
        ereport(ERROR,
            (errcode(ERRCODE_DATA_EXCEPTION),
             errmsg("fastvec dimension mismatch: %d vs %d", a->dim, b->dim)));

    float sum = 0.0f;
    for (int i = 0; i < a->dim; i++)
        sum += a->values[i] * b->values[i];

    PG_RETURN_FLOAT8(sum);
}

PG_FUNCTION_INFO_V1(fastvec_out);


Datum fastvec_out(PG_FUNCTION_ARGS)
{
    FastVec *vec =
        (FastVec*) PG_GETARG_POINTER(0);


    StringInfoData str;

    initStringInfo(&str);

    appendStringInfoChar(&str, '[');


    for(int i = 0; i < vec->dim; i++)
    {
        if(i > 0)
            appendStringInfoChar(&str, ',');

        appendStringInfo(
            &str,
            "%g",
            vec->values[i]
        );
    }


    appendStringInfoChar(&str, ']');


    PG_RETURN_CSTRING(str.data);
}


}
