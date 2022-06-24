uniform mat4  cool_camera_inverse_view_projection;
uniform float cool_camera_far_plane;

vec3 _apply_camera(vec3 pos)
{
    vec4 v = (cool_camera_inverse_view_projection * vec4(pos, 1.));
    return v.xyz / v.w;
}

vec2 _point_on_face()
{
    return mix(vec2(-1.), vec2(1.), _uv);
}

vec3 cool_ray_origin()
{
    return _apply_camera(vec3(_point_on_face(), -1.));
}

vec3 cool_ray_direction()
{
    vec3 begin = vec3(_point_on_face(), -1.);
    vec3 end   = vec3(_point_on_face(), 0.);
    return normalize(_apply_camera(end) - _apply_camera(begin));
}