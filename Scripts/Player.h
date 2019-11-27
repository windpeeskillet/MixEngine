#pragma once
#include "../Mx/Component/Script/MxScript.h"
#include "../Mx/GameObject/MxGameObject.h"
#include "../Mx/Math/MxVector.h"
#include "../Mx/Utils/MxEvent.h"

namespace Scripts {
	using namespace Mix;

	class PlayerAdapter;

	class PlayerScript : public Script {
		MX_DECLARE_RTTI;
	public:
		~PlayerScript() = default;

	private:
		void start() override;

		void update() override;

		void fixedUpdate() override;

		Vector2f getDir();

		Vector2f dirFromGamepad();

		Vector2f dirFromKeyboard();

		SceneObjectHandle<PlayerAdapter> mAdapter;
	
	};
}