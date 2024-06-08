#ifndef COLLISIONTAGS_H
#define COLLISIONTAGS_H

#include <string>

namespace collisionTags
{
	static const std::string wallTag{ "Wall" };
	static const std::string platformTag{ "Platform" };
	static const std::string playerTag{ "Player" };
	static const std::string enemyTag{ "Enemy" };
	static const std::string caughtEnemyTag{ "CaughtEnemy" };
	static const std::string projectileTag{ "Projectile" };
	static const std::string pickUp{ "PickUp" };
	static const std::string bubbleTag{ "Bubble" };
	static const std::string OccupiedBubbleTag{ "OccupiedBubble" };
}

#endif // !COLLISIONTAGS_H
