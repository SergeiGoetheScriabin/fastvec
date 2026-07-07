#include <cstdlib>

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
