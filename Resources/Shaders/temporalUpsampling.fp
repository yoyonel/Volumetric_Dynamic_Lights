
varying float2      uv;
varying float4      viewRay;
varying float4      prevViewRay;

uniform sampler2D   texVolumetricPrevFrame;
uniform sampler2D   texVolumDepthPrevFrame;
uniform sampler2D   texVolumDepthCurrFrame;

uniform float       timeBasedMaxContribution;
uniform float2      linearDepthParam;
uniform float3      viewPos;
uniform float3      prevViewPos;
uniform mat4        prevFrameVP;








#define D_TEST_DEPTH 0


void main()
{
    // Sample the half res min depth
    float4 currDepth = texture2D(texVolumDepthCurrFrame,uv.xy);
    float depth = DecodeFloatRGBA(currDepth.rgba);
    // Reconstruct linear distance (0..1 mapping to 0..far)
    float linearDepth01 = linearDepthParam.y / (depth - linearDepthParam.x);
    // Reconstruct world space position
    float3 worldPos = viewPos + (viewRay.xyz/viewRay.www)*linearDepth01;
    
    
    // Compute previous frame screen sapce UV
    float4 prevFrameProjPos = prevFrameVP * float4(worldPos,1.0);
    prevFrameProjPos /= prevFrameProjPos.wwww;
    float2 prevFrameUV = prevFrameProjPos.xy*0.5+0.5;



    float4 prevVolum = texture2D(texVolumetricPrevFrame, prevFrameUV);
    float4 prevDepth = texture2D(texVolumDepthPrevFrame, prevFrameUV);
    float prevDepth2 = DecodeFloatRGBA(prevDepth.rgba);
    float prevLinearDepth01 = linearDepthParam.y / (prevDepth2 - linearDepthParam.x);
    
    // We ignore object motion for now
    float3 prevWorldPos = prevViewPos + (prevViewRay.xyz/prevViewRay.www)*prevLinearDepth01;

#if 0
        ////////// This section contains bad code, it can be ignored/removed

    prevVolum.a = 0.1; // Contribution of new frame data (in target buffer). TODO: make this frame rate dependent
    prevVolum.rgb *= 1.0-prevVolum.a;
#if D_TEST_DEPTH
    if( abs(prevLinearDepth01-linearDepth01)>0.1
        || prevFrameUV.x<0.0
        || prevFrameUV.y<0.0
        || prevFrameUV.x>1.0
        || prevFrameUV.y>1.0
    )
    {
        prevVolum = float4(0.0,0.0,0.0,1.0);
        //discard;
    }
#else
    // compare world space position
    float fragmentAceptation = 1.0;
    if( length(prevWorldPos-worldPos)>0.5
        || prevFrameUV.x<0.0
        || prevFrameUV.y<0.0
        || prevFrameUV.x>1.0
        || prevFrameUV.y>1.0
    )
    {
        prevVolum = float4(0.0,0.0,0.0,1.0);
        //discard;
        fragmentAceptation = 0.0;
    }
#endif
#else
    // Temporal up-sampling

    if(    prevFrameUV.x<0.0
        || prevFrameUV.y<0.0
        || prevFrameUV.x>1.0
        || prevFrameUV.y>1.0
    )
    {
        prevVolum = float4(0.0,0.0,0.0,1.0);
        discard;
    }


    float maxNewContribution = timeBasedMaxContribution; // 0.3 would mean the new contribution will be 1/5 of the new picture and accumulated on previous frame data
    #if 0
        float acceptationScale = 1.0;
        float fragmentAceptation = min(1.0, max(0.0, 1.0/(length(prevWorldPos-worldPos)*acceptationScale + 0.0001 ) ));
    #elif 0
        float fragmentAceptation =  length(prevWorldPos-worldPos)>1.0 ? 0.0 : 1.0;
    #elif 1
        float acceptationScale = 0.15;
        float worldDepthDiff = length(prevWorldPos-worldPos);
        float fragmentAceptation = exp( -worldDepthDiff*acceptationScale );
        //fragmentAceptation = worldDepthDiff>1.0 ? 0.0 : fragmentAceptation;
    #elif 0
        float acceptationScale = 0.15;
        float worldDepthDiff = length(prevWorldPos-worldPos);
        float fragmentAceptation = exp( -worldDepthDiff*acceptationScale ) * exp(-0.03*length(worldPos-viewPos));
    #endif
    
    // todo try color diff: i will have to read both data instead of using blending

    float temporalSampleFactor = 1.0 - (1.0-maxNewContribution)*(fragmentAceptation);
    
    // TODO make that transparent
    //temporalSampleFactor = (prevLinearDepth01>0.99999 && linearDepth01>0.99999) ? temporalSampleFactor : 1.0;
    
    // TODO: throw a bit more rays where it matter, in the new area!!!!

    prevVolum.a = temporalSampleFactor;
    prevVolum.rgb *= 1.0 - prevVolum.a;
#endif

//prevVolum.r = 1.0-fragmentAceptation;

    gl_FragColor = prevVolum;



// debug exp aceptance weight
#if 0
gl_FragColor = mix( float4(1.0, 0.0, 0.0, 0.5), float4(0.0, 1.0, 0.0, 0.5), float4(fragmentAceptation) );
return;
#endif


#if 0
//gl_FragColor = float4( 50.0*abs(uv-prevFrameUV), 0.0, 0.0);

//gl_FragColor = float4( uv.x>=0.99?1:0, prevFrameUV.x>=0.99?1:0, 0.0, 0.0);

//gl_FragColor = float4( abs(prevLinearDepth01-prevLinearDepth01)*100.0, 0.0, 0.0, 0.0);


//gl_FragColor.rgb = prevWorldPos;
//gl_FragColor.rgb = worldPos;
gl_FragColor.rgb = abs( sin( uv.x<0.5 ? prevWorldPos : worldPos ));

//gl_FragColor.rgb = float3( uv.x<0.5 ? prevLinearDepth01 : linearDepth01 )*0.1;
//gl_FragColor.rgb = float3( uv.x<0.5 ? prevDepth.rgb : currDepth.rgb );

//gl_FragColor.rgb = float3( length(prevWorldPos-worldPos) );

gl_FragColor.a = 0.0;
#endif


}




