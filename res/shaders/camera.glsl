uniform mat4 cool_camera_inverse_view_projection;

vec3 __apply_camera(vec3 pos)
{
    vec4 v = (cool_camera_inverse_view_projection * vec4(pos, 1.));
    return v.xyz / v.w;
}

vec2 __point_on_face()
{
    return mix(vec2(-1.), vec2(1.), vTexCoords);
}

vec3 cool_ray_origin()
{
    return __apply_camera(vec3(__point_on_face(), -1.));
}

vec3 cool_ray_direction()
{
    vec3 begin = vec3(__point_on_face(), -1.);
    vec3 end   = vec3(__point_on_face(), 0.);
    return normalize(__apply_camera(end) - __apply_camera(begin));
}