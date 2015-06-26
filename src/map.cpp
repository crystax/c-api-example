#include <map.h>
#include <dlfcn.h>
#include <stdexcept>

typedef void *(*map_impl_create_t)();
typedef void (*map_impl_destroy_t)(void *p);
typedef void *(*map_impl_geo_impl_t)(void *p);

static bool map_impl_initialized = false;
static map_impl_create_t map_impl_create = NULL;
static map_impl_destroy_t map_impl_destroy = NULL;
static map_impl_geo_impl_t map_impl_geo_impl = NULL;

static void map_init()
{
    // guarded by mutex; not implemented here since it's not important for logic
    if (map_impl_initialized)
        return;

    void *pc = ::dlopen(LIBV, RTLD_NOW);
    if (!pc)
        throw std::runtime_error("dlopen() failed");

    map_impl_create = reinterpret_cast<map_impl_create_t>(::dlsym(pc, "map_impl_create"));
    map_impl_destroy = reinterpret_cast<map_impl_destroy_t>(::dlsym(pc, "map_impl_destroy"));
    map_impl_geo_impl = reinterpret_cast<map_impl_geo_impl_t>(::dlsym(pc, "map_impl_geo_impl"));

    ::dlclose(pc);

    map_impl_initialized = true;
}

map::map()
{
    map_init();

    if (!map_impl_create)
        throw std::runtime_error("map API is not available");

    pimpl = map_impl_create();

    g = new geo(map_impl_geo_impl(pimpl));
}

map::~map()
{
    delete g;

    if (map_impl_destroy)
        map_impl_destroy(pimpl);
}

geo const &map::location() const
{
    return *g;
}
