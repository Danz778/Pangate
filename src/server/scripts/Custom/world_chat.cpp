#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"
#include "World.h"

// Configuration constants
enum WorldChatConfig
{
    MIN_LEVEL_TO_CHAT   = 10,             // Minimum level required to send messages
    COLOR_CHAT_PREFIX   = 0xFF00FF00,     // Green color for [World] prefix
    COLOR_CHAT_TEXT     = 0xFFFFFFFF      // White color for default message text
};

class Custom_World_Chat : public PlayerScript
{
public:
    Custom_World_Chat() : PlayerScript("Custom_World_Chat") {}

    void OnChat(Player* player, uint32 type, uint32 lang, std::string& msg) override
    {
        // Intercept only standard text chat messages (Type 1 = CHAT_MSG_SAY / CHAT_MSG_TEXT_EMOTE handling depending on core setup)
        // Adjust the type filter based on your target channel if necessary.
        if (type != CHAT_MSG_SAY)
            return;

        // Check if the message starts with the world chat trigger command
        if (msg.rfind("#world ", 0) != 0)
            return;

        // Check if player meets the level requirement
        if (player->GetLevel() < MIN_LEVEL_TO_CHAT)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("You must be at least level %u to use World Chat.", MIN_LEVEL_TO_CHAT);
            msg.clear(); // Abort the original 'say' message
            return;
        }

        // Extract the actual message content after the trigger command
        std::string actualMsg = msg.substr(7); 
        if (actualMsg.empty() || actualMsg.find_first_not_of(' ') == std::string::npos)
        {
            msg.clear();
            return;
        }

        // Get Player details for styling
        std::string name = player->GetName();
        uint8 playerClass = player->getClass();
        
        // Convert class IDs to standard hex colors for chat styling
        std::string classColor;
        switch (playerClass)
        {
            case CLASS_WARRIOR:     classColor = "ffc79c6e"; break;
            case CLASS_PALADIN:     classColor = "fff58cba"; break;
            case CLASS_HUNTER:      classColor = "ffabd473"; break;
            case CLASS_ROGUE:       classColor = "fffff569"; break;
            case CLASS_PRIEST:      classColor = "fffffffff"; break;
            case CLASS_DEATH_KNIGHT:classColor = "ffc41f3b"; break;
            case CLASS_SHAMAN:      classColor = "ff0070de"; break;
            case CLASS_MAGE:        classColor = "ff3fc7eb"; break;
            case CLASS_WARLOCK:     classColor = "ff8787ed"; break;
            case CLASS_DRUID:       classColor = "ffff7d0a"; break;
            case CLASS_MONK:        classColor = "ff00ff96"; break; // MoP Class
            default:                classColor = "fffffffff"; break;
        }

        // Construct the global broadcast string using MoP Hypertext colors
        // Format: [World] [ClassColoredName]: message
        std::ostringstream ss;
        ss << "|cff00ff00[World]|r |c" << classColor << "[" << name << "]|r: " << actualMsg;

        // Broadcast to every player currently logged into the world
        sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());

        // Clear the original localized 'Say' message text so it doesn't double-post locally
        msg.clear();
    }
};

void AddSC_Custom_World_Chat()
{
    new Custom_World_Chat();
}
