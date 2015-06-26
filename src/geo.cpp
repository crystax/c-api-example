#include <geo.h>

#include <dlfcn.h>

#include <stdexcept>

extern "C"
{

void *geo_impl_create();
void geo_impl_destroy(void *p);

double geo_impl_latitude(void *p);
double geo_impl_longitude(void *p);

} // extern "C"

typedef double (*geo_impl_altitude_t)(void *p);

static bool geo_impl_initialized = false;
static geo_impl_altitude_t geo_impl_altitude = NULL;

static void geo_init()
{
    // guarded by mutex; not implemented here since it's not important for logic
    if (geo_impl_initialized)
        return;

    void *pc = ::dlopen(LIBV, RTLD_NOW);
    if (!pc)
        throw std::runtime_error("dlopen() failed");

    geo_impl_altitude = reinterpret_cast<geo_impl_altitude_t>(::dlsym(pc, "geo_impl_altitude"));

    ::dlclose(pc);

    geo_impl_initialized = true;
}

geo::geo()
    :pimpl(geo_impl_create()), own(true)
{
    geo_init();
}

geo::geo(void *p)
    :pimpl(p), own(false)
{
    geo_init();
}

geo::~geo()
{
    if (own)
        geo_impl_destroy(pimpl);
}

double geo::latitude() const
{
    return geo_impl_latitude(pimpl);
}

double geo::longitude() const
{
    return geo_impl_longitude(pimpl);
}

double geo::altitude() const
{
    if (!geo_impl_altitude)
        throw std::runtime_error("geo::altitude() not available");

    return geo_impl_altitude(pimpl);
}
