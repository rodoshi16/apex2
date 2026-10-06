***Order.hpp***

In c++, we have hpp and cpp files because hpp is the declaration and cpp is the implementation. 

We did a namespace apex in every file because we want to have variables specific to the apex namespace so we can refer back to it saying apex::Order

*Why use uint64_t to store prices and more?*

You could use an int but uint64_t is unsigned(doesn't allow negatives) and is 64 bits compared to a 32 bit integer. 

It is a non negative 64 bit identifier.

*Libraries*

#pragma once
#include <cstdint>

Library you need to include integers and pragma says don't process the header more than once. 