#version 330 core
out vec4 color;

in vec3 Normal;
in vec3 frag_pos;

uniform vec3 light_color;
uniform vec3 view_pos;


void main()
{
    vec3 light_ray = normalize(vec3(0.0f, 0.0f, -5.0f) - frag_pos);

    vec3 view_ray = normalize(view_pos - frag_pos);
    vec3 norm = normalize(Normal); 

    float diff = max(dot(norm, light_ray), 0.0f);
    vec3 diffuse = vec3(1.0f, 1.0f, 1.0f) * (diff * light_color);

    vec3 ambient = vec3(1.0f, 1.0f, 1.0f) * light_color;
    
    vec3 reflect_ray = reflect(-light_ray, norm);

    float spec = pow(max(dot(view_ray, reflect_ray), 0.0), 32.0f);
    vec3 specular = vec3(0.5f, 0.5f, 0.5f) * (spec * vec3(0.5f, 0.5f, 0.5f));

    vec3 result = diffuse + specular + ambient;

    color = vec4(4*result, 1.0f);

}