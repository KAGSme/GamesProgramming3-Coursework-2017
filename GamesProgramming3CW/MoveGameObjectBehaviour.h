#ifndef _MOVEGAMEOBJECTBEHAVIOUR_H
#define _MOVEGAMEOBJECTBEHAVIOUR_H

#include "Common.h"
#include "BaseComponent.h"

class MoveGameObjectBehaviour : public BaseComponent
{
public:
	MoveGameObjectBehaviour();
	~MoveGameObjectBehaviour();
	void OnBegin();
	void Update(float deltaTime);
};
#endif

