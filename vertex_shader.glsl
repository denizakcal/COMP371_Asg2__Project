#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec3 normal;

out vec4 fragmentColor;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

mat4 world_matrix = model_matrix;

vec4 worldPos = vec4(position.x,position.y,position.z,1) * world_matrix;
vec4 lightPos = vec4(0, 20, 0, 1);
vec4 lightDir = normalize(lightPos - worldPos);
vec4 normalWorld = vec4(normal.x, normal.y, normal.z, 0) * world_matrix;
float attenuation = dot(lightDir,normalWorld);

void main()
{
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position.x, position.y, position.z, 1.0);
    fragmentColor = vertexColor * attenuation;
}
