-- create placeholder type
CREATE TYPE fastvec;


-- input/output functions
CREATE FUNCTION cosine_distance(fastvec, fastvec)
RETURNS float8
AS 'MODULE_PATHNAME', 'fastvec_cosine_distance'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION inner_product(fastvec, fastvec)
RETURNS float8
AS 'MODULE_PATHNAME', 'fastvec_inner_product'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION fastvec_in(cstring)
RETURNS fastvec
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;


CREATE FUNCTION fastvec_out(fastvec)
RETURNS cstring
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION l2_distance(fastvec, fastvec)
RETURNS float8
AS 'MODULE_PATHNAME', 'fastvec_l2_distance'
LANGUAGE C IMMUTABLE STRICT;

-- replace shell type with real type

CREATE TYPE fastvec (
    INPUT = fastvec_in,
    OUTPUT = fastvec_out
);
