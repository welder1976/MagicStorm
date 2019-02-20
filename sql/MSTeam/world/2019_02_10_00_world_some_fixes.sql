DELETE FROM gameobject WHERE guid = 184384 AND id = 142075;
UPDATE creature_addon SET path_id = 1802240, emote = 0 WHERE guid = 180224;
UPDATE creature SET position_x = -9462.29, position_y = 87.8151, position_z = 58.4211, orientation = 0.733038, spawndist = 0, MovementType = 2 WHERE guid = 180224;
UPDATE creature_template SET AIName = "SmartAI" WHERE entry = 2046;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2046 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (2046*100, 2046*100+1) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(2046, 0, 0, 0, 34, 0, 100, 0, 2, 1, 0, 0, 80, 2046*100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Andrew Krighton - On WP 1 Reached - Start Script"),
(2046, 0, 1, 0, 34, 0, 100, 0, 2, 3, 0, 0, 80, 2046*100+1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Andrew Krighton - On WP 3 Reached - Start Script"),
(2046*100, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 2.37365, "Andrew Krighton - On Script - Set Orientation (2.37365)"),
(2046*100, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 17, 233, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Andrew Krighton - On Script - Set Emote State (233)"),
(2046*100, 9, 2, 0, 0, 0, 100, 0, 17000, 17000, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Andrew Krighton - On Script - Set Emote State (0)"),
(2046*100+1, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0.733038, "Andrew Krighton - On Script - Set Orientation (0.733038)");
DELETE FROM waypoint_data WHERE id = 1802240;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, delay) VALUES
(1802240, 1, -9464.05957, 88.933998, 58.3437, 0),
(1802240, 2, -9464.05957, 88.933998, 58.3437, 20000),
(1802240, 3, -9462.29, 87.8151, 58.4211, 0),
(1802240, 4, -9462.29, 87.8151, 58.4211, 60000);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid = 177313 AND id = 46982;
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274684, 274851, 274366, 274636, 274555, 274809, 274883, 274849, 274686, 274374, 274884, 274846, 274691, 274378, 274377);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274770, 274431, 274816, 274815, 274808, 274381, 274380, 274383);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274735, 274369, 274848, 274364, 274885, 274631, 274553, 274633, 274632, 274688, 274733, 274556);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274635, 274685, 274728, 274813, 274371, 274807, 274806, 274774, 274845);
DELETE FROM creature WHERE guid = 318123 AND id = 20075;
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274692, 274850, 274768, 274557, 274854, 274637, 274847, 274810, 274730, 274689);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274551, 274771, 274769, 274729, 274552, 274772, 274773, 274844, 274687);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274640, 274367, 274736, 274554, 274690, 274365, 274638, 274811, 274732, 274370, 274734, 274775, 274778, 274639, 274812);
UPDATE creature_addon SET path_id = 1776330, emote = 0 WHERE guid = 177633;
UPDATE creature SET position_x = -9464.87, position_y = 93.9896, position_z = 58.5271, orientation = 0, spawndist = 0, MovementType = 2 WHERE guid = 177633;
UPDATE creature_template SET AIName = "SmartAI" WHERE entry = 54;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 54 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (54*100, 54*100+1) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(54, 0, 0, 0, 34, 0, 100, 0, 2, 1, 0, 0, 80, 54*100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On WP 1 Reached - Start Script"),
(54, 0, 1, 0, 34, 0, 100, 0, 2, 3, 0, 0, 80, 54*100+1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On WP 3 Reached - Start Script"),
(54*100, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0.811598, "Corina Steele - On Script - Set Orientation (0.811598)"),
(54*100, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 17, 233, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On Script - Set Emote State (233)"),
(54*100, 9, 2, 0, 0, 0, 100, 0, 27000, 27000, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On Script - Set Emote State (0)"),
(54*100+1, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 3.124597, "Corina Steele - On Script - Set Orientation (3.124597)"),
(54*100+1, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 17, 69, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On Script - Set Emote State (69)"),
(54*100+1, 9, 2, 0, 0, 0, 100, 0, 16000, 16000, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On Script - Set Emote State (0)"),
(54*100+1, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0.00000, "Corina Steele - On Script - Set Orientation (0.00000)");
DELETE FROM waypoint_data WHERE id = 1776330;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, delay) VALUES
(1776330, 1, -9460.31543, 101.147774, 58.344334, 0),
(1776330, 2, -9460.31543, 101.147774, 58.344334, 30000),
(1776330, 3, -9464.87, 93.9896, 58.5271, 0),
(1776330, 4, -9464.87, 93.9896, 58.5271, 80000);
DELETE FROM `waypoints` WHERE `entry` = 3848;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES 
(3848, 1, 2954.25,-3215.41,169.205, 'Kayneth Stillwind '),
(3848, 2, 2966.65,-3213.95,168.914, 'Kayneth Stillwind '),
(3848, 3, 2954.23,-3215.45,169.206, 'Kayneth Stillwind ');
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3848 AND `source_type` = 0 AND id >= 1;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3848*100 AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(3848,0,1,0,25,0,100,0,0,0,0,0,53,0,3848,1,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Reset - Start Waypoint"),
(3848,0,2,0,40,0,100,0,1,3848,0,0,54,5000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On WP 1 Reached - Pause Waypoint"),
(3848,0,3,4,40,0,100,0,2,3848,0,0,54,20000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On WP 2 Reached - Pause Waypoint"),
(3848,0,4,0,61,0,100,0,2,3848,0,0,80,3848*100,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On WP 2 Reached - Start Script"),
(3848,0,5,0,40,0,100,0,3,3848,0,0,54,5000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On WP 3 Reached - Pause Waypoint"),
(3848*100,9,0,0,0,0,100,0,3000,3000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.965562,"Kayneth Stillwind - On Script - Set Orientation 0.965562"),
(3848*100,9,1,0,0,0,100,0,3000,3000,0,0,17,69,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Script - Set Emote State 69"),
(3848*100,9,2,0,0,0,100,0,3000,3000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Script - Set Emote State 0"),
(3848*100,9,3,0,0,0,100,0,3000,3000,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Kayneth Stillwind - On Script - Say Line 0");
DELETE FROM `creature_text` WHERE `CreatureID` = 3848;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `Comment`, `BroadcastTextID`) VALUES 
(3848, 0, 0, 'Putting the bottles away, %s sighs.', 16, 0, 100, 0, 0, 0, 'Kayneth Stillwind', 14108);
UPDATE creature SET position_x = 905.604, position_y = -403.547, position_z = -48.7295, spawndist = 0, MovementType = 2 WHERE guid = 321579 AND id = 9056;
DELETE FROM creature_addon WHERE guid = 321579;
INSERT INTO creature_addon (guid, path_id, bytes2) VALUES
(321579, 3215790, 1);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 9056 AND `source_type` = 0 AND `id` >= 5;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (9056*100, 9056*100+1, 9056*100+2) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(9056,0,5,0,34,0,100,0,2,11,0,0,80,9056*100,2,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On WP 11 Reached - Start Script"),
(9056,0,6,0,34,0,100,0,2,18,0,0,80,9056*100+1,2,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On WP 18 Reached - Start Script"),
(9056,0,7,0,34,0,100,0,2,23,0,0,80,9056*100+2,2,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On WP 23 Reached - Start Script"),
(9056*100,9,0,0,0,0,100,0,3000,3000,0,0,17,133,0,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On Script - Set Emote State (133)"),
(9056*100,9,1,0,0,0,100,0,10000,10000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On Script - Set Emote State (0)"),
(9056*100+1,9,0,0,0,0,100,0,3000,3000,0,0,17,173,0,0,0,0,0,0,0,0,0,0,0,0,0,"Fineous Darkvire - On Script - Set Emote State (173)"),
(9056*100+1,9,1,0,0,0,100,0,10000,10000,0,0,17,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On Script - Set Emote State (0)"),
(9056*100+2,9,0,0,0,0,100,0,1000,1000,0,0,90,8,0,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On Script - Set Unit Field Bytes 1 (8)"),
(9056*100+2,9,1,0,0,0,100,0,4000,4000,0,0,91,8,0,0,0,0,0,1,0,0,0,0,0,0,0,"Fineous Darkvire - On Script - Set Unit Field Bytes 1 (0)");
DELETE FROM `waypoint_data` WHERE `id` = 3215790;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`) VALUES 
(3215790, 1, 975.107, -354.152, -69.1219, 0, 0),
(3215790, 2, 984.444, -363.944, -65.9066, 0, 0),
(3215790, 3, 984.38, -372.827, -66.4086, 0, 0),
(3215790, 4, 976.479, -381.185, -63.9267, 0, 0),
(3215790, 5, 962.877, -395.447, -60.8377, 0, 0),
(3215790, 6, 950.417, -408.589, -57.1351, 0, 0),
(3215790, 7, 941.736, -417.331, -55.0396, 0, 0),
(3215790, 8, 931.446, -413.318, -55.3833, 0, 0),
(3215790, 9, 923.409, -403.986, -51.104, 0, 0),
(3215790, 10, 914.499, -394.179, -49.4412, 0, 0),
(3215790, 11, 905.604, -403.547, -48.7295, 0, 0),
(3215790, 12, 905.604, -403.547, -48.7295, 0, 15000),
(3215790, 13, 912.664, -392.947, -49.2744, 0, 0),
(3215790, 14, 918.441, -398, -49.6367, 0, 0),
(3215790, 15, 926.414, -407.296, -52.6216, 0, 0),
(3215790, 16, 930.038, -418.35, -55.3877, 0, 0),
(3215790, 17, 930.151, -424.848, -55.8645, 0, 0),
(3215790, 18, 927.594, -433.715, -56.5236, 0, 0),
(3215790, 19, 927.594, -433.715, -56.5236, 0, 15000),
(3215790, 20, 936.123, -427.94, -56.1072, 0, 0),
(3215790, 21, 939.641, -426.605, -55.7614, 0, 0),
(3215790, 22, 944.406, -426.659, -54.9984, 0, 0),
(3215790, 23, 946.326, -428.053, -54.6023, 0, 0),
(3215790, 24, 946.326, -428.053, -54.6023, 0, 8000),
(3215790, 25, 943.776, -418.105, -54.9468, 0, 0),
(3215790, 26, 947.044, -410.439, -55.9664, 0, 0),
(3215790, 27, 955.107, -403.439, -59.5107, 0, 0),
(3215790, 28, 961.657, -396.63, -60.8377, 0, 0),
(3215790, 29, 968.284, -389.058, -60.8377, 0, 0),
(3215790, 30, 978.015, -379.612, -64.6384, 0, 0),
(3215790, 31, 986.547, -370.816, -66.5624, 0, 0),
(3215790, 32, 974.031, -355.625, -69.1521, 0, 0),
(3215790, 33, 967.597, -349.477, -71.3905, 0, 0),
(3215790, 34, 963.267, -343.735, -71.7394, 0, 0);
UPDATE creature_template_addon SET emote = 214 WHERE entry = 46983;
DELETE FROM creature WHERE guid = 10297;
INSERT INTO creature (guid, id, map, zoneId, areaId, position_x, position_y, position_z, orientation) VALUES
(10297, 46985, 0, 12, 87, -9462.1, 126.892, 58.962, 4.66003);
UPDATE creature_template SET AIName = "SmartAI" WHERE entry = 46983;
DELETE FROM smart_scripts WHERE entryorguid = 46983;
INSERT INTO smart_scripts (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(46983, 0, 0, 0, 1, 0, 100, 0, 1, 1, 10000, 10000, 11, 45425, 0, 0, 0, 0, 0, 10, 10297, 0, 0, 0, 0, 0, 0, "Benjamin Foxworthy - OOC - Cast Spell 'Shot'");
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (179030, 178999, 179035, 179049, 179051, 179047);
DELETE FROM `creature_text` WHERE `CreatureID` IN (49869, 951);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `Comment`, `BroadcastTextID`) VALUES
(49869, 0, 0, "HELP!", 12, 0, 100, 0, 0, 0, 'Stormwind Infantry', 49897),
(49869, 0, 1, "I need a heal!", 12, 0, 100, 0, 0, 0, 'Stormwind Infantry', 49898),
(49869, 0, 2, "I could use a heal, brother!", 12, 0, 100, 0, 0, 0, 'Stormwind Infantry', 49895),
(49869, 0, 3, "Make yourself useful and heal me, Paxton!", 12, 0, 100, 0, 0, 0, 'Stormwind Infantry', 49896),
(49869, 1, 0, "Come, monsters! We will crush you!", 14, 0, 15, 0, 0, 0, 'Stormwind Infantry', 49804),
(49869, 1, 1, "I'll kill a hundred more of your battle worgs!", 14, 0, 15, 0, 0, 0, 'Stormwind Infantry', 49807),
(49869, 1, 2, "Look alive, men! There are orcs and worgs about!", 14, 0, 15, 0, 0, 0, 'Stormwind Infantry', 49809),
(49869, 1, 3, "Your filthy dogs won't be enough!", 14, 0, 15, 0, 0, 0, 'Stormwind Infantry', 49806),
(49869, 1, 4, "Your worgs are no match for the might of Stormwind!", 14, 0, 15, 0, 0, 0, 'Stormwind Infantry', 49808),
(951, 0, 0, 'BY THE LIGHT BE RENEWED!', 12, 0, 100, 0, 0, 0, 'Brother Paxton', 49892),
(951, 0, 1, 'AND I LAY MY HANDS UPON YOU!', 12, 0, 100, 0, 0, 0, 'Brother Paxton', 49890),
(951, 0, 2, 'Let the Holy Light embrace you!', 12, 0, 100, 0, 0, 0, 'Brother Paxton', 49891),
(951, 1, 0, 'Be healed, $gbrother:sister;!', 12, 0, 100, 0, 0, 0, 'Brother Paxton', 49889),
(951, 1, 1, 'FIGHT ON, $GBROTHER:SISTER;!', 12, 0, 100, 0, 0, 0, 'Brother Paxton', 49893);
DELETE FROM `waypoint_data` WHERE `id` = 951*100;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `delay`) VALUES
(951*100, 1, -8833.074, -152.085, 80.349, 0),
(951*100, 2, -8830.21, -146.632, 80.5787, 0),
(951*100, 3, -8826.04, -146.105, 80.6188, 0),
(951*100, 4, -8822.08, -146.456, 80.877, 0),
(951*100, 5, -8817.2, -150.924, 81.5878, 0),
(951*100, 6, -8813.55, -153.822, 82.0283, 8000),
(951*100, 7, -8811.99, -159.78, 81.614, 0),
(951*100, 8, -8810.78, -164.51, 81.306, 0),
(951*100, 9, -8813.96, -163.8, 81.108, 0),
(951*100, 10, -8821.86, -155.42, 80.585, 0),
(951*100, 11, -8833.074, -152.085, 80.349, 0),
(951*100, 12, -8830.21, -146.632, 80.5787, 0),
(951*100, 13, -8826.04, -146.105, 80.6188, 0),
(951*100, 14, -8822.08, -146.456, 80.877, 8000),
(951*100, 15, -8817.2, -150.924, 81.5878, 0),
(951*100, 16, -8813.55, -153.822, 82.0283, 0),
(951*100, 17, -8811.99, -159.78, 81.614, 0),
(951*100, 18, -8810.78, -164.51, 81.306, 0),
(951*100, 19, -8813.96, -163.8, 81.108, 0),
(951*100, 20, -8821.86, -155.42, 80.585, 0),
(951*100, 21, -8833.074, -152.085, 80.349, 8000),
(951*100, 22, -8830.21, -146.632, 80.5787, 0),
(951*100, 23, -8826.04, -146.105, 80.6188, 0),
(951*100, 24, -8822.08, -146.456, 80.877, 0),
(951*100, 25, -8817.2, -150.924, 81.5878, 0),
(951*100, 26, -8813.55, -153.822, 82.0283, 0),
(951*100, 27, -8811.99, -159.78, 81.614, 0),
(951*100, 28, -8810.78, -164.51, 81.306, 0),
(951*100, 29, -8813.96, -163.8, 81.108, 0),
(951*100, 30, -8821.86, -155.42, 80.585, 0),
(951*100, 31, -8833.074, -152.085, 80.349, 8000),
(951*100, 32, -8830.21, -146.632, 80.5787, 0),
(951*100, 33, -8826.04, -146.105, 80.6188, 0),
(951*100, 34, -8822.08, -146.456, 80.877, 0),
(951*100, 35, -8817.2, -150.924, 81.5878, 0),
(951*100, 36, -8813.55, -153.822, 82.0283, 0),
(951*100, 37, -8811.99, -159.78, 81.614, 0),
(951*100, 38, -8810.78, -164.51, 81.306, 8000),
(951*100, 39, -8813.96, -163.8, 81.108, 0),
(951*100, 40, -8821.86, -155.42, 80.585, 0),
(951*100, 41, -8833.074, -152.085, 80.349, 0),
(951*100, 42, -8830.21, -146.632, 80.5787, 0),
(951*100, 43, -8826.04, -146.105, 80.6188, 0),
(951*100, 44, -8822.08, -146.456, 80.877, 0),
(951*100, 45, -8817.2, -150.924, 81.5878, 0),
(951*100, 46, -8813.55, -153.822, 82.0283, 0),
(951*100, 47, -8811.99, -159.78, 81.614, 0),
(951*100, 48, -8810.78, -164.51, 81.306, 8000),
(951*100, 49, -8813.96, -163.8, 81.108, 0),
(951*100, 50, -8821.86, -155.42, 80.585, 0),
(951*100, 51, -8833.074, -152.085, 80.349, 0),
(951*100, 52, -8830.21, -146.632, 80.5787, 0),
(951*100, 53, -8826.04, -146.105, 80.6188, 0),
(951*100, 54, -8822.08, -146.456, 80.877, 0),
(951*100, 55, -8817.2, -150.924, 81.5878, 0),
(951*100, 56, -8813.55, -153.822, 82.0283, 8000),
(951*100, 57, -8811.99, -159.78, 81.614, 0),
(951*100, 58, -8810.78, -164.51, 81.306, 8000),
(951*100, 59, -8813.96, -163.8, 81.108, 0),
(951*100, 60, -8821.86, -155.42, 80.585, 0);
UPDATE creature_template SET unit_flags = 2, flags_extra = 2, AIName = "", ScriptName = "npc_brother_paxton" WHERE entry = 951;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 951 AND `source_type` = 0;
UPDATE creature SET spawndist = 0, MovementType = 2 WHERE guid = 177921 AND id = 951;
UPDATE creature_template_addon SET path_id = 951*100 WHERE entry = 951;
UPDATE creature_template_addon SET emote = 483 WHERE entry IN (197, 42216);
UPDATE creature SET modelid = 32727 WHERE guid = 177877 AND id = 42216;
UPDATE creature_template SET unit_flags2 = 0, AIName = "", ScriptName = "npc_stormwind_injured_soldier" WHERE entry = 50047;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50047 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 50047*100 AND `source_type` = 9;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 50047 AND `SourceEntry` = 93072;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`Comment`) VALUES
(18,50047,93072,0,9,0,28812,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,1,9,0,28813,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,2,9,0,29082,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,3,9,0,28811,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,4,9,0,28810,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,5,9,0,28808,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,6,9,0,28809,0,0,0,'Required quest active for spellclick'),
(18,50047,93072,7,9,0,28806,0,0,0,'Required quest active for spellclick');
DELETE FROM `creature_text` WHERE `CreatureID` = 50039;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `Comment`, `BroadcastTextID`) VALUES
(50039, 0, 0, "We'll kill anybody for the right price!", 12, 0, 50, 0, 0, 0, 'Goblin Assassin', 49837),
(50039, 0, 1, "Time to join your friends, kissin' the dirt!", 12, 0, 50, 0, 0, 0, 'Goblin Assassin', 49838),
(50039, 0, 2, "DIE!!!", 12, 0, 50, 0, 0, 0, 'Goblin Assassin', 49839),
(50039, 0, 3, "We're gonna burn this place to the ground!", 12, 0, 50, 0, 0, 0, 'Goblin Assassin', 49840);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE id = 49874;
UPDATE creature_template SET AIName = "", ScriptName = "" WHERE entry = 49874;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 49874 AND `source_type` = 0;
DELETE FROM creature_template_addon WHERE entry = 49874;
UPDATE creature SET MovementType = 2 WHERE guid IN (178204, 178233) AND id = 49874;
UPDATE creature SET MovementType = 2, ScriptName = "npc_blackrock_spy" WHERE guid IN (178280, 178205, 178460, 178347, 178342, 178345) AND id = 49874;
UPDATE creature SET position_x = -8901.361, position_y = -42.065, position_z = 87.123, orientation = 5.2409 WHERE guid = 178205 AND id = 49874;
UPDATE creature SET position_x = -8826.83, position_y = -77.074, position_z = 86.163, orientation = 5.638 WHERE guid = 178347 AND id = 49874;
UPDATE creature SET position_x = -8837.45, position_y = -119.131, position_z = 80.527, orientation = 5.968 WHERE guid = 178342 AND id = 49874;
UPDATE creature SET position_x = -8822.1, position_y = -102.17, position_z = 84.22, orientation = 6.282 WHERE guid = 178345 AND id = 49874;
DELETE FROM creature_addon WHERE guid IN (178280, 178205, 178204, 178460, 178347, 178242, 178342, 178345, 178341, 178238, 178233);
INSERT INTO creature_addon (guid, path_id, bytes1, bytes2, emote, auras) VALUES
(178280, 178280*10, 0, 1, 0, ""),
(178205, 178205*10, 0, 1, 0, ""),
(178204, 178204*10, 0, 1, 0, "92857"),
(178460, 178460*10, 0, 1, 0, ""),
(178347, 178347*10, 0, 1, 0, ""),
(178242, 0, 8, 1, 0, "80676"),
(178342, 178342*10, 0, 1, 0, ""),
(178345, 178345*10, 0, 1, 0, ""),
(178341, 0, 8, 1, 0, "80676"),
(178238, 0, 8, 1, 0, "80676"),
(178233, 178233*10, 0, 1, 0, "92857");
DELETE FROM `waypoint_data` WHERE `id` IN (178280*10, 178205*10, 178204*10, 178460*10, 178347*10, 178342*10, 178345*10, 178233*10);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`) VALUES
(178280*10, 1, -8928.65, -68.88, 89.889, 0, 0),
(178280*10, 2, -8921.778, -72.851, 88.831, 5.255, 6000),
(178280*10, 3, -8928.65, -68.88, 89.889, 0, 0),
(178280*10, 4, -8936.066, -76.566, 88.553, 5.1663, 6000),
(178205*10, 1, -8901.361, -42.065, 87.123, 0, 0),
(178205*10, 2, -8897.179, -53.335, 87.453, 4.6251, 6000),
(178205*10, 3, -8901.361, -42.065, 87.123, 0, 0),
(178205*10, 4, -8920.372, -45.709, 89.271, 4.8129, 6000),
(178204*10, 1, -8924.495, -42.766, 90.559, 0, 0),
(178204*10, 2, -8937.65, -34.89, 93.718, 0, 0),
(178204*10, 3, -8948.479, -41.211, 91.784, 0, 0),
(178204*10, 4, -8943.493, -47.925, 91.344, 0, 0),
(178204*10, 5, -8916.122, -57.985, 87.113, 0, 0),
(178204*10, 6, -8900.264, -60.966, 86.637, 0, 0),
(178204*10, 7, -8892.944, -48.276, 87.02, 0, 0),
(178204*10, 8, -8906.961, -40.336, 88.76, 0, 0),
(178460*10, 1, -8871.169, -93.006, 82.75, 0, 0),
(178460*10, 2, -8869.837, -109.285, 81.046, 3.5944, 6000),
(178460*10, 3, -8871.169, -93.006, 82.75, 0, 0),
(178460*10, 4, -8883.453, -94.134, 84.15, 3.9439, 6000),
(178347*10, 1, -8826.83, -77.074, 86.163, 0, 0),
(178347*10, 2, -8831.75, -76.168, 85.896, 3.839, 6000),
(178347*10, 3, -8826.83, -77.074, 86.163, 0, 0),
(178347*10, 4, -8822.304, -87.685, 84.942, 3.796, 6000),
(178342*10, 1, -8837.45, -119.131, 80.527, 0, 0),
(178342*10, 2, -8832.41, -121.71, 80.772, 5.097, 6000),
(178342*10, 3, -8837.45, -119.131, 80.527, 0, 0),
(178342*10, 4, -8843.257, -120.911, 80.562, 3.474, 6000),
(178345*10, 1, -8822.1, -102.17, 84.22, 0, 0),
(178345*10, 2, -8816.05, -106.07, 83.22, 4.537, 6000),
(178345*10, 3, -8822.1, -102.17, 84.22, 0, 0),
(178345*10, 4, -8829.87, -104.307, 83.68, 4.827, 6000),
(178233*10, 1, -8852.61, -48.558, 85.448, 0, 0),
(178233*10, 2, -8862.82, -67.132, 85.453, 0, 0),
(178233*10, 3, -8866.67, -76.237, 83.728, 0, 0),
(178233*10, 4, -8850.55, -85.159, 83.776, 0, 0),
(178233*10, 5, -8823.33, -64.355, 88.549, 0, 0),
(178233*10, 6, -8823.47, -26.203, 89.645, 0, 0),
(178233*10, 7, -8834.408, -22.7703, 88.771, 0, 0),
(178233*10, 8, -8839.365, -29.207, 88.939, 0, 0);

DELETE FROM `creature_text` WHERE `CreatureID` = 49874;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `Comment`, `BroadcastTextID`) VALUES
(49874, 0, 0, "Orc KILL $r!", 12, 0, 50, 0, 0, 0, 'Blackrock Spy', 42876),
(49874, 0, 1, "Blackrock take forest!", 12, 0, 50, 0, 0, 0, 'Blackrock Spy', 42879),
(49874, 0, 2, "The grapes were VERY TASTY!", 12, 0, 50, 0, 0, 0, 'Blackrock Spy', 42880),
(49874, 0, 3, "Beg for life!", 12, 0, 50, 0, 0, 0, 'Blackrock Spy', 42877);
-- 179871 179874 179873 179877 179876
