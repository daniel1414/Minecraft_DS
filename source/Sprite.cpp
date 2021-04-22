#include <nds.h>
#include <nds/arm9/trig_lut.h>

#include "Log.h"
#include "Sprite.h"

#include <algorithm>

std::map<std::string, Sprite> Sprite::s_Registry;

std::map<OamState*, bool[128]> Sprite::s_TakenIDs;
std::map<OamState*, uint8_t> Sprite::s_TakenIDsCount;
std::map<OamState*, uint8_t> Sprite::s_PaletteOffset;
std::map<OamState*, bool[256]> Sprite::s_TakenPaletteIDs;
std::map<OamState*, bool[32]> Sprite::s_TakenRotScaleIDs;

std::map<OamState*, std::string> Sprite::s_OamName;

void Sprite::init(SpriteMapping mapping, bool extPalette)
{
    vramSetBankB(VRAM_B_MAIN_SPRITE);
    vramSetBankD(VRAM_D_SUB_SPRITE);
    oamInit(&oamMain, mapping, extPalette);
    oamInit(&oamSub, mapping, extPalette);
    *SPRITE_PALETTE = TRANSPARENT_COLOR;
    *SPRITE_PALETTE_SUB = TRANSPARENT_COLOR;
    s_PaletteOffset[&oamMain] = 1;
    s_PaletteOffset[&oamSub] = 1;
    s_TakenPaletteIDs[&oamMain][0] = true;
    s_TakenPaletteIDs[&oamSub][0] = true;
    s_OamName[&oamMain] = "Main";
    s_OamName[&oamSub] = "Sub";
}

Sprite* Sprite::create(const SpriteAttributes& attributes)
{
    if(s_Registry.count(attributes.name) == 0)
    {
        if(!isFull(attributes.oam))
        {
            if(attributes.paletteLen / 2 > (256 - s_PaletteOffset[attributes.oam]))
            {
                paletteCleanUp(attributes.oam);
            }
            if(attributes.paletteLen / 2 < (256 - s_PaletteOffset[attributes.oam]))
            {
                s_Registry.emplace(attributes.name, attributes);
                return &s_Registry.find(attributes.name)->second;
            }
            else 
            {
                LOG("Palette full for sprite %s in oam %s", attributes.name.c_str(), s_OamName[attributes.oam]);
            }
        }
        else 
        {
            LOG("OAM state %s is full!", s_OamName[attributes.oam]);
        }
    }
    else 
    {
        LOG("Sprite with name %s already exists!", attributes.name.c_str());
    }
    return nullptr;
}
Sprite* Sprite::destroy(const std::string& name)
{
    auto it = s_Registry.find(name);
    if(it != s_Registry.end())
    {
        s_Registry.erase(it->first);
    }
    return nullptr;
}
void Sprite::updateAll()
{
    for(const auto& r : s_Registry)
    {
        r.second.update();
    }
    oamUpdate(&oamMain);
    oamUpdate(&oamSub);

    if(s_PaletteOffset[&oamMain] + MAX_SPRITE_PALETTE_LEN > 255)
        paletteCleanUp(&oamMain);
    else if(s_PaletteOffset[&oamSub] + MAX_SPRITE_PALETTE_LEN > 255)
        paletteCleanUp(&oamSub);
}
void Sprite::paletteCleanUp(OamState* oam)
{
    LOG("Palette cleanup! %s", (oam == &oamMain ? "main" : "sub"));
    int oldOffset = -1;
    int newOffset = -1;
    
    while(s_TakenPaletteIDs[oam][++newOffset]);
    while(s_TakenPaletteIDs[oam][++oldOffset]);

    while(oldOffset < s_PaletteOffset[oam])
    {
        while(!s_TakenPaletteIDs[oam][++oldOffset] && oldOffset < s_PaletteOffset[oam]);
        // get the sprite at the given offset
        auto it = std::find_if(s_Registry.begin(), s_Registry.end(), [&](auto& p){
            return p.second.m_Attributes.oam == oam && p.second.m_Attributes.paletteOffset == oldOffset;});
        if(it != s_Registry.end())
        {
            Sprite& sprite = it->second;
            LOG("Sprite %s at offset %d, oldOffset = %d", sprite.m_Attributes.name.c_str(), sprite.m_Attributes.paletteOffset, oldOffset);
            u16* palettePtr = oam == &oamMain ? SPRITE_PALETTE : SPRITE_PALETTE_SUB;
            dmaCopy(palettePtr + oldOffset, palettePtr + newOffset, sprite.m_Attributes.paletteLen - 1);
            sprite.addPaletteOffset(newOffset - oldOffset);
            for(int i = 0; i < sprite.m_Attributes.paletteLen / 2; ++i)
            {
                s_TakenPaletteIDs[oam][oldOffset + i] = false;
                s_TakenPaletteIDs[oam][newOffset + i] = true;
            }
            sprite.m_Attributes.paletteOffset = newOffset;
            newOffset = newOffset + sprite.m_Attributes.paletteLen / 2;
            oldOffset = newOffset;
        }
        else 
        {
            LOG("end of sprites! %d", 0);
            s_PaletteOffset[oam] = newOffset;
        }
    }
}
void Sprite::move(int x, int y)
{
    m_Attributes.x = x;
    m_Attributes.y = y;
    changeOamIfNeeded();
}
void Sprite::moveByOffset(int xOffset, int yOffset)
{
    m_Attributes.x = m_Attributes.x + xOffset;
    m_Attributes.y = m_Attributes.y + yOffset;
    changeOamIfNeeded();
}
void Sprite::rotate(int degrees)
{
    if(m_Attributes.affineIndex == -1)
    {
        setAffineIndex();
    }
    m_Attributes.rotation += degrees;
    LOG("degrees = %d", m_Attributes.rotation);
    oamRotateScale(m_Attributes.oam, m_Attributes.affineIndex, degreesToAngle(m_Attributes.rotation), m_Attributes.scaleX, m_Attributes.scaleY);
}
void Sprite::scale(int scaleX, int scaleY)
{
    if(m_Attributes.affineIndex == -1)
    {
        setAffineIndex();
    }
    m_Attributes.scaleX = (m_Attributes.scaleX * scaleX) >> 8;
    m_Attributes.scaleY = (m_Attributes.scaleY * scaleY) >> 8;
    oamRotateScale(m_Attributes.oam, m_Attributes.affineIndex, degreesToAngle(m_Attributes.rotation), m_Attributes.scaleX, m_Attributes.scaleY);
}
void Sprite::scaleX(int scaleFactor)
{
    scale(scaleFactor, intToFixed(1, 8));
}
void Sprite::scaleY(int scaleFactor)
{
    scale(intToFixed(1, 8), scaleFactor);
}
uint8_t Sprite::getWidth() const 
{
    return spriteSizeIntoWidthHeight(m_Attributes.size).first; 
}
uint8_t Sprite::getHeight() const 
{
    return spriteSizeIntoWidthHeight(m_Attributes.size).second; 
}
void Sprite::printPaletteOffsets()
{
    for(int i = 0; i < 256; ++i)
    {
        LOG("oam main i = %d, taken = %d", i, s_TakenPaletteIDs[&oamMain][i]);
    }
    LOG("oam main paletteOffset = %d", s_PaletteOffset[&oamMain]);
    for(int i = 0; i < 256; ++i)
    {
        LOG("oam sub i = %d, taken = %d", i, s_TakenPaletteIDs[&oamSub][i]);
    }
    LOG("oam sub paletteOffset = %d", s_PaletteOffset[&oamSub]);
}
Sprite::Sprite(const SpriteAttributes& attributes) : m_Attributes(attributes)
{
    init();
}
Sprite::~Sprite()
{
    deinit();
}
void Sprite::init()
{
    m_Attributes.id = -1;
    while(s_TakenIDs[m_Attributes.oam][++m_Attributes.id]);
    s_TakenIDs[m_Attributes.oam][m_Attributes.id] = true;
    ++s_TakenIDsCount[m_Attributes.oam];

    m_Attributes.gfx = oamAllocateGfx(m_Attributes.oam, m_Attributes.size, m_Attributes.colorFormat);
    oamSetGfx(m_Attributes.oam, m_Attributes.id, m_Attributes.size, m_Attributes.colorFormat, m_Attributes.gfx);
    dmaCopy(m_Attributes.tiles, m_Attributes.gfx, m_Attributes.tilesLen);
    setPaletteOffset();
    dmaCopy(((uint16_t*)m_Attributes.palette + 1), (m_Attributes.oam == &oamMain ? (SPRITE_PALETTE + m_Attributes.paletteOffset) : (SPRITE_PALETTE_SUB + m_Attributes.paletteOffset)), m_Attributes.paletteLen - 2);
    for(uint8_t i = m_Attributes.paletteOffset; i < m_Attributes.paletteOffset + m_Attributes.paletteLen / 2 - 1; ++i)
    {
        s_TakenPaletteIDs[m_Attributes.oam][i] = true;
    }
    if(m_Attributes.affineIndex > -1)
    {
        setAffineIndex();
        oamRotateScale(m_Attributes.oam, m_Attributes.affineIndex, degreesToAngle(m_Attributes.rotation), m_Attributes.scaleX, m_Attributes.scaleY);
    }
}
void Sprite::deinit()
{
    //m_Attributes.hidden = true;
    s_TakenIDs[m_Attributes.oam][m_Attributes.id] = false;
    --s_TakenIDsCount[m_Attributes.oam];
    if(m_Attributes.affineIndex != -1)
        s_TakenRotScaleIDs[m_Attributes.oam][m_Attributes.affineIndex] = false;

    for(int i = m_Attributes.paletteOffset; i < m_Attributes.paletteOffset + m_Attributes.paletteLen / 2 - 1; ++i)
    {
        s_TakenPaletteIDs[m_Attributes.oam][i] = false;
    }
    oamFreeGfx(m_Attributes.oam, m_Attributes.gfx);
    oamClearSprite(m_Attributes.oam, m_Attributes.id);
}
void Sprite::setPaletteOffset()
{
    m_Attributes.paletteOffset = s_PaletteOffset[m_Attributes.oam];
    s_PaletteOffset[m_Attributes.oam] += m_Attributes.paletteLen / 2 - 1;

    addPaletteOffset(m_Attributes.paletteOffset - 1);
}
void Sprite::addPaletteOffset(int offset)
{
    auto size = spriteSizeIntoWidthHeight(m_Attributes.size);
    uint16_t* mem_ptr = m_Attributes.gfx;
    for (int i = 0; i < size.first * size.second / 2; ++i)
    {
        uint8_t left = mem_ptr[i] >> 8;
        uint8_t right = mem_ptr[i] & 0x00FF;
        if(left)
            left += offset;
        if(right)
            right += offset;
        mem_ptr[i] = (left << 8) | (right);
    }
}
void Sprite::changeOamIfNeeded()
{
    if((m_Attributes.oam == &oamMain) ^ (m_Attributes.y < 192))
    {
        deinit();
        m_Attributes.oam = m_Attributes.oam == &oamMain ? &oamSub : &oamMain;
        init();
    }
}
void Sprite::update() const
{
    auto size = spriteSizeIntoWidthHeight(m_Attributes.size);
    int y = 192 - m_Attributes.y - (size.second / 2);
    if(m_Attributes.y >= 0 && m_Attributes.y < 384)
        y = 192 - (m_Attributes.y % 192) - (size.second / 2);
    oamSet(m_Attributes.oam, m_Attributes.id, m_Attributes.x - (size.first / 2), y, m_Attributes.priority, m_Attributes.paletteAlpha, 
        m_Attributes.size, m_Attributes.colorFormat, m_Attributes.gfx, m_Attributes.affineIndex, m_Attributes.sizeDouble,
        m_Attributes.hidden, m_Attributes.hflip, m_Attributes.vflip, m_Attributes.mosaic);
}
void Sprite::setAffineIndex()
{
    int affineID = -1;
    while(s_TakenRotScaleIDs[m_Attributes.oam][++affineID]);
    LOG("New affine ID = %d on Oam %s", affineID, s_OamName[m_Attributes.oam]);
    m_Attributes.affineIndex = affineID;
    s_TakenRotScaleIDs[m_Attributes.oam][affineID] = true;
    oamSetAffineIndex(m_Attributes.oam, m_Attributes.id, m_Attributes.affineIndex, m_Attributes.sizeDouble);
}
bool Sprite::isFull(OamState* oam)
{
    return s_TakenIDsCount[oam] == 128;
}

/* some helpers */

std::pair<int, int> spriteSizeIntoWidthHeight(SpriteSize size)
{
    int width = 0, height = 0;
    switch(size)
    {
        case SpriteSize_8x8:
            width = 8; height = 8;
            break;
        case SpriteSize_16x16:
            width = 16; height = 16;
            break;
        case SpriteSize_32x32:
            width = 32; height = 32;
            break;
        case SpriteSize_64x64:
            width = 64; height = 64;
            break;
        default:
            width = 32; height = 32;
            break;
    }
    return {width, height};
}