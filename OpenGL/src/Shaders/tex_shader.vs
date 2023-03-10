
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 frag_pos;

uniform mat4 model;
uniform mat3 norm_model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    vec3 normal = vec3(0.0f, 0.0f, 1.0f);
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoords = aTexCoords;
    frag_pos = vec3(model * vec4(aPos, 1.0f));
    Normal = norm_model * normal;  
}

