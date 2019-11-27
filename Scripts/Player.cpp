#include "Player.h"
#include "../Mx/Input/MxInput.h"
#include "../Mx/Component/Camera/MxCamera.h"
#include "../Mx/Log/MxLog.h"
#include "../Mx/Window/MxWindow.h"
#include "PlayerAdapter.h"

namespace Scripts {
	MX_IMPLEMENT_RTTI(PlayerScript, Script);

	void PlayerScript::start() {
		mAdapter = mGameObject->getComponent<PlayerScript>;
	}
	void PlayerScript::update() {
		mAdapter->move(getDir());

		mAdapter->attack();
	}

	void PlayerScript::fixedUpdate() {
	}

	Vector2f PlayerScript::getDir() {
		auto dir = dirFromKeyboard() + dirFromGamepad();
		return !(dir.length() > 0.0f) ? Vector2f::Zero : dir.normalize();
	}

	Vector2f PlayerScript::dirFromGamepad() {
		auto dir = Input::Get()->getGamepadLeftStickAxis();
		return !(dir.length() > 0.0f) ? Vector2f::Zero : dir.normalize();
	}

	Vector2f PlayerScript::dirFromKeyboard() {
		Vector2f dir;
		if (Input::Get()->isButtonHold(ButtonCode::W)) {
			dir += Vector2f::Up;
		}
		if (Input::Get()->isButtonHold(ButtonCode::A)) {
			dir += Vector2f::Left;
		}
		if (Input::Get()->isButtonHold(ButtonCode::S)) {
			dir += Vector2f::Down;
		}
		if (Input::Get()->isButtonHold(ButtonCode::D)) {
			dir += Vector2f::Right;
		}
		return dir.length() > 0.0f ? dir.normalize() : Vector2f::Zero;
	}
}