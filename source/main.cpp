#include <nds.h>
#include <stdio.h>
#include <nds/arm9/input.h>
#include <nds/arm9/videoGL.h>
#include <nds/arm9/console.h>
#include <nds/timers.h>

#include "texture_classic.h"
#include "item_slot.h"
#include "center_cross.h"

#include "Log.h"
#include "Renderer.h"
#include "Sprite.h"
#include "World.h"
#include "Player.h"

#define DT_TIMER 0 

void dTTimerCallback()
{
	timerStart(DT_TIMER, ClockDivider_1024, 0, dTTimerCallback);
}

Player* player = nullptr;

// culling - CCW are front
int grass_texture;

World* world = nullptr;

bool cullFrustum = false;

int main(void) {

	videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_0_2D);

	consoleDemoInit();
	
	vramSetBankA(VRAM_A_TEXTURE_SLOT0);
	vramSetBankF(VRAM_F_TEX_PALETTE_SLOT0);
	Sprite::init(SpriteMapping_1D_32, false);

	glInit();

	glEnable(GL_BLEND | GL_ANTIALIAS | GL_TEXTURE_2D | GL_FOG);
	Renderer::init();
	
	glClearColor(3, 3, 3, 31);
	glFogColor(25, 25, 25, 31);
	glClearPolyID(63);
	glClearDepth(GL_MAX_DEPTH);

	glViewport(0, 0, 255, 191);

	glGenTextures(1, &grass_texture);
	glBindTexture(0, grass_texture);
	glTexImage2D(0, 0, GL_RGB256, TEXTURE_SIZE_256, TEXTURE_SIZE_256, 0, TEXGEN_OFF, (void*)texture_classicBitmap);
	glTexParameter(0, GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | GL_TEXTURE_COLOR0_TRANSPARENT);
	glColorTableEXT(0, 0, texture_classicPalLen / 2, 0, 0, (u16*)texture_classicPal);

	uint8 diffuse = 31;
	uint8 ambient = 31;

	//glMaterialf(GL_EMISSION, RGB15(emission, emission, emission));
	glMaterialf(GL_AMBIENT, RGB15(ambient, ambient, ambient));
	glMaterialf(GL_DIFFUSE, RGB15(diffuse, diffuse, diffuse));
	glMaterialShinyness();

	glLight(0, RGB15(31, 31, 31), 0, floattov10(-0.97f), 0);

	int angle = 0;

	SpriteAttributes spriteAttr = {"centerCross", 120, 296, (void*)center_crossTiles, center_crossTilesLen, (void*)center_crossPal, center_crossPalLen};
	Sprite* center_cross = Sprite::create(spriteAttr);
	spriteAttr = {"itemSlot", 0, 16, (void*)item_slotTiles, item_slotTilesLen, (void*)item_slotPal, item_slotPalLen, SpriteSize_16x16};
	Sprite* itemSlot = Sprite::create(spriteAttr);

	world = new World();

	player = new Player(world);

	timerStart(DT_TIMER, ClockDivider_1024, 0, dTTimerCallback);

	while(1) {
		
		int32 deltaTime = timerElapsed(DT_TIMER);
		// clear the depth buffer
		glClearDepth(GL_MAX_DEPTH);
		// key input
		scanKeys();
		uint32 keysH = keysHeld();
		uint32 keysD = keysDown();

		if(keysD & KEY_B)
		{
			player->jump();
		}
		if(keysH)
		{
			player->processKeyInput(keysH, deltaTime);
		}
		if(keysD & KEY_R)
		{
			player->destroyCube();
		}
		if(keysD & KEY_L)
		{
			player->placeCube();
		}
		//touch input
		touchPosition touchPos;
		touchRead(&touchPos);
		player->processTouchInput(touchPos);
		player->update(deltaTime);

		//lighting
		/* int x, y;
		x = cosLerp(angle);
		y = sinLerp(angle);
		glLight(0, RGB15(31, 31, 31), f32tov10(x), f32tov10(y), 0);

		angle += 50; */

		// rendering
		Renderer::beginScene();

		glBindTexture(0, 0);
		Renderer::drawArrow({0, inttof32(CHUNK_SIZE_Y), 0}, {inttof32(1), 0, 0}, RGB15(31, 0, 0));
		Renderer::drawArrow({0, inttof32(CHUNK_SIZE_Y), 0}, {0, inttof32(1), 0}, RGB15(0, 31, 0));
		Renderer::drawArrow({0, inttof32(CHUNK_SIZE_Y), 0}, {0, 0, inttof32(1)}, RGB15(0, 0, 31));

		glBindTexture(0, grass_texture);

		world->drawTerrain(player->getCamera());
		world->drawPlants(player->getCamera());
		
		Renderer::endScene();
		
		Sprite::updateAll();

		swiWaitForVBlank();
	}

	return 0;
}
