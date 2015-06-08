
varying float2      uv;

uniform sampler2D   hdrLinTex;
uniform sampler2D   hdrVolTex;


uniform sampler2D hiResDepthTex;
uniform sampler2D halfDepthTex;

uniform float4 upsampleOffsets;

uniform float4 rtResolutionAndInv;



void main()
{
    // Sample the depth
    float depth  = texture2D(hiResDepthTex,uv).x;

    float4 depth4Diff;
    depth4Diff.x = DecodeFloatRGBA(texture2D(halfDepthTex,uv+upsampleOffsets.xy).rgba);
    depth4Diff.y = DecodeFloatRGBA(texture2D(halfDepthTex,uv+upsampleOffsets.xw).rgba);
    depth4Diff.z = DecodeFloatRGBA(texture2D(halfDepthTex,uv+upsampleOffsets.zw).rgba);
    depth4Diff.w = DecodeFloatRGBA(texture2D(halfDepthTex,uv+upsampleOffsets.zy).rgba);
    
    depth4Diff = abs(depth4Diff - depth);
    
    float weightMultiplier = 1000.0;
    float4 weights = 1.0f/(depth4Diff*weightMultiplier+1.0f);
    weights *= 1.0f/dot(weights, float4(1.0));

    // To improve: bilinear weights would be required

    float4 upsampleUvs = (floor((uv.xyxy+upsampleOffsets)*rtResolutionAndInv.xyxy)+0.5) * rtResolutionAndInv.zwzw;
#if 0
    float4 c00 = texture2D(hdrVolTex,upsampleUvs.xy).rgba;
    float4 c01 = texture2D(hdrVolTex,upsampleUvs.xw).rgba;
    float4 c11 = texture2D(hdrVolTex,upsampleUvs.zw).rgba;
    float4 c10 = texture2D(hdrVolTex,upsampleUvs.zy).rgba;
    float4 hdrVol = weights.x*c00 + weights.y*c01 + weights.z*c11 + weights.w*c10;
#elif 1
    float2 upsampleOffset = weights.x*upsampleUvs.xy + weights.y*upsampleUvs.xw + weights.z*upsampleUvs.zw + weights.w*upsampleUvs.zy;
    float4 hdrVol = texture2D(hdrVolTex,upsampleOffset).rgba;
#else
    float4 hdrVol = texture2D(hdrVolTex,uv);
#endif
    

    float4 hdrLin = texture2D(hdrLinTex,uv);

    float3 color = hdrLin.rgb + hdrVol.rgb;
    //color = hdrVol.rgb;
    //color = float3(depth00);
    
    color = pow(color, float3(1.0/2.2));

    gl_FragColor = float4(color, 1.0);
    
    
    //gl_FragColor = float4(hdrVol.rgb, 1.0);
    //gl_FragColor = float4(hdrLin.rgb, 1.0);
}




