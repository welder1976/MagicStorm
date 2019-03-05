UPDATE quest_objectives SET ObjectID = 98486 WHERE QuestID = 40077 AND ID = 280276;
UPDATE creature SET spawndist = 10, MovementType = 1 WHERE id = 94655;
DELETE FROM creature_sparring_template WHERE AttackerEntry IN (98458, 98486, 94704, 93112, 93115, 94654, 101597, 96931, 96278, 102724, 98158, 96277, 108408, 98160, 97034, 96504, 96564, 96402, 98165);
INSERT INTO creature_sparring_template (AttackerEntry, VictimEntry, HealthLimitPct) VALUES
(98458, 98486, 85),
(98486, 98458, 99),
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
UPDATE creature_template SET unit_flags = 570426112 WHERE entry IN (97594, 97712, 98622);
UPDATE creature SET spawndist = 10, MovementType = 1 WHERE id = 99656;
UPDATE creature_template SET unit_flags = 33555202 WHERE entry = 99656;
UPDATE creature SET position_x = 945.594238, position_y = 2877.602051, position_z = -5.200341, orientation = 0.867166 WHERE guid = 20541128 AND id = 97594;
