#include "MxAudioListener.h"

#include "../../GameObject/MxGameObject.h"
#include "../../Time/MxTime.h"

namespace Mix {
    MX_IMPLEMENT_RTTI_NO_CREATE_FUNC(AudioListener, Behaviour)
    MX_IMPLEMENT_DEFAULT_CLASS_FACTORY(AudioListener)

    void AudioListener::init() {
        if(mCore)
            return;
        if(!mGameObject)
            throw IndependentComponentError(getTypeName());
        mCore = Audio::Core();
        mLastPos = mGameObject->transform().position();
        mUseFixedUpdate = velocityUpdateMode == Audio::VelocityUpdateMode::FIXED ||
                          velocityUpdateMode == Audio::VelocityUpdateMode::AUTO /*&& mGameObject->getComponent<Rigidbody>()*/;

        // mListenerIdx = sListenerNum++;
    }

    void AudioListener::lateUpdate() {
        if(!mUseFixedUpdate)
            updatePosAndVel(Time::DeltaTime());
    }

    void AudioListener::fixedUpdate() {
        if(mUseFixedUpdate)
            updatePosAndVel(Time::FixedDeltaTime());
    }

    void AudioListener::updatePosAndVel(const float _deltaTime) {
        auto trans = mGameObject->transform();

        auto pos = trans.position();
        auto vel = (pos - mLastPos) / _deltaTime;
        mLastPos = pos;

        auto fvPos = Audio::glmVec3ToFmodVec(pos),
             fvVel = Audio::glmVec3ToFmodVec(vel),
             fvForward = Audio::glmVec3ToFmodVec(trans.forward()),
             fvUp = Audio::glmVec3ToFmodVec(trans.up());

        if(mCore)
            mCore->set3DListenerAttributes(mListenerIdx, &fvPos, &fvVel, &fvForward, &fvUp);
    }
}
