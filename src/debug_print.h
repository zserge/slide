#ifndef SLIDE_DEBUG_PRINT_H
#define SLIDE_DEBUG_PRINT_H

#include <iostream>

#ifdef DEBUG
#define debug_print(a) std::cerr << a << std::endl;
#else
#define debug_print(a)
#endif

#endif //SLIDE_DEBUG_PRINT_H
