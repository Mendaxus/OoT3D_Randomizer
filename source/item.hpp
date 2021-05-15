#pragma once

#include <3ds.h>
#include <string>
#include <variant>

#include "hints.hpp"
#include "settings.hpp"

union ItemOverride_Value;

enum ItemType {
    ITEMTYPE_ITEM,
    ITEMTYPE_MAP,
    ITEMTYPE_COMPASS,
    ITEMTYPE_BOSSKEY,
    ITEMTYPE_SMALLKEY,
    ITEMTYPE_TOKEN,
    ITEMTYPE_FORTRESS_SMALLKEY,
    ITEMTYPE_EVENT,
    ITEMTYPE_DROP,
    ITEMTYPE_REFILL,
    ITEMTYPE_SONG,
    ITEMTYPE_SHOP,
    ITEMTYPE_DUNGEONREWARD
};

class Item {
public:
    Item(std::string name_, ItemType type_, int getItemId_, bool advancement_, bool* logicVar_, HintText* hintText_, u16 price_ = 0);
    Item(std::string name_, ItemType type_, int getItemId_, bool advancement_, u8* logicVar_, HintText* hintText_, u16 price_ = 0);
    ~Item();

    void ApplyEffect();
    void UndoEffect();

    ItemOverride_Value Value() const;

    std::string_view GetName() const {
        return name;
    }

    bool IsAdvancement() const {
        return advancement;
    }

    int GetItemID() const {
        return getItemId;
    }

    ItemType GetItemType() const {
        return type;
    }

    u16 GetPrice() const {
        return price;
    }

    void SetPrice(u16 price_) {
        price = price_;
    }

    void SetAsPlaythrough() {
        playthrough = true;
    }

    bool IsPlaythrough() const {
        return playthrough;
    }

    bool IsBottleItem() const {
        return getItemId == 0x0F || //Empty Bottle
               getItemId == 0X14 || //Bottle with Milk
              (getItemId >= 0x8C && getItemId <= 0x94); //Rest of bottled contents
    }

    bool IsMajorItem() const {
        using namespace Settings;
        if (type == ITEMTYPE_TOKEN) {
            return Bridge.Is(RAINBOWBRIDGE_TOKENS) || LACSCondition == LACSCONDITION_TOKENS;
        }

        if (type == ITEMTYPE_DROP || type == ITEMTYPE_EVENT || type == ITEMTYPE_SHOP || type == ITEMTYPE_MAP || type == ITEMTYPE_COMPASS) {
            return false;
        }

        if (type == ITEMTYPE_DUNGEONREWARD && (ShuffleRewards.Is(REWARDSHUFFLE_END_OF_DUNGEON))) {
            return false;
        }

        if (name.find("Bombchus") != std::string::npos && !BombchusInLogic) {
            return false;
        }

        if (type == ITEMTYPE_SMALLKEY && (Keysanity.Is(KEYSANITY_VANILLA) || Keysanity.Is(KEYSANITY_OWN_DUNGEON))) {
            return false;
        }

        if (type == ITEMTYPE_FORTRESS_SMALLKEY && GerudoKeys.Is(GERUDOKEYS_VANILLA)) {
            return false;
        }

        if ((type == ITEMTYPE_BOSSKEY && getItemId != 0x9A) && (BossKeysanity.Is(BOSSKEYSANITY_VANILLA) || BossKeysanity.Is(BOSSKEYSANITY_OWN_DUNGEON))) {
            return false;
        }
            //Ganons Castle Boss Key
        if (getItemId == 0x9A && (GanonsBossKey.Is(GANONSBOSSKEY_VANILLA) || GanonsBossKey.Is(GANONSBOSSKEY_OWN_DUNGEON))) {
            return false;
        }

        return IsAdvancement();
    }

    HintText GetHintText() const {
        return *hintText;
    }

    bool operator== (const Item& right) const {
        return type == right.GetItemType() && getItemId == right.GetItemID();
    }

    bool operator!= (const Item& right) const {
        return !operator==(right);
    }

private:
    std::string name;
    ItemType type;
    int  getItemId;
    bool advancement;
    std::variant<bool*, u8*> logicVar;
    HintText* hintText;
    u16  price;
    bool playthrough = false;
};
