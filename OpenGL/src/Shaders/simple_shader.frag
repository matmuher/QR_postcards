#version 330 core

in vec3 Normal;
in vec3 frag_pos;

out vec4 color;

uniform vec3 view_pos;

uniform int count_lights;

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

uniform Light light; 


void main()
{
    vec3 result = vec3(0.0);

    vec3 norm = normalize(Normal);
    vec3 light_ray = normalize(light.position - frag_pos);

    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    float diff = max(dot(norm, light_ray), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 ambient = light.ambient * material.ambient;

    vec3 view_ray = normalize(view_pos - frag_pos);
    vec3 reflect_ray = reflect(-light_ray, norm);

    float spec = pow(max(dot(view_ray, reflect_ray), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;

    result = (ambient + diffuse + specular);
    color = vec4(result, 1.0f);
}
