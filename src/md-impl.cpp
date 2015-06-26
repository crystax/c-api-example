class md_impl
{
public:
    md_impl() :alt(1.3) {}
    virtual ~md_impl() {}

    double location_altitude() const {return alt;}

private:
    double alt;
};

extern "C"
{

void *md_impl_create()
{
    return new md_impl();
}

void md_impl_destroy(void *p)
{
    delete reinterpret_cast<md_impl*>(p);
}

double md_impl_location_altitude(void *p)
{
    return reinterpret_cast<md_impl*>(p)->location_altitude();
}

}
