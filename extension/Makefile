EXTENSION = fastvec
MODULE_big = fastvec

OBJS = fastvec.o vector.o

DATA = fastvec--1.0.sql

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)

CXX = clang++

PG_CPPFLAGS = -std=c++20 -O3 -fno-exceptions -fno-rtti

SHLIB_LINK += -lc++

include $(PGXS)
