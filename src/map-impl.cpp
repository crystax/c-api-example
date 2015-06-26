#include "geo-impl.h"

class map_impl
{
    map_impl(map_impl const &);
    map_impl &operator=(map_impl const &);

public:
    map_impl()
        :loc(new geo_impl())
    {
    }

    ~map_impl()
    {
        delete loc;
    }

    geo_impl *location() {return loc;}

private:
    geo_impl *loc;
};

extern "C"
{

void *map_impl_create()
{
    return new map_impl();
}

void map_impl_destroy(void *p)
{
    delete reinterpret_cast<map_impl*>(p);
}

void *map_impl_geo_impl(void *p)
{
    return reinterpret_cast<map_impl*>(p)->location();
}

} // extern "C"
