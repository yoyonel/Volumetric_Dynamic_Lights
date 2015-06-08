
#ifdef D_DEPTH_PACKED_AS_RGBA
float4 EncodeFloatRGBA( float v )
{
    float4 enc = float4(1.0, 255.0, 65025.0, 16581375.0) * v;
    enc = fract(enc);
    enc -= enc.yzww * float4(1.0/255.0,1.0/255.0,1.0/255.0,0.0);
    return enc;
}
float DecodeFloatRGBA( float4 rgba )
{
    return dot( rgba, float4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) );
}
#else
float4 EncodeFloatRGBA( float v )
{
    return float4(v);
}
float DecodeFloatRGBA( float4 rgba )
{
    return rgba.r;
}
#endif
