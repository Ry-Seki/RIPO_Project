/*
 *	@file	Singleton.h
 *  @author Riku
 */

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

/*
 *	シングルトンの基底クラス
 */
template <typename T>
class Singleton {
protected:
	inline static T* instance = nullptr;

protected:
	Singleton() = default;
	virtual ~Singleton() = default;
	
public:		
	// コピー、譲渡の禁止
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator = (const Singleton&) = delete;
	Singleton& operator = (Singleton&&) = delete;

	/*
	 *	インスタンスの取得
	 */
	static T& GetInstance() {
		if (!instance) instance = new T();
		return *instance;
	}
	/*
	 *	インスタンスの破棄
	 */
	static void DestroyInstance() {
		if (!instance) return;

		delete instance;
		instance = nullptr;
	}
};

#endif // !_SINGLETON_H_
