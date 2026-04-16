/*
   - KR_ObjectMng.cpp - (DxLib)
*/
#include "KR_ObjectMng.h"

//KrLib名前空間.
namespace KR
{
	//更新処理.
	void ObjectMng::Update() {

		for (auto i = objects.begin(); i != objects.end(); ) {
			//activeなobjectのみ.
			if ((*i)->isActive) {
				//更新.
				(*i)->Update();
				//消滅条件を満たしたら.
				if ((*i)->IsErase()) {
					i = objects.erase(i); //削除する.
				}
				else {
					i++; //次のオブジェクト.
				}
			}
			else {
				i++; //次のオブジェクト.
			}
		}
	}
	//描画処理.
	void ObjectMng::Draw() {
		//activeなobjectのみ.
		for (ObjectShape* i : objects) if (i->isActive) {
			i->Draw();
		}
	}
}