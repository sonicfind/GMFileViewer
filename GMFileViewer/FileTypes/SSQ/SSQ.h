#pragma once
#include "GMArray.h"
#include "IMXEntry.h"
#include "XGEntry.h"
#include "ModelSetups/Model_Setup.h"
#include "CameraSetup/CameraSetup.h"
#include "SpriteSetups/SpriteSetups.h"
#include "TextureAnimations/TextureAnimation.h"
#include "PSetup/PSetup.h"
#include "XGM/XGM.h"

class SSQ
{
	GMArray<glm::mat4> m_matrices;
	GMArray<std::unique_ptr<float[]>> m_depthBuffers;
	GMArray<IMXEntry> m_imxEntries;
	GMArray<XGEntry> m_xgEntries;
	GMArray<std::unique_ptr<Model_Setup>> m_models;
	CameraSetup m_camera;
	SpritesSetup m_sprites;
	GMArray<TexAnim> m_textureAnims;
	PSetup m_pSetup;

	XGM* m_pack = nullptr;
public:
	SSQ(const std::filesystem::path& filePath);
	void saveToFile(const std::filesystem::path& filePath) const;
	
	void loadSequence(XGM& pack);
	void unloadSequence();
	void update(float frame);
	void draw(bool drawTransparents);

	void mixedUpdateAndDraw(float frame);
};
