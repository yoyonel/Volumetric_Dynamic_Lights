
varying float3 vtxNormal;
varying float4 vtxColor;
varying float4 vtxPosition;

uniform float4 lightPosRad;
uniform float4 lightColor;

// scattering param


// Lambert diffuse
float lighting(float4 sphereLight, float3 p, float3 n)
{
    float3 L        = sphereLight.xyz - p.xyz;
    float  Ldist    = length(sphereLight.xyz - p.xyz);
    float3 Lnorm    = L / Ldist;

    float Lambert  = max( 0.0, dot(n,Lnorm) *0.5 + 0.5 );
    float linearAtenuation = min( 1.0, max(0.0, (sphereLight.w-Ldist)/sphereLight.w));
    float attenuation = max(0.0, min( 1.0, linearAtenuation / (Ldist*Ldist)));
    
    return attenuation*Lambert;
}



void main()
{
    float light = lighting(lightPosRad, vtxPosition.xyz, vtxNormal);

    float3 color = vtxColor.rgb * light * lightColor.rgb;
    gl_FragColor  = float4(color, 1.0);
    
    //gl_FragColor  = float4(vtxPosition.xyz, 1.0);
}




