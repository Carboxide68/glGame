
#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec3 texCoord;
} fs_in;

uniform samplerCube depthMap;

uniform vec3 playerPos;

uniform bool shadows;

uniform float far_plane;

uniform float bias;
uniform float diskRadius;

uniform vec3 ambient;

struct Light {
    vec3 color;
    vec3 position;
    int luminosity;
};

uniform Light light;

struct Material {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    uint illum;
    float specE;
    float opacity;
    float opticalDensity;

};

uniform Material material;

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
); 

float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - light.position;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float shadow = 0.0;
    int samples  = 20;
    float viewDistance = length(playerPos - fragPos);
    for(int i = 0; i < samples; ++i) {
        float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}  

void main() {

    vec3 ambient = ambient * material.ambient;
    vec3 normal = normalize(fs_in.Normal);
    // diffuse
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float diff = abs(dot(lightDir, normal));
    vec3 diffuse = light.color * (diff * material.diffuse);
    // specular
    vec3 viewDir = normalize(playerPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(abs(dot(normal, halfwayDir)), material.specE);
    vec3 specular = spec * (light.color * material.specular);    
    // calculate shadow
    float shadow = 0;
    if (shadows) {
        shadow = ShadowCalculation(fs_in.FragPos);
    }
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));    
    
    FragColor = vec4(lighting, 1.0);
}