#include <md.h>
#include <dlfcn.h>
#include <stdexcept>

typedef void *(*md_impl_create_t)();
typedef void (*md_impl_destroy_t)(void *p);
typedef double (*md_impl_location_altitude_t)(void *p);

static bool md_impl_initialized = false;
static md_impl_create_t md_impl_create = NULL;
static md_impl_destroy_t md_impl_destroy = NULL;
static md_impl_location_altitude_t md_impl_location_altitude = NULL;

static void md_init()
{
    if (md_impl_initialized)
        return;

    void *pc = ::dlopen(LIBV, RTLD_NOW);
    if (!pc)
        throw std::runtime_error("dlopen() failed");

    md_impl_create = reinterpret_cast<md_impl_create_t>(::dlsym(pc, "md_impl_create"));
    md_impl_destroy = reinterpret_cast<md_impl_destroy_t>(::dlsym(pc, "md_impl_destroy"));
    md_impl_location_altitude = reinterpret_cast<md_impl_location_altitude_t>(::dlsym(pc, "md_impl_location_altitude"));

    ::dlclose(pc);

    md_impl_initialized = true;
}

md::md()
{
    md_init();

    pimpl = md_impl_create ? md_impl_create() : NULL;
}

md::~md()
{
    if (md_impl_destroy)
        md_impl_destroy(pimpl);
}

double md::location_altitude() const
{
    if (version::apilevel() >= 2)
        throw std::runtime_error("Method md::location_altitude() is deprecated! Use geo::altitude() instead!");

    return md_impl_location_altitude(pimpl);
}
