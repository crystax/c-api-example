#include <map.h>

class map::impl
{
public:
    geo const &location() const {return loc;}

private:
    geo loc;
};

map::map()
    :pimpl(new map::impl())
{}

map::~map()
{
    delete pimpl;
}

geo const &map::location() const
{
    return pimpl->location();
}
