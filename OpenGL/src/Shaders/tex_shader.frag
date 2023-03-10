#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 frag_pos;

uniform mat3 norm_model;

uniform sampler2D Texture;

void main()
{  
    FragColor = texture(Texture, TexCoords);
}

