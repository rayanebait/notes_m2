#ifndef COUNT_H
#define COUNT_H

#include <stdint.h>

#define LETTER_GET(l) (( (l) >> 27) + 'a')
#define OCC_GET(o) (((o) & 0x07ffffff))
typedef uint32_t letter_t;



#endif /*COUNT_H*/