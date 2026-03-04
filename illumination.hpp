#ifndef ILLUMINATION_HPP
#define ILLUMINATION_HPP

#include "ppm.hpp"


struct Vec3 
{
    double x, y, z;
};


Vec3 vec3(double x, double y, double z);
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_scale(Vec3 a, double s);
Vec3 vec3_mul(Vec3 a, Vec3 b);
double vec3_dot(Vec3 a, Vec3 b);
double vec3_length(Vec3 a);
Vec3 vec3_normalize(Vec3 a);
Vec3 vec3_clamp(Vec3 a);


typedef enum 
{
    OBJ_CAMERA,
    OBJ_SPHERE,
    OBJ_PLANE
} ObjType;

typedef struct 
{
    ObjType type;
    Vec3 position;
    Vec3 normal;
    double radius;
    Vec3 c_diff;
    Vec3 c_spec;
    double ns;
    double cam_width;
    double cam_height;
} Object;

typedef struct 
{
    Vec3 position;
    Vec3 color;
    double radial_a0;
    double radial_a1;
    double radial_a2;
    double theta;
    double angular_a0;
    Vec3 direction;
} Light;


#define MAX_OBJECTS 128
#define MAX_LIGHTS 64
#define PI 3.14159265358979323846


double intersect_sphere(Vec3 origin, Vec3 dir, const Object *s);
double intersect_plane (Vec3 origin, Vec3 dir, const Object *p);


int load_scene(const char *filename, Object *objects, int *obj_count, Light  *lights,  int *light_count, Object **camera);
Vec3 shade(Vec3 hit, Vec3 normal, Vec3 view_dir, const Object *obj, const Object *objects, int obj_count, const Light  *lights,  int light_count);


uint8_t float_to_u8(double v);


#endif