#pragma once

class UGameInstance {
public:
	virtual void Init();

	virtual void OnStart();

	virtual void Shutdown();
};
