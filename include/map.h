#ifndef MAP_H
#define MAP_H

#include <version.h>
#include <geo.h>

#if APILEVEL >= 2

class map
{
    map(map const &);
    map &operator=(map const &);

public:
    map();
    ~map();

    geo const &location() const;

private:
    class impl;
    impl *pimpl;
};

#endif /* APILEVEL >= 2 */

#endif /* MAP_H */
