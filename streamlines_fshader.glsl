#version 330 core

// in vec3 FragPos;
// in vec3 Normal;

// out vec4 fragColor;

// uniform vec3 lightPos;
// uniform vec3 viewPos;
// uniform vec3 lightColor;
// uniform vec3 objectColor;

// void main()
// {
//     // Ambient Beleuchtung
//     float ambientStrength = 0.1;
//     vec3 ambient = ambientStrength * lightColor;

//     // Diffuse Beleuchtung
//     vec3 norm = normalize(Normal);
//     vec3 lightDir = normalize(lightPos - FragPos);
//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = diff * lightColor;

//     // Specular Beleuchtung
//     float specularStrength = 0.5;
//     vec3 viewDir = normalize(viewPos - FragPos);
//     vec3 reflectDir = reflect(-lightDir, norm);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//     vec3 specular = specularStrength * spec * lightColor;

//     vec3 result = (ambient + diffuse + specular) * objectColor;
//     fragColor = vec4(result, 1.0);
// }

in vec3 FragPos;
in vec3 Normal;

// Material color
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;

out vec4 fragColor;

void main()
{
    vec3 ambient = 0.2 * objectColor;

    vec3 N = normalize(Normal);
    vec3 L = normalize(lightPos - FragPos);

    // Lambert's cosine law
    float lambertian = max(dot(N, L), 0.0);
    vec3 diffuse = 0.5 * objectColor;

    float specular = 0.0;

    if(lambertian > 0.0) {
        vec3 R = reflect(-L, N);      // Reflected light vector
        vec3 V = normalize(-FragPos); // Vector to viewer

        // Compute the specular term
        float specAngle = max(dot(R, V), 0.0);
        specular = pow(specAngle, 4);
    }

    fragColor = vec4(ambient + lambertian * diffuse + specular * lightColor, 0.5);
}
