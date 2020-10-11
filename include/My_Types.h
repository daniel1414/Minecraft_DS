#ifndef _MY_TYPES_H_
#define _MY_TYPES_H_

typedef struct vec3f32
{
    int x, y, z; // fixed point format 20.12
    vec3f32& operator=(const vec3f32& right)
    {
        this->x = right.x; 
        this->y = right.y;
        this->z = right.z;
        return *this;
    }
    vec3f32& operator-=(const vec3f32& right)
    {
        this->x -= right.x;
        this->y -= right.y;
        this->z -= right.z;
        return *this;
    }
    vec3f32& operator+=(const vec3f32& right)
    {
        this->x += right.x;
        this->y += right.y;
        this->z += right.z;
        return *this;
    }
} vec3f32;

typedef struct vec3f
{
    float x, y, z;
    vec3f& operator=(const vec3f right)
    {
        this->x = right.x;
        this->y = right.y;
        this->z = right.z;
        return *this;
    }
} vec3f;

/* normalize a vec3f32 */
void normalize_vec3f32(vec3f32& v);

/* normalize a vec3f */
void normalize_vec3f(vec3f& v);

/* calculate the cross product between two vec3f32's */
vec3f32 cross_vec3f32(const vec3f32& a, const vec3f32& b);

#endif /* _MY_TYPES_H_ */