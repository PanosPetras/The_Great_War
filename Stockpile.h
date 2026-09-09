#ifndef Stockpile_H
#define Stockpile_H

#include "PerGood.h"

/*An amount held of every good - what a country has in its warehouses, what a
factory eats in a day, what it costs to build one. Goods are counted in whole
units, so the payload is an int; the same container carries prices and
throughput shares with a payload of their own.

Money is not in here. It is not produced, consumed or priced the way a good is,
so it lives on the Country that owns the warehouse.*/
using Stockpile = PerGood<int>;

#endif
