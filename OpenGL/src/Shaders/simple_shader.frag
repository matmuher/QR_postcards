#version 330 core

in vec3 Normal;
in vec3 frag_pos;
in vec2 TexCoords;

out vec4 color;

uniform vec3 view_pos;
uniform int count_lights;
uniform sampler2D texture_diffuse1;
uniform float ambientStrength;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define N_LIGHTS 50

uniform Light lights[N_LIGHTS]; 


vec3 calc_light(vec3 tex_diffuse, Light light, vec3 norm, vec3 frag_pos, vec3 view_ray);


void main()
{
    vec4 tex = texture(texture_diffuse1, TexCoords);
    vec3 tex_diffuse = vec3(tex.x, tex.y, tex.z);

    vec3 result = vec3(0.0);
    vec3 view_ray = normalize(view_pos - frag_pos);
    vec3 norm = normalize(Normal);

    for (int i = 0; i < count_lights; i++)
    {
        result += calc_light(tex_diffuse, lights[i], norm, frag_pos, view_ray);
    }

    color = vec4(result, 1.0f);
}



vec3 calc_light(vec3 tex_diffuse, Light light, vec3 norm, vec3 frag_pos, vec3 view_ray)
{
    vec3 light_ray = normalize(light.position - frag_pos);

    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    float diff = max(dot(norm, light_ray), 0.0);
    vec3 diffuse = light.diffuse * (diff * tex_diffuse);

    vec3 ambient = ambientStrength * light.ambient * tex_diffuse;

    
    vec3 reflect_ray = reflect(-light_ray, norm);

    float spec = pow(max(dot(view_ray, reflect_ray), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}