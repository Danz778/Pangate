-- Profession boost items (110100, 110101) wired to battle_pay_boost_profession script.
-- Idempotent: safe to run multiple times.
DELETE FROM `item_template` WHERE `entry` IN (110100, 110101);
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`, `name`, `displayid`, `Quality`, `Flags`, `FlagsExtra`, `Flags3`, `Unk430_1`, `Unk430_2`, `BuyCount`, `BuyPrice`, `SellPrice`, `InventoryType`, `AllowableClass`, `AllowableRace`, `ItemLevel`, `RequiredLevel`, `RequiredSkill`, `RequiredSkillRank`, `requiredspell`, `requiredhonorrank`, `RequiredCityRank`, `RequiredReputationFaction`, `RequiredReputationRank`, `maxcount`, `stackable`, `ContainerSlots`, `stat_type1`, `stat_value1`, `stat_unk1_1`, `stat_unk2_1`, `stat_type2`, `stat_value2`, `stat_unk1_2`, `stat_unk2_2`, `stat_type3`, `stat_value3`, `stat_unk1_3`, `stat_unk2_3`, `stat_type4`, `stat_value4`, `stat_unk1_4`, `stat_unk2_4`, `stat_type5`, `stat_value5`, `stat_unk1_5`, `stat_unk2_5`, `stat_type6`, `stat_value6`, `stat_unk1_6`, `stat_unk2_6`, `stat_type7`, `stat_value7`, `stat_unk1_7`, `stat_unk2_7`, `stat_type8`, `stat_value8`, `stat_unk1_8`, `stat_unk2_8`, `stat_type9`, `stat_value9`, `stat_unk1_9`, `stat_unk2_9`, `stat_type10`, `stat_value10`, `stat_unk1_10`, `stat_unk2_10`, `ScalingStatDistribution`, `ScalingStatValue`, `DamageType`, `delay`, `RangedModRange`, `spellid_1`, `spelltrigger_1`, `spellcharges_1`, `spellcooldown_1`, `spellcategory_1`, `spellcategorycooldown_1`, `spellid_2`, `spelltrigger_2`, `spellcharges_2`, `spellcooldown_2`, `spellcategory_2`, `spellcategorycooldown_2`, `spellid_3`, `spelltrigger_3`, `spellcharges_3`, `spellcooldown_3`, `spellcategory_3`, `spellcategorycooldown_3`, `spellid_4`, `spelltrigger_4`, `spellcharges_4`, `spellcooldown_4`, `spellcategory_4`, `spellcategorycooldown_4`, `spellid_5`, `spelltrigger_5`, `spellcharges_5`, `spellcooldown_5`, `spellcategory_5`, `spellcategorycooldown_5`, `bonding`, `description`, `PageText`, `LanguageID`, `PageMaterial`, `startquest`, `lockid`, `Material`, `sheath`, `RandomProperty`, `RandomSuffix`, `itemset`, `MaxDurability`, `area`, `Map`, `BagFamily`, `TotemCategory`, `socketColor_1`, `socketContent_1`, `socketColor_2`, `socketContent_2`, `socketColor_3`, `socketContent_3`, `socketBonus`, `GemProperties`, `RequiredDisenchantSkill`, `ArmorDamageModifier`, `AllowableGender`, `RequiredPlayerRank`, `RequiredArenaTeamRank`, `RequiredArenaTeamRating`, `RequiredReputationFaction2`, `RequiredReputationRank2`, `RequiredReputationFaction3`, `RequiredReputationRank3`, `RequiredReputationFaction4`, `RequiredReputationRank4`, `RequiredReputationFaction5`, `RequiredReputationRank5`, `DisenchantId`, `ExtendedCost`, `DisenchantMinSkill`, `DisenchantMinLevel`, `DisenchantMaxLevel`, `DisenchantChance`, `DisenchantProbability`, `DisenchantId2`, `ExtendedCost2`, `DisenchantMinSkill2`, `DisenchantMinLevel2`, `DisenchantMaxLevel2`, `DisenchantChance2`, `DisenchantProbability2`) VALUES
(110100,0,1,-1,"Profession Boost",30658,4,64,0,0,1,1,1,0,0,0,-1,-1,1,32,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12883,0,-1,0,4,60000,0,0,0,-1,0,-1,0,0,0,-1,0,-1,0,0,0,-1,0,-1,0,0,0,-1,0,-1,1,"Gives you the ability to level up any of your professions to 600.",0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),
(110101,0,1,-1,"Minor Profession Boost",30658,4,64,0,0,1,1,1,0,0,0,-1,-1,1,32,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12883,0,-1,0,4,60000,0,0,0,-1,0,-1,0,0,0,-1,0,-1,0,0,0,-1,0,-1,0,0,0,-1,0,-1,1,"Gives you the opportunity to level up any of your professions to the maximum available (75, 150, 225, 300, 375, 450, 525 or 600 points).",0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

DELETE FROM `item_script_names` WHERE `Id` IN (110100, 110101);
INSERT INTO `item_script_names` (`Id`, `ScriptName`) VALUES
(110100,"battle_pay_boost_profession"),
(110101,"battle_pay_boost_profession_small");

DELETE FROM `npc_text` WHERE `ID` IN (20010, 20011);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(20010,"Greetings $n, you have just obtained the profession boost item which allows you to raise any profession you have to level 600.","Greetings $n, you have just obtained the profession boost item which allows you to raise any profession you have to level 600."),
(20011,"Greetings $n, you have just obtained the minor profession boost item which allows you to raise any profession you have to the maximum level available (75, 150, 225, 300, 375, 450, 525 or 600 points).","Greetings $n, you have just obtained the minor profession boost item which allows you to raise any profession you have to the maximum level available (75, 150, 225, 300, 375, 450, 525 or 600 points).");

DELETE FROM `gossip_menu_option` WHERE `menu_id` = 51002;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_text_female`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`, `box_text_female`) VALUES
(51002,0,3,"|TInterface/Icons/spell_holy_sealofsacrifice:28|t First Aid",NULL,0,1,0,0,0,0,NULL,NULL),
(51002,1,3,"|TInterface/Icons/trade_blacksmithing:28|t Blacksmithing",NULL,1,1,0,0,0,0,NULL,NULL),
(51002,2,3,"|TInterface/Icons/inv_misc_armorkit_17:28|t Leatherworking",NULL,2,1,0,0,0,0,NULL,NULL),
(51002,3,3,"|TInterface/Icons/trade_alchemy:28|t Alchemy",NULL,3,1,0,0,0,0,NULL,NULL),
(51002,4,3,"|TInterface/Icons/spell_nature_naturetouchgrow:28|t Herbalism",NULL,4,1,0,0,0,0,NULL,NULL),
(51002,5,3,"|TInterface/Icons/inv_misc_food_15:28|t Cooking",NULL,5,1,0,0,0,0,NULL,NULL),
(51002,6,3,"|TInterface/Icons/trade_mining:28|t Mining",NULL,6,1,0,0,0,0,NULL,NULL),
(51002,7,3,"|TInterface/Icons/trade_tailoring:28|t Tailoring",NULL,7,1,0,0,0,0,NULL,NULL),
(51002,8,3,"|TInterface/Icons/trade_engineering:28|t Engineering",NULL,8,1,0,0,0,0,NULL,NULL),
(51002,9,3,"|TInterface/Icons/trade_engraving:28|t Enchanting",NULL,9,1,0,0,0,0,NULL,NULL),
(51002,10,3,"|TInterface/Icons/trade_fishing:28|t Fishing",NULL,10,1,0,0,0,0,NULL,NULL),
(51002,11,3,"|TInterface/Icons/inv_misc_pelt_wolf_01:28|t Skinning",NULL,11,1,0,0,0,0,NULL,NULL),
(51002,12,3,"|TInterface/Icons/inv_misc_gem_01:28|t Jewelcrafting",NULL,12,1,0,0,0,0,NULL,NULL),
(51002,13,3,"|TInterface/Icons/inv_inscription_tradeskill01:28|t Inscription",NULL,13,1,0,0,0,0,NULL,NULL),
(51002,14,3,"|TInterface/Icons/trade_archaeology:28|t Archaeology",NULL,14,1,0,0,0,0,NULL,NULL);
