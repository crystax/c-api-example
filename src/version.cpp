#include <version.h>

extern "C" unsigned int version_apilevel();

unsigned int version::apilevel()
{
    return version_apilevel();
}
