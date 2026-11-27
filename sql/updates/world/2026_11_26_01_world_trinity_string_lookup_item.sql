-- Restore the in-game output format used by `.lookup item` (LANG_ITEM_LIST_CHAT = 512,
-- Language.h:520). cs_lookup.cpp:421/465 calls PSendSysMessage with three arguments:
-- (itemId, itemId, name), so content_default MUST have exactly three printf specifiers
-- in that order. If the row is missing, GetTrinityString() (ObjectMgr.cpp:8438) returns
-- the literal "<error>"; if content_default/content_locN are empty, SendSysMessage()
-- emits an empty chat line - in both cases the item name never appears in game.
--
-- The item link has 10 property fields after the entry, matching
-- ItemChatLink::Initialize (ChatLink.cpp:136: propsCount = 10).
--
-- Backup: the previous row is discarded. Save it first if it is customized:
--   SELECT * FROM `trinity_string` WHERE `entry` = 512;
-- All locale-specific columns are left NULL on purpose so GetTrinityString()
-- falls back to content_default for every client locale.

DELETE FROM `trinity_string` WHERE `entry` = 512;
INSERT INTO `trinity_string` (`entry`, `content_default`) VALUES
(512, '%u - |cffffffff|Hitem:%u:0:0:0:0:0:0:0:0:0|h[%s]|h|r');

-- Takes effect after a worldserver restart (or `.reload trinity_string`).
-- Rollback: DELETE FROM `trinity_string` WHERE `entry` = 512;
