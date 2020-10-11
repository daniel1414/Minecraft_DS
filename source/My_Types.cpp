#include "My_Types.h"

#include <nds/arm9/math.h>

void normalize_vec3f32(vec3f32& v)
{
    int32 magnitude = sqrtf32(mulf32(v.x, v.x) + mulf32(v.y, v.y) + mulf32(v.z, v.z));

	v.x = divf32(v.x, magnitude);
	v.y = divf32(v.y, magnitude);
	v.z = divf32(v.z, magnitude);
}

void normalize_vec3f(vec3f& v)
{
	v.x = floattof32(v.x);
	v.y = floattof32(v.y);
	v.z = floattof32(v.z);
	
	int32 magnitude = sqrtf32(mulf32(v.x, v.x) + mulf32(v.y, v.y) + mulf32(v.z, v.z));

	v.x = f32tofloat(divf32(v.x, magnitude));
	v.y = f32tofloat(divf32(v.y, magnitude));
	v.z = f32tofloat(divf32(v.z, magnitude));
}

vec3f32 cross_vec3f32(const vec3f32& a, const vec3f32& b)
{
    vec3f32 result;
    result.x = mulf32(a.y, b.z) - mulf32(b.y, a.z);
	result.y = mulf32(a.z, b.x) - mulf32(b.z, a.x);
	result.z = mulf32(a.x, b.y) - mulf32(b.x, a.y);
    return result;
}