#include "Cube.h"

#include <nds/arm9/video.h>
#include <nds/arm9/videoGL.h>

#include "Log.h"

Cube::Cube(vec3f32 position, int textures[3]) : m_position(position)
{
    for(int i = 0; i < 3; i++)
    {
        m_textures[i] = textures[i];
    }
}

void Cube::draw(vec3f32* camera_position, vec3f32* camera_front, vec3f32* camera_world_up)
{
	MATRIX_CONTROL = GL_MODELVIEW;
	MATRIX_PUSH = 1;
	
	LOG("position in drawcall: (%d, %d, %d)", camera_position->x, camera_position->y, camera_position->z);
	gluLookAtf32(camera_position->x, camera_position->y, camera_position->z,
				 camera_front->x, camera_front->y, camera_front->z,
		    	 camera_world_up->x, camera_world_up->y, camera_world_up->z);

    glTranslatef32(m_position.x, m_position.y, m_position.z);

    GFX_POLY_FORMAT = POLY_ALPHA(31) | POLY_CULL_FRONT;

    GFX_BEGIN = GL_QUADS;
	// front
	glBindTexture(0, m_textures[0]);
	glNormal3f(0.0f, 0.0f, 0.97f);
    
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5, 0.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
	// right
	glNormal3f(0.97f, 0.0f, 0.0f); 
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f,  0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f,-0.5f, -0.5f);	
	// back
	glNormal3f(0.0f, 0.0f, -0.97f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	
	// left
	glNormal3f(-0.97f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	// top
	glBindTexture(0, m_textures[1]);
	glNormal3f(0.0f, 0.97f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
	// bottom
	glBindTexture(0, m_textures[2]);
	glNormal3f(0.0f, -0.97f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);

	GFX_END = 1;

    MATRIX_POP = 1;
}

void Cube::draw() const
{
    glTranslatef32(m_position.x, m_position.y, m_position.z);

    GFX_POLY_FORMAT = POLY_ALPHA(31) | POLY_CULL_FRONT;

    GFX_BEGIN = GL_QUADS;
	// front
	glBindTexture(0, m_textures[0]);
	glNormal3f(0.0f, 0.0f, 0.97f);
    
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5, 0.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
	// right
	glNormal3f(0.97f, 0.0f, 0.0f); 
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f,  0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f,-0.5f, -0.5f);	
	// back
	glNormal3f(0.0f, 0.0f, -0.97f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	
	// left
	glNormal3f(-0.97f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	// top
	glBindTexture(0, m_textures[1]);
	glNormal3f(0.0f, 0.97f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
	// bottom
	glBindTexture(0, m_textures[2]);
	glNormal3f(0.0f, -0.97f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);

	GFX_END = 1;

    MATRIX_POP = 1;	
}