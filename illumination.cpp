#include "illumination.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>


Vec3 vec3(double x, double y, double z)
{
    Vec3 v = {x, y, z};

    return v;
}

Vec3 vec3_add(Vec3 a, Vec3 b)
{
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 vec3_sub(Vec3 a, Vec3 b)
{
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 vec3_scale(Vec3 a, double s)
{
    return vec3(a.x * s, a.y * s, a.z * s);
}

Vec3 vec3_mul(Vec3 a, Vec3 b)
{
    return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

double vec3_dot(Vec3 a, Vec3 b)
{
    return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

double vec3_length(Vec3 a)
{
    return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

Vec3 vec3_normalize(Vec3 a)
{
    double len = vec3_length(a);

    if (len < 1e-10) 
    {
        return vec3(0.0, 0.0, 0.0);
    }

    return vec3(a.x / len, a.y / len, a.z / len);
}

Vec3 vec3_clamp(Vec3 a)
{
    Vec3 out;

    if (a.x < 0.0)
    {
        out.x = 0.0;
    }
    
    else if (a.x > 1.0)
    {
        out.x = 1.0;
    }

    else
    {
        out.x = a.x;
    }

    if (a.y < 0.0)
    {
        out.y = 0.0;
    }

    else if (a.y > 1.0)
    {
        out.y = 1.0;
    }

    else
    {
        out.y = a.y;
    }

    if (a.z < 0.0)
    {
        out.z = 0.0;
    }

    else if (a.z > 1.0)
    {
        out.z = 1.0;
    }

    else
    {
        out.z = a.z;
    }

    return out;
}


double intersect_sphere(Vec3 origin, Vec3 dir, const Object *s)
{
    Vec3   oc      = vec3_sub(s->position, origin);
    double t_close = vec3_dot(oc, dir);
    double oc_len2 = vec3_dot(oc, oc);

    if (t_close < 0.0 && oc_len2 > s->radius * s->radius)
    {
        return -1.0;
    }

    double d2 = oc_len2 - t_close * t_close;
    double r2 = s->radius * s->radius;

    if (d2 > r2)
    {
        return -1.0;
    }

    double a  = sqrt(r2 - d2);
    double t1 = t_close - a;
    double t2 = t_close + a;

    if (t1 > 1e-6)
    {
        return t1;
    }

    if (t2 > 1e-6)
    {
        return t2;
    }

    return -1.0;
}

double intersect_plane(Vec3 origin, Vec3 dir, const Object *p)
{
    double denom = vec3_dot(p->normal, dir);

    if (fabs(denom) < 1e-10)
    {
        return -1.0;
    }

    Vec3 op = vec3_sub(p->position, origin);
    double t  = vec3_dot(p->normal, op) / denom;

    if (t > 1e-6)
    {
        return t;
    }

    return -1.0;
}

static void parse_object_props(char *line, Object *obj)
{
    char *tok = strtok(line, " \t\r\n");

    while (tok)
    {
        size_t len = strlen(tok);

        if (len > 0 && tok[len - 1] == ':')
        {
            tok[len - 1] = '\0';

            const char *key = tok;

            if (strcmp(key, "width") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok)
                {
                    obj->cam_width = atof(tok);
                }
            }

            else if (strcmp(key, "height") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->cam_height = atof(tok);
                }
            }

            else if (strcmp(key, "radius") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->radius = atof(tok);
                }
            }

            else if (strcmp(key, "ns") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->ns = atof(tok);
                }
            }

            else if (strcmp(key, "c_diff") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->c_diff.x = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->c_diff.y = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->c_diff.z = atof(tok);
                }
            }

            else if (strcmp(key, "c_spec") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->c_spec.x = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->c_spec.y = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->c_spec.z = atof(tok);
                }
            }

            else if (strcmp(key, "position") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->position.x = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->position.y = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->position.z = atof(tok);
                }
            }

            else if (strcmp(key, "normal") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->normal.x = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->normal.y = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    obj->normal.z = atof(tok);
                }
            }
        }

        tok = strtok(NULL, " \t\r\n");
    }
}

static void parse_light_props(char *line, Light *light)
{
    char *tok = strtok(line, " \t\r\n");

    while (tok)
    {
        size_t len = strlen(tok);

        if (len > 0 && tok[len - 1] == ':')
        {
            tok[len - 1] = '\0';

            const char *key = tok;

            if (strcmp(key, "position") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->position.x = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->position.y = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->position.z = atof(tok);
                }
            }

            else if (strcmp(key, "color") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->color.x = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->color.y = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->color.z = atof(tok);
                }
            }

            else if (strcmp(key, "radial_a0") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->radial_a0 = atof(tok);
                }
            }

            else if (strcmp(key, "radial_a1") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->radial_a1 = atof(tok);
                }
            }

            else if (strcmp(key, "radial_a2") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->radial_a2 = atof(tok);
                }
            }

            else if (strcmp(key, "theta") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->theta = atof(tok);
                }
            }

            else if (strcmp(key, "angular_a0") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->angular_a0 = atof(tok);
                }
            }

            else if (strcmp(key, "direction") == 0)
            {
                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->direction.x = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->direction.y = atof(tok);
                }

                tok = strtok(NULL, " \t\r\n");

                if (tok) 
                {
                    light->direction.z = atof(tok);
                }
            }
        }

        tok = strtok(NULL, " \t\r\n");
    }
}

int load_scene(const char *filename,
               Object *objects, int *obj_count,
               Light  *lights,  int *light_count,
               Object **camera)
{
    FILE *fp = fopen(filename, "r");

    if (!fp)
    {
        fprintf(stderr, "Error: cannot open scene file '%s'\n", filename);

        return 0;
    }

    char line[1024];
    *obj_count = 0;
    *light_count = 0;
    *camera = NULL;

    if (!fgets(line, sizeof(line), fp))
    {
        fclose(fp);

        return 0;
    }

    {
        int i = (int)strlen(line) - 1;

        while (i >= 0 && (line[i] == '\n' || line[i] == '\r' || line[i] == ' '))
        {
            line[i--] = '\0';
        }
    }

    if (strcmp(line, "img410scene") != 0)
    {
        fprintf(stderr, "Error: missing 'img410scene' header\n");

        fclose(fp);

        return 0;
    }

    while (fgets(line, sizeof(line), fp))
    {
        {
            int i = (int)strlen(line) - 1;

            while (i >= 0 && (line[i] == '\n' || line[i] == '\r'))
            {    
                line[i--] = '\0';
            }
        }

        if (strlen(line) == 0) 
        {
            continue;
        }

        if (strncmp(line, "end", 3) == 0) 
        {
            break;
        }

        char linecopy[1024];

        strncpy(linecopy, line, sizeof(linecopy) - 1);
        linecopy[sizeof(linecopy) - 1] = '\0';

        char *type_tok = strtok(linecopy, " \t\r\n");

        if (!type_tok) 
        {
            continue;
        }

        if (strcmp(type_tok, "light") == 0)
        {
            if (*light_count >= MAX_LIGHTS)
            {
                fprintf(stderr, "Warning: light limit reached, skipping\n");

                continue;
            }

            Light *lt = &lights[*light_count];

            memset(lt, 0, sizeof(Light));

            char rest[1024];
            char *after = line + strlen(type_tok);

            strncpy(rest, after, sizeof(rest) - 1);

            rest[sizeof(rest) - 1] = '\0';

            {
                int i = (int)strlen(rest) - 1;

                while (i >= 0 && (rest[i] == ';' || rest[i] == ' ' ||
                                   rest[i] == '\r' || rest[i] == '\n'))
                {
                    rest[i--] = '\0';
                }
            }

            parse_light_props(rest, lt);

            lt->direction = vec3_normalize(lt->direction);

            (*light_count)++;

            continue;
        }

        if (*obj_count >= MAX_OBJECTS)
        {
            fprintf(stderr, "Warning: object limit reached, skipping\n");

            continue;
        }

        Object *obj = &objects[*obj_count];

        memset(obj, 0, sizeof(Object));

        obj->ns = 20.0;

        if (strcmp(type_tok, "camera") == 0)
        {
            obj->type = OBJ_CAMERA;
        }

        else if (strcmp(type_tok, "sphere") == 0)
        {
            obj->type = OBJ_SPHERE;
        }

        else if (strcmp(type_tok, "plane") == 0)
        {
            obj->type = OBJ_PLANE;
        }

        else
        {
            continue;
        }

        char rest[1024];
        char *after = line + strlen(type_tok);

        strncpy(rest, after, sizeof(rest) - 1);

        rest[sizeof(rest) - 1] = '\0';

        {
            int i = (int)strlen(rest) - 1;

            while (i >= 0 && (rest[i] == ';'  || rest[i] == ' ' ||
                               rest[i] == '\r' || rest[i] == '\n'))
            {
                rest[i--] = '\0';
            }
        }

        parse_object_props(rest, obj);

        if (obj->type == OBJ_CAMERA) 
        {
            *camera = obj;
        }

        (*obj_count)++;
    }

    fclose(fp);

    return 1;
}

static int in_shadow(Vec3 hit,
                     const Light  *light,
                     const Object *objects, int obj_count)
{
    Vec3 to_light = vec3_sub(light->position, hit);
    double dist = vec3_length(to_light);
    Vec3 shadow_dir = vec3_normalize(to_light);

    for (int i = 0; i < obj_count; i++)
    {
        const Object *obj = &objects[i];
        double t = -1.0;

        if (obj->type == OBJ_SPHERE)
        {
            t = intersect_sphere(hit, shadow_dir, obj);
        }

        else if (obj->type == OBJ_PLANE)
        {
            t = intersect_plane(hit, shadow_dir, obj);
        }

        if (t > 1e-6 && t < dist - 1e-6)
        {
            return 1;
        }
    }

    return 0;
}

static double radial_attenuation(const Light *light, double dist)
{
    double denom = light->radial_a2 * dist * dist
                 + light->radial_a1 * dist
                 + light->radial_a0;

    if (fabs(denom) < 1e-12)
    {
        return 1.0;
    }

    return 1.0 / denom;
}

static double angular_attenuation(const Light *light, Vec3 hit)
{
    if (light->theta <= 0.0 || vec3_length(light->direction) < 1e-9)
    {
        return 1.0;
    }

    Vec3 V_O = vec3_normalize(vec3_sub(hit, light->position));
    double cos_alpha = vec3_dot(V_O, light->direction);
    double cos_theta = cos(light->theta * PI / 180.0);

    if (cos_alpha < cos_theta) 
    {
        return 0.0;
    }

    return pow(cos_alpha, light->angular_a0);
}

Vec3 shade(Vec3 hit, Vec3 normal, Vec3 view_dir,
           const Object *obj,
           const Object *objects, int obj_count,
           const Light  *lights,  int light_count)
{
    Vec3 colour = vec3(0.0, 0.0, 0.0);

    for (int li = 0; li < light_count; li++)
    {
        const Light *light = &lights[li];

        if (in_shadow(hit, light, objects, obj_count))
        {
            continue;
        }

        Vec3 to_light = vec3_sub(light->position, hit);
        double dist = vec3_length(to_light);
        Vec3 L = vec3_normalize(to_light);
        Vec3 N = normal;
        Vec3 V = vec3_normalize(view_dir);
        double f_rad = radial_attenuation(light, dist);
        double f_ang = angular_attenuation(light, hit);
        double NdotL = vec3_dot(N, L);

        if (NdotL < 0.0) 
        {
            NdotL = 0.0;
        }

        Vec3 diffuse = vec3_scale(vec3_mul(light->color, obj->c_diff), NdotL);
        Vec3 specular = vec3(0.0, 0.0, 0.0);

        if (NdotL > 0.0)
        {
            Vec3 R = vec3_sub(vec3_scale(N, 2.0 * NdotL), L);
            double RdotV = vec3_dot(R, V);

            if (RdotV > 0.0)
            {
                specular = vec3_scale(vec3_mul(light->color, obj->c_spec), pow(RdotV, obj->ns));
            }
        }

        Vec3 contribution = vec3_scale(vec3_add(diffuse, specular), f_rad * f_ang);

        colour = vec3_add(colour, contribution);
    }

    return colour;
}


uint8_t float_to_u8(double v)
{
    int iv = (int)(v * 255.0 + 0.5);

    if (iv < 0) 
    {
        return 0;
    }

    if (iv > 255) 
    {
        return 255;
    }

    return (uint8_t)iv;
}


int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s width height input.scene output.ppm\n", argv[0]);

        return 1;
    }

    int img_width = atoi(argv[1]);
    int img_height = atoi(argv[2]);
    const char *scene_file = argv[3];
    const char *output_file = argv[4];

    if (img_width <= 0 || img_height <= 0)
    {
        fprintf(stderr, "Error: invalid image dimensions %dx%d\n", img_width, img_height);

        return 1;
    }

    Object objects[MAX_OBJECTS];
    Light lights[MAX_LIGHTS];
    int obj_count = 0;
    int light_count = 0;
    Object *camera = NULL;

    if (!load_scene(scene_file, objects, &obj_count, lights,  &light_count, &camera))
    {
        return 1;
    }

    if (!camera)
    {
        fprintf(stderr, "Error: no camera found in scene\n");

        return 1;
    }

    double view_width  = camera->cam_width;
    double view_height = camera->cam_height;
    Image img;
    img.width = img_width;
    img.height = img_height;
    img.maxVal = 255;
    img.pixels = (Color *)calloc((size_t)(img_width * img_height), sizeof(Color));

    if (!img.pixels)
    {
        fprintf(stderr, "Error: out of memory\n");

        return 1;
    }

    Vec3 origin = vec3(0.0, 0.0, 0.0);

    for (int py = 0; py < img_height; py++)
    {
        for (int px = 0; px < img_width; px++)
        {
            double u = view_width * ((px + 0.5) / img_width - 0.5);
            double v = view_height * (0.5 - (py + 0.5) / img_height);
            Vec3 dir = vec3_normalize(vec3(u, v, -1.0));
            double t_min = DBL_MAX;
            Object *hit = NULL;

            for (int i = 0; i < obj_count; i++)
            {
                Object *obj = &objects[i];

                double t = -1.0;

                if (obj->type == OBJ_SPHERE)
                {
                    t = intersect_sphere(origin, dir, obj);
                }

                else if (obj->type == OBJ_PLANE)
                {
                    t = intersect_plane(origin, dir, obj);
                }

                if (t > 0.0 && t < t_min)
                {
                    t_min = t;

                    hit = obj;
                }
            }

            if (!hit) 
            {
                continue;
            }

            Vec3 hit_pos = vec3_add(origin, vec3_scale(dir, t_min));
            Vec3 normal;

            if (hit->type == OBJ_SPHERE)
            {
                normal = vec3_normalize(vec3_sub(hit_pos, hit->position));
            }

            else
            {
                normal = vec3_normalize(hit->normal);

                if (vec3_dot(normal, dir) > 0.0)
                {
                    normal = vec3_scale(normal, -1.0);
                }
            }

            Vec3 view_dir = vec3_scale(dir, -1.0);
            Vec3 colour = shade(hit_pos, normal, view_dir, hit, objects, obj_count, lights,  light_count);

            colour = vec3_clamp(colour);

            int idx = py * img_width + px;
            img.pixels[idx].red   = float_to_u8(colour.x);
            img.pixels[idx].green = float_to_u8(colour.y);
            img.pixels[idx].blue  = float_to_u8(colour.z);
        }
    }

    if (write_ppm(output_file, &img) != 0)
    {
        free_image(&img);

        return 1;
    }

    free_image(&img);

    printf("Rendered %dx%d image to '%s'\n", img_width, img_height, output_file);

    return 0;
}


//for x in width:
    //for y in height:
        //Construct Ro and Rd 
        //for object in objects:
            //t = object.intersect(Ro, Rd)
            //Keep the minimum positive t value 
            //Keep the corresponding object
        //use the object's color for the pixel color
        //need to know the specular object color
        //need to know the normal at the intersection
        //I = (0, 0, 0) - entirely in shadow
        //for light in lights:
            //L = construct a ray from the intersection to the light
            //for object in objects:
                //t = object.intersect(Lo, Ld)
                //if t < distance to L then it's in shadow
                //break when you find a shadow
            //if shadow: continue
            //else: light the scene
            //radial attenuation: applies to all lights
            //angular attenuation: just spotlights
            //N = from file for plane
            //N = intersection point - sphere center for sphere
            //V = view vector (-Rd)
            //R = reflection of L about N (you wrote this code already in 3D math library)
            //I_diff = O_diff * I_i (N . L) if N.L > 0
            //I_spec = O_spec * I_i (V . R)^n if V.R > 0 and N.L > 0
            //f_rad = see slide 7
            //f_ang = see slide 10
            //I += f_rad * f_ang * (I_spec + I_diff)
        //clamp(I, (1, 1, 1))
        //assign the color to an element in the pixmap


//Shadow:
//X---------------------------------->L
//                 t