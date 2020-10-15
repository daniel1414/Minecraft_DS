#include "GameSystem.h"

#include <nds.h>
#include <nds/arm9/video.h>
#include <nds/arm9/videoGL.h>
#include <nds/arm9/background.h>
#include <nds/arm9/input.h>

#include "Log.h"

// texture includes
#include "grass_top.h"
#include "grass_side.h"
#include "grass_bottom.h"

u8 GameSystem::m_frames = 0;

GameSystem::GameSystem()
{
    init_hardware();
    load_textures();
    m_world.generate_terrain(m_textures);
	m_player.init_camera();
    FPSTimerCallback();
}

GameSystem::~GameSystem()
{
    free(m_textures);
}

void GameSystem::run()
{
    while(1)
    {
		m_frames++;

		m_player.process_input();

		m_player.update_camera();
        m_world.render_cubes(m_player.get_position(), m_player.get_camera_front());

		MATRIX_POP = 1;

		GFX_FLUSH = 0;

		swiWaitForVBlank();
	}
}

void GameSystem::FPSTimerCallback()
{
    // timer callback is triggered every second, so the frame count is the fps
	timerStart(3, ClockDivider_1024, UINT16_MAX - 32728, FPSTimerCallback);
	LOG("FPS: %u", m_frames);
	m_frames = 0;
}

void GameSystem::init_hardware()
{
    // initialize the required hardware
	videoSetMode(MODE_0_3D);
	vramSetBankA(VRAM_A_TEXTURE_SLOT0);
	vramSetBankF(VRAM_F_TEX_PALETTE_SLOT0);	

	glInit();
	glEnable(GL_TEXTURE_2D | GL_BLEND);

	glClearColor(2, 4, 20, 31);
	glClearPolyID(63);
	glViewport(0, 0, 255, 191);
}

void GameSystem::load_textures()
{
    m_textures = (int*)malloc(3 * sizeof(int));
    glGenTextures(3, m_textures);

    glBindTexture(0, m_textures[0]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_16, TEXTURE_SIZE_16, 0, TEXGEN_TEXCOORD, (void*)grass_sideBitmap);
	glColorTableEXT(0, 0, grass_sidePalLen / 2, 0, 0, (u16*)grass_sidePal);

	glBindTexture(0, m_textures[1]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_16, TEXTURE_SIZE_16, 0, TEXGEN_TEXCOORD, (void*)grass_topBitmap);
	glColorTableEXT(0, 0, grass_topPalLen / 2, 0, 0, (u16*)grass_topPal);

	glBindTexture(0, m_textures[2]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_16, TEXTURE_SIZE_16, 0, TEXGEN_TEXCOORD, (void*)grass_bottomBitmap);
	glColorTableEXT(0, 0, grass_bottomPalLen / 2, 0, 0, (u16*)grass_bottomPal);

	glMaterialf(GL_EMISSION, RGB15(31, 31, 31));
}