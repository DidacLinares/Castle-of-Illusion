#ifndef _NONPLAYERENTITY_INCLUDE
#define _NONPLAYERENTITY_INCLUDE


#include "Entity.h"
#include "Player.h"



class NonPlayerEntity : public Entity {
protected:
	Player* player;
public:
	void setPlayer(Player* player);

};

#endif // _NONPLAYERENTITY_INCLUDE
