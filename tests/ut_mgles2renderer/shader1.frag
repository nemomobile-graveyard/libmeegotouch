varying lowp vec2 fragTexCoord;
uniform sampler2D texture0;
uniform lowp float opacity;

void main(void)
{
    gl_FragColor = vec4(fragTexCoord.x, fragTexCoord.y, 0.5, 1.0);
}
