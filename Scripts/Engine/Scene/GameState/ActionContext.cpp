/*
 *  @file   ActionContext.cpp
 *  @author Seki
 */

#include "ActionContext.h"
#include "../../../Data/SaveData.h"
#include "../../Time.h"

/*
 *	@biref		セーブデータに情報を渡す
 *	@return		GameProgressData
 */
GameProgressData ActionContext::GetSaveData() const {
    GameProgressData data{};
    data.playTime = Time::totalTime;
    data.elapsedDay = elapsedDay;
    data.isHalfDay = isHalf;
    return data;
}
/*
 *	@brief		セーブデータから情報を設定
 *  @param[in]	const GameProgressData& data
 */
void ActionContext::ApplyLoadData(const GameProgressData& data) {
    elapsedDay = data.elapsedDay;
    isHalf = data.isHalfDay;
    Time::AddTotalTime(data.playTime);
}
