#include "pch.h"
#include "FireEffect.h"

static const char* FIRE_SHADER = 
"texture framebuffer;\n"
"texture FireTexture;\n"
"texture NoiseTexture;\n"
"texture AlphaTexture;\n"
"\n"
"vec2 Position;\n"
"float Width;\n"
"float Height;\n"
"\n"
"float DistortionScale;\n"
"float DistortionBias;\n"
"\n"
"float FrameTime;\n"
"\n"
"effect\n"
"{\n"
"	float ScaleNoise1 = 1.0;\n"
"	float ScaleNoise2 = 2.3;\n"
"	float ScaleNoise3 = 3.3;\n"
"	\n"
"	float ScrollSpeed1 = 1.;\n"
"	float ScrollSpeed2 = 2.;\n"
"	float ScrollSpeed3 = 3.;\n"
"	\n"
"\n"
"\n"
"	if (_in.x > Position.x && _in.x < (Position.x + Width) &&\n"
"	    _in.y > Position.y && _in.y < (Position.y + Height)) {\n"
"		\n"
"		vec2 srcPos = _in - Position;\n"
"		srcPos.x = srcPos.x / Width;\n"
"		srcPos.y = 1.0 - (srcPos.y / Height);\n"
"		\n"
"		vec2 noisePos1 = vec2(srcPos.x * ScaleNoise1, srcPos.y + (FrameTime * ScrollSpeed1));\n"
"		vec2 noisePos2 = vec2(srcPos.x * ScaleNoise2, srcPos.y + (FrameTime * ScrollSpeed2));\n"
"		vec2 noisePos3 = vec2(srcPos.x * ScaleNoise3, srcPos.y + (FrameTime * ScrollSpeed3));\n"
"	\n"
"		// wrap around\n"
"		while (true) {\n"
"			if (noisePos1.x >= 1.0)\n"
"				noisePos1.x -= 1.0;\n"
"			else\n"
"				break;\n"
"		}		\n"
"		while (true) {\n"
"			if (noisePos2.x >= 1.0)\n"
"				noisePos2.x -= 1.0;\n"
"			else\n"
"				break;\n"
"		}\n"
"		while (true) {\n"
"			if (noisePos3.x >= 1.0)\n"
"				noisePos3.x -= 1.0;\n"
"			else\n"
"				break;\n"
"		}\n"
"		while (true) {\n"
"			if (noisePos1.y >= 1.0)\n"
"				noisePos1.y -= 1.0;\n"
"			else\n"
"				break;\n"
"		}		\n"
"		while (true) {\n"
"			if (noisePos2.y >= 1.0)\n"
"				noisePos2.y -= 1.0;\n"
"			else\n"
"				break;\n"
"		}\n"
"		while (true) {\n"
"			if (noisePos3.y >= 1.0)\n"
"				noisePos3.y -= 1.0;\n"
"			else\n"
"				break;\n"
"		}	\n"
"				\n"
"		vec4 noise1 = texture2D(NoiseTexture, noisePos1);\n"
"		vec4 noise2 = texture2D(NoiseTexture, noisePos2);\n"
"		vec4 noise3 = texture2D(NoiseTexture, noisePos3);\n"
"		\n"
"		noise1 = (noise1 - 0.5) / 2.0;\n"
"		noise2 = (noise2 - 0.5) / 2.0;\n"
"		noise3 = (noise3 - 0.5) / 2.0;\n"
"		\n"
"		vec4 noise = noise1 + noise2 + noise3;\n"
"		\n"
"		float pertub = ((1.0 - srcPos.y) * DistortionScale) + DistortionBias;\n"
"		\n"
"		vec2 noiseCoords = (noise.xy * pertub) + srcPos.xy;\n"
"		\n"
"		vec4 fireColor = texture2D(FireTexture, noiseCoords);\n"
"		vec4 alphaColor = texture2D(AlphaTexture, noiseCoords);\n"
"		\n"
"		fireColor.a = alphaColor;\n"
"		\n"
"		_out = fireColor;\n"
"		\n"
"	}\n"
"	else {\n"
"		_out = texture2D(framebuffer, _in);\n"
"	}\n"
"}\n"
;


FireEffect::FireEffect(float wW, float wH)
: worldWidth(wW), worldHeight(wH), width(wW), height(wH), frameTime(0.0f)
{ 
	LoadFromMemory(FIRE_SHADER);
	SetInitialParameters();
}

void FireEffect::Update(float elapsed)
{
	frameTime += elapsed;
	while (frameTime > 1.0f)
		frameTime -= 1.0f;

	SetParameter("FrameTime", frameTime);
}

void FireEffect::SetInitialParameters()
{
	frameTime = sf::Randomizer::Random(0.0f, 0.5f);

	SetTexture("framebuffer", nullptr);

	SetParameter("Position", 0.f, 0.f);
	SetParameter("Width", 1.0f);
	SetParameter("Height", 1.0f);
	
	SetParameter("FrameTime", frameTime);
	
	SetParameter("DistortionScale", 0.25f);
	SetParameter("DistortionBias", 0.3f);
}

void FireEffect::SetPosition(float x, float y)
{
	SetPosition(Vector2f(x, y));
}

void FireEffect::SetPosition(Vector2f pos)
{
	position = pos;
	SetParameter("Position", pos.x / worldWidth, 1.f - ((pos.y + height) / worldHeight));
}

void FireEffect::SetWidth(float w)
{
	width = w;
	SetParameter("Width", w / worldWidth);
}

void FireEffect::SetHeight(float h)
{
	height = h;
	SetParameter("Height", h / worldHeight);
	SetPosition(position);
}

void FireEffect::SetDistortion(float scale, float bias)
{
	SetParameter("DistortionScale", scale);
	SetParameter("DistortionBias", bias);
}

void FireEffect::SetFireTexture(sf::Image *ft)
{
	SetTexture("FireTexture", ft);
}

void FireEffect::SetNoiseTexture(sf::Image *nt)
{
	SetTexture("NoiseTexture", nt);
}

void FireEffect::SetAlphaTexture(sf::Image *at)
{
	SetTexture("AlphaTexture", at);
}
