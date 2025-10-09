/*
 *	@file Singleton.h
 */

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

/*
 *	シングルトンの基底クラス
 */
template <typename T>
class Singleton {
protected:
	Singleton() = default;
	~Singleton() = default;
	
public:		
	// コピー、譲渡の禁止
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator = (const Singleton&) = delete;
	Singleton& operator = (Singleton&&) = delete;

	// インスタンスの取得
	static T& GetInstance() {
		static T instance;
		return instance;
	}
};

#endif // !_SINGLETON_H_
