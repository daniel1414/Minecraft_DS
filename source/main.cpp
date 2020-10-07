#include <nds.h>
#include <nds/arm9/video.h>
#include <nds/arm9/videoGL.h>
#include <nds/arm9/background.h>
#include <nds/arm9/input.h>

#include "Log.h"

#include "grass_side.h"

void drawCube();

int main(void) {
	// initialize the required hardware
	videoSetMode(MODE_0_3D);
	vramSetBankA(VRAM_A_TEXTURE_SLOT0);
	vramSetBankF(VRAM_F_TEX_PALETTE_SLOT1);

	//dmaCopy(grass_sideBitmap, (void*)VRAM_A, grass_sideBitmapLen);
	//dmaCopy(grass_sidePal, (void*)VRAM_F, grass_sidePalLen);

	glInit();
	glEnable(GL_TEXTURE_2D | GL_BLEND);

	glClearColor(2, 4, 20, 31);
	glClearDepth(GL_MAX_DEPTH);
	glViewport(0, 0, 255, 191);
	
	
	int texture;
	glGenTextures(1, &texture);
	glBindTexture(0, texture);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_16, TEXTURE_SIZE_16, 0, TEXGEN_TEXCOORD, (void*)grass_sideBitmap);
	glColorTableEXT(0, 0, grass_sidePalLen / 2, 0, 0, (u16*)grass_sidePal);
	glMaterialf(GL_EMISSION, RGB15(25, 25, 25));
	
	//GFX_TEX_FORMAT = (1 << 20) | (1 << 23) | (3 << 26);

	//GFX_PAL_FORMAT = 0;
	
	int addr, width;
	glGetColorTableParameterEXT(0, GL_COLOR_TABLE_FORMAT_EXT, &addr);
	glGetColorTableParameterEXT(0, GL_COLOR_TABLE_WIDTH_EXT, &width);
	LOG("Palette addres: %x, width: %d", addr, width);
	
	MATRIX_CONTROL = GL_PROJECTION;
	MATRIX_IDENTITY = 1;	

	float asp = (256.0 / 192.0), fov = 70.0;

	gluPerspective(fov, asp, 0.1f, 30.0f);
	gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	

	s16 hor_angle = 0;
	s16 ver_angle = 0;
	while(1) {

		MATRIX_CONTROL = GL_MODELVIEW;
		MATRIX_PUSH = 1;

		//MATRIX_IDENTITY = 1;
		
		u32 hor_c = floattof32(fixedToFloat(cosLerp(degreesToAngle(hor_angle)), 12));
		u32 hor_s = floattof32(fixedToFloat(sinLerp(degreesToAngle(hor_angle)), 12));
		u32 ver_c = floattof32(fixedToFloat(cosLerp(degreesToAngle(ver_angle)), 12));
		u32 ver_s = floattof32(fixedToFloat(sinLerp(degreesToAngle(ver_angle)), 12));
		// rotate the cube somehow arount y
		MATRIX_MULT3x3 = hor_c; MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = hor_s;
		MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = floattof32(1.0); MATRIX_MULT3x3 = 0;
		MATRIX_MULT3x3 = -hor_s; MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = hor_c;
		// rotate the cube around x
		MATRIX_MULT3x3 = floattof32(1.0); MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = 0;
		MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = ver_c; MATRIX_MULT3x3 = ver_s;
		MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = -ver_s; MATRIX_MULT3x3 = ver_c;

		drawCube();

		MATRIX_POP = 1;

		GFX_FLUSH = 0;

		scanKeys();
		int keys = keysHeld();
		if(keys & KEY_LEFT)
		{
			hor_angle += 1;
		}
		if(keys & KEY_RIGHT)
		{
			hor_angle -= 1;
		}
		if(keys & KEY_UP)
		{
			ver_angle -= 1;
		}
		if(keys & KEY_DOWN)
		{
			ver_angle += 1;
		}
		swiWaitForVBlank();
	}
	return 0;
}

void drawCube()
{
	GFX_POLY_FORMAT = POLY_ALPHA(31) | POLY_CULL_FRONT | POLY_ID(0);
	GFX_BEGIN = GL_QUADS;

	// front
	glNormal3f(0.0f, 0.0f, 0.97f);
	glTexCoord2f(0.0f, 1.0f);
	//GFX_TEX_COORD = (16 << 20); // lower left coordinate
	glVertex3f(-0.5f, -0.5, 0.5f);

	glTexCoord2f(0.0f, 0.0f);
	//GFX_TEX_COORD = 0;	// top felt coordinate
	glVertex3f(-0.5f, 0.5f, 0.5f);

	glTexCoord2f(1.0f, 0.0f);
	//GFX_TEX_COORD = (16 << 4); // top right coordinate
	glVertex3f(0.5f, 0.5f, 0.5f);

	glTexCoord2f(1.0f, 1.0f);
	//GFX_TEX_COORD = ((16 << 4) | (16 << 20)); // lower right coordinate
	glVertex3f(0.5f, -0.5f, 0.5f);

	// back
	glNormal3f(0.0f, 0.0f, -0.97f);
	glColor3b(255, 0, 0);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glColor3b(0, 0, 255);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);

	// top
	glNormal3f(0.0f, 0.97f, 0.0f);
	glColor3b(255, 0, 0);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glColor3b(0, 0, 255);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(0.5f, 0.5f, 0.5f);
	
	// bottom
	glNormal3f(0.0f, -0.97f, 0.0f);
	glColor3b(255, 0, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glColor3b(0, 0, 255);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(0.5f, -0.5f, -0.5f);
	
	// right
	glNormal3f(0.97f, 0.0f, 0.0f);
	GFX_TEX_COORD = (16 << 20); // lower left coordinate
	glVertex3f(0.5f, -0.5f, 0.5f);
	GFX_TEX_COORD = 0;	// top felt coordinate
	glVertex3f(0.5f, 0.5f, 0.5f);
	GFX_TEX_COORD = (16 << 4); // top right coordinate
	glVertex3f(0.5f, 0.5f, -0.5f);
	GFX_TEX_COORD = ((16 << 4) | (16 << 20)); // lower right coordinate
	glVertex3f(0.5f, -0.5f, -0.5f);
	
	// left
	glNormal3f(-0.97f, 0.0f, 0.0f);
	GFX_TEX_COORD = (16 << 20); // lower left coordinate
	glVertex3f(-0.5f, -0.5f, -0.5f);
	GFX_TEX_COORD = 0;	// top felt coordinate
	glVertex3f(-0.5f, 0.5f, -0.5f);
	GFX_TEX_COORD = (16 << 4); // top right coordinate
	glVertex3f(-0.5f, 0.5f, 0.5f);
	GFX_TEX_COORD = ((16 << 4) | (16 << 20)); // lower right coordinate
	glVertex3f(-0.5f, -0.5f, 0.5f);

	GFX_END = 1;
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