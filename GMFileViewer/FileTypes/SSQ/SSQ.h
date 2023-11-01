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
	GMArray<IMXEntry> m_imxEntries;
	GMArray<XGEntry> m_xgEntries;
	GMArray<std::unique_ptr<Model_Setup>> m_models;
	CameraSetup m_camera;
	SpritesSetup m_sprites;
	GMArray<TexAnim> m_textureAnims;
	PSetup m_pSetup;

public:
	SSQ(const std::filesystem::path& filePath);
	void saveToFile(const std::filesystem::path& filePath) const;
	
	void loadSequence(Graphics& gfx, XGM& pack);
	void update(Graphics& gfx, XGM& pack, float frame);
	void draw(Graphics& gfx, XGM& pack, bool drawTransparents);

	void mixedUpdateAndDraw(Graphics& gfx, XGM& pack, float frame);
};
