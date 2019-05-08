#pragma once
#include <vector>
#include "hitable.h"


//TODO: Refactor to use smart pointers
class hitable_list : public hitable
{
private:
    /* data */
public:
    hitable_list() { }
    hitable_list(std::vector<std::unique_ptr<hitable>> l, int n) {list = std::move(l); list_size = n; }
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    std::vector<std::unique_ptr<hitable>> list;
    int list_size;
    ~hitable_list() { }
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for(size_t i = 0; i < list_size; i++)
    {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}