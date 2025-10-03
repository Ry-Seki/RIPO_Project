/*
 *	@file	Engine.h
 */
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <DxLib.h>
#include <EffekseerForDXLib.h>

class Engine {
private:

public:
	Engine() = default;
	~Engine() = default;

	int Run();
	int Initialize();
	void Teardown();
	void Update();
	void Render();
};
#endif // !_ENGINE_H_


