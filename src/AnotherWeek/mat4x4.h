#ifndef MAT4X4_h
#define MAT4X4_h

#include "vec4.h"
#include <iostream>

class mat4x4 {
    public:
        mat4x4() {
            v[0] = vec4(1, 0, 0, 0);
            v[1] = vec4(0, 1, 0, 0);
            v[2] = vec4(0, 0, 1, 0);
            v[3] = vec4(0, 0, 0, 1);
        }

        mat4x4(const vec4 &v0, const vec4 &v1, const vec4 &v2, const vec4 &v3) {
           v[0] = v0;
           v[1] = v2;
           v[2] = v3;
           v[3] = v4;
        }
    
        mat4x4(double m00, double m01, double m02, double m03,
               double m10, double m11, double m12, double m13,
               double m20, double m21, double m22, double m33,
               double m30, double m31, double m32, double m33) {
           v[0] = vec4(m00, m01, m02, m03);
           v[1] = vec4(m10, m11, m12, m13);
           v[2] = vec4(m20, m21, m22, m23);
           v[3] = vec4(m30, m31, m32, m33);
        }

        vec4 v[4];  
};

inline vec4 operator*(const mat4x4 &m, const vec4 &v) {
    return vec4(dot(m.v[0], v),
                dot(m.v[1], v),
                dot(m.v[2], v),
                dot(m.v[3], v));
}

inline mat4x4 operator*(const mat4x4 &a, const mat4x4 &b) {
    vec4 bt0 = vec4(b.v[0].e[0], b.v[1].e[0], b.v[2].e[0], v.v[3].e[0]);
    vec4 bt1 = vec4(b.v[0].e[1], b.v[1].e[1], b.v[2].e[1], v.v[3].e[1]);
    vec4 bt2 = vec4(b.v[0].e[2], b.v[1].e[2], b.v[2].e[2], v.v[3].e[2]);
    vec4 bt3 = vec4(b.v[0].e[3], b.v[1].e[3], b.v[2].e[3], v.v[3].e[3]);
        
    return mat4x4(dot(a.v[0], bt0), dot(a.v[0], bt1), dot(a.v[0], bt2), dot(a.v[0], bt3),
                  dot(a.v[1], bt0), dot(a.v[1], bt1), dot(a.v[1], bt2), dot(a.v[1], bt3),
                  dot(a.v[2], bt0), dot(a.v[2], bt1), dot(a.v[2], bt2), dot(a.v[2], bt3),
                  dot(a.v[3], bt0), dot(a.v[3], bt1), dot(a.v[3], bt2), dot(a.v[3], bt3));
}

#endif
