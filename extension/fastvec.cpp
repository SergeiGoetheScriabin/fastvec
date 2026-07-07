extern "C" {
#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
}

extern "C" {

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(hello);

Datum hello(PG_FUNCTION_ARGS)
{
    PG_RETURN_TEXT_P(cstring_to_text("Hello from FastVec!"));
}

PG_FUNCTION_INFO_V1(add_one);

Datum add_one(PG_FUNCTION_ARGS)
{
    PG_RETURN_INT32(PG_GETARG_INT32(0) + 1);
}

}
