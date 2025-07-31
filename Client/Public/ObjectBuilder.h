#pragma once
#include"UIObject.h"
#include"GameInstance.h"
#include"Client_Defines.h"
BEGIN(Client)


namespace ObjectBuilderSystem {

	

	void BuildObject(json NODE, shared_ptr<CGameObject> NodePtr); //this 넣고  Initialize에서 시작한다 . 
	
	
}
END

