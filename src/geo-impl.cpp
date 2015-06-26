#include "geo-impl.h"
#include <iostream>

geo_impl::geo_impl()
{
    lat = 1.1;
    lon = 2.2;
#if APILEVEL >= 2
    alt = 3.3;
#endif // APILEVEL >= 2
}

geo_impl::~geo_impl()
{
}

extern "C"
{

void *geo_impl_create()
{
    return new geo_impl();
}

void geo_impl_destroy(void *p)
{
    delete reinterpret_cast<geo_impl*>(p);
}

double geo_impl_latitude(void *p)
{
    return reinterpret_cast<geo_impl*>(p)->latitude();
}

double geo_impl_longitude(void *p)
{
    return reinterpret_cast<geo_impl*>(p)->longitude();
}

#if APILEVEL >= 2
double geo_impl_altitude(void *p)
{
    return reinterpret_cast<geo_impl*>(p)->altitude();
}
#endif // APILEVEL >= 2

} // extern "C"
