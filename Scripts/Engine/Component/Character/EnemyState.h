/*
 *	@file	EnemyState.h
 *  @author	kuu
 */
#ifndef _ENEMYSTATE_H_
#define _ENEMYSTATE_H_

class EnemyState {
public:
	EnemyState() = default;
	~EnemyState() = default;

	virtual void Start();
	virtual void Update(); 
	virtual void Execute();

};

#endif // !_ENEMYSTATE_H_