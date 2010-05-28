varying lowp vec2 fragTexCoord;
uniform sampler2D texture0;
uniform lowp float dimRatio;

void main(void)
{
    lowp vec4 baseColor = texture2D(texture0, fragTexCoord);

    baseColor.r *= dimRatio;
    baseColor.g *= dimRatio;
    baseColor.b *= dimRatio;

    gl_FragColor = baseColor;
}
