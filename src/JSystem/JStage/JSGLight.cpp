#include <JSystem/JStage/JSGLight.hpp>

JStage::TLight::~TLight() { }

s32 JStage::TLight::JSGFGetType() const { return 5; }

JStage::TELight JStage::TLight::JSGGetLightType() const { return TELIGHT_Unk1; }

void JStage::TLight::JSGSetLightType(JStage::TELight) { }

GXColor JStage::TLight::JSGGetColor() const
{
	return (GXColor) { 0xFF, 0xFF, 0xFF, 0xFF };
}

void JStage::TLight::JSGSetColor(GXColor) { }

void JStage::TLight::JSGGetDistanceAttenuation(f32*, f32*, GXDistAttnFn*) const
{
}

void JStage::TLight::JSGSetDistanceAttenuation(f32, f32, GXDistAttnFn) { }

void JStage::TLight::JSGGetAngleAttenuation(f32*, GXSpotFn*) const { }

void JStage::TLight::JSGSetAngleAttenuation(f32, GXSpotFn) { }

void JStage::TLight::JSGGetPosition(Vec*) const { }

void JStage::TLight::JSGSetPosition(const Vec&) { }

void JStage::TLight::JSGGetDirection(Vec*) const { }

void JStage::TLight::JSGSetDirection(const Vec&) { }
