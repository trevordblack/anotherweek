#ifndef VEC4_H
#define VEC4_H
//==============================================================================================
// Originally written in 2019 by Trevor David Black
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include <iostream>

class vec4 {
    public:
        vec4() : e{0,0,0,0} {}
        vec4(double e0, double e1, double e2, double e3) : e{e0, e1, e2, e3} {}

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }
        double w() const { return e[3]; }

        vec4 operator-() const { return vec4(-e[0], -e[1], -e[2], -e[3]); }
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }

        vec4& operator+=(const vec4 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            e[3] += v.e[3];
            return *this;
        }

        vec4& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            e[3] *= t;
            return *this;
        }

        vec4& operator/=(const double t) {
            return *this *= 1/t;
        }

        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2] + e[3]*e[3];
        }

        inline static vec4 random() {
            return vec4(random_double(), random_double(), random_double(), random_double());
        }

        inline static vec4 random(double min, double max) {
            return vec4(random_double(min, max),
                        random_double(min, max),
                        random_double(min, max),
                        random_double(min, max));
        }

        double e[4];
};


// vec4 Utility Functions

inline std::ostream& operator<<(std::ostream &out, const vec4 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2] << ' ' v.e[3];
}

inline vec4 operator+(const vec4 &u, const vec4 &v) {
    return vec4(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2], u.e[3] + v.e[3]);
}

inline vec4 operator-(const vec4 &u, const vec4 &v) {
    return vec4(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2], u.e[3] - v.e[3]);
}

inline vec4 operator*(const vec4 &u, const vec4 &v) {
    return vec4(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2], u.e[3] * v.e[3]);
}

inline vec4 operator*(double t, const vec4 &v) {
    return vec4(t*v.e[0], t*v.e[1], t*v.e[2], t*e[3]);
}

inline vec4 operator*(const vec4 &v, double t) {
    return t * v;
}

inline vec4 operator/(vec4 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec4 &u, const vec4 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2]
         + u.e[3] * v.e[3];
}

inline vec4 unit_vector(vec4 v) {
    return v / v.length();
}

#endif
