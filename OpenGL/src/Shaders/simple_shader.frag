#version 330 core

in vec3 Normal;
in vec3 frag_pos;

out vec4 color;

uniform vec3 obj_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

uniform float constant;
uniform float linear;
uniform float quadratic;

uniform int count_lights;

void main()
{
    vec3 result = vec3(0.0);

    vec3 norm = normalize(Normal);
    vec3 light_ray = normalize(light_pos - frag_pos);

    float distance = length(light_pos - frag_pos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);

    float diff = max(dot(norm, light_ray), 0.0);
    vec3 diffuse = diff * light_color;

    float ambient_strength = 0.1f;
    vec3 ambient = ambient_strength * light_color;

    float specular_strenght = 0.5f;
    vec3 view_ray = normalize(view_pos - frag_pos);
    vec3 reflect_ray = reflect(-light_ray, norm);

    float spec = pow(max(dot(view_ray, reflect_ray), 0.0), 200);
    vec3 specular = specular_strenght * spec * light_color;

    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;

    result = (ambient + diffuse + specular) * obj_color;
    color = vec4(result, 1.0f);
}
