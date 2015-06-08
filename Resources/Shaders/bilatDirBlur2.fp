
varying float2      uv;

uniform float4      blurDirStep;
uniform sampler2D   textureSrc;
uniform sampler2D   textureDepth;



void bilateralBlend(float centerDepth, float4 sampleRGBA, float sampleDepth, out float4 rgbaAccum, out float weightAccum )
{
    rgbaAccum   += sampleRGBA;
    weightAccum += 1.0;
}

float bilateralBlendWeight(float centerDepth, float sampleDepth)
{
#if 1
    const float bilateralStrenght = 1000.0;
    float weight = 1.0/ (bilateralStrenght*abs(centerDepth-sampleDepth)+1.0);
    return weight;
#else
    // From LotF
    #define BLUR_DEPTH_FALLOFF 100.0
    float depthDiff = abs ( sampleDepth - centerDepth );
    float r2 = BLUR_DEPTH_FALLOFF * depthDiff;
    float w = exp ( -r2*r2 );
    return w;
#endif
}

void main()
{
float2 uvCenter = uv;

float4 rgbaAccum  = texture2D(textureSrc,uvCenter).rgba;
float weightAccum = 1.0;
float centerDepth = DecodeFloatRGBA(texture2D(textureDepth,uvCenter).rgba);

float4 sampleRGBA;
float sampleDepth;

//////////
uvCenter-=blurDirStep.xy;
sampleRGBA  = texture2D(textureSrc,uvCenter);
sampleDepth = DecodeFloatRGBA(texture2D(textureDepth,uvCenter).rgba);
{
    float weight = bilateralBlendWeight(centerDepth,sampleDepth);
    rgbaAccum   += sampleRGBA*weight;
    weightAccum += weight;
}
//////////
uvCenter-=blurDirStep.xy;
sampleRGBA  = texture2D(textureSrc,uvCenter);
sampleDepth = DecodeFloatRGBA(texture2D(textureDepth,uvCenter).rgba);
{
    float weight = bilateralBlendWeight(centerDepth,sampleDepth);
    rgbaAccum   += sampleRGBA*weight;
    weightAccum += weight;
}
//////////
uvCenter-=blurDirStep.xy;
sampleRGBA  = texture2D(textureSrc,uvCenter);
sampleDepth = DecodeFloatRGBA(texture2D(textureDepth,uvCenter).rgba);
{
    float weight = bilateralBlendWeight(centerDepth,sampleDepth);
    rgbaAccum   += sampleRGBA*weight;
    weightAccum += weight;
}
//////////
uvCenter-=blurDirStep.xy;
sampleRGBA  = texture2D(textureSrc,uvCenter);
sampleDepth = DecodeFloatRGBA(texture2D(textureDepth,uvCenter).rgba);
{
    float weight = bilateralBlendWeight(centerDepth,sampleDepth);
    rgbaAccum   += sampleRGBA*weight;
    weightAccum += weight;
}



uvCenter=uv;



//////////
uvCenter+=blurDirStep.xy;
sampleRGBA  = texture2D(textureSrc,uvCenter);
sampleDepth = DecodeFloatRGBA(texture2D(textureDepth,uvCenter).rgba);
{
    float weight = bilateralBlendWeight(centerDepth,sampleDepth);
    rgbaAccum   += sampleRGBA*weight;
    weightAccum += weight;
}
//////////
uvCenter+=blurDirStep.xy;
sampleRGBA  = texture2D(textureSrc,uvCenter);
sampleDepth = DecodeFloatRGBA(texture2D(textureDepth,uvCenter).rgba);
{
    float weight = bilateralBlendWeight(centerDepth,sampleDepth);
    rgbaAccum   += sampleRGBA*weight;
    weightAccum += weight;
}
//////////
uvCenter+=blurDirStep.xy;
sampleRGBA  = texture2D(textureSrc,uvCenter);
sampleDepth = DecodeFloatRGBA(texture2D(textureDepth,uvCenter).rgba);
{
    float weight = bilateralBlendWeight(centerDepth,sampleDepth);
    rgbaAccum   += sampleRGBA*weight;
    weightAccum += weight;
}
//////////
uvCenter+=blurDirStep.xy;
sampleRGBA  = texture2D(textureSrc,uvCenter);
sampleDepth = DecodeFloatRGBA(texture2D(textureDepth,uvCenter).rgba);
{
    float weight = bilateralBlendWeight(centerDepth,sampleDepth);
    rgbaAccum   += sampleRGBA*weight;
    weightAccum += weight;
}



gl_FragColor  = rgbaAccum / weightAccum;
//gl_FragColor  = float4( weightAccum *0.03);
//gl_FragColor  = float4( centerDepth);

}
