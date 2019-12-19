#ifndef SRT_H
#define SRT_H

#include "mat4x4.h"
#include "vec4.h"

mat4x4 scale_matrix(double sx, double sy, double sz)
{
    return mat4x4(sx,  0,  0, 0,
                   0, sy,  0, 0,
                   0,  0, sz, 0,
                   0,  0,  0, 1);
}

mat4x4 translation_matrix(double tx, ty, double tz)
{
    return mat4x4(0, 0, 0, tx,
                  0, 0, 0, ty,
                  0, 0, 0, tz,
                  0, 0, 0,  1);
}

mat4x4 rotation_x_matrix(double theta)
{
    double c = cos(theta);
    double s = sin(theta);
    return mat4x4(1, 0,  0, 0,
                  0, c, -s, 0,
                  0, s,  c, 0,
                  0, 0,  0, 1);
}

mat4x4 rotation_y_matrix(double theta)
{
    double c = cos(theta);
    double s = sin(theta);
    return mat4x4( c, 0, s, 0,
                   0, 1, 0, 0,
                  -s, 0, c, 0,
                   0, 0, 0, 1);
}

mat4x4 rotation_z_matrix(double theta)
{
    return mat4x4(c, -s, 0, 0,
                  s,  c, 0, 0,
                  0,  0, 1, 0,
                  0,  0, 0, 1);
}

class srt {
    public:
        srt() {}
        srt(double sx, double sy, double sz,
            double rx, double ry, double rz,
            double tx, double ty, double tz)
        {
            mat4x4 scale = scale_matrix(sx, sy, sz);
            mat4x4 rot_x = rotation_x_matrix(rx); 
            mat4x4 rot_y = rotation_y_matrix(ry); 
            mat4x4 rot_z = rotation_z_matrix(rz); 
            mat4x4 trans = translation_matrix(tx, ty, tz);

            m = scale * rot_x * rot_y * rot_z * trans;
        }


        mat4x4 m;
}



#endif
