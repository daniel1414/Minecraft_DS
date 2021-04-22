#include <nds.h>
#include <stdio.h>
#include <nds/arm9/input.h>
#include <nds/arm9/videoGL.h>
#include <nds/arm9/console.h>
#include <nds/arm9/boxtest.h>

#include <vector>

#include "grass_side.h"
#include "texture_classic.h"
#include "center_cross.h"

#include "Log.h"
#include "Renderer.h"
#include "Sprite.h"
#include "World.h"

Camera* mainCamera = nullptr;

// culling - CCW are front
int grass_texture;

World* world = nullptr;

bool cullFrustum = true;

int main(void) {

	videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_0_2D);

	consoleDemoInit();
	
	vramSetBankA(VRAM_A_TEXTURE_SLOT0);
	vramSetBankF(VRAM_F_TEX_PALETTE_SLOT0);
	Sprite::init(SpriteMapping_1D_32, false);

	glInit();

	glEnable(GL_BLEND | GL_ANTIALIAS | GL_TEXTURE_2D);
	Renderer::init();
	
	glClearColor(3, 3, 3, 31);
	glClearPolyID(63);
	glClearDepth(GL_MAX_DEPTH);

	glViewport(0, 0, 255, 191);

	glGenTextures(1, &grass_texture);
	glBindTexture(0, grass_texture);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_512, TEXTURE_SIZE_512, 0, TEXGEN_OFF, (void*)texture_classicBitmap);
	glTexParameter(0, GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | GL_TEXTURE_COLOR0_TRANSPARENT);
	glColorTableEXT(0, 0, texture_classicPalLen / 2, 0, 0, (u16*)texture_classicPal);

	glMaterialf(GL_EMISSION, RGB15(31, 31, 31));

	mainCamera =  new PerspectiveCamera({inttof32(0), inttof32(CHUNK_SIZE_Y + 2), inttof32(3)}, 60, floattof32(256.0f / 192.0f), floattof32(0.1f), inttof32(20));

	SpriteAttributes centerCrossAttr = {"centerCross", 256 / 2, 192 / 2, (void*)center_crossTiles, center_crossTilesLen, (void*)center_crossPal, center_crossPalLen, SpriteSize_16x16};
	Sprite* center_cross = Sprite::create(centerCrossAttr);

/* 	Vec3 pos = {inttof32(1), 0, inttof32(-1)};
	Vec3 pos2 = {inttof32(2), inttof32(-1), 0};
	pos2 += pos;
	LOG("(%x, %x, %x)", pos2.x, pos2.y, pos2.z); */
	
	world = new World();

	while(1) {

		// clear the depth buffer
		glClearDepth(GL_MAX_DEPTH);
		// key input
		scanKeys();
		uint32 keysH = keysHeld();
		uint32 keysD = keysDown();

		if(keysH)
		{
			mainCamera->processKeyInput(keysH, floattof32(0.1f));
			//chunks[0]->updateDrawList(mainCamera->getPosition());
		}
		if(keysD & KEY_A)
		{
			world->destroyCube(mainCamera->getPosition(), -(Vec3)mainCamera->getFront());
		}
		//touch input
		touchPosition touchPos;
		touchRead(&touchPos);
		mainCamera->processTouchInput(touchPos);

		//bool inFrustum = mainCamera->isInFrustum((Vec3*)&cube->getPosition(), floattof32(0.86f));
		//iprintf("\x1b[2;1HinFrustum: %d", inFrustum);

		// rendering
		Renderer::beginScene();

		glBindTexture(0, 0);
		Renderer::drawArrow({0, inttof32(CHUNK_SIZE_Y), 0}, {inttof32(1), 0, 0}, RGB15(31, 0, 0));
		Renderer::drawArrow({0, inttof32(CHUNK_SIZE_Y), 0}, {0, inttof32(1), 0}, RGB15(0, 31, 0));
		Renderer::drawArrow({0, inttof32(CHUNK_SIZE_Y), 0}, {0, 0, inttof32(1)}, RGB15(0, 0, 31));

		glBindTexture(0, grass_texture);
		//mainChunk->draw(mainCamera);
		//secondChunk->draw(mainCamera);

		world->drawTerrain(mainCamera);
		//cube->draw();
		Renderer::endScene();

		Sprite::updateAll();

		swiWaitForVBlank();
	}

	return 0;
}
