-- create placeholder type
CREATE TYPE fastvec;


-- input/output functions

CREATE FUNCTION fastvec_in(cstring)
RETURNS fastvec
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;


CREATE FUNCTION fastvec_out(fastvec)
RETURNS cstring
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;


-- replace shell type with real type

CREATE TYPE fastvec (
    INPUT = fastvec_in,
    OUTPUT = fastvec_out
);
