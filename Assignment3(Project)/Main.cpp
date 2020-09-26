#include "Source\Core\Window.h"
#include "Source\Core\Input\KeyboardState.h"
#include "Source\Core\Input\MouseState.h"
#include "Source\Core\Parsing\OBJFormat.h"
#include "Source\Core\Utilities\Utilities.h"
#include "Source\Core\FileManegment\FileListener.h"
#include "Source\Core\Physics\Transform3D.h"
//Graphics
#include "Source\Core\Graphics\API\IDeviceContext.h"
#include "Source\Core\Graphics\API\IShader.h"
#include "Source\Core\Graphics\API\IPipelineState.h"
#include "Source\Core\Graphics\API\IVertexBuffer.h"
#include "Source\Core\Graphics\API\IIndexBuffer.h"
#include "Source\Core\Graphics\API\IUniformBuffer.h"
#include "Source\Core\Graphics\API\IRenderTarget2D.h"
#include "Source\Core\Graphics\API\ITexture2D.h"
#include "Source\Core\Graphics\API\ITextureCube.h"
#include "Source\Core\Graphics\API\IFrameBuffer.h"
#include "Source\Core\Graphics\RenderManager.h"
#include "Source\Core\Graphics\Mesh.h"
#include "Source\Core\Graphics\Camera2D.h"
#include "Source\Core\Graphics\Camera3D.h"
#include "Source\Core\Graphics\MeshFactory.h"
#include "Source\Core\Graphics\API\IDepthBuffer.h"
#include "Source\Core\Graphics\API\IRenderTargetDepth.h"
#include "Source\Core\Graphics\API\IInputLayout.h"
#include "Source\Core\Graphics\DirectionalLight.h"
#include "Source\Core\Memory\EngineObject.h"
#include "Source\Core\Graphics\Terrain.h"
#include "Source\Core\Graphics\PBRMaterial.h"

#include <iostream>
#include <Windows.h>

//Biggest factor to performance
#define BACKBUFFER_WIDTH 2560
#define BACKBUFFER_HEIGHT 1440
//With smaller sizes small speculars are wierd
#define BLUR_BUFFER_WIDTH (uint16)(BACKBUFFER_WIDTH * (1.0f / 2.0f))
#define BLUR_BUFFER_HEIGHT (uint16)(BACKBUFFER_HEIGHT * (1.0f / 2.0f))
//Shadow map size does not seem to the bottleneck in this application
#define SHADOW_WIDTH 2048
#define SHADOW_HEIGHT SHADOW_WIDTH
#define SPHERES_X 8
#define SPHERES_Y SPHERES_X

#define D3D11

namespace Graphics
{
	///////////////////
	//RENDERFUNCTIONS//
	///////////////////

	void Render(const Mesh& mesh, const IDeviceContext* const context, const IPipelineState* const state, 
		IUniformBuffer* const transformBuffer, const Transform3D& transform)
	{
		//Transform data
		Transform3D::Data data;

		//Bind mesh
		mesh.Bind();

		//Update the transform
		data.transformMatrix = transform.GetTransformMatrix();
		data.transformMatrix.Transpose();
		//Update the normal
		data.normalMatrix = transform.GetRotationMatrix();
		data.normalMatrix.Transpose();

		//Update buffer
		transformBuffer->Update(&data, 1, 0);

		//Draw
		context->DrawIndexed(state, mesh.GetIndexCount(), 0);

		//Unbind mesh
		mesh.Unbind();
	}

	void Render(const Mesh& mesh, const IDeviceContext* const context, IUniformBuffer* const transformBuffer,
		const MaterialInstance* material, const Transform3D& transform)
	{
		//Bind material
		material->Bind();

		//Render
		Render(mesh, context, material->GetPipelineState(), transformBuffer, transform);

		//Unbind material
		material->Unbind();
	}

	void RenderScreenQuad(const ITexture2D* const finaltarget, const ITexture2D* const glowtarget, const IDeviceContext* const context, 
		const IPipelineState* const state)
	{
		//Bind pipelinestate
		state->Bind();

		//Bind rendertargettexture
		finaltarget->BindPS(0);
		glowtarget->BindPS(1);
		//Render to screen and clear it
		context->MakeWindowTarget();
		context->ClearWindow(ColorF::BLACK);

		//Draw
		context->Draw(state, 4, 0);
		context->Present();
		//Unbind texture
		finaltarget->UnbindPS();
		glowtarget->UnbindPS();

		state->Unbind();
	}

	void ShadingStage(const ITexture2D* const albedo, const ITexture2D* const normal, const ITexture2D* const position,
		const ITexture2D* const material, const ITexture2D* const shadowMap, const IFrameBuffer* const frameBuffer, 
		const IDeviceContext* const context, const IPipelineState* const state)
	{
		//Bind state
		state->Bind();

		//Bind textures
		albedo->BindPS(0);
		normal->BindPS(1);
		position->BindPS(2);
		material->BindPS(3);
		shadowMap->BindPS(4);

		//Bind framebuffer - all pixels are always rendered to no need to clear
		frameBuffer->Bind();

		//Draw
		context->Draw(state, 4, 0);

		//Unbind frameBuffer
		frameBuffer->Unbind();

		//Unbind textures
		albedo->UnbindPS();
		normal->UnbindPS();
		position->UnbindPS();
		material->UnbindPS();
		shadowMap->UnbindPS();

		//Unbind state
		state->Unbind();
	}
}

////////
//MAIN//
////////

int main()
{
	//RUNTIME DEBUGFLAGS
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

	using namespace Graphics;
	using namespace Math;

	//RENDERMANAGER
	//Rendermanager to keep track of different texturesizes
	RenderSettings renderSettings;
	renderSettings.SetResolution(BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT);


	//WINDOW
#ifdef D3D11
	Window window(L"3D-Programming - Assignment 3 (Project) - D3D11");
	window.CreateContext(GRAPHICS_API_D3D11, renderSettings);
#else
	Window window(L"3D-Programming - Assignment 3 (Project) - OpenGL");
	window.CreateContext(GRAPHICS_API_OPENGL, renderSettings);
#endif
	const IDeviceContext* context = window.GetDeviceContext();
	


	//FILELISTENER
	FileListener fileListener("Shaders/");
	


	//SHADERS
	EngineObject<IShader> deferredPassVS = IShader::Create(context, "DeferredPassVS", "Shaders/", &fileListener, SHADER_VERTEX);
	EngineObject<IShader> geometry = IShader::Create(context, "BackfaceCullingGS", "Shaders/", &fileListener, SHADER_GEOMETRY);
	EngineObject<IShader> deferredPassPS = IShader::Create(context, "DeferredPassPS", "Shaders/", &fileListener, SHADER_PIXEL);

	EngineObject<IShader> terrainDeferredPassVS = IShader::Create(context, "TerrainVS", "Shaders/", &fileListener, SHADER_VERTEX);
	EngineObject<IShader> terrainDeferredPassHS = IShader::Create(context, "TerrainHS", "Shaders/", &fileListener, SHADER_HULL);
	EngineObject<IShader> terrainDeferredPassDS = IShader::Create(context, "TerrainDS", "Shaders/", &fileListener, SHADER_DOMAIN);
	EngineObject<IShader> terrainDeferredPassPS = IShader::Create(context, "TerrainPS", "Shaders/", &fileListener, SHADER_PIXEL);

	EngineObject<IShader> quadVS = IShader::Create(context, "FullscreenQuadVS", "Shaders/", &fileListener, SHADER_VERTEX);
	
	EngineObject<IShader> shadowPassVS = IShader::Create(context, "ShadowPassVS", "Shaders/", &fileListener, SHADER_VERTEX);
	EngineObject<IShader> shadowPassPS = IShader::Create(context, "ShadowPassPS", "Shaders/", &fileListener, SHADER_PIXEL);

	EngineObject<IShader> planePS = IShader::Create(context, "HDRGammaPassPS", "Shaders/", &fileListener, SHADER_PIXEL);
	EngineObject<IShader> pbrLightPassPS = IShader::Create(context, "PBRLightPassPS", "Shaders/", &fileListener, SHADER_PIXEL);
	EngineObject<IShader> glowPS = IShader::Create(context, "GlowPS", "Shaders/", &fileListener, SHADER_PIXEL);



	//INPUTLAYOUTS
	Attribute attributes[] = 
	{
		{ ATTRIBUTETYPE_VECTOR3F, "POSITION", 0 },
		{ ATTRIBUTETYPE_VECTOR3F, "NORMAL", 0 },
		{ ATTRIBUTETYPE_VECTOR3F, "TANGENT", 0 },
		{ ATTRIBUTETYPE_VECTOR2F, "TEXCOORD", 0 },
	};
	Attribute attributes2[] =
	{
		{ ATTRIBUTETYPE_VECTOR3F, "POSITION", 0 },
		{ ATTRIBUTETYPE_VECTOR2F, "TEXCOORD", 0 },
	};
	EngineObject<IInputLayout> inputLayout = IInputLayout::Create(context, attributes, 4);
	EngineObject<IInputLayout> inputLayoutTerrain = IInputLayout::Create(context, attributes2, 2);



	//FRAMEBUFFERS
	//Create G-Buffers
	//Make sure they are filtered with point/nearest
	SamplerSettings settings;
	settings.filter = FILTER_POINT;
	EngineObject<IRenderTarget2D> albedoBuffer = IRenderTarget2D::Create(context, renderSettings.GetResolution().x,
		renderSettings.GetResolution().y, TEXTUREFORMAT_RGBA_BYTE);
	albedoBuffer->GetTexture()->SetSamplerSettings(settings);
	EngineObject<IRenderTarget2D> normalBuffer = IRenderTarget2D::Create(context, renderSettings.GetResolution().x,
		renderSettings.GetResolution().y, TEXTUREFORMAT_RGBA_BYTE);
	normalBuffer->GetTexture()->SetSamplerSettings(settings);
	EngineObject<IRenderTarget2D> positionBuffer = IRenderTarget2D::Create(context, renderSettings.GetResolution().x,
		renderSettings.GetResolution().y, TEXTUREFORMAT_RGBA_FLOAT16);
	positionBuffer->GetTexture()->SetSamplerSettings(settings);
	EngineObject<IRenderTarget2D> aoTarget = IRenderTarget2D::Create(context, renderSettings.GetResolution().x,
		renderSettings.GetResolution().y, TEXTUREFORMAT_R_BYTE);
	aoTarget->GetTexture()->SetSamplerSettings(settings);
	//Create DepthBuffer
	EngineObject<IDepthBuffer> depthBuffer = IDepthBuffer::Create(context, renderSettings.GetResolution().x,
		renderSettings.GetResolution().y, TEXTUREFORMAT_DEPTH16);
	//Create finaltarget
	EngineObject<IRenderTarget2D> finalTarget = IRenderTarget2D::Create(context, renderSettings.GetResolution().x,
		renderSettings.GetResolution().y, TEXTUREFORMAT_RGBA_FLOAT16);
	//Create ShadowMap
	EngineObject<IRenderTargetDepth> shadowMap = IRenderTargetDepth::Create(context, SHADOW_WIDTH, SHADOW_HEIGHT, TEXTUREFORMAT_DEPTH16);
	settings.filter = FILTER_LINEAR;
	settings.clamp = CLAMP_BORDER;
	settings.borderColor = ColorF::WHITE;
	shadowMap->GetTexture()->SetSamplerSettings(settings);

	IRenderTarget2D* renderTargets[] =
	{
		albedoBuffer,
		normalBuffer,
		positionBuffer,
		aoTarget
	};

	EngineObject<IRenderTarget2D> glowBuffers[2];
	glowBuffers[0] = IRenderTarget2D::Create(context, BLUR_BUFFER_WIDTH, BLUR_BUFFER_HEIGHT, TEXTUREFORMAT_RGBA_FLOAT16);
	glowBuffers[1] = IRenderTarget2D::Create(context, BLUR_BUFFER_WIDTH, BLUR_BUFFER_HEIGHT, TEXTUREFORMAT_RGBA_FLOAT16);

	//Create framebuffers
	EngineObject<IFrameBuffer> frameBuffer = IFrameBuffer::Create(context, renderTargets, 4);
	frameBuffer->AttachDepthBuffer(depthBuffer);
	EngineObject<IFrameBuffer> shadowBuffer = IFrameBuffer::Create(context, nullptr, 0);
	shadowBuffer->AttachDepthBuffer(shadowMap);
	EngineObject<IFrameBuffer> finalBuffer = IFrameBuffer::Create(context, &finalTarget, 1);
	EngineObject<IFrameBuffer> glowFrameBuffers[2];
	glowFrameBuffers[0] = IFrameBuffer::Create(context, &glowBuffers[0], 1);
	glowFrameBuffers[1] = IFrameBuffer::Create(context, &glowBuffers[1], 1);


	//TEXTURES
	//Ground Material
	SamplerSettings terrainSetting;
	terrainSetting.clamp = CLAMP_REPEAT;
	EngineObject<ITexture2D> albedo = ITexture2D::Create(context, "albedo.tga", "Textures/Sand/", TEXTUREFORMAT_RGBA_BYTE, true);
	albedo->SetSamplerSettings(terrainSetting);
	EngineObject<ITexture2D> roughness = ITexture2D::Create(context, "roughness.tga", "Textures/Sand/", TEXTUREFORMAT_R_BYTE, true);
	roughness->SetSamplerSettings(terrainSetting);
	EngineObject<ITexture2D> ao = ITexture2D::Create(context, "ao.tga", "Textures/Sand/", TEXTUREFORMAT_R_BYTE, true);
	ao->SetSamplerSettings(terrainSetting);
	EngineObject<ITexture2D> displacementMap = ITexture2D::Create(context, "height.tga", "Textures/Sand/", TEXTUREFORMAT_R_BYTE, true);
	displacementMap->SetSamplerSettings(terrainSetting);
	EngineObject<ITexture2D> normalMap = ITexture2D::Create(context, "normal.tga", "Textures/Sand/", TEXTUREFORMAT_RGBA_BYTE, true);
	normalMap->SetSamplerSettings(terrainSetting);

	//Brick material
	EngineObject<ITexture2D> stoneAl = ITexture2D::Create(context, "albedo.png", "Textures/Brick/", TEXTUREFORMAT_RGBA_BYTE, true);
	EngineObject<ITexture2D> stoneR = ITexture2D::Create(context, "roughness.png", "Textures/Brick/", TEXTUREFORMAT_R_BYTE, true);
	EngineObject<ITexture2D> stoneH = ITexture2D::Create(context, "height.png", "Textures/Brick/", TEXTUREFORMAT_R_BYTE, false);
	EngineObject<ITexture2D> stoneAO = ITexture2D::Create(context, "ao.png", "Textures/Brick/", TEXTUREFORMAT_R_BYTE, true);
	EngineObject<ITexture2D> stoneM = ITexture2D::Create(context, "metal.png", "Textures/Brick/", TEXTUREFORMAT_R_BYTE, true);
	EngineObject<ITexture2D> stoneN = ITexture2D::Create(context, "normal.png", "Textures/Brick/", TEXTUREFORMAT_RGBA_BYTE, true);

	//IntegrationMap for PBR
	SamplerSettings integrationSetting;
	integrationSetting.filter = FILTER_POINT;
	EngineObject<ITexture2D> integrationMap = ITexture2D::Create(context, "integrationMap.png", "Textures/", TEXTUREFORMAT_RGBA_BYTE, false);
	integrationMap->SetSamplerSettings(integrationSetting);

	//RadianceMap for PBR - HDR should be used files, only uses bytes since I want to avoid using float32 since it is a very slow format
	std::string crosses[] =
	{
		"arches_0.png",
		"arches_1.png",
		"arches_2.png",
		"arches_3.png",
		"arches_4.png",
		"arches_5.png",
		"arches_6.png",
		"arches_7.png",
		"arches_8.png",
		"arches_9.png",
		"arches_10.png",
	};
	SamplerSettings cubeSetting;
	EngineObject<ITextureCube> radianceMap = ITextureCube::Create(context, crosses, 11, "Textures/Arches/", TEXTURECROSS_VERTICAL, TEXTUREFORMAT_RGBA_BYTE);

	//IrradianceMap for PBR - HDR files should be used, only uses bytes since I want to avoid using float32 since it is a very slow format
	std::string irrad = "arches_irrad.png";
	EngineObject<ITextureCube> irradianceMap = ITextureCube::Create(context, &irrad, 1, "Textures/Arches/", TEXTURECROSS_VERTICAL, TEXTUREFORMAT_RGBA_BYTE);



	//DEPTHSTATE
	DepthState depthState;
	depthState.depthTest = false;



	//RASTERIZERSTATE
	RasterizerState rasterizerState;
	//Meshes
	rasterizerState.wireframe = false;
	rasterizerState.cull = CULLMODE_NONE;



	//VIEWPORT
	Viewport viewport;
	viewport.width = renderSettings.GetResolution().x;
	viewport.height = renderSettings.GetResolution().y;
	viewport.x = 0;
	viewport.y = 0;



	//MATERIALS
	//Meshes
	PBRMaterial meshMaterial(context);
	meshMaterial.SetInputLayout(inputLayout);
	meshMaterial.SetTopology(TOPOLOGY_TRIANGLELIST);
	meshMaterial.SetVertexShader(deferredPassVS);
	meshMaterial.SetGeomatryShader(geometry);
	meshMaterial.SetPixelShader(deferredPassPS);
	meshMaterial.SetViewport(viewport);
	meshMaterial.SetRasterizerState(rasterizerState);
	//Terrain
	PBRMaterial terrainMat(context);
	terrainMat.SetInputLayout(inputLayoutTerrain);
	terrainMat.SetTopology(TOPOLOGY_PATCH_3_POINTS);
	terrainMat.SetVertexShader(terrainDeferredPassVS);
	terrainMat.SetHullShader(terrainDeferredPassHS);
	terrainMat.SetDomainShader(terrainDeferredPassDS);
	terrainMat.SetPixelShader(terrainDeferredPassPS);
	terrainMat.SetViewport(viewport);
	terrainMat.SetRasterizerState(rasterizerState);

	//SPHERE MATERIALS
	PBRMaterialInstance** sphereMaterials = new PBRMaterialInstance*[SPHERES_X * SPHERES_Y];
	for (int32 i = 0; i < SPHERES_X * SPHERES_Y; i++)
	{
		//Create material instances
		sphereMaterials[i] = dynamic_cast<PBRMaterialInstance*>(meshMaterial.CreateInstance());		
		
		if (sphereMaterials[i] == nullptr)
			continue;

		//Setup material
		sphereMaterials[i]->SetAlbedo(ColorF::SOFTYELLOW);
		sphereMaterials[i]->SetMetallic(0.1f + ((int32)(i / SPHERES_Y) * (1.0f / SPHERES_Y)));
		sphereMaterials[i]->SetRoughness(0.1f + ((int32)(i % SPHERES_X) * (1.0f / SPHERES_X)));
	}

	//CUBEMATERIAL
	PBRMaterialInstance* stoneMaterial = dynamic_cast<PBRMaterialInstance*>(meshMaterial.CreateInstance());
	stoneMaterial->SetAlbedo(stoneAl);
	stoneMaterial->SetAmbientOcclusion(stoneAO);
	stoneMaterial->SetHeightMap(stoneH);
	stoneMaterial->SetMetallic(stoneM);
	stoneMaterial->SetNormalMap(stoneN);
	stoneMaterial->SetRoughness(stoneR);
	stoneMaterial->SetParallaxScale(0.025f);
	stoneMaterial->SetParallaxLayersMin(4.0f);
	stoneMaterial->SetParallaxLayersMax(8.0f);

	//TERRAIN MATERIAL
	PBRMaterialInstance* terrainMatInstance = dynamic_cast<PBRMaterialInstance*>(terrainMat.CreateInstance());
	terrainMatInstance->SetAlbedo(albedo);
	terrainMatInstance->SetRoughness(roughness);
	terrainMatInstance->SetNormalMap(normalMap);
	terrainMatInstance->SetDisplacementMap(displacementMap);
	terrainMatInstance->SetAmbientOcclusion(ao);



	//PIPELINESTATES
	EngineObject<IPipelineState> screenState = IPipelineState::Create(context);
	screenState->SetVertexShader(quadVS);
	screenState->SetPixelShader(planePS);
	screenState->SetTopology(TOPOLOGY_TRIANGLESTRIP);
	screenState->SetDepthState(depthState);

	//Rendering to windowtarget
	rasterizerState.wireframe = false;
	rasterizerState.depthClip = false;
	screenState->SetRasterizerState(rasterizerState);

	EngineObject<IPipelineState> shadeState = IPipelineState::Create(context);
	shadeState->SetVertexShader(quadVS);
	shadeState->SetPixelShader(pbrLightPassPS);
	shadeState->SetTopology(TOPOLOGY_TRIANGLESTRIP);
	shadeState->SetViewport(viewport);
	shadeState->SetDepthState(depthState);
	shadeState->SetRasterizerState(rasterizerState);

	//Viewport for Glowblur
	viewport.width = BLUR_BUFFER_WIDTH;
	viewport.height = BLUR_BUFFER_HEIGHT;

	EngineObject<IPipelineState> glowState = IPipelineState::Create(context);
	glowState->SetVertexShader(quadVS);
	glowState->SetPixelShader(glowPS);
	glowState->SetTopology(TOPOLOGY_TRIANGLESTRIP);
	glowState->SetViewport(viewport);
	glowState->SetDepthState(depthState);
	glowState->SetRasterizerState(rasterizerState);

	//States for shadowmapping
	viewport.width = SHADOW_WIDTH;
	viewport.height = SHADOW_HEIGHT;
	
	depthState.depthTest = true;

	rasterizerState.cull = CULLMODE_BACK;
	rasterizerState.depthClip = true;

	EngineObject<IPipelineState> shadowState = IPipelineState::Create(context);
	shadowState->SetVertexShader(shadowPassVS);
	shadowState->SetPixelShader(shadowPassPS);
	shadowState->SetInputLayout(inputLayout);
	shadowState->SetTopology(TOPOLOGY_TRIANGLELIST);
	shadowState->SetViewport(viewport);
	shadowState->SetDepthState(depthState);
	shadowState->SetRasterizerState(rasterizerState);
	


	//CREATE GEOMETRY
	//Create terrain
	Terrain terrain(context, "heightTerrain.png", "Textures/", 80.0f, 80, 4.0f, 2.0f, 0.05f, 4.0f, 48.0f);
	//Create sphere data
	GeometryData sphereData = Meshfactory::CreateSphere(3);
	Mesh mesh(context, sphereData);
	//Create cube
	GeometryData cubeData = Meshfactory::CreateCube();
	Mesh cube(context, cubeData);



	//CAMERA
	Camera3D camera;
	camera.SetFov(PI_F / 4.0f);
	camera.SetPosition(Vector3(-15.0f, 17.5f, -15.0f));
	camera.SetRotation(Vector3(-PI_F / 6.0f, -PI_F / 4.0f, 0.0f));
	camera.SetAspectRatio(((float)BACKBUFFER_WIDTH / (float)BACKBUFFER_HEIGHT));
	camera.SetFarPlane(80.0f);
	camera.SetNearPlane(0.1f);
	camera.UpdateMatrices();

	Camera::Data cameraBuffer;
	cameraBuffer.matrix = camera.GetMatrix();
	cameraBuffer.matrix.Transpose();
	cameraBuffer.position = camera.GetPosition();
	cameraBuffer.rotationMatrix = Matrix4::Rotation(camera.GetRotation().x, camera.GetRotation().y, camera.GetRotation().z);
	cameraBuffer.rotationMatrix.Transpose();



	//LIGHT
	DirectionalLight light;
	light.SetViewport(30.0f, 30.0f);
	light.SetFarPlane(200.0);
	light.SetNearPlane(1.0f);
	light.SetRotation(Vector3(0.0f, 0.0f, 0.0f));
	light.SetColor(ColorF::WARMWHITE);
	light.SetIBLIntensity(2.0f);
	light.SetIntensity(25.0f);

	DirectionalLight::Data lightData;
	lightData.color = ColorF::WARMWHITE;
	lightData.intensity = 25.0f;
	lightData.direction = light.GetDirection();
	lightData.lightMatrix = light.GetMatrix();
	lightData.lightMatrix.Transpose();
	lightData.iblIntensity = 2.0f;



	//TRANSFORMS
	//Transform cubes
	Transform3D cubeTransform;
	cubeTransform.SetPosition(Vector3(0.0f, 2.5f, 0.0f));
	cubeTransform.SetScale(1.5f);
	//Transform spheres
	Transform3D sphereTransforms[SPHERES_X * SPHERES_Y];
	for (int i = 0; i < SPHERES_X * SPHERES_Y; i++)
	{
		sphereTransforms[i].SetPosition(Vector3(
			-4.375f + (1.25f * (int32)(i % SPHERES_X)),
			3.5f + (1.25f * (int32)(i / SPHERES_Y)),
			5.0f));
	}




	//GLOWDATA
	struct GlowData
	{
		float mode = 0.0f;
		float brighness = 3.0f;
		Math::Vector2 padding;
	};

	GlowData glowData;
	glowData.brighness = 3.0f;
	glowData.mode = 0.0f;


	
	//UNIFORMBUFFERS
	EngineObject<IUniformBuffer> transformBuffer = IUniformBuffer::Create(context, nullptr, 0, sizeof(Transform3D::Data), 1);
	EngineObject<IUniformBuffer> cameraUniformBuffer = IUniformBuffer::Create(context, &cameraBuffer, 1, sizeof(Camera::Data), 1);
	EngineObject<IUniformBuffer> lightBuffer = IUniformBuffer::Create(context, &lightData, 1, sizeof(DirectionalLight::Data), 1);
	EngineObject<IUniformBuffer> glowUniform = IUniformBuffer::Create(context, &glowData, 1, sizeof(GlowData), 1);



	//Variables to control camera
	float camSpeed = 1.0f;
	//Deltatime
	float dt = 0.0f;
	//Clock for deltatime
	Clock clock;
	//Bloom or not
	bool bloom = true;
	//TODO: Remove from main
	//Message
	MSG msg = { 0 };
	//Input states
	KeyboardState keyboardState;
	KeyboardState lastKeyboardState;
	MouseState mouseState;
	MouseState lastMouseState;
	//Show window
	window.Show();
	//Buffer that is used to output to the window
	const ITexture2D* buffer = finalTarget->GetTexture();	
	

	//MAIN LOOP
	while (window.IsActive())
	{
		//Let timers tick
		clock.Tick();

		//Handle all the callbacks for updated files
		fileListener.HandleChanges();

		//Update input
		lastKeyboardState = keyboardState;
		lastMouseState = mouseState;
		if (window.HasFocus())
		{
			keyboardState = KeyboardState::GetCurrentState();
			mouseState = MouseState::GetCurrentState(window);
		}
		else
		{
			keyboardState = KeyboardState();
			mouseState = MouseState();
		}

		//Handle OS-Messages
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//UPDATE
		//Driver crashes when wireframemode is turned on for meshes, do not know why (TimeOut)
		//TODO: FIX

		if (keyboardState.IsKeyDown(KEY_2) && lastKeyboardState.IsKeyUp(KEY_2))
		{
			bloom = !bloom;

			//Because of OPENGL
			glowFrameBuffers[0]->Bind();
			glowFrameBuffers[0]->ClearColor(ColorF::BLACK);
			glowFrameBuffers[0]->Unbind();
			glowFrameBuffers[1]->Bind();
			glowFrameBuffers[1]->ClearColor(ColorF::BLACK);
			glowFrameBuffers[1]->Unbind();
		}

		dt = (float)clock.DeltaTime().AsSeconds();

		//Set cameraspeed
		Vector3 cameraSpeed;
		if (keyboardState.IsKeyDown(KEY_SHIFT))
			camSpeed = 5.0f;
		else
			camSpeed = 1.0f;

		if (keyboardState.IsKeyDown(KEY_A))
			cameraSpeed.x -= camSpeed * dt;
		if (keyboardState.IsKeyDown(KEY_D))
			cameraSpeed.x += camSpeed * dt;

		if (keyboardState.IsKeyDown(KEY_W))
			cameraSpeed.z += camSpeed * dt;
		if (keyboardState.IsKeyDown(KEY_S))
			cameraSpeed.z -= camSpeed * dt;

		if (keyboardState.IsKeyDown(KEY_UP))
			cameraSpeed.y += camSpeed * dt;
		if (keyboardState.IsKeyDown(KEY_DOWN))
			cameraSpeed.y -= camSpeed * dt;

		//Rotate camera
		if (mouseState.IsButtonDown(BUTTON_MOUSE_RIGHT) && window.HasFocus())
		{
			Point diff = (window.GetSize() / 2) - mouseState.GetPosition();
			camera.Rotate(Vector3(to_radians_f((float)diff.y), to_radians_f((float)diff.x), 0.0f));

			MouseState::SetPosition(window.GetSize() / 2, window);
		}

		//Move camera
		if (cameraSpeed.LengthSqrd() > 0.0f)
		{
			//Rotate forward and right
			Vector3 forward = Vector3::FORWARD * camera.GetRotationMatrix();
			Vector3 right = Vector3::RIGHT * camera.GetRotationMatrix();
			Vector3 up = Vector3::UP * camera.GetRotationMatrix();

			camera.Translate((forward * cameraSpeed.z) + (right * cameraSpeed.x) + (up * cameraSpeed.y));
		}

		//Update camera
		if (camera.UpdateMatrices())
		{
			cameraBuffer.matrix = camera.GetMatrix();
			cameraBuffer.matrix.Transpose();
			cameraBuffer.position = camera.GetPosition();
			cameraBuffer.rotationMatrix = Matrix4::Rotation(camera.GetRotation().x, camera.GetRotation().y, camera.GetRotation().z);
			cameraBuffer.rotationMatrix.Transpose();

			cameraUniformBuffer->Update(&cameraBuffer, 1, 0);
		}

		//Move light
		if (keyboardState.IsKeyDown(KEY_I))
			light.SetRotation(light.GetRotation() + Vector3(-(PI_F / 10.0f) * dt));
		if (keyboardState.IsKeyDown(KEY_K))
			light.SetRotation(light.GetRotation() + Vector3((PI_F / 10.0f) * dt));

		//Move light
		if (keyboardState.IsKeyDown(KEY_LEFT))
			light.SetRotation(light.GetRotation() + Vector3(0.0f, 0.0f, -(PI_F / 10.0f) * dt));
		if (keyboardState.IsKeyDown(KEY_RIGHT))
			light.SetRotation(light.GetRotation() + Vector3(0.0f, 0.0f, (PI_F / 10.0f) * dt));

		//Update light
		if (light.UpdateMatrices())
		{			
			lightData.lightMatrix = light.GetMatrix();
			lightData.lightMatrix.Transpose();
			lightData.direction = light.GetDirection();
			lightBuffer->Update(&lightData, 1, 0);
		}

		//Update cube rotation
		cubeTransform.Rotate(Vector3(0.0f, PI_F / 10 * dt, 0.0f));




		//RENDER
		//Bind buffers constant for all stages
		transformBuffer->BindVS(1);

		//SHADOW PASS
		//TODO: Move to renderer
		//Bind pipelinestate
		shadowState->Bind();
		//Bind and clear framebuffer
		shadowBuffer->Bind();
		shadowBuffer->ClearDepth(1.0f);
		//Bind buffers
		lightBuffer->BindVS(0);
		//Do not render terrain since tesselation makes shadows look weird
		//Render spheres
		for (int i = 0; i < SPHERES_X * SPHERES_Y; i++)
			Render(mesh, context, shadowState, transformBuffer, sphereTransforms[i]);
		//Cube
		Render(cube, context, shadowState, transformBuffer, cubeTransform);
		//Unbind buffers
		lightBuffer->UnbindVS();
		//Unbind framebuffer
		shadowBuffer->Unbind();
		//Unbind pipelinestate
		shadowState->Unbind();




		//RENDER PASS
		//Bind framebuffer
		frameBuffer->Bind();
		frameBuffer->ClearColor(ColorF::CORNFLOWERBLUE);
		frameBuffer->ClearDepth(1.0f);
		//--TERRAIN--
		cameraUniformBuffer->BindHS(0);
		cameraUniformBuffer->BindDS(0);
		//Render terrain
		terrain.Render(context, terrainMatInstance);
		//Unbind buffers
		cameraUniformBuffer->UnbindHS();
		cameraUniformBuffer->UnbindDS();
		//--OTHER GEOMETRY--
		//Bind buffers
		//TODO: Move to renderer
		cameraUniformBuffer->BindVS(0);
		transformBuffer->BindVS(1);
		//Render spheres
		for (int i = 0; i < SPHERES_X * SPHERES_Y; i++)
			Render(mesh, context, transformBuffer, sphereMaterials[i], sphereTransforms[i]);
		//Render cube
		Render(cube, context, transformBuffer, stoneMaterial, cubeTransform);
		//Unbind buffers
		cameraUniformBuffer->UnbindVS();
		transformBuffer->UnbindVS();
		//Unbind framebuffer
		frameBuffer->Unbind();




		//SHADE
		//Textures
		//TODO: Move environmentmaps to renderer, keep in a class (t.ex ReflectionSphere?)
		integrationMap->BindPS(5);
		radianceMap->BindPS(6);
		irradianceMap->BindPS(7);
		//Bind buffers
		//TODO: Move Light- and CameraBuffer to renderer
		lightBuffer->BindPS(0);
		cameraUniformBuffer->BindPS(1);
		//Shade
		ShadingStage(
			albedoBuffer->GetTexture(), //Buffer with albedo
			normalBuffer->GetTexture(), //Buffer with normal and roughness
			positionBuffer->GetTexture(), //Buffer with position and metallness
			aoTarget->GetTexture(), //Buffer with ambient occlusion
			shadowMap->GetTexture(), //Shadowmap
			finalBuffer, context, shadeState
		);
		//Unbind textures
		irradianceMap->UnbindPS();
		radianceMap->UnbindPS();
		integrationMap->UnbindPS();
		//Buffers
		cameraUniformBuffer->UnbindPS();
		lightBuffer->UnbindPS();




		//GLOW POST-PROCESSS
		//TODO: Should be inbeded into a class and one function
		//Gather
		glowState->Bind();
		if (bloom)
		{
			glowUniform->BindPS(0);

			glowData.mode = 0.0f;
			glowUniform->Update(&glowData, 1, 0);

			glowFrameBuffers[0]->Bind();
			finalTarget->GetTexture()->BindPS(0);
			context->Draw(glowState, 4, 0);
			finalTarget->GetTexture()->UnbindPS();
			glowFrameBuffers[0]->Unbind();
			//BLUR
			for (int i = 0; i < 2; i++)
			{
				//vertical blur
				glowData.mode = 1.0f;
				glowUniform->Update(&glowData, 1, 0);

				glowFrameBuffers[1]->Bind();
				glowBuffers[0]->GetTexture()->BindPS(0);
				context->Draw(glowState, 4, 0);
				glowBuffers[0]->GetTexture()->UnbindPS();
				glowFrameBuffers[1]->Unbind();

				//Horizontal blur
				glowData.mode = 2.0f;
				glowUniform->Update(&glowData, 1, 0);

				glowFrameBuffers[0]->Bind();
				glowBuffers[1]->GetTexture()->BindPS(0);
				context->Draw(glowState, 4, 0);
				glowBuffers[1]->GetTexture()->UnbindPS();
				glowFrameBuffers[0]->Unbind();
			}

			glowUniform->UnbindPS();
		}
		glowState->Unbind();



		//Change what buffer we render
		if (keyboardState.IsKeyDown(KEY_3) && lastKeyboardState.IsKeyUp(KEY_3))
			buffer = shadowMap->GetTexture();
		if (keyboardState.IsKeyDown(KEY_4) && lastKeyboardState.IsKeyUp(KEY_4))
			buffer = finalTarget->GetTexture();
		if (keyboardState.IsKeyDown(KEY_5) && lastKeyboardState.IsKeyUp(KEY_5))
			buffer = normalBuffer->GetTexture();
		if (keyboardState.IsKeyDown(KEY_6) && lastKeyboardState.IsKeyUp(KEY_6))
			buffer = positionBuffer->GetTexture();
		if (keyboardState.IsKeyDown(KEY_7) && lastKeyboardState.IsKeyUp(KEY_7))
			buffer = aoTarget->GetTexture();
		if (keyboardState.IsKeyDown(KEY_8) && lastKeyboardState.IsKeyUp(KEY_8))
			buffer = albedoBuffer->GetTexture();

		//Render to screenquad
		screenState->SetViewport(context->GetWindowViewport());
		RenderScreenQuad(buffer, glowBuffers[0]->GetTexture(), context, screenState);
	}


	//Release materials
	for (int i = 0; i < SPHERES_X * SPHERES_Y; i++)
	{
		ReleaseObject_S(sphereMaterials[i]);
	}
	ReleaseBuffer_S(sphereMaterials);
	ReleaseObject_S(stoneMaterial);
	ReleaseObject_S(terrainMatInstance);

	return 0;
}