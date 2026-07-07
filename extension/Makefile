EXTENSION = fastvec
MODULE_big = fastvec

OBJS = fastvec.o

DATA = fastvec--1.0.sql

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)

CXX = g++

PG_CPPFLAGS = -std=c++20 -O3 

include $(PGXS)
