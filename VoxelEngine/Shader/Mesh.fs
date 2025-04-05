#version 330 core

in vec3 color;
in vec3 normal;
out vec4 FragColor;

void main()
{
    //FragColor= mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.2);
    //FragColor = texture(texture1, texCoord);
    FragColor = vec4(color, 1.0);
    //FragColor = vec4(0.5f, 0.6f, 0.7f, 1.0f);
    //FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
    //FragColor = texture(texture2, texCoord) * vec4(ourColor, 1.0);
    //FragColor = texture(texture1, texCoord);
    //FragColor = texture(texture2, texCoord);
}