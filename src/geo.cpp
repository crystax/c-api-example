#include <geo.h>

class geo::impl
{
public:
    impl()
    {
        lat = 1.1;
        lon = 2.2;
#if APILEVEL >= 2
        alt = 3.3;
#endif // APILEVEL >= 2
    }

    double latitude() const   {return lat;}
    double longitude() const  {return lon;}
#if APILEVEL >= 2
    double altitude() const   {return alt;}
#endif // APILEVEL >= 2

private:
    double lat;
    double lon;
#if APILEVEL >= 2
    double alt;
#endif // APILEVEL >= 2
};

geo::geo()
    :pimpl(new geo::impl)
{
}

geo::~geo()
{
    delete pimpl;
}

double geo::latitude() const
{
    return pimpl->latitude();
}

double geo::longitude() const
{
    return pimpl->longitude();
}

#if APILEVEL >= 2
double geo::altitude() const
{
    return pimpl->altitude();
}
#endif // APILEVEL >= 2
