#include <md.h>
#include <stdexcept>

#if APILEVEL < 2
class md::impl
{
public:
    impl() :alt(1.3) {}

    double location_altitude() const {return alt;}

private:
    double alt;
};
#endif // APILEVEL < 2

md::md()
#if APILEVEL < 2
    :pimpl(new md::impl())
#endif // APILEVEL < 2
{}

md::~md()
{
#if APILEVEL < 2
    delete pimpl;
#endif // APILEVEL < 2
}

double md::location_altitude() const
{
#if APILEVEL < 2
    return pimpl->location_altitude();
#else // APILEVEL >= 2
    throw std::runtime_error("Method md::location_altitude() is deprecated! Use geo::altitude() instead!");
#endif // APILEVEL >= 2
}
