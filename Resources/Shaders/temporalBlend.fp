
varying float2      uv;

uniform sampler2D   texVolumetricPrevFrame;

void main()
{    
    float4 prevFrame = texture2D(texVolumetricPrevFrame,uv.xy);

    gl_FragColor = float4(float3(1.0,1.0,1.0)*prevFrame.xyz, 0.9);
}




