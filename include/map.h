#ifndef MAP_H
#define MAP_H

#include <version.h>
#include <geo.h>

class map
{
    map(map const &);
    map &operator=(map const &);

public:
    map();
    ~map();

    geo const &location() const;

private:
    void *pimpl;
    geo *g;
};

#endif /* MAP_H */
