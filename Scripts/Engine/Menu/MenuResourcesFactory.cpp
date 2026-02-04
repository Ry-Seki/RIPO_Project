/*
 *  @file   MenuResourceFactory.cpp
 *  @author Seki
 */

#include "MenuResourcesFactory.h"
#include "../UI/Button/ButtonFactory.h"
#include "../../Data/UI/MenuInfo.h"
#include "../../Data/UI/Button/ButtonInfo.h"
#include "../UI/Rect.h"
#include "../GameEnum.h"

namespace {
    /*
     *  @brief      ボタンの種類識別
     *  @param[in]  const std::string& type
     *  @return     GameEnum::ButtonType
     */
    GameEnum::ButtonType ParseButtonType(const std::string& type) {
        if (type == "Single")    return GameEnum::ButtonType::SinglePressButton;
        if (type == "LongPress") return GameEnum::ButtonType::LongPressButton;
        return GameEnum::ButtonType::Invalid;
    }
    /*
     *  @brief      JSON->矩形へ変換
     *  @param[in]  const 
     *  @return     Rect JSON& rectJSON
     */
    Rect ParseRect(const JSON& rectJSON) {
        return Rect(
            rectJSON.value("X", 0),
            rectJSON.value("Y", 0),
            rectJSON.value("Width", 0),
            rectJSON.value("Height", 0)
        );
    }
    /*
     *  @brief      JSON->ボタン情報へ変換
     *  @param[in]  const JSON& json
     *  @return     ButtonInfo
     */
    ButtonInfo ParseButton(const JSON& json) {
        ButtonInfo info{};
        info.type = ParseButtonType(json.value("Type", "Invalid"));
        info.rect = ParseRect(json["Rect"]);

        if (json.contains("Resources")) {
            const auto& resource = json["Resources"];
            info.resourceList.resize(static_cast<int>(GameEnum::ButtonRendererState::Max));;
            info.resourceList[static_cast<int>(GameEnum::ButtonRendererState::Idle)]
                = resource.value("Idle", "");
            info.resourceList[static_cast<int>(GameEnum::ButtonRendererState::Hover)]
                = resource.value("Hover", "");
            info.resourceList[static_cast<int>(GameEnum::ButtonRendererState::Press)]
                = resource.value("Press", "");
            info.resourceList[static_cast<int>(GameEnum::ButtonRendererState::Select)]
                = resource.value("Select", "");
            info.resourceList[static_cast<int>(GameEnum::ButtonRendererState::Disable)]
                = resource.value("Disable", "");
        }
        return info;
    }
}

/*
 *	@brief		メニュークラスに必要なリソースの生成
 *	@param[in]	const JSON& json
 *	@return		MenuInfo
 */
MenuInfo MenuResourcesFactory::Create(const JSON& json) {
    MenuInfo result{};

    if (json.contains("Button")) {
        for (const auto& button : json["Buttons"]) {
            auto buttonInfo = ParseButton(button);
            result.buttonList.push_back(ButtonFactory::CreateButton(buttonInfo));
        }
    }
}
