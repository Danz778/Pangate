#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"
#include "World.h"

class CustomPunishAnnouncer : public CommandScript
{
public:
    CustomPunishAnnouncer() : CommandScript("CustomPunishAnnouncer") { }

    // Using the OnBeforeCommandHandler hook to intercept .ban and .mute executions
    void OnBeforeCommandHandler(std::string& commandText, WorldSession* session) override
    {
        if (commandText.empty())
            return;

        // Normalize command string for precise checking
        std::string cmd = commandText;
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        bool isBan = (cmd.rfind("ban ", 0) == 0 || cmd.rfind(".ban ", 0) == 0);
        bool isMute = (cmd.rfind("mute ", 0) == 0 || cmd.rfind(".mute ", 0) == 0);

        if (!isBan && !isMute)
            return;

        // Tokenize command parts to safely grab parameters
        std::vector<std::string> tokens;
        std::stringstream ss(commandText);
        std::string token;
        while (ss >> token)
        {
            tokens.push_back(token);
        }

        // Standard subcommands format: .ban account/character [target] [duration] [reason]
        // Standard mute format: .mute [target] [duration] [reason]
        if (isBan && tokens.size() >= 4)
        {
            std::string subCommand = tokens[1]; // account, character, etc.
            std::string target = tokens[2];
            std::string duration = tokens[3];
            std::string reason = "No reason specified";

            if (tokens.size() > 4)
            {
                reason = "";
                for (size_t i = 4; i < tokens.size(); ++i)
                    reason += tokens[i] + " ";
            }

            // Clean up type for visibility
            if (subCommand == "account" || subCommand == "character" || subCommand == "playeraccount")
            {
                std::string gmName = session ? session->GetPlayer()->GetName() : "System";
                
                // Format & BroadCast Ban Message
                std::ostringstream msg;
                msg << "|cffff0000[Punishment Announcement]|r Character/Account |cff00ffff" 
                    << target << "|r has been |cffff0000BANNED|r for |cffffcc00" 
                    << duration << "|r. Reason: |cff00ff00" << reason << "|r";
                
                sWorld->SendServerMessage(SERVER_MSG_STRING, msg.str().c_str());
            }
        }
        else if (isMute && tokens.size() >= 3)
        {
            std::string target = tokens[1];
            std::string duration = tokens[2];
            std::string reason = "No reason specified";

            if (tokens.size() > 3)
            {
                reason = "";
                for (size_t i = 3; i < tokens.size(); ++i)
                    reason += tokens[i] + " ";
            }

            std::string gmName = session ? session->GetPlayer()->GetName() : "System";

            // Format & BroadCast Mute Message
            std::ostringstream msg;
            msg << "|cffffcc00[Punishment Announcement]|r Player |cff00ffff" 
                << target << "|r has been |cffffaa00MUTED|r for |cffffcc00" 
                << duration << " minutes|r. Reason: |cff00ff00" << reason << "|r";

            sWorld->SendServerMessage(SERVER_MSG_STRING, msg.str().c_str());
        }
    }
};

void AddSC_custom_punish_announcer()
{
    new CustomPunishAnnouncer();
}
