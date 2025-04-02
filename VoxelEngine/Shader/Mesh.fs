#version 330 core

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

out vec4 FragColor;
void main()
{
    //FragColor= mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.2);
    //FragColor = texture(texture1, texCoord);
    FragColor = vec4(ourColor, 1.0);
    //FragColor = vec4(0.5f, 0.6f, 0.7f, 1.0f);
    //FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
    //FragColor = texture(texture2, texCoord) * vec4(ourColor, 1.0);
    //FragColor = texture(texture1, texCoord);
    //FragColor = texture(texture2, texCoord);
}