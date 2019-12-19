//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "common/rtweekend.h"
#include "common/camera.h"
#include "common/rtw_stb_image.h"
#include "common/texture.h"
#include "box.h"
#include "bvh.h"
#include "constant_medium.h"
#include "hittable_list.h"
#include "material.h"
#include "moving_sphere.h"
#include "sphere.h"

#include <iostream>


vec3 ray_color(const ray& r, hittable *world, int depth) {
    hit_record rec;
    if (depth <= 0 || !world->hit(r, 0.001, infinity, rec))
    {
        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }

    ray scattered;
    vec3 attenuation;
    vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, world, depth-1);
}

int main() {
    int nx = 600;
    int ny = 600;
    int num_samples = 100;
    int max_depth = 50;

    std::cout << "P3\n" << nx << ' ' << ny << "\n255\n";

    hittable **list = new hittable*[1];
    material *red = new lambertian(new constant_texture(vec3(0.85, 0.05, 0.05)));
    list[0] = new rect(red);
    hittable *world = new hittable_list(list, 1);

    vec3 lookfrom(0, 2, 2);
    vec3 lookat(0, 0, 0);

    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;

    camera cam(
        lookfrom, lookat, vec3(0,1,0), vfov, double(nx)/ny, aperture, dist_to_focus, 0.0, 1.0);

    for (int j = ny-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < nx; ++i) {
            vec3 color;
            for (int s = 0; s < num_samples; ++s) {
                auto u = (i + random_double()) / nx;
                auto v = (j + random_double()) / ny;
                ray r = cam.get_ray(u, v);
                color += ray_color(r, world, max_depth);
            }
            color.write_color(std::cout, num_samples);
        }
    }

    std::cerr << "\nDone.\n";
}
