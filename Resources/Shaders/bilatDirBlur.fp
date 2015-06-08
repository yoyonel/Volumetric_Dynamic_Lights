
varying float2      uv;

uniform float4      blurDirStep;
uniform sampler2D   textureSrc;

void main()
{
float2 lUV     = uv;

// Simple directional lur, not a bilateral blur
float4 sample  = texture2D(textureSrc,lUV);
lUV           += blurDirStep.xy;
sample        += texture2D(textureSrc,lUV);
lUV           += blurDirStep.xy;
sample        += texture2D(textureSrc,lUV);
lUV           += blurDirStep.xy;
sample        += texture2D(textureSrc,lUV);
lUV           += blurDirStep.xy;
sample        += texture2D(textureSrc,lUV);
lUV           += blurDirStep.xy;
sample        += texture2D(textureSrc,lUV);
lUV           += blurDirStep.xy;
sample        += texture2D(textureSrc,lUV);
lUV           += blurDirStep.xy;
sample        += texture2D(textureSrc,lUV);
lUV           += blurDirStep.xy;
sample        += texture2D(textureSrc,lUV);
lUV           += blurDirStep.xy;

gl_FragColor  = sample * 1.0 / 9.0;

}
