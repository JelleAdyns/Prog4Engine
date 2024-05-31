#ifndef SCENESTATE_H
#define SCENESTATE_H

class SceneState
{
public:
	SceneState() = default;
	virtual ~SceneState() = default;

	SceneState(const SceneState&) = delete;
	SceneState(SceneState&&) noexcept = delete;
	SceneState& operator= (const SceneState&) = delete;
	SceneState& operator= (SceneState&&) noexcept = delete;

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual void OnSuspend() = 0;
	virtual void OnResume() = 0;

};

#endif // !SCENESTATE_H
