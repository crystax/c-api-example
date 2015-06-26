#ifndef GEO_H
#define GEO_H

#include <version.h>

class geo
{
    geo(geo const &);
    geo &operator=(geo const &);

    friend class map;
    geo(void *p);

public:
    geo();
    ~geo();

    double latitude() const;
    double longitude() const;
    double altitude() const;

private:
    void *pimpl;
    bool own;
};

#endif /* GEO_H */
