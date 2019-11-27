#pragma once
#include "../Mx/Component/Script/MxScript.h"
#include "../Mx/GameObject/MxGameObject.h"
#include "../Mx/Math/MxVector.h"
#include "../Mx/Utils/MxEvent.h"

namespace Scripts {
	using namespace Mix;

	class PlayerAdapter : public Script {
		MX_DECLARE_RTTI;
	public:
		~PlayerAdapter() = default;

		void move(const Vector2f& _dir);

		void attack();

		void setMoveSpeed(float _speed);

	private:
		void awake() override;

		void start() override;

		void update() override;

		void fixedUpdate() override;

		float mMoveSpeed = 0.5f;
		float mAccelerate = 0.5f;
		float mDecelerate = 0.5f;
		Vector2f mSmoothMove;


	};
}