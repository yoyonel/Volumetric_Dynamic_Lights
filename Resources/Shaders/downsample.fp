
varying float4      vtxUV0;
varying float4      vtxUV1;

uniform float2      texCoordOffset;
uniform sampler2D   textureSrc;

void main()
{    
    float4 sample00 = texture2D(textureSrc,vtxUV0.xy);
    float4 sample01 = texture2D(textureSrc,vtxUV0.zw);
    float4 sample11 = texture2D(textureSrc,vtxUV1.xy);
    float4 sample10 = texture2D(textureSrc,vtxUV1.zw);

    gl_FragColor = (sample00+sample01+sample11+sample10)*0.25;
}




