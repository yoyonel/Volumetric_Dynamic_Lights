//
//  main.cpp
//
//  Created by Sébastien Hillaire
//


#include <iostream>
#include <time.h>
#include <math.h>

#ifdef __linux__
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include <glew/glew.h>
#include <glut/glut.h>
#endif


#include "GPUProgramManager.h"
#include "Camera3D.h"
#include "Frustum.h"
#include "TextureManager.h"
#include "OGLShapes.h"

#include "FrameBufferObject.h"
#include "CubeBufferObject.h"
#include "GpuTimerQuery.h"

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

//#define D_GPU_PERF

static const bool depthAsRGBA = true;

clock_t lastClock, currentClock;

Frustum  gFrustum;
Camera3D gCamera;
bool gbUp;
bool gbDown;
bool gbLeft;
bool gbRight;

/**
int             giWindowViewPortWidth=640;
int             giWindowViewPortHeight=480;
float           gfWindowViewPortWidth=640.0f;
float           gfWindowViewPortHeight=480.0f;
/**/
/**/
int             giWindowViewPortWidth=1280;
int             giWindowViewPortHeight=720;
float           gfWindowViewPortWidth=1280.0f;
float           gfWindowViewPortHeight=720.0f;
/**/
/**
int             giWindowViewPortWidth=1920;
int             giWindowViewPortHeight=1080;
float           gfWindowViewPortWidth=1920.0f;
float           gfWindowViewPortHeight=1080.0f;
/**/

float gfElapsedTime;

bool useBilateralUpsampling = true;
bool useTemporalNoise = false;
float movingLight = 0.0f;
int useTemporalUpsampling = 0;
float maxCurrentFrameContribution = 0.3f;

// forward declarations
void releaseData();
void updateView();

Matrix4 gProjectionMatrix;
Matrix4 gPrevVPMatrix;
Matrix4 gPrevInvVP;
Vector3 gPrevViewPos;

const GPUProgram* gpupSurface = NULL;
GLuint gpupSurface_lightPosRad = 0;
GLuint gpupSurface_lightColor = 0;
GLuint gpupSurface_objectMat = 0;

const GPUProgram* gpupVolumetricPointLight = NULL;
GLuint            gpupVolumetricPointLight_viewDepthTex = 0;
GLuint            gpupVolumetricPointLight_shadowCubeMapIndirectionTex = 0;
GLuint            gpupVolumetricPointLight_virtualShadowCubeMapTex = 0;
GLuint            gpupVolumetricPointLight_halfDepthTex = 0;
GLuint            gpupVolumetricPointLight_noise3dTex = 0;
GLuint            gpupVolumetricPointLight_noise2dTex = 0;
GLuint            gpupVolumetricPointLight_invVP = 0;
GLuint            gpupVolumetricPointLight_viewPos = 0;
GLuint            gpupVolumetricPointLight_linearDepthParam = 0;
GLuint            gpupVolumetricPointLight_lightPosRad = 0;
GLuint            gpupVolumetricPointLight_lightColor = 0;
GLuint            gpupVolumetricPointLight_cubeShadowProjParam = 0;
GLuint            gpupVolumetricPointLight_noiseTexturesSize = 0;
GLuint            gpupVolumetricPointLight_noiseOffset = 0;
GLuint            gpupVolumetricPointLight_rtResolutionAndInv = 0;


const GPUProgram* gpupBilatDirBlur = NULL;
GLuint            gpupBilatDirBlur_blurDirStep = 0;
GLuint            gpupBilatDirBlur_textureSrc = 0;


const GPUProgram* gpupTemporalBlend = NULL;
GLuint            gpupTemporalBlend_texVolumetricPrevFrame = 0;

const GPUProgram* gpupTemporalUpsample = NULL;
GLuint            gpupTemporalUpsample_texVolumetricPrevFrame = 0;
GLuint            gpupTemporalUpsample_texVolumDepthPrevFrame = 0;
GLuint            gpupTemporalUpsample_texVolumDepthCurrFrame = 0;
GLuint            gpupTemporalUpsample_viewPos = 0;
GLuint            gpupTemporalUpsample_prevViewPos = 0;
GLuint            gpupTemporalUpsample_linearDepthParam = 0;
GLuint            gpupTemporalUpsample_invVP = 0;
GLuint            gpupTemporalUpsample_prevInvVP = 0;
GLuint            gpupTemporalUpsample_prevFrameVP = 0;
GLuint            gpupTemporalUpsample_timeBasedMaxContribution = 0;


const GPUProgram* gpupBilatDirBlur2 = NULL;
GLuint            gpupBilatDirBlur2_blurDirStep = 0;
GLuint            gpupBilatDirBlur2_textureSrc = 0;
GLuint            gpupBilatDirBlur2_textureDepth = 0;


const GPUProgram* gpupDepthDownSample = NULL;
GLuint            gpupDepthDownSample_hiResDepthTex = 0;
GLuint            gpupDepthDownSample_offsets = 0;


const GPUProgram* gpupComposite = NULL;
GLuint            gpupComposite_hdrLinTex = 0;
GLuint            gpupComposite_hdrVolTex = 0;
GLuint            gpupComposite_hiResDepthTex = 0;
GLuint            gpupComposite_halfDepthTex = 0;
GLuint            gpupComposite_upsampleOffsets = 0;
GLuint            gpupComposite_rtResolutionAndInv = 0;


GLuint giRenderedTexture;
GLuint giRedTexture;
GLuint giDensityNoiseTexture;
GLuint giRaymarchNoiseTexture;
unsigned int gu2dNoiseTextureSize;
unsigned int gu3dNoiseTextureSize;

FrameBufferObject* lpMainFBO;
FrameBufferObject* lpVolumetricFBO[2];
FrameBufferObject* lpHalfResDepthFBO[2];
FrameBufferObject* lpTempHalfResColorFBO;

CubeBufferObject* cubeShadow;

int currentVolumetricFboId = 0;

int volumetricBlur = 0;

void
reshape (int w, int h)
{
    int giWindowViewPortWidth = w;
    int giWindowViewPortHeight = h;
    if (giWindowViewPortWidth<=0 )
    {
        giWindowViewPortWidth=1;
    }
    if (giWindowViewPortHeight<=0 )
    {
        giWindowViewPortHeight=1;
    }
    
    glViewport (0, 0, (GLsizei)giWindowViewPortWidth, (GLsizei)giWindowViewPortHeight);
    gProjectionMatrix.setPerspective(1.0f, gfWindowViewPortWidth/gfWindowViewPortHeight, 1.0f, 1000.0f);
    
    /*    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    //glOrtho(0.0, (GLdouble)gfWindowViewPortWidth, 0.0, (GLdouble)gfWindowViewPortHeight, 0.0, 1.0);
    //glFrustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 100.0f);
    gluPerspective(70.0f, gfWindowViewPortWidth/gfWindowViewPortHeight, 0.1f, 1000.0f);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();*/
}

void renderScene()
{
    Matrix4 mat;
    
    // Shitty way to send opbject and matrix but anyway...
    
#if 1
    //
    glColor3f(1.0f,1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(1.0,0.0,1.0);
    {
        Matrix4 matT;
        matT.setTranslation(Vector3(1.0,0.0,1.0));
        glUniformMatrix4fv(gpupSurface_objectMat, 1, false, matT.toFloatPtr());
    }
    glutSolidSphere(1,20, 20);
    glPopMatrix();
    
    glColor3f(1.0f,1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(3.0,0.0,3.0);
    {
        Matrix4 matT;
        matT.setTranslation(Vector3(3.0,0.0,3.0));
        glUniformMatrix4fv(gpupSurface_objectMat, 1, false, matT.toFloatPtr());
    }
    glutSolidTeapot(1.0);
    glPopMatrix();
    
    glColor3f(1.0f,1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(6.0,0.0,5.0);
    {
        Matrix4 matT;
        matT.setTranslation(Vector3(6.0,0.0,5.0));
        glUniformMatrix4fv(gpupSurface_objectMat, 1, false, matT.toFloatPtr());
    }
    glutSolidTorus(0.5, 2.0, 20.0, 20.0);
    glPopMatrix();
#endif
    
#if 1
    glColor3f(1.0f,1.0f, 1.0f);
    for(int i=0; i<16; ++i)
    {
        //
        glPushMatrix();
        glTranslatef(-1.0 + float(i)*0.4,5.0,5.0);//
        glRotatef(90.0f, 1.0, 0.0, 0.0);
        {
            Matrix4 matT;
            matT.setTranslation(Vector3(-1.0 + float(i)*0.4,5.0,5.0));
            Matrix4 matR;
            matR.setXRotation(90.0f*DEG2RAD);
            mat = matT * matR;
            glUniformMatrix4fv(gpupSurface_objectMat, 1, false, mat.toFloatPtr());
        }
        glutSolidCone(0.1, 10.0, 10, 10);
        glPopMatrix();
    }
#endif
    
    
    glColor3f(0.9f,0.9f,0.9f);
    mat.identity();
    glUniformMatrix4fv(gpupSurface_objectMat, 1, false, mat.toFloatPtr());
    glBegin(GL_QUADS);
    {
        const float edgem = -100.0f;
        const float edgep =  100.0f;
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(edgem,-2.0f,edgem);
        glVertex3f(edgem,-2.0f,edgep);
        glVertex3f(edgep,-2.0f,edgep);
        glVertex3f(edgep,-2.0f,edgem);
    }
    glEnd();
}

void
display ()
{
    static float lfGlobalTime = 0;
    // update clock
    currentClock = clock();
    gfElapsedTime = float(currentClock - lastClock) / CLOCKS_PER_SEC;
    lastClock=currentClock;
    lfGlobalTime+=gfElapsedTime;

    float lfPitch = gbUp   ? -1.0f : 0.0f;
    lfPitch = gbDown ?  1.0f : lfPitch;
    float lfYaw = gbLeft   ?  1.0f : 0.0f;
    lfYaw = gbRight ? -1.0f : lfYaw;
    gCamera.update(gfElapsedTime, lfPitch, lfYaw);
    
    const float mainViewNearDepth = 0.1f;
    const float mainViewFarDepth = 100.0f;
    const float linearDepthProjectionA = mainViewFarDepth / (mainViewFarDepth - mainViewNearDepth);
    const float linearDepthProjectionB = (-mainViewFarDepth * mainViewNearDepth) / (mainViewFarDepth - mainViewNearDepth);
    
    // Compute matrices
    Matrix4 lViewMatrix;
    gCamera.getViewMatrix(lViewMatrix);
    gProjectionMatrix.setPerspective(90.0f, gfWindowViewPortWidth/gfWindowViewPortHeight, mainViewNearDepth, mainViewFarDepth);
    Matrix4 lViewProjectionMatrix = gProjectionMatrix * lViewMatrix;
    Matrix4 invVP = lViewProjectionMatrix;
    invVP.inverse();
    
    Vector4 lightPosRad = Vector4(2.7f+ movingLight*3.0*sinf(lfGlobalTime*2.0f), 1.5f + 0*sinf(lfGlobalTime*2.0f), 3.0f, 7.0f);
    Vector4 lightColor = Vector4(4.0f,2.0f,1.0f,0.0f) *0.7;
    //Vector4 lightColor = Vector4(3.0f,4.0f,1.0f,0.0f) *1;
    
    //set matrix
    glMatrixMode (GL_PROJECTION);
    //glLoadMatrixf(gProjectionMatrix.toFloatPtr());
    glLoadIdentity();
    glMatrixMode (GL_MODELVIEW);
    //glLoadMatrixf(lViewMatrix.toFloatPtr());
    glLoadMatrixf(lViewProjectionMatrix.toFloatPtr());
    
#ifdef D_GPU_PERF
    GpuTimerQuery gpuTimerDownSample;
    GpuTimerQuery gpuTimerBlur;
    GpuTimerQuery gpuTimerComposite;
#endif // D_GPU_PERF
    
    Vector3 lCameraPosition;
    gCamera.getPosition(lCameraPosition);
    
    Vector4 halfResRtSizeAndInv(
                gfWindowViewPortWidth*0.5f,
                gfWindowViewPortHeight*0.5f,
                1.0/(gfWindowViewPortWidth*0.5f),
                1.0/(gfWindowViewPortHeight*0.5f)
                );
    
    //
    // MAIN SCENE into linear HDR buffer
    //
    
    lpMainFBO->enableRenderToColorAndDepth(0);
    lpMainFBO->saveAndSetViewPort();
    glClearColor(0.15f, 0.15f, 0.15f, 0.0f);
    glClearDepth(1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    gpupSurface->bindGPUProgram();
    glUniform4fv(gpupSurface_lightPosRad, 1, lightPosRad.toFloatPtr());
    glUniform4fv(gpupSurface_lightColor, 1, lightColor.toFloatPtr());
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);
    
    renderScene();
    
    GPUProgram::unbindGPUProgram();
    
    lpMainFBO->restoreViewPort();
    FrameBufferObject::disableRenderToColorDepth();
    
    
    
    
    
    
    //
    // GENERATE CUBE SHADOW MAP
    //
    Vector3 lightPos = Vector3(lightPosRad.x, lightPosRad.y, lightPosRad.z);
    cubeShadow->setCenterPosition(lightPos);
    cubeShadow->setClipPlanes(0.01f, lightPosRad.w);
    cubeShadow->updateFrustums();
    
    Vector2 cubeShadowProjParam;
    {
        const float Near = 0.01;
        const float Far =lightPosRad.w;
        cubeShadowProjParam.y = Far / (Far - Near);
        cubeShadowProjParam.x = Near * cubeShadowProjParam.y;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    cubeShadow->clearAllBuffers(true, true);
    cubeShadow->enableRenderToColorAndDepth();
    
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix();
    cubeShadow->saveAndSetProjectionMatrix();
    gpupSurface->bindGPUProgram();
    for(int f=0; f<6; f++)
    {
        CBO_Frustum cubeFace = CBO_Frustum(f);
        
        cubeShadow->saveAndSetViewPort(cubeFace);
        Frustum& cubeFaceFrustum = cubeShadow->getFrustum(cubeFace);
        glMatrixMode (GL_MODELVIEW);
        glLoadMatrixf(cubeFaceFrustum.getModelViewMat());
        
        renderScene();
        
        cubeShadow->restoreViewPort();
    }
    GPUProgram::unbindGPUProgram();
    cubeShadow->restoreProjectionMatrix();
    glMatrixMode (GL_MODELVIEW);
    glPopMatrix();
    FrameBufferObject::disableRenderToColorDepth();
    
    
    
    
    
    // Switch
    if(useTemporalUpsampling>0)
    {
        currentVolumetricFboId = 1-currentVolumetricFboId;
    }
    FrameBufferObject* curVolumetricFBO = lpVolumetricFBO[currentVolumetricFboId];
    FrameBufferObject* curVolumDepthFBO = lpHalfResDepthFBO[currentVolumetricFboId];
    
    
    //
    // DOWNSAMPLE DEPTH from full res to half resolution
    //
    {
        curVolumDepthFBO->enableRenderToColorAndDepth(0);
        curVolumDepthFBO->saveAndSetViewPort();
        
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        //glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        //glClear (GL_COLOR_BUFFER_BIT);
        
        // pack depth into RGBA as I have not succeeded using gl_Fragdpeht to ouput the min depth at half res
        glActiveTextureARB(GL_TEXTURE0);
        lpMainFBO->bindDepthTexture();
        gpupDepthDownSample->bindGPUProgram();
        gpupDepthDownSample->setTextureToUniform(gpupDepthDownSample_hiResDepthTex, 0);
        const float depthDownSampleOffsetX = 0.5f / float(lpMainFBO->getWidth());
        const float depthDownSampleOffsetY = 0.5f / float(lpMainFBO->getHeight());
        Vector4 depthDownSampleOffsets = Vector4(depthDownSampleOffsetX,depthDownSampleOffsetY,-depthDownSampleOffsetX,-depthDownSampleOffsetY);
        glUniform4fv(gpupDepthDownSample_offsets, 1, depthDownSampleOffsets.toFloatPtr());
        renderScreenQuad();
        
        curVolumDepthFBO->restoreViewPort();
        FrameBufferObject::disableRenderToColorDepth();
    }
    
    
    //
    // RENDER VOLUMETRIC BUFFER
    //
    curVolumetricFBO->enableRenderToColorAndDepth(0);
    curVolumetricFBO->saveAndSetViewPort();
    glClearColor(0.0f, 0.25f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear (GL_COLOR_BUFFER_BIT);
    
    
    glActiveTextureARB(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, giRaymarchNoiseTexture);
    glActiveTextureARB(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_3D, giDensityNoiseTexture);
    glActiveTextureARB(GL_TEXTURE3);
    curVolumDepthFBO->bindColorTexture(0);
    glActiveTextureARB(GL_TEXTURE2);
    cubeShadow->bindIndirectionCubeMap();
    glActiveTextureARB(GL_TEXTURE1);
    cubeShadow->bindVirtualDepthMap();
    glActiveTextureARB(GL_TEXTURE0);
    lpMainFBO->bindDepthTexture(); // high res depth with linear filtering
    
    gpupVolumetricPointLight->bindGPUProgram();
    gpupVolumetricPointLight->setTextureToUniform(gpupVolumetricPointLight_viewDepthTex, 0);
    gpupVolumetricPointLight->setTextureToUniform(gpupVolumetricPointLight_virtualShadowCubeMapTex, 1);
    gpupVolumetricPointLight->setTextureToUniform(gpupVolumetricPointLight_shadowCubeMapIndirectionTex, 2);
    gpupVolumetricPointLight->setTextureToUniform(gpupVolumetricPointLight_halfDepthTex, 3);
    gpupVolumetricPointLight->setTextureToUniform(gpupVolumetricPointLight_noise3dTex, 4);
    gpupVolumetricPointLight->setTextureToUniform(gpupVolumetricPointLight_noise2dTex, 5);
    glUniformMatrix4fv(gpupVolumetricPointLight_invVP, 1, false, invVP.toFloatPtr());
    glUniform3fv(gpupVolumetricPointLight_viewPos, 1, lCameraPosition.toFloatPtr());
    glUniform4fv(gpupVolumetricPointLight_rtResolutionAndInv, 1, halfResRtSizeAndInv.toFloatPtr());
    glUniform2f(gpupVolumetricPointLight_linearDepthParam, linearDepthProjectionA, linearDepthProjectionB);
    glUniform4fv(gpupVolumetricPointLight_lightPosRad, 1, lightPosRad.toFloatPtr());
    glUniform4fv(gpupVolumetricPointLight_lightColor, 1, lightColor.toFloatPtr());
    glUniform2fv(gpupVolumetricPointLight_cubeShadowProjParam, 1, cubeShadowProjParam.toFloatPtr());
    Vector4 noiseTexturesSize(gu2dNoiseTextureSize,gu3dNoiseTextureSize,0.0f,0.0f);
    glUniform4fv(gpupVolumetricPointLight_noiseTexturesSize, 1, noiseTexturesSize.toFloatPtr());
    unsigned int intNoiseOffsetX = rand01() * 128.0f * float(useTemporalNoise);
    unsigned int intNoiseOffsetY = rand01() * 128.0f * float(useTemporalNoise);
    Vector2 noiseOffset = Vector2(intNoiseOffsetX,intNoiseOffsetY);
    glUniform2fv(gpupVolumetricPointLight_noiseOffset, 1, noiseOffset.toFloatPtr());
    renderScreenQuad(); // fullscreen pass for a sphere light, TODO: change that!!!
    GPUProgram::unbindGPUProgram();
    curVolumetricFBO->restoreViewPort();
    FrameBufferObject::disableRenderToColorDepth();
    
    
    
    
    
    
    //
    // Temporal upsampling  (extra pass to make it toggable, could be a shader permutation of course)
    //
    if(useTemporalUpsampling>0)
    {
        int prevVolumetricFboId = 1-currentVolumetricFboId;
        FrameBufferObject* prevVolumetricFBO = lpVolumetricFBO[prevVolumetricFboId];
        FrameBufferObject* prevVolumDepthFBO = lpHalfResDepthFBO[prevVolumetricFboId];
        
        if(useTemporalUpsampling==1)
        {
            //
            glActiveTextureARB(GL_TEXTURE0);
            prevVolumetricFBO->bindColorTexture(0);
            
            gpupTemporalBlend->bindGPUProgram();
            gpupTemporalBlend->setTextureToUniform(gpupTemporalBlend_texVolumetricPrevFrame, 0);
            
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            curVolumetricFBO->enableRenderToColorAndDepth(0);
            curVolumetricFBO->saveAndSetViewPort();
            renderScreenQuad();
            curVolumetricFBO->restoreViewPort();
            glDisable(GL_BLEND);
            FrameBufferObject::disableRenderToColorDepth();
        }
        else
        {
            //
            glActiveTextureARB(GL_TEXTURE2);
            curVolumDepthFBO->bindColorTexture(0);
            glActiveTextureARB(GL_TEXTURE1);
            prevVolumDepthFBO->bindColorTexture(0);
            glActiveTextureARB(GL_TEXTURE0);
            prevVolumetricFBO->bindColorTexture(0);
            
            // Set filtering to nearest
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            
            gpupTemporalUpsample->bindGPUProgram();
            gpupTemporalUpsample->setTextureToUniform(gpupTemporalUpsample_texVolumetricPrevFrame, 0);
            gpupTemporalUpsample->setTextureToUniform(gpupTemporalUpsample_texVolumDepthPrevFrame, 1);
            gpupTemporalUpsample->setTextureToUniform(gpupTemporalUpsample_texVolumDepthCurrFrame, 2);
            
            glUniformMatrix4fv(gpupTemporalUpsample_invVP, 1, false, invVP.toFloatPtr());
            glUniformMatrix4fv(gpupTemporalUpsample_prevInvVP, 1, false, gPrevInvVP.toFloatPtr());
            
            glUniform3fv(gpupTemporalUpsample_viewPos, 1, lCameraPosition.toFloatPtr());
            glUniform3fv(gpupTemporalUpsample_prevViewPos, 1, gPrevViewPos.toFloatPtr());
            
            glUniform2f(gpupTemporalUpsample_linearDepthParam, linearDepthProjectionA, linearDepthProjectionB);
            glUniformMatrix4fv(gpupTemporalUpsample_prevFrameVP, 1, false, gPrevVPMatrix.toFloatPtr());
            
            // Compute the maximum contribution of the new frame
            gfElapsedTime = gfElapsedTime;
            const float temporalUpSamplingBaseFrameTime = 1 / 30.0f;
            //const float timeBasedMaxContribution = 0.3f;
            const float timeBasedMaxContribution = max( maxCurrentFrameContribution * (max(0.001f,gfElapsedTime) / temporalUpSamplingBaseFrameTime), maxCurrentFrameContribution);
            glUniform1f(gpupTemporalUpsample_timeBasedMaxContribution, timeBasedMaxContribution);
            
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_SRC_ALPHA);
            curVolumetricFBO->enableRenderToColorAndDepth(0);
            curVolumetricFBO->saveAndSetViewPort();
            renderScreenQuad();
            curVolumetricFBO->restoreViewPort();
            FrameBufferObject::disableRenderToColorDepth();
            glDisable(GL_BLEND);
            
            // Restore filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            
            
            glActiveTextureARB(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTextureARB(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTextureARB(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    
    
    
    //
    // Bilateral blur
    //
    if(volumetricBlur==1)
    {
        // Simple blend
        lpTempHalfResColorFBO->enableRenderToColorAndDepth(0);
        lpTempHalfResColorFBO->saveAndSetViewPort();
        glActiveTextureARB(GL_TEXTURE1);
        curVolumDepthFBO->bindColorTexture(0);
        glActiveTextureARB(GL_TEXTURE0);
        curVolumetricFBO->bindColorTexture(0);
        gpupBilatDirBlur->bindGPUProgram();
        {
            Vector4 blurDirStep = Vector4(1.0f/float(curVolumetricFBO->getWidth()), 0.0f, 0.0f, 0.0f);
            glUniform4fv(gpupBilatDirBlur_blurDirStep, 1, blurDirStep.toFloatPtr());
            gpupBilatDirBlur->setTextureToUniform(gpupBilatDirBlur_textureSrc, 0);
        }
        renderScreenQuad();
        lpTempHalfResColorFBO->restoreViewPort();
        FrameBufferObject::disableRenderToColorDepth();
        
        curVolumetricFBO->enableRenderToColorAndDepth(0);
        curVolumetricFBO->saveAndSetViewPort();
        glActiveTextureARB(GL_TEXTURE1);
        curVolumDepthFBO->bindColorTexture(0);
        glActiveTextureARB(GL_TEXTURE0);
        lpTempHalfResColorFBO->bindColorTexture(0);
        gpupBilatDirBlur->bindGPUProgram();
        {
            Vector4 blurDirStep = Vector4(0.0f, 1.0f/float(curVolumetricFBO->getHeight()), 0.0f, 0.0f);
            glUniform4fv(gpupBilatDirBlur_blurDirStep, 1, blurDirStep.toFloatPtr());
            gpupBilatDirBlur->setTextureToUniform(gpupBilatDirBlur_textureSrc, 0);
        }
        renderScreenQuad();
        curVolumetricFBO->restoreViewPort();
    }
    else if (volumetricBlur==2)
    {
        // Temporal
        lpTempHalfResColorFBO->enableRenderToColorAndDepth(0);
        lpTempHalfResColorFBO->saveAndSetViewPort();
        glActiveTextureARB(GL_TEXTURE1);
        curVolumDepthFBO->bindColorTexture(0);
        glActiveTextureARB(GL_TEXTURE0);
        curVolumetricFBO->bindColorTexture(0);
        gpupBilatDirBlur2->bindGPUProgram();
        {
            Vector4 blurDirStep = Vector4(1.0f/float(curVolumetricFBO->getWidth()), 0.0f, 0.0f, 0.0f);
            glUniform4fv(gpupBilatDirBlur2_blurDirStep, 1, blurDirStep.toFloatPtr());
            gpupBilatDirBlur2->setTextureToUniform(gpupBilatDirBlur2_textureSrc, 0);
            gpupBilatDirBlur2->setTextureToUniform(gpupBilatDirBlur2_textureDepth, 1);
        }
        renderScreenQuad();
        lpTempHalfResColorFBO->restoreViewPort();
        FrameBufferObject::disableRenderToColorDepth();
        
        curVolumetricFBO->enableRenderToColorAndDepth(0);
        curVolumetricFBO->saveAndSetViewPort();
        glActiveTextureARB(GL_TEXTURE1);
        curVolumDepthFBO->bindColorTexture(0);
        glActiveTextureARB(GL_TEXTURE0);
        lpTempHalfResColorFBO->bindColorTexture(0);
        gpupBilatDirBlur2->bindGPUProgram();
        {
            Vector4 blurDirStep = Vector4(0.0f, 1.0f/float(curVolumetricFBO->getHeight()), 0.0f, 0.0f);
            glUniform4fv(gpupBilatDirBlur2_blurDirStep, 1, blurDirStep.toFloatPtr());
            gpupBilatDirBlur2->setTextureToUniform(gpupBilatDirBlur2_textureSrc, 0);
            gpupBilatDirBlur2->setTextureToUniform(gpupBilatDirBlur2_textureDepth, 1);
        }
        renderScreenQuad();
        curVolumetricFBO->restoreViewPort();
    }
    
    FrameBufferObject::disableRenderToColorDepth();
    
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_ALWAYS);
    
    //
    // COMPOSITE TO BACK BUFFER
    //
    glActiveTextureARB(GL_TEXTURE3);
    curVolumDepthFBO->bindColorTexture(0);
    glActiveTextureARB(GL_TEXTURE2);
    lpMainFBO->bindDepthTexture();
    glActiveTextureARB(GL_TEXTURE1);
    curVolumetricFBO->bindColorTexture(0);
    glActiveTextureARB(GL_TEXTURE0);
    lpMainFBO->bindColorTexture(0);
    gpupComposite->bindGPUProgram();
    gpupComposite->setTextureToUniform(gpupComposite_hdrLinTex, 0);
    gpupComposite->setTextureToUniform(gpupComposite_hdrVolTex, 1);
    gpupComposite->setTextureToUniform(gpupComposite_hiResDepthTex, 2);
    gpupComposite->setTextureToUniform(gpupComposite_halfDepthTex, 3);
    {
        const float upsampleOffsetX = 1.0f / float(curVolumetricFBO->getWidth());
        const float upsampleOffsetY = 1.0f / float(curVolumetricFBO->getHeight());
        Vector4 upsampleOffsets = Vector4(upsampleOffsetX,upsampleOffsetY,-upsampleOffsetX,-upsampleOffsetY);
        glUniform4fv(gpupComposite_upsampleOffsets, 1, upsampleOffsets.toFloatPtr());
    }
    glUniform4fv(gpupComposite_rtResolutionAndInv, 1, halfResRtSizeAndInv.toFloatPtr());
    renderScreenQuad();
    //glActiveTextureARB(GL_TEXTURE1);
    //glDisable(GL_TEXTURE_2D);
    GPUProgram::unbindGPUProgram();
    
    
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

#if 0
    // Texture debug
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, 256, 256);
    
    glActiveTextureARB(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    Texture::textureEnvMode(GL_REPLACE);
    //cubeShadow->bindVirtualColorMap();
    //cubeShadow->bindVirtualDepthMap();
    //lpVolumetricFBO[0]->bindColorTexture(0);
    //lpHalfResDepthFBO[0]->bindColorTexture(0);
    //lpTempHalfResColorFBO->bindColorTexture(0);
    renderScreenQuad();
    glPopAttrib();
#endif
    
    
    GLenum lGlError = glGetError();
    if(lGlError!=GL_NO_ERROR)
    {
        printf("OpenGL error %d.\n", lGlError);
    }
    
#ifdef D_GPU_PERF
    glFinish();
#endif // D_GPU_PERF
    glFinish();
    

    glFlush();
    //    glSwapAPPLE();
    glutSwapBuffers();
    glutPostRedisplay ();
    
    //if(useTemporalNoise) // test reprojection
    {
        gPrevVPMatrix = lViewProjectionMatrix;
        gPrevInvVP = invVP;
        gPrevViewPos = lCameraPosition;
    }
    
    int i=0;
    i=i;
}







void keyboardSpecial(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        gbUp = true;
        break;
    case GLUT_KEY_DOWN:
        gbDown = true;
        break;
    case GLUT_KEY_LEFT:
        gbLeft = true;
        break;
    case GLUT_KEY_RIGHT:
        gbRight = true;
        break;
    default:
        break;
    }
}

void keyboardSpecialUp(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        gbUp = false;
        break;
    case GLUT_KEY_DOWN:
        gbDown = false;
        break;
    case GLUT_KEY_LEFT:
        gbLeft = false;
        break;
    case GLUT_KEY_RIGHT:
        gbRight = false;
        break;
    default:
        break;
    }
}

void keyboardDown(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'z':
    case 'w':
        gCamera.setForward(true);
        break;
    case 'q':
    case 'a':
        gCamera.setLeft(true);
        break;
    case 's':
        gCamera.setBackward(true);
        break;
    case 'd':
        gCamera.setRight(true);
        break;
    case ' ':
        gCamera.setUp(true);
        break;
    case 'c':
        gCamera.setDown(true);
        break;

    case 't':
        useTemporalUpsampling++;
        if (useTemporalUpsampling>2)
        {
            useTemporalUpsampling = 0;
        }
        break;
    case 'n':
        useTemporalNoise = !useTemporalNoise;
        break;
    case 'l':
        movingLight = 1.0f - movingLight;
        break;
    case '-':
        maxCurrentFrameContribution += 0.05f;
        maxCurrentFrameContribution = min(maxCurrentFrameContribution,0.5f);
        break;
    case '+':
        maxCurrentFrameContribution -= 0.05f;
        maxCurrentFrameContribution = max(maxCurrentFrameContribution, 0.05f);
        break;


        /*case 'b':
            useBilateralUpsampling = !useBilateralUpsampling;
            break;*/

    case 'b':
        volumetricBlur++;
        if (volumetricBlur>2)
        {
            volumetricBlur = 0;
        }
        break;


    default:
        break;
    }
}

void keyboardUp(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'z':
    case 'w':
        gCamera.setForward(false);
        break;
    case 'q':
    case 'a':
        gCamera.setLeft(false);
        break;
    case 's':
        gCamera.setBackward(false);
        break;
    case 'd':
        gCamera.setRight(false);
        break;
    case ' ':
        gCamera.setUp(false);
        break;
    case 'c':
        gCamera.setDown(false);
        break;

    case 27:    // escap key
        releaseData();
        exit(0);
        break;

    default:
        break;
    }

}








void
releaseData()
{
    glDeleteTextures(1, &giRenderedTexture);
    glDeleteTextures(1, &giRedTexture);
    glDeleteTextures(1, &giDensityNoiseTexture);
    delete lpMainFBO;
    delete lpVolumetricFBO[0];
    delete lpVolumetricFBO[1];
    delete lpHalfResDepthFBO[0];
    delete lpHalfResDepthFBO[1];
    delete lpTempHalfResColorFBO;
    delete cubeShadow;
}

void
initializeOpenGL()
{
    glClearColor(0, 0, 0, 0);
    glClearDepth(1.0);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    
    
    glClientActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &giRenderedTexture);
    glBindTexture(GL_TEXTURE_2D, giRenderedTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GLubyte* gTexture = (GLubyte*) malloc(8*8*sizeof(GLubyte)*3);
    for (int s=0; s<8; s++)
    {
        for (int t=0; t<8; t++)
        {
            int addr = t*8+s;
            addr*=3;
            if(s%2)
            {
                gTexture[addr  ]=255*(t%5);
                gTexture[addr+1]=255*(t%5);//
                gTexture[addr+2]=255*(t%5);
            }
            else
            {
                gTexture[addr  ]=155*(1-t%2)+100;
                gTexture[addr+1]=155*(1-t%2)+100;
                gTexture[addr+2]=155*(1-t%2)+100;
            }
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 8, 8, 0, GL_RGB, GL_UNSIGNED_BYTE, gTexture);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    glGenTextures(1, &giRedTexture);
    glBindTexture(GL_TEXTURE_2D, giRedTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gTexture[0] = 0;
    gTexture[1] = 255;
    gTexture[2] = 0;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, gTexture);
    glBindTexture(GL_TEXTURE_2D, 0);
    free(gTexture);
    
    
    
    
    
    
    glClientActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &giDensityNoiseTexture);
    glBindTexture(GL_TEXTURE_3D, giDensityNoiseTexture);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    gu3dNoiseTextureSize = 16;
    GLubyte* densityNoiseTexture = (GLubyte*) malloc(gu3dNoiseTextureSize*gu3dNoiseTextureSize*gu3dNoiseTextureSize*sizeof(GLubyte));
    for (int s=0; s<gu3dNoiseTextureSize; s++)
    {
        for (int t=0; t<gu3dNoiseTextureSize; t++)
        {
            for (int r=0; r<gu3dNoiseTextureSize; r++)
            {
                int addr = r*gu3dNoiseTextureSize*gu3dNoiseTextureSize + t*gu3dNoiseTextureSize + s;
                float noise = rand01()*0.9 + 0.1;
                float ubNoise = GLubyte(noise*255.f);
                densityNoiseTexture[addr  ]=ubNoise;
            }
        }
    }
    glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE8, gu3dNoiseTextureSize, gu3dNoiseTextureSize, gu3dNoiseTextureSize
                 , 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, densityNoiseTexture);
    glBindTexture(GL_TEXTURE_3D, 0);
    free(densityNoiseTexture);

    
    
    
    
    glClientActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &giRaymarchNoiseTexture);
    glBindTexture(GL_TEXTURE_2D, giRaymarchNoiseTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gu2dNoiseTextureSize = 8; // bilateral blur, 1 center tap and 4 on each side
    GLubyte* gRayMarchNoiseTex = (GLubyte*) malloc(gu2dNoiseTextureSize*gu2dNoiseTextureSize*sizeof(GLubyte));
    for (int s=0; s<gu2dNoiseTextureSize; s++)
    {
        for (int t=0; t<gu2dNoiseTextureSize; t++)
        {
            int addr = t*gu2dNoiseTextureSize+s;
            float noise = rand01();
            float ubNoise = GLubyte(noise*255.0f);
            gRayMarchNoiseTex[addr]=ubNoise;
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, gu2dNoiseTextureSize, gu2dNoiseTextureSize, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, gRayMarchNoiseTex);
    glBindTexture(GL_TEXTURE_2D, 0);
    free(gRayMarchNoiseTex);
    
    // Add
    std::vector<std::string> glslIncludedHeaders;
    glslIncludedHeaders.push_back(std::string("./Resources/Shaders/defines.glsl"));
    glslIncludedHeaders.push_back(std::string("./Resources/Shaders/depthFunc.glsl"));
    
    std::vector<std::string> glslDefines;
    if(depthAsRGBA)
        glslDefines.push_back(std::string("D_DEPTH_PACKED_AS_RGBA"));
    
    
    GPUProgramManager& gpupMan = GPUProgramManager::getInstance();
    gpupMan.setIncludedHeaderFiles(glslIncludedHeaders);
    gpupMan.setDefines(glslDefines);
    
    if(!gpupMan.loadGPUProgramFromDisk("Shader1", "./Resources/Shaders/Shader.vp", "./Resources/Shaders/Shader.fp"))
    {
        printf("Gpu program not found!");
    }
    else
    {
        gpupSurface = GPUProgramManager::getInstance().getGPUProgram("Shader1");
        gpupSurface_lightPosRad = gpupSurface->getUniformLocation("lightPosRad");
        gpupSurface_lightColor = gpupSurface->getUniformLocation("lightColor");
        gpupSurface_objectMat = gpupSurface->getUniformLocation("objectMat");
    }
    
    if(!gpupMan.loadGPUProgramFromDisk("VolPL", "./Resources/Shaders/VolumetricPointLight.vp", "./Resources/Shaders/VolumetricPointLight.fp"))
    {
        printf("Gpu program not found!");
    }
    else
    {
        gpupVolumetricPointLight = GPUProgramManager::getInstance().getGPUProgram("VolPL");
        gpupVolumetricPointLight_viewDepthTex = gpupVolumetricPointLight->getUniformLocation("viewDepthTex");
        gpupVolumetricPointLight_shadowCubeMapIndirectionTex = gpupVolumetricPointLight->getUniformLocation("shadowCubeMapIndirectionTex");
        gpupVolumetricPointLight_virtualShadowCubeMapTex = gpupVolumetricPointLight->getUniformLocation("virtualShadowCubeMapTex");
        gpupVolumetricPointLight_halfDepthTex = gpupVolumetricPointLight->getUniformLocation("halfDepthTex");
        gpupVolumetricPointLight_noise3dTex = gpupVolumetricPointLight->getUniformLocation("noise3dTex");
        gpupVolumetricPointLight_noise2dTex = gpupVolumetricPointLight->getUniformLocation("noise2dTex");
        gpupVolumetricPointLight_invVP = gpupVolumetricPointLight->getUniformLocation("invVP");
        gpupVolumetricPointLight_viewPos = gpupVolumetricPointLight->getUniformLocation("viewPos");
        gpupVolumetricPointLight_linearDepthParam = gpupVolumetricPointLight->getUniformLocation("linearDepthParam");
        gpupVolumetricPointLight_lightPosRad = gpupVolumetricPointLight->getUniformLocation("lightPosRad");
        gpupVolumetricPointLight_lightColor = gpupVolumetricPointLight->getUniformLocation("lightColor");
        gpupVolumetricPointLight_cubeShadowProjParam = gpupVolumetricPointLight->getUniformLocation("cubeShadowProjParam");
        gpupVolumetricPointLight_noiseTexturesSize = gpupVolumetricPointLight->getUniformLocation("noiseTexturesSize");
        gpupVolumetricPointLight_noiseOffset = gpupVolumetricPointLight->getUniformLocation("noiseOffset");
        gpupVolumetricPointLight_rtResolutionAndInv = gpupVolumetricPointLight->getUniformLocation("rtResolutionAndInv");
    }
    
    if(!gpupMan.loadGPUProgramFromDisk("Comp", "./Resources/Shaders/composite.vp", "./Resources/Shaders/composite.fp"))
    {
        printf("Gpu program not found!");
    }
    else
    {
        gpupComposite = GPUProgramManager::getInstance().getGPUProgram("Comp");
        gpupComposite_hdrLinTex = gpupComposite->getUniformLocation("hdrLinTex");
        gpupComposite_hdrVolTex = gpupComposite->getUniformLocation("hdrVolTex");
        gpupComposite_hiResDepthTex = gpupComposite->getUniformLocation("hiResDepthTex");
        gpupComposite_halfDepthTex = gpupComposite->getUniformLocation("halfDepthTex");
        gpupComposite_upsampleOffsets = gpupComposite->getUniformLocation("upsampleOffsets");
        gpupComposite_rtResolutionAndInv = gpupComposite->getUniformLocation("rtResolutionAndInv");
    }
    
    if(!gpupMan.loadGPUProgramFromDisk("dds", "./Resources/Shaders/depthDownSample.vp", "./Resources/Shaders/depthDownSample.fp"))
    {
        printf("Gpu program not found!");
    }
    else
    {
        gpupDepthDownSample = GPUProgramManager::getInstance().getGPUProgram("dds");
        gpupDepthDownSample_hiResDepthTex = gpupDepthDownSample->getUniformLocation("hiResDepthTex");
        gpupDepthDownSample_offsets = gpupDepthDownSample->getUniformLocation("offsets");
    }
    
    if(!gpupMan.loadGPUProgramFromDisk("bdb", "./Resources/Shaders/bilatDirBlur.vp", "./Resources/Shaders/bilatDirBlur.fp"))
    {
        printf("Gpu program not found!");
    }
    else
    {
        gpupBilatDirBlur = GPUProgramManager::getInstance().getGPUProgram("bdb");
        gpupBilatDirBlur_blurDirStep = gpupBilatDirBlur->getUniformLocation("blurDirStep");
        gpupBilatDirBlur_textureSrc = gpupBilatDirBlur->getUniformLocation("textureSrc");
    }
    
    if(!gpupMan.loadGPUProgramFromDisk("bdb2", "./Resources/Shaders/bilatDirBlur2.vp", "./Resources/Shaders/bilatDirBlur2.fp"))
    {
        printf("Gpu program not found!");
    }
    else
    {
        gpupBilatDirBlur2 = GPUProgramManager::getInstance().getGPUProgram("bdb2");
        gpupBilatDirBlur2_blurDirStep = gpupBilatDirBlur2->getUniformLocation("blurDirStep");
        gpupBilatDirBlur2_textureSrc = gpupBilatDirBlur2->getUniformLocation("textureSrc");
        gpupBilatDirBlur2_textureDepth = gpupBilatDirBlur2->getUniformLocation("textureDepth");
    }
    
    if(!gpupMan.loadGPUProgramFromDisk("tempBlnd", "./Resources/Shaders/temporalBlend.vp", "./Resources/Shaders/temporalBlend.fp"))
    {
        printf("Gpu program not found!");
    }
    else
    {
        gpupTemporalBlend = GPUProgramManager::getInstance().getGPUProgram("tempBlnd");
        gpupTemporalBlend_texVolumetricPrevFrame = gpupTemporalBlend->getUniformLocation("texVolumetricPrevFrame");
    }
    if(!gpupMan.loadGPUProgramFromDisk("tempUps", "./Resources/Shaders/temporalUpsampling.vp", "./Resources/Shaders/temporalUpsampling.fp"))
    {
        printf("Gpu program not found!");
    }
    else
    {
        gpupTemporalUpsample = GPUProgramManager::getInstance().getGPUProgram("tempUps");
        gpupTemporalUpsample_texVolumetricPrevFrame = gpupTemporalUpsample->getUniformLocation("texVolumetricPrevFrame");
        gpupTemporalUpsample_texVolumDepthPrevFrame = gpupTemporalUpsample->getUniformLocation("texVolumDepthPrevFrame");
        gpupTemporalUpsample_texVolumDepthCurrFrame = gpupTemporalUpsample->getUniformLocation("texVolumDepthCurrFrame");
        
        gpupTemporalUpsample_viewPos = gpupTemporalUpsample->getUniformLocation("viewPos");
        gpupTemporalUpsample_prevViewPos = gpupTemporalUpsample->getUniformLocation("prevViewPos");
        gpupTemporalUpsample_linearDepthParam = gpupTemporalUpsample->getUniformLocation("linearDepthParam");
        gpupTemporalUpsample_invVP = gpupTemporalUpsample->getUniformLocation("invVP");
        gpupTemporalUpsample_prevInvVP = gpupTemporalUpsample->getUniformLocation("prevInvVP");
        gpupTemporalUpsample_prevFrameVP = gpupTemporalUpsample->getUniformLocation("prevFrameVP");
        
        gpupTemporalUpsample_timeBasedMaxContribution = gpupTemporalUpsample->getUniformLocation("timeBasedMaxContribution");
    }
    
    
    
    
    const unsigned int colorBufferInternalFormat = GL_RGBA16F_ARB;
    const unsigned int colorBufferInternalFormat32F = GL_RGBA16F_ARB;
    const unsigned int colorBufferInternalFormatRGBA8 = GL_RGBA8;
    const unsigned int colorBufferSWRAP = GL_CLAMP_TO_EDGE;
    const unsigned int colorBufferTWRAP = GL_CLAMP_TO_EDGE;
    const unsigned int colorBufferFilterLinear = GL_LINEAR;
    const unsigned int colorBufferFilterNearest = GL_NEAREST;
    
    const unsigned int halfResDepthFormat = depthAsRGBA ? colorBufferInternalFormatRGBA8 : colorBufferInternalFormat32F;
    
    lpMainFBO = new FrameBufferObject(giWindowViewPortWidth, giWindowViewPortHeight,
                                      1,
                                      &colorBufferInternalFormat,
                                      &colorBufferSWRAP,
                                      &colorBufferTWRAP,
                                      &colorBufferFilterNearest,
                                      &colorBufferFilterNearest,
                                      FBO_DepthBufferType_TEXTURE,
                                      GL_NEAREST,
                                      GL_NEAREST
                                      );
    int resolutionDiv = 2; // if this is changed then downsample will have to be adapted too (it only works for 2 today)
    for(int i=0; i<2; ++i)
    {
        lpVolumetricFBO[i] = new FrameBufferObject(giWindowViewPortWidth/resolutionDiv, giWindowViewPortHeight/resolutionDiv,
                                                   1,
                                                   &colorBufferInternalFormat,
                                                   &colorBufferSWRAP,
                                                   &colorBufferTWRAP,
                                                   &colorBufferFilterLinear,
                                                   &colorBufferFilterLinear,
                                                   //&colorBufferFilteringNearest,
                                                   //&colorBufferFilteringNearest,
                                                   FBO_DepthBufferType_NONE
                                                   );
        lpHalfResDepthFBO[i] = new FrameBufferObject(giWindowViewPortWidth/resolutionDiv, giWindowViewPortHeight/resolutionDiv,
                                                     1,
                                                     &halfResDepthFormat,
                                                     &colorBufferSWRAP,
                                                     &colorBufferTWRAP,
                                                     &colorBufferFilterNearest,
                                                     &colorBufferFilterNearest,
                                                     FBO_DepthBufferType_NONE,
                                                     GL_NEAREST,
                                                     GL_NEAREST
                                                     );
    }
    
    lpTempHalfResColorFBO = new FrameBufferObject(giWindowViewPortWidth/resolutionDiv, giWindowViewPortHeight/resolutionDiv,
                                                  1,
                                                  &colorBufferInternalFormatRGBA8,
                                                  &colorBufferSWRAP,
                                                  &colorBufferTWRAP,
                                                  &colorBufferFilterLinear,
                                                  &colorBufferFilterNearest,
                                                  FBO_DepthBufferType_NONE
                                                  );
    
    const unsigned int cubeShadowRes = 32;
    cubeShadow = new CubeBufferObject(cubeShadowRes, cubeShadowRes, false, GL_RGBA8);
}

int main (int argc, char *argv[])
{   

    glutInit (&argc, argv);

    glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (giWindowViewPortWidth, giWindowViewPortHeight);
    glutCreateWindow ("Software rasterizer");
    glutReshapeFunc (reshape);
    glutDisplayFunc (display);
    glutSpecialFunc(keyboardSpecial);
    glutSpecialUpFunc(keyboardSpecialUp);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        printf("Error: %s\n",glewGetErrorString(err));
    }

    printf("%s\n",argv[0]);
    
    printf("Shading language version: %s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    printf("GL_VENDOR    : %s\n",glGetString(GL_VENDOR));
    printf("GL_RENDERER  : %s\n",glGetString(GL_RENDERER));
    printf("GL_VERSION   : %s\n",glGetString(GL_VERSION));
    printf("GL_EXTENSIONS: %s\n",glGetString(GL_EXTENSIONS));
    

    gfElapsedTime = 0.0f;
    gPrevVPMatrix.identity();
    
    initializeOpenGL();
    
    gCamera.setTranslationSpeed(5.0f);
    gCamera.setRotationSpeed(2.0f, 2.0f);
    Vector3 lStartPos(0.0f,0.0f,0.0f);
    lStartPos.set(-4.0f, 1.0f,3.0f);
    gCamera.setPosition(lStartPos);
#define M_PI_4	0.78539816339744830962
    gCamera.setYaw(-M_PI_4*2);
#undef M_PI_4
    lastClock = clock();
    currentClock = clock();
    glutMainLoop ();
    return 0;
}


/*
 INTEL HD 3000 Extensions
 
 GL_ARB_transpose_matrix
 GL_ARB_vertex_program
 GL_ARB_vertex_blend
 GL_ARB_window_pos
 GL_ARB_shader_objects
 GL_ARB_vertex_shader
 GL_ARB_shading_language_100
 GL_EXT_multi_draw_arrays
 GL_EXT_clip_volume_hint
 GL_EXT_rescale_normal
 GL_EXT_draw_range_elements
 GL_EXT_fog_coord
 GL_EXT_gpu_program_parameters
 GL_EXT_geometry_shader4
 GL_EXT_transform_feedback
 GL_APPLE_client_storage
 GL_APPLE_specular_vector
 GL_APPLE_transform_hint
 GL_APPLE_packed_pixels
 GL_APPLE_fence
 GL_APPLE_vertex_array_object
 GL_APPLE_vertex_program_evaluators
 GL_APPLE_element_array
 GL_APPLE_flush_render
 GL_APPLE_aux_depth_stencil
 GL_NV_texgen_reflection
 GL_NV_light_max_exponent
 GL_IBM_rasterpos_clip
 GL_SGIS_generate_mipmap
 GL_ARB_point_parameters
 GL_ARB_texture_env_crossbar
 GL_ARB_texture_border_clamp
 GL_ARB_multitexture
 GL_ARB_texture_env_add
 GL_ARB_texture_cube_map
 GL_ARB_texture_env_dot3
 GL_ARB_multisample
 GL_ARB_texture_env_combine
 GL_ARB_texture_compression
 GL_ARB_texture_mirrored_repeat
 GL_ARB_shadow
 GL_ARB_depth_texture
 GL_ARB_fragment_program
 GL_ARB_fragment_program_shadow
 GL_ARB_fragment_shader
 GL_ARB_occlusion_query
 GL_ARB_point_sprite
 GL_ARB_texture_non_power_of_two
 GL_ARB_vertex_buffer_object
 GL_ARB_pixel_buffer_object
 GL_ARB_draw_buffers
 GL_ARB_color_buffer_float
 GL_ARB_half_float_vertex
 GL_ARB_texture_rg
 GL_ARB_texture_compression_rgtc
 GL_ARB_framebuffer_object
 GL_EXT_draw_buffers2
 GL_EXT_framebuffer_object
 GL_EXT_framebuffer_blit
 GL_EXT_framebuffer_multisample
 GL_EXT_texture_rectangle
 GL_ARB_texture_rectangle
 GL_EXT_texture_env_add
 GL_EXT_blend_color
 GL_EXT_blend_minmax
 GL_EXT_blend_subtract
 GL_EXT_texture_lod_bias
 GL_EXT_abgr
 GL_EXT_bgra
 GL_EXT_stencil_wrap
 GL_EXT_texture_filter_anisotropic
 GL_EXT_separate_specular_color
 GL_EXT_secondary_color
 GL_EXT_blend_func_separate
 GL_EXT_shadow_funcs
 GL_EXT_stencil_two_side
 GL_EXT_texture_compression_s3tc
 GL_EXT_texture_compression_dxt1
 GL_EXT_texture_sRGB
 GL_EXT_blend_equation_separate
 GL_EXT_packed_depth_stencil
 GL_EXT_framebuffer_sRGB
 GL_EXT_provoking_vertex
 GL_APPLE_flush_buffer_range
 GL_APPLE_ycbcr_422
 GL_APPLE_rgb_422
 GL_APPLE_vertex_array_range
 GL_APPLE_texture_range
 GL_APPLE_float_pixels
 GL_ATI_texture_float
 GL_ARB_texture_float
 GL_ARB_half_float_pixel
 GL_APPLE_pixel_buffer
 GL_APPLE_object_purgeable
 GL_NV_blend_square
 GL_NV_fog_distance
 GL_NV_conditional_render
 GL_ATI_texture_env_combine3 GL_ATI_separate_stencil
 GL_SGIS_texture_edge_clamp
 GL_SGIS_texture_lod
 GL_EXT_texture_array
 GL_EXT_vertex_array_bgra
 GL_ARB_instanced_arrays
 GL_ARB_depth_buffer_float
 */
