#include "Hitbox.h"

bool HitboxPointBox(const Float2& pointpos,const Box& box, float customradius)
{
    if (customradius > 0.0f)
    {
        // Find the closest point on the rectangle to the circle
        float closestX = customlib::clamp(pointpos.x, box.pos1.x, box.pos2.x);
        float closestY = customlib::clamp(pointpos.y, box.pos1.y, box.pos2.y);

        // Calculate distance between circle center and closest point
        float distanceX = pointpos.x - closestX;
        float distanceY = pointpos.y - closestY;
        float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

        // Compare squared distance to squared radius
        return distanceSquared < (customradius * customradius);
    }
        return pointpos.x > box.pos1.x &&
            pointpos.y > box.pos1.y &&
            pointpos.x < box.pos2.x &&
            pointpos.y < box.pos2.y;
}


bool HitboxPointBox(const Float2* pointpos, const Box& box, float customradius)
{
    if (pointpos->x - customradius > box.pos1.x &&
        pointpos->y - customradius > box.pos1.y &&
        pointpos->x + customradius < box.pos2.x &&
        pointpos->y + customradius < box.pos2.y) {
        return true;
    }
    return false;
}

bool HitboxEntityBox(const Entity* pointpos, const Box& box, float customradius)
{
    if (
        pointpos->pos.x - customradius > box.pos1.x &&
        pointpos->pos.y - customradius > box.pos1.y &&
        pointpos->pos.x + customradius < box.pos2.x &&
        pointpos->pos.y + customradius < box.pos2.y
        ) 
    {
        return true;
    }
    return false;
}

bool HitboxPointPoint(const Float2& pos, const Float2& pos2, float customradius1, float customradius2)
{
    float distance_x = (pos.x - pos2.x) * (pos.x - pos2.x);
    float distance_y = (pos.y - pos2.y) * (pos.y - pos2.y);
    float distance_xy = distance_x + distance_y;

    // Sum of radii squared. both are squared to remove roots.
    float radiussquared = (customradius1 + customradius2)
        * (customradius1 + customradius2);

    return distance_xy <= radiussquared;
}

bool HitboxPointPoint(const Entity& entity1, const Float2& pos2, float customradius1, float customradius2)
{
    float distance_x = (entity1.pos.x - pos2.x) * (entity1.pos.x - pos2.x);
    float distance_y = (entity1.pos.y - pos2.y) * (entity1.pos.y - pos2.y);
    float distance_xy = distance_x + distance_y;

    // Sum of radii squared. both are squared to remove roots.
    float radiussquared = (entity1.radius + customradius1 + customradius2) 
                        * (entity1.radius + customradius1 + customradius2);

    return distance_xy <= radiussquared;
}

bool HitboxPointPoint(const Entity* entity1, const Float2& pos2, float customradius1, float customradius2)
{
    float distance_x = (entity1->pos.x - pos2.x) * (entity1->pos.x - pos2.x);
    float distance_y = (entity1->pos.y - pos2.y) * (entity1->pos.y - pos2.y);
    float distance_xy = distance_x + distance_y;

    // Sum of radii squared. both are squared to remove roots.
    float radiussquared = (entity1->radius + customradius1 + customradius2)
        * (entity1->radius + customradius1 + customradius2);

    return distance_xy <= radiussquared;
}


bool HitboxPointPoint(const Entity& entity1, const Entity* entity2, float customradius1, float customradius2)
{
    float distance_x = (entity1.pos.x - entity2->pos.x) * (entity1.pos.x - entity2->pos.x);
    float distance_y = (entity1.pos.y - entity2->pos.y) * (entity1.pos.y - entity2->pos.y);
    float distance_xy = distance_x + distance_y;

    // Sum of radii squared. both are squared to remove roots.
    float radiussquared = (entity1.radius + customradius1 + entity2->radius + customradius2)
        * (entity1.radius + customradius1 + entity2->radius + customradius2);

    return distance_xy <= radiussquared;
}

bool HitboxPointPoint(const Entity* entity1, const Entity* entity2, float customradius1, float customradius2)
{
    float distance_x = (entity1->pos.x - entity2->pos.x) * (entity1->pos.x - entity2->pos.x);
    float distance_y = (entity1->pos.y - entity2->pos.y) * (entity1->pos.y - entity2->pos.y);
    float distance_xy = distance_x + distance_y;

    // Sum of radii squared. both are squared to remove roots.
    float radiussquared = (entity1->radius + customradius1 + entity2->radius + customradius2)
                         * (entity1->radius + customradius1 + entity2->radius + customradius2);

    return distance_xy <= radiussquared;
}

//SHARED_PTR

bool HitboxPointPoint(const Entity& entity1, const std::shared_ptr<Entity>& entity2, float customradius1, float customradius2)
{
    float distance_x = (entity1.pos.x - entity2->pos.x) * (entity1.pos.x - entity2->pos.x);
    float distance_y = (entity1.pos.y - entity2->pos.y) * (entity1.pos.y - entity2->pos.y);
    float distance_xy = distance_x + distance_y;

    // Sum of radii squared. both are squared to remove roots.
    float radiussquared = (entity1.radius + customradius1 + entity2->radius + customradius2)
        * (entity1.radius + customradius1 + entity2->radius + customradius2);

    return distance_xy <= radiussquared;
}

bool HitboxPointPoint(const Entity* entity1, const std::shared_ptr<Entity>& entity2, float customradius1, float customradius2)
{
    float distance_x = (entity1->pos.x - entity2->pos.x) * (entity1->pos.x - entity2->pos.x);
    float distance_y = (entity1->pos.y - entity2->pos.y) * (entity1->pos.y - entity2->pos.y);
    float distance_xy = distance_x + distance_y;

    // Sum of radii squared. both are squared to remove roots.
    float radiussquared = (entity1->radius + customradius1 + entity2->radius + customradius2)
        * (entity1->radius + customradius1 + entity2->radius + customradius2);

    return distance_xy <= radiussquared;
}

bool HitboxEntityBox(const std::shared_ptr<Entity>& pointpos, const Box& box, float customradius)
{
    if (
        pointpos->pos.x - customradius > box.pos1.x &&
        pointpos->pos.y - customradius > box.pos1.y &&
        pointpos->pos.x + customradius < box.pos2.x &&
        pointpos->pos.y + customradius < box.pos2.y
        )
    {
        return true;
    }
    return false;
}