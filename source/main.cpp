#include <nds.h>
#include <nds/arm9/video.h>
#include <nds/arm9/videoGL.h>
#include <nds/arm9/background.h>
#include <nds/arm9/input.h>

#include "Log.h"

void drawCube();

int main(void) {

	// initialize the required hardware
	videoSetMode(MODE_0_3D);
	REG_POWERCNT = vu16(POWER_LCD | POWER_3D_CORE | POWER_2D_A | POWER_2D_B | POWER_MATRIX | POWER_SWAP_LCDS);
	
	glInit();
	LOG("GFX_STATUS 2: %x", GFX_STATUS);
	GFX_CLEAR_COLOR = RGB15(2, 14, 20) | (31 << 16);
	GFX_CLEAR_DEPTH = 0x7FFF;
	GFX_VIEWPORT = (255 << 16) | (191 << 24);
	
	MATRIX_CONTROL = GL_PROJECTION;
	MATRIX_IDENTITY = 1;

	// move camera to the right
	u32 t = floattof32(1.0f), b = floattof32(-1.0f), n = floattof32(0.1f), f = floattof32(30.0f), l = floattof32(-1.3f), r = floattof32(1.3f);

	LOG("%f, %f, %f, %f, %f, %f", t, b, n, f, l, r);
	// ortho - works fully functional
	MATRIX_MULT4x4 = divf32(floattof32(2.0f), r - l); MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0;
	MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = divf32(floattof32(2.0f), (t - b)); MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0;
	MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = divf32(floattof32(2.0f), (n - f)); MATRIX_MULT4x4 = 0;
	MATRIX_MULT4x4 = divf32((l + r), (l - r)); MATRIX_MULT4x4 = divf32((b + t), (b - t)); 
	MATRIX_MULT4x4 = divf32((n + f), (n - f)); MATRIX_MULT4x4 = floattof32(1.0f);
	//glOrthof32(l, r, b, t, n, f);
	float asp = (256.0 / 192.0), fov = 70.0;
	// perspective
	/* MATRIX_MULT4x4 = divf32(mulf32(floattof32(2.0f), n), (r - l)); MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0;
	MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = divf32(mulf32(floattof32(2.0f), n), (t - b)); MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0;
	MATRIX_MULT4x4 = divf32((r + l), (r - l)); 
	MATRIX_MULT4x4 = divf32((t + b), (t - b)); MATRIX_MULT4x4 = divf32((n + f), (n - f)); MATRIX_MULT4x4 = floattof32(-1.0f);
	MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = 0; MATRIX_MULT4x4 = divf32(mulf32(floattof32(2.0f), mulf32(n, f)), (n - f)); MATRIX_MULT4x4 = 0;
	 */
	//gluPerspective(70.0f, 256.0f / 192.0f, 0.1f, 30.0f);
	gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	// rotate the camera 90 degrees to the left
	/* s16 angle = 0;
	MATRIX_MULT3x3 = cosLerp(degreesToAngle(angle)); MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = sinLerp(degreesToAngle(angle));
	MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = floatToFixed(1.0, 12); MATRIX_MULT3x3 = 0;
	MATRIX_MULT3x3 = -sinLerp(degreesToAngle(angle)); MATRIX_MULT3x3 = 0; MATRIX_MULT3x3 = cosLerp(degreesToAngle(angle)); */

	s16 hor_angle = 0;
	s16 ver_angle = 0;
	while(1) {

		MATRIX_PUSH = 1;

		glTranslatef32(0, 0, floattof32(-5.0));
		glScalef(0.5, 0.5, 0.5);

		MATRIX_CONTROL = GL_MODELVIEW;
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
		//glRotatef32(degreesToAngle(hor_angle), 0.5, 0.5, 0.0);
		drawCube();
		

		MATRIX_POP = 1;

		GFX_FLUSH = 0;

		scanKeys();
		int keys = keysHeld();
		if(keys & KEY_LEFT)
		{
			hor_angle -= 1;
		}
		if(keys & KEY_RIGHT)
		{
			hor_angle += 1;
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
	GFX_POLY_FORMAT = POLY_ALPHA(31) | POLY_CULL_NONE;

	GFX_BEGIN = GL_TRIANGLES;
	// front
	GFX_COLOR = RGB15(31, 0, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(0, 0, 31);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(0, 0, 31);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(31, 0, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	// back
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(31, 0, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(0, 0, 31);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	// top
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(0, 0, 31);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(0, 0, 31);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	// bottom
	GFX_COLOR = RGB15(31, 0, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(31, 0, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(31, 0, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(31, 0, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	// right
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(31, 0, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(0, 0, 31);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	// left
	GFX_COLOR = RGB15(31, 0, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(0, 0, 31);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(0, 0, 31);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(-1.0, 12);
	GFX_COLOR = RGB15(0, 31, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);
	GFX_COLOR = RGB15(31, 0, 0);
	GFX_VERTEX16 = (floatToFixed(-1.0, 12) & 0xFFFF) | (floatToFixed(-1.0, 12) << 16);
	GFX_VERTEX16 = floatToFixed(1.0, 12);

	GFX_END = 1;
}