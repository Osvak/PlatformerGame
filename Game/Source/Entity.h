#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

enum class EntityType
{
    PLAYER,
    ENEMY_SKELETON,
    ITEM,
    MAP,
    UNKNOWN
};

class Entity
{
public:

    Entity(EntityType type) : type(type), active(true) {}

    virtual bool Update(float dt)
    {
        return true;
    }

    virtual bool CleanUp()
    {
        return true;
    }

    virtual bool LoadState(pugi::xml_node&)
    {
        return true;
    }

    virtual bool SaveState(pugi::xml_node&) const
    {
        return true;
    }

public:

    EntityType type;
    bool active = true;
    SString name;         // Entity name identifier
    //uint32 id;            // Entity identifier?

    // Possible properties, it depends on how generic we
    // want our Entity class, maybe it's not renderable...
    fPoint position;
    bool renderable = false;
};

#endif // __ENTITY_H__