#version 330 core

uniform vec3 camPos;

in vec3 color;
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

void main()
{
    vec3 lightPos = vec3(0.0, 1000.0, 0.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 lightDir = normalize(lightPos - fragPos);

    float ambientStr = 0.1;
    vec3 ambient = ambientStr * lightColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStr = 0.5;
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStr * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * color;
    fragColor = vec4(result, 1.0);

    //FragColor= mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.2);
    //FragColor = texture(texture1, texCoord);
    //fragColor = vec4(color, 1.0);
    //FragColor = vec4(0.5f, 0.6f, 0.7f, 1.0f);
    //FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
    //FragColor = texture(texture2, texCoord) * vec4(ourColor, 1.0);
    //FragColor = texture(texture1, texCoord);
    //FragColor = texture(texture2, texCoord);
}