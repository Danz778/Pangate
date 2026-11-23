/*
 * item_service_tokens.cpp  (FINAL)
 *
 * ONE ItemScript ("item_service_token") that handles every shop token by item entry:
 *
 *   110005-110008  honor / justice / valor / conquest  1000 points
 *   110009-110014  gold 1k / 5k / 10k / 30k / 80k / 150k
 *   110015         level up to 90
 *   110016-110019  rename / change faction / change race / customize
 *
 * NOT in this file (they need a gossip menu / the donate-coin config, so they stay in their own scripts):
 *   110001-110004  Pandaria-Coins            -> wow_token_1 / _2 / _5 / _10   (wow_token.cpp)
 *   110100-110101  Profession Boost (+Minor) -> battle_pay_boost_profession / _small (battle_pay_items.cpp)
 *
 * Adding a new token = add one row to the tables below + one row in item_script_names
 * (ScriptName 'item_service_token'). No new class needed.
 *
 * API names follow battle_pay_items.cpp (GetLevel, PLAYER_FIELD_XP, ModifyCurrency with 5 args),
 * because that file is the one known to build on this core.
 */

#include "ScriptMgr.h"
#include "Chat.h"
#include "DBCStores.h"
#include "Item.h"
#include "ItemPrototype.h"
#include "Player.h"
#include "WorldSession.h"
#include "World.h"
#include <sstream>
#include <string>

namespace
{
    enum ServiceTokenItems : uint32
    {
        ITEM_TOKEN_HONOR_1000     = 110005,
        ITEM_TOKEN_JUSTICE_1000   = 110006,
        ITEM_TOKEN_VALOR_1000     = 110007,
        ITEM_TOKEN_CONQUEST_1000  = 110008,

        ITEM_TOKEN_GOLD_1K        = 110009,
        ITEM_TOKEN_GOLD_5K        = 110010,
        ITEM_TOKEN_GOLD_10K       = 110011,
        ITEM_TOKEN_GOLD_30K       = 110012,
        ITEM_TOKEN_GOLD_80K       = 110013,
        ITEM_TOKEN_GOLD_150K      = 110014,

        ITEM_TOKEN_LEVEL90        = 110015,
        ITEM_TOKEN_RENAME         = 110016,
        ITEM_TOKEN_FACTION        = 110017,
        ITEM_TOKEN_RACE           = 110018,
        ITEM_TOKEN_CUSTOMIZE      = 110019,
    };

    struct GoldToken     { uint32 Item; int64 Gold; };
    struct CurrencyToken { uint32 Item; uint32 CurrencyId; uint32 Amount; };
    struct LevelToken    { uint32 Item; uint8 Level; };
    struct ServiceToken  { uint32 Item; AtLoginFlags Flag; char const* Chat; char const* Screen; };

    GoldToken const GoldTokens[] =
    {
        { ITEM_TOKEN_GOLD_1K,     1000 },
        { ITEM_TOKEN_GOLD_5K,     5000 },
        { ITEM_TOKEN_GOLD_10K,   10000 },
        { ITEM_TOKEN_GOLD_30K,   30000 },
        { ITEM_TOKEN_GOLD_80K,   80000 },
        { ITEM_TOKEN_GOLD_150K, 150000 },
    };

    CurrencyToken const CurrencyTokens[] =
    {
        { ITEM_TOKEN_HONOR_1000,    CURRENCY_TYPE_HONOR_POINTS,    1000 },
        { ITEM_TOKEN_JUSTICE_1000,  CURRENCY_TYPE_JUSTICE_POINTS,  1000 },
        { ITEM_TOKEN_VALOR_1000,    CURRENCY_TYPE_VALOR_POINTS,    1000 },
        { ITEM_TOKEN_CONQUEST_1000, CURRENCY_TYPE_CONQUEST_POINTS, 1000 },
    };

    LevelToken const LevelTokens[] =
    {
        { ITEM_TOKEN_LEVEL90, 90 },
    };

    ServiceToken const ServiceTokens[] =
    {
        { ITEM_TOKEN_RENAME,    AT_LOGIN_RENAME,
          "Name change activated! Please log out and log back in to choose a new name.",
          "Relog to change your name!" },
        { ITEM_TOKEN_FACTION,   AT_LOGIN_CHANGE_FACTION,
          "Faction change activated! Please log out and log back in to select your new faction.",
          "Relog to change your faction!" },
        { ITEM_TOKEN_RACE,      AT_LOGIN_CHANGE_RACE,
          "Race change activated! Please log out and log back in to select your new race.",
          "Relog to change your race!" },
        { ITEM_TOKEN_CUSTOMIZE, AT_LOGIN_CUSTOMIZE,
          "Customization activated! Please log out and log back in to change your appearance.",
          "Relog to customize your character!" },
    };

    template<typename T, size_t N>
    T const* FindToken(T const (&table)[N], uint32 entry)
    {
        for (T const& row : table)
            if (row.Item == entry)
                return &row;
        return nullptr;
    }

    void Say(Player* player, char const* text)
    {
        ChatHandler(player->GetSession()).SendSysMessage(text);
    }

    void Announce(Player* player, std::string const& chatMsg, char const* screenMsg)
    {
        ChatHandler(player->GetSession()).SendSysMessage(chatMsg.c_str());
        player->GetSession()->SendNotification("%s", screenMsg);
    }

    // same total cap ModifyCurrency would clamp to (Player::GetCurrencyTotalCap is private)
    uint32 CurrencyTotalCap(CurrencyTypesEntry const* currency)
    {
        uint32 cap = currency->TotalCap;
        if (currency->ID == CURRENCY_TYPE_HONOR_POINTS && sWorld->getIntConfig(CONFIG_CURRENCY_MAX_HONOR_POINTS))
            cap = sWorld->getIntConfig(CONFIG_CURRENCY_MAX_HONOR_POINTS);
        else if (currency->ID == CURRENCY_TYPE_JUSTICE_POINTS && sWorld->getIntConfig(CONFIG_CURRENCY_MAX_JUSTICE_POINTS))
            cap = sWorld->getIntConfig(CONFIG_CURRENCY_MAX_JUSTICE_POINTS);
        return cap;
    }
}

class item_service_token : public ItemScript
{
public:
    item_service_token() : ItemScript("item_service_token") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        uint32 const entry = item->GetEntry();

        GoldToken const* gold         = FindToken(GoldTokens, entry);
        CurrencyToken const* currency = FindToken(CurrencyTokens, entry);
        LevelToken const* level       = FindToken(LevelTokens, entry);
        ServiceToken const* service   = FindToken(ServiceTokens, entry);

        if (!gold && !currency && !level && !service)
            return false; // unknown entry: let the core handle it

        // from here the use is always "handled" (true) so the item spell is never cast
        if (!Check(player, item))
            return true;

        if (gold)
            return HandleGold(player, item, *gold);
        if (currency)
            return HandleCurrency(player, item, *currency);
        if (level)
            return HandleLevel(player, item, *level);
        return HandleService(player, item, *service);
    }

private:
    // common checks; false = stop (a message was already sent)
    static bool Check(Player* player, Item* item)
    {
        if (player->IsInCombat() || player->InArena() || player->InBattleground())
        {
            Announce(player, "|cffff0000You may not use this token whilst you are in combat or present in an arena or battleground.|r",
                             "You cannot use this item now!");
            return false;
        }

        if (!player->HasItemCount(item->GetEntry(), 1, true))
        {
            Say(player, "|cffff0000You do not have the necessary token.|r");
            return false;
        }

        return true;
    }

    static bool HandleGold(Player* player, Item* item, GoldToken const& token)
    {
        if (!player->ModifyMoney(token.Gold * GOLD)) // refuses above the gold cap, the token is kept
        {
            Announce(player, "|cffff0000Maximum allowed gold limit exceeded. The token was not used.|r",
                             "Gold limit exceeded!");
            return true;
        }

        player->DestroyItemCount(item->GetEntry(), 1, true);

        std::ostringstream msg;
        msg << "|cff00ff00You just received " << token.Gold << " gold.|r";
        Say(player, msg.str().c_str());
        player->SaveToDB();
        return true;
    }

    static bool HandleCurrency(Player* player, Item* item, CurrencyToken const& token)
    {
        CurrencyTypesEntry const* currency = sCurrencyTypesStore.LookupEntry(token.CurrencyId);
        if (!currency)
            return true;

        uint32 precision = (currency->Flags & CURRENCY_FLAG_HIGH_PRECISION) ? CURRENCY_PRECISION : 1;
        uint32 count = token.Amount * precision;

        uint32 totalCap = CurrencyTotalCap(currency);
        if (totalCap && player->GetCurrency(token.CurrencyId, false) + count > totalCap)
        {
            Announce(player, "|cffff0000You can not hold that many more points of this currency. The token was not used.|r",
                             "Currency cap reached!");
            return true;
        }

        player->ModifyCurrency(token.CurrencyId, int32(count), true, true, true);
        player->DestroyItemCount(item->GetEntry(), 1, true);

        std::ostringstream msg;
        msg << "|cff00ff00You just received " << token.Amount << " " << currency->Name << ".|r";
        Say(player, msg.str().c_str());
        player->SaveToDB();
        return true;
    }

    static bool HandleLevel(Player* player, Item* item, LevelToken const& token)
    {
        if (player->GetLevel() >= token.Level)
        {
            Announce(player, "|cffff0000Your character is already at or above that level.|r",
                             "Your level is already high enough!");
            return true;
        }

        if (token.Level > sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
        {
            Announce(player, "|cffff0000This level is not available on this realm.|r",
                             "Level not available!");
            return true;
        }

        player->GiveLevel(token.Level);
        player->InitTalentForLevel();
        player->SetUInt32Value(PLAYER_FIELD_XP, 0);
        player->DestroyItemCount(item->GetEntry(), 1, true);
        player->SaveToDB();

        std::ostringstream msg;
        msg << "|cff00ff00Congratulations! Your character is now level " << uint32(token.Level) << ".|r";
        Announce(player, msg.str(), "Level up!");
        return true;
    }

    static bool HandleService(Player* player, Item* item, ServiceToken const& token)
    {
        // only a pending character service blocks a second token
        if (player->HasAtLoginFlag(AtLoginFlags(AT_LOGIN_RENAME | AT_LOGIN_CUSTOMIZE | AT_LOGIN_CHANGE_FACTION | AT_LOGIN_CHANGE_RACE)))
        {
            Announce(player, "|cffff0000You have already activated a character service. Log out to the character screen to use it first.|r",
                             "A service is already pending!");
            return true;
        }

        player->SetAtLoginFlag(token.Flag);
        player->DestroyItemCount(item->GetEntry(), 1, true);
        player->SaveToDB();

        std::string msg = std::string("|cff00ff00") + token.Chat + "|r";
        Announce(player, msg, token.Screen);
        return true; // true = the item spell is not cast
    }
};

void AddSC_item_service_tokens()
{
    new item_service_token();
}
