#version 330

in vec2 fragTexCoord;
in vec4 fragColor;   
in vec3 fragNormal;

out vec4 finalColor; 

uniform sampler2D texture0; 
uniform vec4 colDiffuse;    
void main()                 
{   
    vec3 lightdir = vec3(0.3333f, 0.58333f, 0.08333f);
    float light = dot(fragNormal, lightdir) + 0.4f;

    vec4 texelColor = texture(texture0, fragTexCoord);
    vec4 mainColor  = texelColor*colDiffuse*fragColor; 
    finalColor = vec4(mainColor.xyz * light, 1.0f);
}

vec3 lightdir = vec3(0.3333f, 0.58333f, 0.08333f);
float light = dot(fragNormal, lightdir);
