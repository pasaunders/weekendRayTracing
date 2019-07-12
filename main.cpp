#include <iostream>
#include <vector>
#include <memory>
#include <float.h>
#include <random>
#include "vec3.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

vec3 color(const ray& r, std::unique_ptr<hitable> &world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, INFINITY, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth+1);
        } else 
        {
            return vec3(0,0,0);
        }
    } else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }   
}

int main() {


    int nx = 200;
    int ny = 100;
    int ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    std::vector<std::unique_ptr<hitable>> list(4);
    list[0] = std::make_unique<sphere>(vec3(0.0, 0.0, -1), 0.5, std::make_shared<lambertian>(vec3(0.8, 0.3, 0.3)));
    list[1] = std::make_unique<sphere>(vec3(0.0, -100.5, -1.0), 100, std::make_shared<lambertian>(vec3(0.8, 0.8, 0.0)));
    list[2] = std::make_unique<sphere>(vec3(1.0, 0.0, -1), 0.5, std::make_shared<metal>(vec3(0.8, 0.6, 0.2), 1.0));
    list[3] = std::make_unique<sphere>(vec3(-1.0, 0.0, -1), 0.5, std::make_shared<metal>(vec3(0.8, 0.8, 0.8), 0.3));
    std::unique_ptr<hitable> world = std::make_unique<hitable_list>(std::move(list), list.size());
    camera cam;
    for(int j = ny-1; j >=0 ; j--) {
        for(int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++)
            {
                float u = float(i + getRand()) / float(nx);
                float v = float(j + getRand()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0);
            }
            col /= float(ns);
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) ); // "gamma 2" transform.
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
    return 0;
}