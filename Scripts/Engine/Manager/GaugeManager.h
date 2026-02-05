/*
 *	@file	GaugeManager.h
 *	@author	kuu
 */

#ifndef _GAUGEMANAGER_H_
#define _GAUGEMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"

class GaugeManager : public Singleton<GaugeManager> {
	// ƒtƒŒƒ“ƒhéŒ¾
	friend class Singleton<GaugeManager>;

private:

private:
	GaugeManager();
	~GaugeManager() = default;

public:
	/*
	 *	‰Šú‰»ˆ—
	 */
	void Initialize(Engine& setEngine);

	/*
	 *	•`‰æ‚·‚é
	 */
	void RendGauge();



};

#endif // !_GAUGEMANAGER_H_