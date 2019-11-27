#include "Player.h"
#include "../Mx/Input/MxInput.h"
#include "../Mx/Component/Camera/MxCamera.h"
#include "../Mx/Log/MxLog.h"
#include "../Mx/Window/MxWindow.h"
#include "PlayerAdapter.h"

namespace Scripts {
	MX_IMPLEMENT_RTTI(PlayerAdapter, Script);

	void PlayerAdapter::move(const Vector2f& _dir) {
		auto moveDir = _dir.length() > 0.0f ? _dir.normalize() : Vector2f::Zero;

		mSmoothMove = Vector2f::Lerp(mSmoothMove, moveDir * mMoveSpeed, mAccelerate);
		mSmoothMove = Vector2f::Lerp(mSmoothMove, Vector2f::Zero, mDecelerate);
		transform()->translate(mSmoothMove.x, mSmoothMove.y, 0.0f, Space::World);

		int hDir = moveDir.x > 0.0f ? -1 : (moveDir.x < 0.0f ? 1 : 0);
		
	}

	void PlayerAdapter::attack() {
		//auto weapon = mGameObject->getComponent<...>();
		//if (weapon != nullptr)
		//	weapon->attack();
	}

	void PlayerAdapter::setMoveSpeed(float _speed) {
		mMoveSpeed = _speed;
	}

	void PlayerAdapter::awake() {
	}

	void PlayerAdapter::update() {
	}

	void PlayerAdapter::fixedUpdate() {
	}
}