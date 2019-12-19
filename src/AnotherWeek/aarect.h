#ifndef AARECT_H
#define AARECT_H
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
#include "hittable.h"

class rect : public hittable {
    public:
        rect() {}
        rect(material* mat_ptr) : mp(mat_ptr) {}
        rect(material* mat_ptr, srt* srt0) : mp(mat_ptr) {
            transform = srt0;
        }

        virtual bool hit(const ray& r, double t0, double t1, hit_record& rec) const;
        virtual bool bounding_box(double t0, double t1, aabb& output_box) const;

        material  *mp;
};

bool rect::bounding_box(double t0, double t1, aabb& output_box) const {
    output_box = aabb(vec3(-0.5, -0.0001, 0.5), vec3(0.5, 0.0001, 0.5));
    return true;
}

bool rect::hit(const ray& r, double t0, double t1, hit_record& rec) const {
    auto t = -r.origin().y() / r.direction().y();
    if (t < t0 || t > t1)
        return false;
    auto x = r.origin().x() + t*r.direction().x();
    auto z = r.origin().z() + t*r.direction().z();
    if (x < -0.5 || x > 0.5 || z < -0.5 || z > 0.5)
        return false;
    rec.u = x + 0.5;
    rec.v = z + 0.5;
    rec.t = t;
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    rec.normal = vec3(0, 1, 0);
    return true;
}

#endif
