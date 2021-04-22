#pragma once

#include <string>
#include <map>

#include <nds/arm9/sprite.h>
#include <nds/arm9/trig_lut.h>

#define MAX_SPRITE_PALETTE_LEN 16 
#define TRANSPARENT_COLOR RGB15(31,0,31)

class Sprite;
struct SpriteAttributes;

class SpriteRegistry
{
public:
    static SpriteRegistry* get();

    void add(SpriteAttributes& attributes);
    void remove(const std::string& name);
    Sprite* getSprite(const std::string& name);
    void update();
private:
    void paletteCleanUp(OamState* oam);
private:
    SpriteRegistry();
    static SpriteRegistry* s_Instance;
    std::map<std::string, Sprite> m_Registry;
};

struct SpriteAttributes
{
    SpriteAttributes(const std::string& Name, int X, int Y, void* Tiles, int TilesLen, void* Palette, int PaletteLen, 
        SpriteSize Size = SpriteSize_32x32)
        : name(Name), x(X), y(Y), tiles(Tiles), tilesLen(TilesLen), palette(Palette), paletteLen(PaletteLen), size(Size) {
            oam  = y > 192 ? &oamSub : &oamMain;
        }
    
    std::string name;
    int x, y;
    void* tiles;
    int tilesLen;
    void* palette;
    int paletteLen;
    SpriteSize size = SpriteSize_32x32;
    SpriteColorFormat colorFormat = SpriteColorFormat_256Color;
    OamState* oam = &oamMain;
    int priority = 0;
    int paletteAlpha = 0;
    int affineIndex = -1;
    bool sizeDouble = false;
    bool hidden = false;
    bool hflip = false;
    bool vflip = false;
    int rotation = 0; // in degrees
    int scaleX = intToFixed(1, 8);
    int scaleY = intToFixed(1, 8);
    bool mosaic = false;

    SpriteAttributes& operator=(const SpriteAttributes& other)
    {
        this->name = other.name;
        this->x = other.x; this->y = other.y;
        this->oam = other.oam;
        this->tiles = other.tiles;
        this->tilesLen = other.tilesLen;
        this->palette = other.palette;
        this->paletteLen = other.paletteLen;
        this->paletteOffset = other.paletteOffset;
        this->size = other.size;
        this->colorFormat = other.colorFormat;
        this->priority = other.priority;
        this->paletteAlpha = other.paletteAlpha;
        this->affineIndex = other.affineIndex;
        this->sizeDouble = other.sizeDouble;
        this->hidden = other.hidden;
        this->hflip = other.hflip;
        this->vflip = other.vflip;
        this->mosaic = other.mosaic;
        return *this;
    }
private:
    u16* gfx = nullptr;
    int id = -1;
    uint8_t paletteOffset = 0;

    friend class SpriteRegistry;
    friend class Sprite;
};

class Sprite
{
public:
    static void init(SpriteMapping mapping, bool extPalette);
    static Sprite* create(const SpriteAttributes& attributes);
    static Sprite* destroy(const std::string& name);
    static void updateAll();
    // interface to move sprites around
    void move(int x, int y);
    void moveByOffset(int xOffset, int yOffset);
    void hide() { m_Attributes.hidden = true; }
    void show() { m_Attributes.hidden = false; }
    void rotate(int degrees);
    void scale(int scaleX, int scaleY);
    void scaleX(int scaleFactor);
    void scaleY(int scaleFactor);

    int getX() const { return m_Attributes.x; }
    int getY() const { return m_Attributes.y; }
    const std::string& getName() const { return m_Attributes.name; }
    uint8_t getWidth() const;
    uint8_t getHeight() const;

    /* debugging purposes */
    static void printPaletteOffsets();
    OamState* getOam() const { return m_Attributes.oam; }

    Sprite(const SpriteAttributes& attributes);
    ~Sprite();
private:
    void init();
    void deinit();
    void setPaletteOffset();
    void addPaletteOffset(int offset);
    void changeOamIfNeeded();
    void update() const;
    void setAffineIndex();

    static void paletteCleanUp(OamState* oam);
private:
    SpriteAttributes m_Attributes;

private:
    static bool isFull(OamState* oam);
    static std::map<OamState*, bool[128]> s_TakenIDs;
    static std::map<OamState*, uint8_t> s_TakenIDsCount;
    static std::map<OamState*, uint8_t> s_PaletteOffset;
    static std::map<OamState*, bool[256]> s_TakenPaletteIDs;
    static std::map<OamState*, bool[32]> s_TakenRotScaleIDs;

    static std::map<std::string, Sprite> s_Registry;

    static std::map<OamState*, std::string> s_OamName;
};

/* some helpers */
std::pair<int, int> spriteSizeIntoWidthHeight(SpriteSize size);