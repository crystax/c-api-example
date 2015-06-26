#ifndef GEO_IMPL_H
#define GEO_IMPL_H

class geo_impl
{
public:
    geo_impl();
    ~geo_impl();

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

#endif // GEO_IMPL_H
