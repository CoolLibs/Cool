
// Resources about hexagons:
// https://www.youtube.com/watch?v=VmrIDyYiJBA&ab_channel=TheArtofCode
// https://andrewhungblog.wordpress.com/2018/07/28/shader-art-tutorial-hexagonal-grids/

// pixel -> id of the first grid
vec2 internal_Cool_pix_to_grid_ID1(vec2 uv)
{
    return floor((uv + vec2(0, 0.25)) / vec2(1., sqrt_3) - vec2(0, 0.25));
}
vec2 internal_Cool_id1_to_center(vec2 id)
{
    return (id + 0.5) * vec2(1., sqrt_3);
}

// pixel -> id of the second grid
vec2 internal_Cool_id2_to_center(vec2 id)
{
    return (id + 1.) * vec2(1., sqrt_3);
}
vec2 internal_Cool_pix_to_grid_id2(vec2 uv)
{
    return floor((uv - vec2(0.5) + vec2(0, 0.25)) / vec2(1., sqrt_3) - vec2(0, 0.25));
}


// TODO Use ivec2
vec2 Cool_hex_id(vec2 uv, float grid_size)
{
    uv *= grid_size;

    vec2 id1 = internal_Cool_pix_to_grid_ID1(uv);
    vec2 id2 = internal_Cool_pix_to_grid_id2(uv);

    vec2 center1 = internal_Cool_id1_to_center(id1);
    vec2 center2 = internal_Cool_id2_to_center(id2);

    vec2 id = length_squared(uv - center1) < length_squared(uv - center2)
                  ? id1.x * vec2(1, 0) + id1.y * vec2(1, 2) + vec2(0, 1)
                  : id2.x * vec2(1, 0) + id2.y * vec2(1, 2) + vec2(1, 2);
    return id;
}

vec2 Cool_hex_uv(vec2 uv, float grid_size)
{
    uv *= grid_size;

    vec2 r = vec2(1., sqrt_3);
    vec2 h = r * 0.5;

    vec2 a = mod(uv, r) - h;
    vec2 b = mod(uv - h, r) - h;

    return dot(a, a) < dot(b, b)
               ? a
               : b;
}

vec2 Cool_hex_center_from_hex_id(vec2 id, float grid_size)
{
    return ((id.x + 1) * vec2(1,0) + id.y * vec2(-0.5,sqrt_3/2)) / grid_size;
}