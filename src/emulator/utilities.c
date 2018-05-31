//
// Created by Kapilan M on 31/05/2018.
//

#include "utilities.h"

//  takes bits between begin and end --> [begin,end)
uint32_t bits_extract(uint32_t value, int begin, int end) {
    uint32_t mask = (1 << (end - begin)) - 1;
    return (value >> begin) & mask;
}
