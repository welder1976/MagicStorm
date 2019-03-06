UPDATE quest_objectives SET ObjectID = 98486 WHERE QuestID = 40077 AND ID = 280276;
UPDATE creature SET spawndist = 10, MovementType = 1 WHERE id = 94655;
DELETE FROM creature_sparring_template WHERE AttackerEntry IN (98486, 98484, 94704, 93112, 93115, 94654, 101597, 96931, 96278, 102724, 98158, 96277, 108408, 98160, 97034, 96504, 96564, 96402, 98165);
INSERT INTO creature_sparring_template (AttackerEntry, VictimEntry, HealthLimitPct) VALUES
(98486, 98458, 99),
(98486, 98460, 99),
(98484, 98460, 99),
(94704, 93112, 85),
(94704, 93115, 85),
(94704, 94654, 85),
(93112, 94704, 75),
(93115, 94704, 75),
(94654, 94704, 75),
(101597, 96278, 85),
(101597, 102724, 85),
(96931, 96278, 85),
(96931, 102724, 85),
(96278, 101597, 75),
(102724, 101597, 75),
(96278, 96931, 75),
(102724, 96931, 75),
(98158, 96277, 85),
(96277, 98158, 75),
(108408, 96277, 85),
(108408, 97034, 85),
(98160, 96277, 85),
(98160, 97034, 85),
(96277, 108408, 75),
(96277, 98160, 75),
(97034, 108408, 75),
(97034, 98160, 75),
(96504, 96402, 85),
(96564, 96402, 85),
(96402, 96504, 75),
(96402, 96564, 75),
(98165, 96277, 85),
(96277, 98165, 75),
(96504, 96277, 85),
(96504, 97034, 85),
(96277, 96504, 75),
(97034, 96504, 75);

UPDATE creature_template SET AIName = "SmartAI" WHERE entry IN (98158, 98165);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (98158, 98165) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(98158, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 19, 96277, 15, 0, 0, 0, 0, 0, "Asha Ravensong - On Reset - Start Attack (Creature ID: 96277, 15 Yards)"),
(98158, 0, 1, 0, 1, 0, 100, 0, 10000, 10000, 10000, 10000, 49, 0, 0, 0, 0, 0, 0, 19, 96277, 15, 0, 0, 0, 0, 0, "Asha Ravensong - OOC (10000 - 10000) - Start Attack (Creature ID: 96277, 15 Yards)"),
(98165, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 19, 96277, 15, 0, 0, 0, 0, 0, "Cassiel Nightthorn - On Reset - Start Attack (Creature ID: 96277, 15 Yards)"),
(98165, 0, 1, 0, 1, 0, 100, 0, 10000, 10000, 10000, 10000, 49, 0, 0, 0, 0, 0, 0, 19, 96277, 15, 0, 0, 0, 0, 0, "Cassiel Nightthorn - OOC (10000 - 10000) - Start Attack (Creature ID: 96277, 15 Yards)");

DELETE FROM creature_addon WHERE guid = 20549011;
INSERT INTO creature_addon (guid, path_id, bytes2) VALUES
(20549011, 20549011, 1);
UPDATE creature SET spawndist = 0, MovementType = 2 WHERE guid = 20540913;
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (20541015, 20541043);
UPDATE creature SET position_x = 1074.612305, position_y = 3125.019531, position_z = 31.446449, orientation = 0.536801 WHERE guid = 20541015;
UPDATE creature SET position_x = 1081.16333, position_y = 3114.010986, position_z = 31.446449, orientation = 0.536801 WHERE guid = 20541043;
UPDATE creature_addon SET path_id = 20540913 WHERE guid = 20540913;
DELETE FROM waypoint_data WHERE id = 20540913;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, move_type) VALUES
(20540913, 1, 1140.142944, 3140.919189, 48.3554, 1),
(20540913, 2, 1202.097046, 3058.875488, 29.3603, 1),
(20540913, 3, 1181.62146, 2958.30957, 29.6688, 1),
(20540913, 4, 1097.11401, 2867.13012, 37.8043, 1),
(20540913, 5, 1034.884766, 2858.310547, 50.311005, 1),
(20540913, 6, 936.68927, 2901.73999, 33.559097, 1),
(20540913, 7, 902.931274, 3007.135742, 21.924053, 1),
(20540913, 8, 957.74231, 3093.853271, 25.616138, 1),
(20540913, 9, 1050.824463, 3112.443604, 36.758327, 1),
(20540913, 10, 1078.264771, 3120.937012, 31.737566, 1);
DELETE FROM creature_formations WHERE leaderGUID = 20540913;
INSERT INTO creature_formations (leaderGUID, memberGUID, dist, angle, groupAI) VALUES
(20540913, 20540913, 0, 0, 515),
(20540913, 20541015, 3, 135, 515),
(20540913, 20541043, 3, 225, 515);
UPDATE creature_template SET unit_flags = 570426112 WHERE entry IN (97594, 97712, 98622, 97592);
UPDATE creature SET spawndist = 10, MovementType = 1 WHERE id = 99656;
UPDATE creature_template SET unit_flags = 33555202 WHERE entry = 99656;
UPDATE creature SET position_x = 945.594238, position_y = 2877.602051, position_z = -5.200341, orientation = 0.867166 WHERE guid = 20541128 AND id = 97594;
UPDATE creature_template SET DamageModifier = 4 WHERE entry = 98460;
UPDATE creature_template SET DamageModifier = 2 WHERE entry = 98458;
DELETE FROM waypoint_data WHERE id IN (20541053, 20541062);
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, move_type) VALUES
(20541053, 1, 987.824, 3065.82, -10.2226, 1),
(20541053, 2, 1007.28, 3071.19, -5.23683, 1),
(20541053, 3, 1038.66, 3075.08, -3.45688, 1),
(20541053, 4, 1052.28, 3057.13, -7.92438, 1),
(20541053, 5, 1027.66, 3059.17, -6.2193, 1),
(20541053, 6, 1000.07, 3059.66, -8.39689, 1),
(20541053, 7, 985.34, 3033.708, -13.304, 1),
(20541053, 8, 977.36, 3035.561, -13.933, 1),
(20541053, 9, 984.291, 3055.302, -11.248, 1),
(20541053, 10, 989.497, 3064.867, -9.782, 1),
(20541062, 1, 1003.63, 2903.45, -3.1701, 1),
(20541062, 2, 999.71, 2871.21, 4.154523, 1),
(20541062, 3, 976.265, 2874.51, 0.765384, 1),
(20541062, 4, 968.617, 2924.81, -10.5328, 1),
(20541062, 5, 985.578, 2926.59, -9.30305, 1),
(20541062, 6, 1000.152, 2908.08, -4.2131, 1);
UPDATE creature SET spawndist = 0, MovementType = 2 WHERE guid = 20540952 AND id = 98484;
DELETE FROM creature_addon WHERE guid = 20540952;
INSERT INTO creature_addon (guid, path_id, bytes2) VALUES
(20540952, 20540952, 1);
DELETE FROM waypoint_data WHERE id = 20540952;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z) VALUES
(20540952, 1, 1148.568, 3105.49, -10.5744),
(20540952, 2, 1164.705, 3075.21, -14.1472),
(20540952, 3, 1143.868, 3047.14, -15.2349),
(20540952, 4, 1127.461, 3038.11, -16.3809),
(20540952, 5, 1103.789, 3062.13, -10.3401),
(20540952, 6, 1100.759, 3080.68, -10.4846),
(20540952, 7, 1145.86, 3103.505, -10.5034);
UPDATE creature SET spawndist = 0, MovementType = 2 WHERE guid = 20540990 AND id = 98486;
DELETE FROM creature_addon WHERE guid = 20540990;
INSERT INTO creature_addon (guid, path_id, bytes2) VALUES
(20540990, 20540990, 1);
DELETE FROM waypoint_data WHERE id = 20540990;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z) VALUES
(20540990, 1, 1178.935, 3133.77, -10.9478),
(20540990, 2, 1165.222, 3123.54, -10.5311),
(20540990, 3, 1177.422, 3134.48, -10.2481),
(20540990, 4, 1217.249, 3138.13, -19.4444),
(20540990, 5, 1200.377, 3133.42, -14.736);
DELETE FROM creature_formations WHERE leaderGUID = 20540990;
INSERT INTO creature_formations (leaderGUID, memberGUID, dist, angle, groupAI) VALUES
(20540990, 20540990, 0, 0, 515),
(20540990, 20540997, 2, 90, 515);
UPDATE creature SET spawntimesecs = 30 WHERE guid IN (20540975, 20549011, 20549012, 20543737);
UPDATE creature SET spawndist = 0, MovementType = 2 WHERE guid = 20540993 AND id = 98486;
DELETE FROM creature_addon WHERE guid = 20540993;
INSERT INTO creature_addon (guid, path_id, bytes2) VALUES
(20540993, 20540993, 1);
DELETE FROM waypoint_data WHERE id = 20540993;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z) VALUES
(20540993, 1, 875.866, 2972.73, -31.1527),
(20540993, 2, 874.645, 3005.93, -32.6863);
DELETE FROM creature_formations WHERE leaderGUID = 20540993;
INSERT INTO creature_formations (leaderGUID, memberGUID, dist, angle, groupAI) VALUES
(20540993, 20540993, 0, 0, 515),
(20540993, 20541058, 2, 270, 515);
DELETE FROM smart_scripts WHERE entryorguid IN (9848401, 9848402, 9848403, 9848404) AND source_type = 9;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 98484 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(98484, 0, 0, 0, 26, 0, 75, 1, 1, 15, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Mo'arg Brute - In Combat LoS - Say Text Line 0 (No Repeat)"),
(98484, 0, 1, 0, 0, 0, 100, 0, 1000, 2000, 8000, 9000, 11, 200425, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Mo'arg Brute - In Combat - Cast 'Brutal Slam'");
DELETE FROM creature_text WHERE CreatureID = 98484;
INSERT INTO creature_text (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(98484, 0, 0, "For Tyranna!", 12, 0, 100, 0, 0, 55329, 97906, 0, "Mo'arg Brute"),
(98484, 0, 1, "I'm going to destroy you, little demon hunter.", 12, 0, 100, 0, 0, 55325, 97907, 0, "Mo'arg Brute"),
(98484, 0, 2, "Taste my blade.", 12, 0, 100, 0, 0, 55327, 97902, 0, "Mo'arg Brute"),
(98484, 0, 3, "Ahahahahaha! I will cut you down.", 12, 0, 100, 0, 0, 55326, 94883, 0, "Mo'arg Brute"),
(98484, 0, 4, "Pitiful.", 12, 0, 100, 0, 0, 55324, 97904, 0, "Mo'arg Brute"),
(98484, 0, 5, "Die in glorious battle!", 12, 0, 100, 0, 0, 55330, 97905, 0, "Mo'arg Brute"),
(98484, 0, 6, "Hunt this.", 12, 0, 100, 0, 0, 55328, 97903, 0, "Mo'arg Brute"),
(98484, 0, 7, "Death comes soon.", 12, 0, 100, 0, 0, 55332, 97901, 0, "Mo'arg Brute");
DELETE FROM smart_scripts WHERE entryorguid IN (9848601, 9848602, 9848603, 9848604, 9848605, 9848606) AND source_type = 9;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 98486 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(98486, 0, 0, 0, 26, 0, 75, 1, 0, 10, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Wrath Warrior - In Combat LoS - Say Text Line 0 (No Repeat)");
DELETE FROM creature_text WHERE CreatureID = 98486;
INSERT INTO creature_text (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(98486, 0, 0, "You are outmatched and outnumbered.", 12, 0, 100, 0, 0, 55367, 98764, 0, "Wrath Warrior"),
(98486, 0, 1, "This is where you die.", 12, 0, 100, 0, 0, 55362, 98765, 0, "Wrath Warrior"),
(98486, 0, 2, "You will not gain the keystone.", 12, 0, 100, 0, 0, 55365, 98761, 0, "Wrath Warrior"),
(98486, 0, 3, "I live to serve.", 12, 0, 100, 0, 0, 55363, 98759, 0, "Wrath Warrior"),
(98486, 0, 4, "My life for the Legion.", 12, 0, 100, 0, 0, 55364, 98760, 0, "Wrath Warrior"),
(98486, 0, 5, "We will cleanse the universe in fire.", 12, 0, 100, 0, 0, 55370, 98766, 0, "Wrath Warrior"),
(98486, 0, 6, "Brood Queen Tyranna orders your death.", 12, 0, 100, 0, 0, 55366, 98762, 0, "Wrath Warrior"),
(98486, 0, 7, "My blade will cut through you.", 12, 0, 100, 0, 0, 55371, 98763, 0, "Wrath Warrior");
UPDATE creature SET spawndist = 10, MovementType = 1 WHERE id = 99650;
UPDATE gameobject_template SET Data10 = 0 WHERE entry = 244439;
UPDATE smart_scripts SET link = 0 WHERE entryorguid = 244439 AND id = 1 AND source_type = 1;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 94410 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(94410, 0, 0, 0, 10, 0, 100, 0, 1, 20, 500, 500, 11, 188437, 2, 0, 0, 0, 0, 11, 105316, 20, 0, 0, 0, 0, 0, "Allari the Souleater - Within 1-20 Range Out of Combat LoS - Cast 'Channeling cosmetic on npc Questioner Arev'naal"),
(94410, 0, 1, 2, 1, 0, 100, 512, 1000, 1000, 1000, 1000, 33, 94410, 0, 0, 0, 0, 0, 18, 10, 0, 0, 0, 0, 0, 0, "Allari the Souleater - Out of Combat - Quest Credit"),
(94410, 0, 2, 3, 61, 0, 100, 512, 1000, 1000, 1000, 1000, 33, 101534, 0, 0, 0, 0, 0, 18, 10, 0, 0, 0, 0, 0, 0, "Allari the Souleater - Out of Combat - Quest Credit"),
(94410, 0, 3, 0, 61, 0, 100, 512, 1000, 1000, 1000, 1000, 33, 97831, 0, 0, 0, 0, 0, 18, 10, 0, 0, 0, 0, 0, 0, "Allari the Souleater - Out of Combat - Quest Credit"),
(94410, 0, 4, 0, 10, 0, 100, 1, 1, 30, 25000, 25000, 1, 0, 5000, 0, 0, 0, 0, 18, 30, 0, 0, 0, 0, 0, 0, "Allari the Souleater - Within 1-30 Range Out of Combat LoS - Say Line 0 (No Repeat)"),
(94410, 0, 5, 0, 19, 0, 100, 0, 39049, 0, 0, 0, 1, 1, 5000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Allari the Souleater - On Quest 'Eye On the Prize' Taken - Say Line 1");
UPDATE creature_text SET BroadcastTextId = 96676 WHERE CreatureID = 94410 AND GroupID = 1 AND ID = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 98497 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(98497, 0, 0, 0, 26, 0, 100, 1, 1, 15, 0, 0, 87, 9849701, 9849702, 9849703, 9849704, 9849705, 9849706, 1, 0, 0, 0, 0, 0, 0, 0, "Imp Mother - In Combat LoS - Run Random Script (No Repeat)"),
(98497, 0, 1, 0, 0, 0, 100, 0, 2000, 3000, 15000, 18000, 11, 200393, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, "Imp Mother - In Combat - Cast 'Rain of Imp'"),
(98497, 0, 2, 0, 0, 0, 100, 1, 1000, 1000, 5000, 5000, 1, 2, 2000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Imp Mother - In Combat - Say Line 2 (No Repeat)"),
(98497, 0, 3, 0, 2, 0, 100, 1, 30, 40, 0, 0, 1, 4, 5000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Imp Mother - Between 30-40% Health - Say Line 4 (No Repeat)"),
(98497, 0, 4, 0, 1, 0, 100, 1, 1000, 1000, 1000, 1000, 11, 188485, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Imp Mother - Out of Combat - Cast 'Fel Channelling' (No Repeat)"),
(98497, 0, 5, 0, 7, 0, 100, 1, 0, 0, 0, 0, 11, 188485, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Imp Mother - On Evade - Cast 'Fel Channelling' (No Repeat)");
