/* "hopefully you wont need to use this."
 * This is a compat header if your compiler wants to be stupid
 * cant blame you
 * use it!
 */

#ifndef STUPIDGUARD
#define STUPIDGUARD

#include <stddef.h>

//NOT RECOMMENDED ONLY USE ON EMERGENCY
#ifdef max_stupidity
#include <stdint.h>

typedef size_t uint16_t;

#endif
#endif

size_t min(size_t a, size_t b);
