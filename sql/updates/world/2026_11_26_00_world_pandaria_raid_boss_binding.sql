-- Bind the remaining SoO/ToT encounter templates whose ScriptName was still empty.
-- Verified against x_world_pnd: all other raid bosses and both instance_template
-- controllers were already bound correctly and are intentionally left untouched.
-- Only existing rows are updated: no stats, spawns, loot or new templates.
-- Plain UPDATEs: safe to apply more than once.

-- Throne of Thunder (map 1098)
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_primordius'   WHERE `entry` = 69017;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_kazrajin'     WHERE `entry` = 69134;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_dark_animus'  WHERE `entry` = 69427;

-- Siege of Orgrimmar (map 1136)
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_immerseus'    WHERE `entry` = 71543;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_sha_of_pride' WHERE `entry` = 71734;
