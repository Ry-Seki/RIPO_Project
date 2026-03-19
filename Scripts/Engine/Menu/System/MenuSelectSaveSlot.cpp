/*
 *	@file	MenuSelectSaveSlot.cpp
 *	@author	Seki
 */

#include "MenuSelectSaveSlot.h"
#include "MenuConfirm.h"
#include "../MenuManager.h"
#include "../../Audio/AudioUtility.h"
#include "../../Load/LoadManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../Input/InputUtility.h"
#include "../../Save/SaveDataManager.h"
#include "../../Engine.h"
#include "../../Scene/MainGameScene.h"
#include "../MenuResourcesFactory.h"
#include "../../../Data/UI/MenuInfo.h"
#include "../../Manager/FontManager.h"
#include "../../Fade/FadeManager.h"
#include "../../Fade/FadeFactory.h"
#include "../../UI/Text/Dynamic/DynamicText.h"
#include "../../UI/Text/Static/StaticText.h"

#include <algorithm>

namespace {
    constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/System/SaveLoad/SaveSlotResources.json";
    constexpr const char* _NAVIGATION_PATH = "Data/UI/System/SaveLoad/SaveSlotNavigation.json";

    constexpr int _ONE_HOUR_TIME_PER = 3600;
    constexpr int _ONE_MINUTE_TIME_PER = 60;

    // 別名定義
    using SlotTextSet = MenuSelectSaveSlot::SlotTextSet;
    // 関数ポインタの別名定義
    using ExecuteSaveLoad = void(MenuSelectSaveSlot::*)(int, Engine&);
    using CreateTextFunc = std::shared_ptr<TextBase>(*)(const TextInfo&);
    using SlotSetter = void(*)(SlotTextSet&, std::shared_ptr<TextBase>);
    // セーブロード処理の関数テーブル
    const ExecuteSaveLoad execute[static_cast<int>(GameEnum::SaveSlotMenuMode::Max)] ={
        &MenuSelectSaveSlot::ExecuteSave,
        &MenuSelectSaveSlot::ExecuteLoad
    };
    /*
     *  @brief      テキストクラス処理
     *  @param[in]  const TextInfo& info
     */
    template <class T, typename = std::enable_if_t<std::is_base_of_v<TextBase, T>>>
    std::shared_ptr<TextBase> CreateText(const TextInfo& info) {
        return std::make_shared<T>(info);
    }

    const std::unordered_map<GameEnum::TextType, CreateTextFunc> textFactory{
        { GameEnum::TextType::Dynamic, CreateText<DynamicText> },
        { GameEnum::TextType::Static,  CreateText<StaticText> },
    };

    const std::unordered_map<std::string, SlotSetter> slotSetter{
        { "ElapsedDayText",
            [](SlotTextSet& slot, std::shared_ptr<TextBase> text) { slot.elapsedDay = text; } },

        { "HalfDayText",
            [](SlotTextSet& slot, std::shared_ptr<TextBase> text) { slot.halfDay = text; } },

        { "PlayTimeText",
            [](SlotTextSet& slot, std::shared_ptr<TextBase> text) { slot.playTime = text; } },

        { "MoneyText",
            [](SlotTextSet& slot, std::shared_ptr<TextBase> text) { slot.money = text; } },

        { "TreasureText",
            [](SlotTextSet& slot, std::shared_ptr<TextBase> text) { slot.treasure = text; } },
    };
}
/*
 *	@brief	初期化処理
 */
void MenuSelectSaveSlot::Initialize (Engine& engine) {
    auto& load = LoadManager::GetInstance();
    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);
    auto navigation = load.LoadResource<LoadJSON>(_NAVIGATION_PATH);

    load.SetOnComplete([this, &engine, menuJSON, navigation]() {
        // メニューUI生成
        MenuInfo result = MenuResourcesFactory::Create(menuJSON->GetData());
        // メニューUIの所有権移動
        spriteList = std::move(result.spriteList);
        textList = std::move(result.textList);
        buttonList = std::move(result.buttonList);
        // ボタンの登録
        for (const auto& entry : buttonList) {
            if (!entry) continue;
            UIButtonBase* button = entry.get();
            // ボタンの登録
            eventSystem.RegisterButton(button);
        }
        for (auto& sprite : spriteList) {
            if (sprite->GetName() == "SaveLoad") saveLoadSprite = sprite.get();
        }
        // イベントシステムの初期化
        eventSystem.Initialize(0);
        // ボタンの準備前処理
        InitializeSaveSlotButtons(engine);
        // イベントシステムのnavigationの設定
        eventSystem.LoadNavigation(navigation->GetData());
    });
}
/*
 *	@brief	メニューを開く
 */
void MenuSelectSaveSlot::Open () {
    MenuBase::Open();
    // セーブスロットの使用状態の取得
    auto& save = SaveDataManager::GetInstance();

    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
    // ボタンの準備処理
    for (auto& button : buttonList) {
        button->Setup();
    }
    saveLoadSprite->SetFrameIndex(static_cast<int>(saveMode));
    // セーブデータの準備前処理
    SetupSaveSlotData();
    // ロード時、セーブスロットが未使用だった場合選択不可にする
    if (saveMode == GameEnum::SaveSlotMenuMode::Load) UpdateSlotButtonState();
    // スロットテキストの生成
    CreateSlotText();
    // スロットテキストの準備前処理
    SetupAllSlotText();
    FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeIn, [this]() {
        eventSystem.ApplySelection();
        InputUtility::SetActionMapIsActive(GameEnum::ActionMap::MenuAction, true);
    });
}
/*
 *	@brief	更新処理
 */
void MenuSelectSaveSlot::Update (Engine& engine, float unscaledDeltaTime) {
    auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

    // イベントシステムの更新
    eventSystem.Update(unscaledDeltaTime);
    // ボタンの更新
    for (auto& button : buttonList) {
        if (button) button->Update(unscaledDeltaTime);
    }
    // 現在選択されているボタンの取得
    auto button = eventSystem.GetCurrentSelectButton();
    if (!button) return;

    if (!inputHandle && input.buttonDown[static_cast<int>(GameEnum::MenuAction::Decide)]) {
        inputHandle = true;
        button->OnPressDown();
    }
}
/*
 *	@brief	アニメーション等の更新
 */
void MenuSelectSaveSlot::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
    animTimer += unscaledDeltaTime;

    if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
    animTimer = 0;

    for (auto& sprite : spriteList) {
        if (!sprite || sprite.get() == saveLoadSprite) continue;

        int frameCount = sprite->GetFrameCount();
        if (frameCount <= 1) continue;

        int animFrame = sprite->GetCurrentFrame();
        animFrame = (animFrame + 1) % frameCount;
        sprite->SetFrameIndex(animFrame);
    }
}
/*
 *	@brief	描画処理
 */
void MenuSelectSaveSlot::Render () {
    for (auto& sprite : spriteList) {
        if (!sprite->IsVisible() || sprite.get() == saveLoadSprite) continue;
        sprite->Render();
    }
    saveLoadSprite->Render();
    for (auto& button : buttonList) {
        if (!button->IsVisible()) continue;
        button->Render();
    }
    RenderSlotInfo();
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectSaveSlot::Close (Engine& engine) {
    MenuBase::Close(engine);
}
void MenuSelectSaveSlot::Resume() {
    MenuBase::Resume();
    for (auto& button : buttonList) {
        button->Setup();
    }
    // セーブデータの準備前処理
    SetupSaveSlotData();
    // ロード時のみセーブスロットの使用状態判定
    if (saveMode == GameEnum::SaveSlotMenuMode::Load) UpdateSlotButtonState();
}
/*
 *	@brief		ボタンのコールバック登録
 *	@param[in]	int slotIndex
 */
void MenuSelectSaveSlot::SelectButtonExecute(int slotIndex, Engine& engine) {
    auto& save = SaveDataManager::GetInstance();
    AudioUtility::PlaySE("DebugSE");
    // 戻るボタン
    if (slotIndex < 0) {
        ExecuteBack();
    }else {
        // 確認メニューを開く
        OpenConfirmMenu(slotIndex, engine);
    }
}
/*
 *	@brief		戻る処理
 */
void MenuSelectSaveSlot::ExecuteBack() {
    isInteractive = false;
    auto& menu = MenuManager::GetInstance();
    FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 0.5f, FadeDirection::Out, FadeMode::Stop);
    FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
        menu.CloseTopMenu();
        FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 0.5f, FadeDirection::In, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeIn);
    });
}
/*
 *	@brief		確認メニューを開く
 *	@param[in]	int slotIndex
 */
void MenuSelectSaveSlot::OpenConfirmMenu(int slotIndex, Engine& engine) {
    auto& menu = MenuManager::GetInstance();
    auto confirm = menu.GetMenu<MenuConfirm>();
    confirm->SetCallback([this, &menu, slotIndex, &engine](GameEnum::ConfirmResult result) {
        AudioUtility::PlaySE("DebugSE");
        menu.CloseTopMenu();    // 確認メニュー
        if (result != GameEnum::ConfirmResult::Yes) return;
        // セーブかロード処理
        (this->*execute[static_cast<int>(saveMode)])(slotIndex, engine);
    });
    menu.OpenMenu<MenuConfirm>();
}
/*
 *	@brief		セーブスロットボタンの初期化処理
 */
void MenuSelectSaveSlot::InitializeSaveSlotButtons(Engine& engine) {
    int slotCount = SaveDataManager::GetInstance().GetMaxSaveSlot();
    int sizeMax = buttonList.size();
    slotButtonList.clear();
    slotButtonList.reserve(sizeMax);

    for (int i = 0; i < sizeMax; i++) {
        UIButtonBase* button = buttonList[i].get();
        if (!button) continue;
        int slotIndex = (i < slotCount) ? i : -1;
        // スロットボタン構造体に情報を入れる
        SaveSlotButtonEntry entry{};
        entry.slotIndex = slotIndex;
        entry.button = button;
        // ボタン実行処理の登録
        button->RegisterOnClick([this, slotIndex, &engine]() {
            SelectButtonExecute(slotIndex, engine);
        });
        // ボタンに navigation 更新処理を登録
        button->RegisterUpdateSelectButton([this, button]() {
            eventSystem.UpdateSelectButton(button);
        });
        slotButtonList.push_back(entry);
    }
}
/*
 *	@brief		セーブスロットデータの準備前処理
 */
void MenuSelectSaveSlot::SetupSaveSlotData() {
    auto& save = SaveDataManager::GetInstance();
    int sizeMax = save.GetMaxSaveSlot();
    slotDataList.clear();
    slotDataList.resize(sizeMax);
    // スロットデータの取得
    auto slotData = save.GetAllSlotData();
    // 使用状態の取得
    auto usedData = save.GetAllSlotIsUsed();
    for (int i = 0; i < sizeMax; i++) {
        slotDataList[i].data = slotData[i].game;
        slotDataList[i].isUsed = usedData[i];
    }
}
/*
 *	@brief	テキストの生成(セーブスロットのテキスト描画用)
 */
void MenuSelectSaveSlot::CreateSlotText() {
    const int slotSpacing = 205;
    const int white = GetColor(255, 255, 255);

    for (int i = 0, max = slotDataList.size(); i < max; i++) {
        int offsetY = slotSpacing * i;

        SlotTextSet set;
        // スロットに必要なテキストの生成
        for (auto& text : textList) {
            TextInfo info = text->GetTextInfo();
            info.y += offsetY;

            auto textObj = textFactory.at(info.textType)(info);
            textObj->SetColor(white);

            auto itr = slotSetter.find(text->GetName());
            if (itr == slotSetter.end()) {
                assert(false && "TextTypeが未設定でした");
                return;
            }
            itr->second(set, textObj);
        }
        // 設定
        slotButtonList[i].text = set;
    }
}
/*
 *	@brief	全てのセーブスロットテキストの準備前処理
 */
void MenuSelectSaveSlot::SetupAllSlotText() {
    // テキストの中身の文字列の設定
    for (int i = 0, max = slotDataList.size(); i < max; i++) {
        if (!slotDataList[i].isUsed) continue;

        auto& data = slotDataList[i].data;
        auto& slot = slotButtonList[i].text;

        UpdateSlotText(data, slot);
    }
}
/*
 *	@brief	スロットボタンの状態更新処理
 */
void MenuSelectSaveSlot::UpdateSlotButtonState() {
    auto& save = SaveDataManager::GetInstance();
    for (int i = 0, max = save.GetMaxSaveSlot(); i < max; i++) {
        auto& button = slotButtonList[i].button;
        if (!button) continue;
        button->SetIsEnable(slotDataList[i].isUsed);
    }
}
/*
 *	@brief		セーブスロット情報の描画
 */
void MenuSelectSaveSlot::RenderSlotInfo() {
    for (auto& slot : slotButtonList) {
        auto& text = slot.text;
        if (text.elapsedDay) text.elapsedDay->Render();
        if (text.halfDay)    text.halfDay->Render();
        if (text.playTime)   text.playTime->Render();
        if (text.money)      text.money->Render();
        if (text.treasure)   text.treasure->Render();
    }
}
/*
 *	@brief		セーブスロットテキストの更新
 *	@param[in]	const GameProgressData& data
 *	@param[in]	SlotTextSet& text
 */
void MenuSelectSaveSlot::UpdateSlotText(const GameProgressData& data, SlotTextSet& text) {
    text.elapsedDay->SetText(std::to_string(data.elapsedDay));
    text.halfDay->SetText(data.isHalfDay ? "[PM]" : "[AM]");
    text.playTime->SetText(ChangePlayTimeText(data.playTime));
    text.money->SetText(std::to_string(data.currentMoney));
    text.treasure->SetText(std::to_string(data.totalTreasureCount));
}
/*
 *	@brief		セーブスロット情報の更新
 *	@param[in]	SaveSlotButtonEntry& entry
 */
void MenuSelectSaveSlot::UpdateSaveSlot(SaveSlotButtonEntry& entry) {
    auto& save = SaveDataManager::GetInstance();
    int slotIndex = entry.slotIndex;
    if (!entry.button || slotIndex < 0 || slotIndex >= save.GetMaxSaveSlot()) return;
    // 進行データの取得
    auto& progressData = save.GetCurrentSaveData().game;
    // 進行データの更新
    slotDataList[slotIndex].data = progressData;
    // テキストの内容を更新
    UpdateSlotText(progressData, slotButtonList[slotIndex].text);
}
/*
 *	@brief		プレイ時間の変換処理
 *  @param[in]  int playTime
 *	@return		std::string
 */
std::string MenuSelectSaveSlot::ChangePlayTimeText(int playTime) {
    int playTimeSec = playTime;
    std::string playTimeText = "";
    // 時
    int hours = playTimeSec / _ONE_HOUR_TIME_PER;
    playTimeText += std::to_string(hours);
    playTimeText += ":";
    // 分
    int minutes = (playTimeSec % _ONE_HOUR_TIME_PER) / _ONE_MINUTE_TIME_PER;
    playTimeText += std::to_string(minutes);
    playTimeText += ":";
    // 秒
    int seconds = playTimeSec % _ONE_MINUTE_TIME_PER;
    playTimeText += std::to_string(seconds);
    return playTimeText;
}
/*
 *	@brief		セーブ処理
 *  @param[in]  int slotIndex
 */
void MenuSelectSaveSlot::ExecuteSave(int slotIndex, Engine& engine) {
    auto& save = SaveDataManager::GetInstance();
    auto& menu = MenuManager::GetInstance();
    save.SelectSlot(slotIndex);
    if (!save.SaveCurrentSlot()) {
        assert(false && "セーブに失敗しました");
        return;
    }
    else {
        auto& entry = slotButtonList[slotIndex];
        UpdateSaveSlot(entry);
    }
}
/*
 *	@brief		ロード処理
 *  @param[in]  int slotIndex
 */
void MenuSelectSaveSlot::ExecuteLoad(int slotIndex, Engine& engine) {
    auto& save = SaveDataManager::GetInstance();
    auto& menu = MenuManager::GetInstance();
    save.SelectSlot(slotIndex);
    save.ResetClearSaveData();
    if (!save.LoadCurrentSlot()) {
        assert(false && "ロードに失敗しました");
        return;
    }
    else {
        engine.SetNextScene(std::make_shared<MainGameScene>());
        menu.CloseAllMenu();
    }
}
