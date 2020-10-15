#include <nds.h>
#include <nds/arm9/video.h>
#include <nds/arm9/videoGL.h>
#include <nds/arm9/background.h>
#include <nds/arm9/input.h>

#include <vector>

#include "grass_side.h"
#include "grass_top.h"
#include "grass_bottom.h"

#include "Log.h"
#include "Cube.h"
#include "Player.h"

static u16 frames = 0;

void FPSTimerCallback()
{
	// timer callback is triggered every second, so the frame count is the fps
	timerStart(3, ClockDivider_1024, UINT16_MAX - 32728, FPSTimerCallback);
	LOG("FPS: %u", frames);
	frames = 0;
}


int main(void) {
	// initialize the required hardware
	videoSetMode(MODE_0_3D);
	vramSetBankA(VRAM_A_TEXTURE_SLOT0);
	vramSetBankF(VRAM_F_TEX_PALETTE_SLOT0);	

	glInit();
	glEnable(GL_TEXTURE_2D | GL_BLEND);

	glClearColor(2, 4, 20, 31);
	glClearPolyID(63);
	glViewport(0, 0, 255, 191);
	
	
	int textures[3];
	glGenTextures(3, textures);

	glBindTexture(0, textures[0]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_16, TEXTURE_SIZE_16, 0, TEXGEN_TEXCOORD, (void*)grass_sideBitmap);
	glColorTableEXT(0, 0, grass_sidePalLen / 2, 0, 0, (u16*)grass_sidePal);

	glBindTexture(0, textures[1]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_16, TEXTURE_SIZE_16, 0, TEXGEN_TEXCOORD, (void*)grass_topBitmap);
	glColorTableEXT(0, 0, grass_topPalLen / 2, 0, 0, (u16*)grass_topPal);

	glBindTexture(0, textures[2]);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_16, TEXTURE_SIZE_16, 0, TEXGEN_TEXCOORD, (void*)grass_bottomBitmap);
	glColorTableEXT(0, 0, grass_bottomPalLen / 2, 0, 0, (u16*)grass_bottomPal);
	glMaterialf(GL_EMISSION, RGB15(31, 31, 31));
	
	const int square_width = 50;
	const int square_width_half = square_width / 2;
	Cube* cubes = (Cube*)malloc(square_width * square_width * sizeof(Cube));
	for(int x = 0; x < square_width; x++)
	{
		for(int z = 0; z < square_width; z++)
		{
			cubes[x * square_width + z].move({inttof32(x - square_width_half), 0, inttof32(-z + square_width_half)});
			cubes[x * square_width + z].load_textures(textures);
		}
	}
	cubes[square_width * square_width].move({0, inttof32(1), 0});
	cubes[square_width * square_width].load_textures(textures);

	Player player;

	FPSTimerCallback();

	while(1) {

		player.process_input();
		
		player.update_camera();

		for(unsigned int i = 0; i < square_width * square_width + 1; i++)
		{
			cubes[i].draw_face(CUBE_TOP);
		}
		MATRIX_POP = 1;

		GFX_FLUSH = 0;

		frames++;
		swiWaitForVBlank();
	}
	free(cubes);
	return 0;
}


// u32 n = floattof32(0.1f), f = floattof32(30.0f), t = floattof32(0.11), b = -floattof32(0.11), l = floattof32(-0.13f), r = floattof32(0.13f);
// fully functional orthogonal projection
/* MATRIX_MULT4x4 = divf32(floattof32(2.0f), r - l); MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0;
	MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = divf32(floattof32(2.0f), (t - b)); MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0;
	MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = divf32(floattof32(2.0f), (n - f)); MATRIX_MULT4x4 = 0;
	MATRIX_MULT4x4 = divf32((l + r), (l - r)); MATRIX_MULT4x4 = divf32((b + t), (b - t)); 
	MATRIX_MULT4x4 = divf32((n + f), (n - f)); MATRIX_MULT4x4 = floattof32(1.0f); */

// perspective - works with the given values
	/* MATRIX_MULT4x4 = divf32(mulf32(floattof32(2.0f), n), (r - l)); MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0;
	MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = divf32(mulf32(floattof32(2.0f), n), (t - b)); MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0;
	MATRIX_MULT4x4 = divf32((r + l), (r - l)); 
	MATRIX_MULT4x4 = divf32((t + b), (t - b)); MATRIX_MULT4x4 = divf32((n + f), (n - f)); MATRIX_MULT4x4 = floattof32(-1.0f);
	MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = divf32(mulf32(floattof32(2.0f), mulf32(n, f)), (n - f)); MATRIX_MULT4x4 = 0;
	 */

	/* MATRIX_CONTROL = GL_MODELVIEW;
	MATRIX_PUSH = 1;

	//MATRIX_IDENTITY = 1;
	
	u32 hor_c = floattof32(fixedToFloat(cosLerp(degreesToAngle(hor_angle)), 12));
	u32 hor_s = floattof32(fixedToFloat(sinLerp(degreesToAngle(hor_angle)), 12));
	u32 ver_c = floattof32(fixedToFloat(cosLerp(degreesToAngle(ver_angle)), 12));
	u32 ver_s = floattof32(fixedToFloat(sinLerp(degreesToAngle(ver_angle)), 12));
	// rotate the cube somehow around y
	MATRIX_MULT3x3 = hor_c; MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = hor_s;
	MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = floattof32(1.0); MATRIX_MULT3x3 = 0;
	MATRIX_MULT3x3 = -hor_s; MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = hor_c;
	// rotate the cube around x
	MATRIX_MULT3x3 = floattof32(1.0); MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = 0;
	MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = ver_c; MATRIX_MULT3x3 = ver_s;
	MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = -ver_s; MATRIX_MULT3x3 = ver_c; 
	
	MATRIX_POP = 1;*/