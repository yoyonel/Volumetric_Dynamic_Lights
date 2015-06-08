
varying float2      uv;

uniform sampler2D   hiResDepthTex;
uniform float4      offsets;






void main()
{
    //float depth = texture2D(hiResDepthTex,uv).r;
    float depth00 = texture2D(hiResDepthTex,uv+offsets.xy).r;
    float depth01 = texture2D(hiResDepthTex,uv+offsets.xw).r;
    float depth11 = texture2D(hiResDepthTex,uv+offsets.zw).r;
    float depth10 = texture2D(hiResDepthTex,uv+offsets.zy).r;

    float minDepth = min( min(depth00,depth01) , min(depth11,depth10) );

#ifdef D_DEPTH_PACKED_AS_RGBA
    gl_FragColor = minDepth==1.0 ? float4(1.0,0.0,0.0,0.0) : EncodeFloatRGBA(minDepth);
#else
    gl_FragColor = EncodeFloatRGBA(minDepth);
#endif
    
    // That does not work :(
    //gl_FragDepth = 0.5;//min( min(depth00,depth01) , min(depth11,depth10) );
}
