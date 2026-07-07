extern "C" {

#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/varlena.h"

}

#include "vector.hpp"


extern "C" {


PG_FUNCTION_INFO_V1(fastvec_in);


Datum fastvec_in(PG_FUNCTION_ARGS)
{
    char *input = PG_GETARG_CSTRING(0);

    int dim = 0;

    for(char *p = input; *p; p++)
        if(*p == ',')
            dim++;

    dim++;


    Size size =
        VARHDRSZ +
        sizeof(int32) +
        sizeof(float) * dim;


    FastVec *vec =
        (FastVec*) palloc(size);


    vec->vl_len_ = size;
    vec->dim = dim;


    char *ptr = input + 1;


    for(int i = 0; i < dim; i++)
    {
        vec->values[i] = strtof(ptr, &ptr);

        if(*ptr == ',')
            ptr++;
    }


    PG_RETURN_POINTER(vec);
}



PG_FUNCTION_INFO_V1(fastvec_out);


Datum fastvec_out(PG_FUNCTION_ARGS)
{
    FastVec *vec =
        (FastVec*) PG_GETARG_POINTER(0);


    char *result =
        (char*) palloc(128);


    snprintf(
        result,
        128,
        "fastvec[%d]",
        vec->dim
    );


    PG_RETURN_CSTRING(result);
}


}
