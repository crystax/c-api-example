#include <string>
#include <sstream>
#include <iostream>

#include <version.h>
#include <geo.h>
#include <map.h>
#include <md.h>

static std::string dumplocation(geo const &g)
{
    std::ostringstream os;
    os << g.latitude() << "/" << g.longitude();
    if (version::apilevel() >= 2)
    {
        // This is new method, introduced in v2
        os << "/" << g.altitude();
    }

    return os.str();
}

void test()
{
    std::cout << "We're running on system v" << version::apilevel() << "\n";

    geo g;
    std::cout << "my location: " << dumplocation(g) << "\n";
    if (version::apilevel() >= 2)
    {
        // This is new class, introduced in v2
        map m;
        std::cout << "map location: " << dumplocation(m.location()) << "\n";
    }

    try
    {
        // This is class, defined in v1, but deprecated in v2
        md m;
        std::cout << "md altitude: " << m.location_altitude() << "\n";
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << "\n";
    }
}
