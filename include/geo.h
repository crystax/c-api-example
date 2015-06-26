#ifndef GEO_H
#define GEO_H

#include <version.h>

class geo
{
    geo(geo const &);
    geo &operator=(geo const &);

public:
    geo();
    ~geo();

    double latitude() const;
    double longitude() const;
#if APILEVEL >= 2
    double altitude() const;
#endif // APILEVEL >= 2

private:
    class impl;
    impl *pimpl;
};

#endif /* GEO_H */
