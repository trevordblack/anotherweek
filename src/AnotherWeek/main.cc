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

#include "rtweekend.h"

#include "box.h"
#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "hittable_list.h"
#include "material.h"
#include "moving_sphere.h"
#include "rtw_stb_image.h"
#include "sphere.h"
#include "texture.h"

#include <iostream>


vec3 ray_color(const ray& r, const vec3& background, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return vec3(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    vec3 attenuation;
    vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

hittable_list red_quad() {
    hittable_list objects;

    auto red = make_shared<lambertian>(
        make_shared<constant_texture>(vec3(0.85, 0.05, 0.05)));

    objects.add(make_shared<quad>(
        red,
        make_shared<srt>(1.0, 1.0, 1.0,
                         0.0, 0.0, 0.0,
                         0.0, 0.0, 0.0)));
    return objects;
}

hittable_list mirror_and_sphere() {
    hittable_list objects;

    auto red = make_shared<lambertian>(
        make_shared<constant_texture>(vec3(0.85, 0.05, 0.05)));
    auto mirror = make_shared<metal>(vec3(1.0, 1.0, 1.0), 0.0);

    objects.add(make_shared<quad>(
        mirror,
        make_shared<srt>(2.0, 2.0, 1.0,
                         90.0, 0.0, 0.0,
                         0.0, 0.0, 0.0)));
    objects.add(make_shared<sphere>(
        vec3(0.0, 1.0, 0.0),
        0.5,
        red));

    return objects;
}

int main() {
    const int image_width = 600;
    const int image_height = 600;
    const auto aspect_ratio = double(image_width) / image_height;

    hittable_list world;

    int samples_per_pixel = 100;
    int max_depth = 50;

    vec3 lookfrom;
    vec3 lookat;
    vec3 vup(0,1,0);
    auto vfov = 40.0;
    auto aperture = 0.0;
    auto dist_to_focus = 10.0;
    vec3 background(0,0,0);

    switch (1) {
        case 0:
            world = red_quad();
            lookfrom = vec3(0, 2, 2);
            lookat = vec3(0, 0, 0);
            vfov = 40.0;
            background = vec3(0.70, 0.80, 1.00);
            break;

        case 1:
            world = mirror_and_sphere();
            lookfrom = vec3(0, 2, 4);
            lookat = vec3(0, 0, 0);
            vfov = 40.0;
            background = vec3(0.70, 0.80, 1.00);
            break;
    }

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            vec3 color;
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / image_width;
                auto v = (j + random_double()) / image_height;
                ray r = cam.get_ray(u, v);
                color += ray_color(r, background, world, max_depth);
            }
            color.write_color(std::cout, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
