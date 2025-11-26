#pragma once
#include "Primitive.h"
#include "Entity.h"

bool HitboxPointBox(const Float2& pointpos, const Box& box, float customradius = 0);
bool HitboxPointBox(const Float2* pointpos, const Box& box, float customradius = 0);
bool HitboxEntityBox(const Entity* pointpos, const Box& box, float customradius = 0);
bool HitboxPointPoint(const Float2& pos, const Float2& pos2, float customradius1 = 0, float customradius2 = 0);
bool HitboxPointPoint(const Entity& entity1, const Float2& pos2, float customradius1 = 0, float customradius2 = 0);
bool HitboxPointPoint(const Entity* entity1, const Float2& pos2, float customradius1 = 0, float customradius2 = 0);
bool HitboxPointPoint(const Entity& entity1, const Entity* entity2, float customradius1 = 0, float customradius2 = 0);
bool HitboxPointPoint(const Entity* entity1, const Entity* entity2, float customradius1 = 0, float customradius2 = 0);

//SHARED_PTR

bool HitboxPointPoint(const Entity& entity1, const std::shared_ptr<Entity>& entity2, float customradius1 = 0, float customradius2 = 0);
bool HitboxPointPoint(const Entity* entity1, const std::shared_ptr<Entity>& entity2, float customradius1 = 0, float customradius2 = 0);
bool HitboxEntityBox(const std::shared_ptr<Entity>& pointpos, const Box& box, float customradius = 0 );
