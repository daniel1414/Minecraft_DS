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
	vramSetBankD(VRAM_D_TEXTURE_SLOT0);
	vramSetBankF(VRAM_F_TEX_PALETTE_SLOT0);

	//dmaCopy(grass_sideBitmap, (void*)VRAM_D, grass_sideBitmapLen);
	//dmaCopy(grass_sidePal, (void*)VRAM_F, grass_sidePalLen);
	
	glInit();
	glEnable(GL_TEXTURE_2D | GL_ANTIALIAS);

	glClearColor(2, 4, 20, 31);
	glClearPolyID(63);
	glClearDepth(GL_MAX_DEPTH);
	glViewport(0, 0, 255, 191);
	
	
	int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(0, 0, GL_RGB16, TEXTURE_SIZE_16, TEXTURE_SIZE_16, 0, TEXGEN_TEXCOORD, (u8*)grass_sideBitmap);
	glColorTableEXT(0, 0, grass_sideBitmapLen, 0, 0, (u16*)grass_sideBitmap);

	glMaterialf(GL_AMBIENT, RGB15(16, 16 ,16));
	glMaterialf(GL_DIFFUSE, RGB15(16, 16 ,16));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8, 8, 8));
	glMaterialf(GL_EMISSION, RGB15(16, 16 ,16));
	glMaterialShinyness();

	/* MATRIX_CONTROL = GL_MODELVIEW;
	MATRIX_IDENTITY = 1;
	GFX_LIGHT_VECTOR = (floatToFixed(-1.0f, 9) << 20); // light number 1
	GFX_LIGHT_COLOR = RGB15(31, 31, 31);
	GFX_DIFFUSE_AMBIENT = RGB15(31, 31, 31) | (RGB15(10, 10, 10) << 16);
 */
	// rotate the camera 90 degrees to the left
	/* s16 angle = 0;
	MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = floatToFixed(1.0, 12); MATRIX_MULT3x3 = 0;
	MATRIX_MULT3x3 = cosLerp(degreesToAngle(angle)); MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = sinLerp(degreesToAngle(angle));
	MATRIX_MULT3x3 = -sinLerp(degreesToAngle(angle)); MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = cosLerp(degreesToAngle(angle)); */
	MATRIX_PUSH = 1;
	
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
		
		GFX_TEX_FORMAT = (1 << 20) | (1 << 23) | (3 << 26) | (1 << 16);
		GFX_PAL_FORMAT = 8;

		glLight(0, RGB15(0, 0, 0), floattov10(0.7), 0, floattov10(-0.7));
		glBindTexture(GL_TEXTURE_2D, texture);
		
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
	GFX_POLY_FORMAT = POLY_ALPHA(16) | POLY_CULL_FRONT | POLY_FORMAT_LIGHT0;
	GFX_BEGIN = GL_QUADS;

	// front
	glNormal3f(0.0f, 0.0f, 1.0f);
	GFX_TEX_COORD = (16 << 20); // lower left coordinate
	glVertex3f(-0.5f, -0.5, 0.5f);

	GFX_TEX_COORD = 0;	// top felt coordinate
	glVertex3f(-0.5f, 0.5f, 0.5f);

	GFX_TEX_COORD = (16 << 4); // top right coordinate
	glVertex3f(0.5f, 0.5f, 0.5f);

	GFX_TEX_COORD = ((16 << 4) | (16 << 20)); // lower right coordinate
	glVertex3f(0.5f, -0.5f, 0.5f);

	// back
	glColor3b(255, 0, 0);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glColor3b(0, 0, 255);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);

	// top
	glColor3b(255, 0, 0);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glColor3b(0, 0, 255);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(0.5f, 0.5f, 0.5f);
	
	// bottom
	glColor3b(255, 0, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glColor3b(0, 0, 255);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(0.5f, -0.5f, -0.5f);
	
	// right
	glColor3b(255, 0, 0);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glColor3b(0, 0, 255);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(0.5f, -0.5f, -0.5f);
	
	// left
	glColor3b(255, 0, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glColor3b(0, 255, 0);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glColor3b(0, 0, 255);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glColor3b(0, 255, 0);
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