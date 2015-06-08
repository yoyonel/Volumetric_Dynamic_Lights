
varying float2 uv;
varying float3 viewRay;

uniform mat4 invVP;
uniform float3 viewPos;
uniform float2 linearDepthParam;
uniform float4 lightPosRad;
uniform float4 lightColor;
uniform float2 cubeShadowProjParam;

uniform sampler2D viewDepthTex;
uniform sampler2D halfDepthTex;

uniform samplerCube shadowCubeMapIndirectionTex;
uniform sampler2D virtualShadowCubeMapTex;

uniform sampler3D noise3dTex;
uniform sampler2D noise2dTex;
uniform float4 noiseTexturesSize;
uniform float2 noiseOffset;

uniform float4 rtResolutionAndInv;


// ad-hoc Attenuation
float lighting(float4 sphereLight, float3 p)
{
    float3 L        = sphereLight.xyz - p.xyz;
    float  Ldist    = length(sphereLight.xyz - p.xyz);
    float3 Lnorm    = L / Ldist;

    float linearAtenuation = min( 1.0, max(0.0, (sphereLight.w-Ldist)/sphereLight.w));
    float attenuation = linearAtenuation ;//* min( 1.0, 1.0 / (Ldist*Ldist));
    
    return attenuation;
}

// Simple sphere intersection
void intersectSphere(float3 dO, float3 dV, out float3 i0, out float3 i1, out float iDepth0, out float iDepth1)
{
    float3 center = float3(0.0,0.0,0.0);
    float r = 4.75;

    float3 Q = lightPosRad.xyz - dO;
    float c = length(Q);
    float v = dot(Q,dV);
    float d = (lightPosRad.w*lightPosRad.w) - ( c*c - v*v );

    if(d<0.0f)
    {
        iDepth0 = iDepth1 = 0.0;
    }
    else
    {
        d = sqrt(d);
        iDepth0 = v-d;
        iDepth1 = v+d;
        i0 = dO + dV*iDepth0;
        i1 = dO + dV*iDepth1;
    }
}




void main()
{
    float2 uv2 = (uv*rtResolutionAndInv.xy + noiseOffset) / noiseTexturesSize.xx;
    float pixelRayMarchNoise = texture2D(noise2dTex,uv2).r;

    // Sample the half res min depth
    float depth = DecodeFloatRGBA(texture2D(halfDepthTex,uv).rgba);

    // Reconstruct linear distance (0..1 mapping to 0..far)
    float linearDepth01 = linearDepthParam.y / (depth - linearDepthParam.x);
    
    // Reconstruct world space position
    float3 worldPos = viewPos + viewRay*linearDepth01;
    
    // Parameters
    float3 viewVec = worldPos.xyz-viewPos;
    float worldPosDist = length(viewVec);
    float3 viewVecNorm = viewVec/worldPosDist;


    // Compute start and end of sphere light
    float3 dO = viewPos;
    float3 dV = viewVecNorm;
    float3 i0;
    float3 i1;
    float  iDepth0;
    float  iDepth1;
    intersectSphere(dO, dV, i0, i1, iDepth0, iDepth1);
    //
    float startDepth = max(0.0, iDepth0);
    startDepth = min(worldPosDist, startDepth);
    float endDepth = max(0.0, iDepth1);
    endDepth = min(worldPosDist, endDepth);


// Some options
#define D_DEPTH_RAY_DITHER
//#define D_3D_VOLUME_NOISE
//#define D_TRAPEZIUM_INTEGRAL
//#define D_SIMPSON_INTEGRAL

    // Ray march
    const float tScat = 0.08;
    const float tAbs = 0.0;
    const float tExt = 0.0;//tScat + tAbs;
    float3 curPos = viewPos + viewVecNorm*startDepth;
#if 0
    const float stepLen = 0.005; // 200 steps
#else
    #ifdef D_SIMPSON_INTEGRAL
    // I am using Simpson second's rules as the first one was giving some artefact...
    const float stepLen = 1.0/11.0; // 17-2 samples = 15 a multiple of 3 (required)
    #else
    const float stepLen = 1.0/11.0;
    #endif
#endif

    float stepLenWorld = stepLen * (endDepth-startDepth);
    float curOpticalDepth = exp(-tExt * stepLenWorld); // we have a first step into the volume as sampling at the edge is not interesting
    float scatteredLightAmount = 0.0f;
#ifdef D_DEPTH_RAY_DITHER
    curPos += stepLenWorld * viewVecNorm * (2.0*pixelRayMarchNoise-1.0);
#endif
#if defined(D_TRAPEZIUM_INTEGRAL)
    int numStep = int(1.0/stepLen)-2; // -2 as first and last are skiped 
    int stepId = 0;
#elif defined(D_SIMPSON_INTEGRAL)
    int numStep = 17-2; // -2 as first and last are skiped
    int stepId = 0;
#endif

    for(float l=stepLen; l<0.99999; l+=stepLen) // Do not do the first and last steps
    {
        curPos += stepLenWorld * viewVecNorm;
        
        float density = 1.0;
#ifdef D_3D_VOLUME_NOISE
        density = texture3D(noise3dTex, curPos*0.08).r;
#endif
        
        float l1 = lighting(lightPosRad, curPos) * stepLenWorld * tScat * density;    // Scattered light at p, TODO: phase function
        curOpticalDepth *= exp(-tExt * stepLenWorld * density);                       // Atenuate light from p to view (should be half the length at first for center of considered marched segment?)

        // Sample the virtual depth cubemap
        float3 lightDir = (lightPosRad.xyz - curPos);
        float3 lightDirNorm = -normalize(lightDir); // I should not have to normalise here...
        vec2 indirection = textureCube(shadowCubeMapIndirectionTex, lightDirNorm).ra;
        float lightPointDepth = texture2D(virtualShadowCubeMapTex, indirection).r;
        
        // Compute the depth map value for the current position
        float3 lightDirAbs = abs(lightDir.xyz);
        float lightDirDepth = max(lightDirAbs.x, max(lightDirAbs.y, lightDirAbs.z));
        float curPosDepth = (-1.0 / lightDirDepth) * cubeShadowProjParam.x + cubeShadowProjParam.y;
        float shadow = (lightPointDepth>curPosDepth) ? 1.0 : 0.0;

#ifdef D_TRAPEZIUM_INTEGRAL
        float tWeight = (stepId==0 || stepId==(numStep-1)) ? 1.0 : 2.0;
        scatteredLightAmount += curOpticalDepth * l1 * shadow * tWeight;
        stepId++;
#elif defined(D_SIMPSON_INTEGRAL)
        // Simpson's composite rule (quadratic)
        float sWeight = (mod(stepId-1,3) == 2) ? 2 : 3;
        sWeight = (stepId==0 || stepId==(numStep-1)) ? 1.0 : sWeight;
        scatteredLightAmount += curOpticalDepth * l1 * shadow * sWeight;
        stepId++;
#else
        scatteredLightAmount += curOpticalDepth * l1 * shadow;
#endif
    }
    
#ifdef D_TRAPEZIUM_INTEGRAL
    scatteredLightAmount *= 0.5;
#elif defined(D_SIMPSON_INTEGRAL)
    scatteredLightAmount *= 3.0/8.0;
#endif

    gl_FragColor = float4(scatteredLightAmount*lightColor.rgb , curOpticalDepth);
}






