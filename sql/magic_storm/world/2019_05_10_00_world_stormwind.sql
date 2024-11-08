UPDATE creature_template SET gossip_menu_id = 435 WHERE entry = 29712;
UPDATE creature_template_addon SET emote = 379 WHERE entry = 42421;
DELETE FROM creature_addon WHERE guid IN (21011113, 21011114, 20343390, 20343391);
UPDATE creature_template SET AIName = "SmartAI" WHERE entry IN (61839, 61840);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (61839, 61840, -20343390, -20343391) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(61839, 0, 0, 0, 60, 0, 100, 0, 6000, 11000, 6000, 11000, 10, 396, 273, 274, 6, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lucas Severing - On Update - Play Random Emote (396, 273, 274, 6)"),
(61839, 0, 1, 0, 60, 0, 100, 0, 0, 0, 300000, 300000, 11, 79968, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lucas Severing - On Update - Cast Spell Self"),
(61840, 0, 0, 0, 60, 0, 100, 0, 6000, 11000, 6000, 11000, 10, 396, 273, 274, 6, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Naanae - On Update - Play Random Emote (396, 273, 274, 6)"),
(61840, 0, 1, 0, 60, 0, 100, 0, 0, 0, 300000, 300000, 11, 79977, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Naanae - On Update - Cast Spell Self"),
(-20343390, 0, 0, 0, 60, 0, 100, 0, 6000, 11000, 6000, 11000, 10, 396, 273, 274, 6, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Stormwind City Guard - Play Random Emote (396, 273, 274, 6)"),
(-20343391, 0, 0, 0, 60, 0, 100, 0, 6000, 11000, 6000, 11000, 10, 396, 273, 274, 6, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Stormwind City Guard - Play Random Emote (396, 273, 274, 6)");

UPDATE creature SET id = 8666, position_x = -8634.51, position_y = 918.961, position_z = 99.355, orientation = 3.83299, spawntimesecs = 5400, MovementType = 2 WHERE guid = 10651910;
UPDATE creature SET position_x = -8634.51, position_y = 918.961, position_z = 99.355, orientation = 3.83299 WHERE guid = 10651909;

DELETE FROM creature_addon WHERE guid = 10651910;
INSERT INTO creature_addon (guid, path_id, bytes1, bytes2) VALUES
(10651910, 10651910*10, 0, 4097);

DELETE FROM waypoint_data WHERE `id` = 10651910*10;
INSERT INTO waypoint_data (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`) VALUES
(10651910*10, 1, -8641.4, 912.342, 99.1397, 0, 0, 0),
(10651910*10, 2, -8661.71, 894.74, 97.6239, 0, 0, 0),
(10651910*10, 3, -8679.15, 880.967, 97.0168, 0, 0, 0),
(10651910*10, 4, -8681.12, 877.654, 97.0168, 0, 0, 0),
(10651910*10, 5, -8679.29, 873.082, 97.0168, 0, 0, 0),
(10651910*10, 6, -8659.98, 849.329, 97.0168, 0, 0, 0),
(10651910*10, 7, -8639.96, 825.073, 96.6251, 0, 0, 0),
(10651910*10, 8, -8636.74, 813.025, 96.6486, 0, 0, 0),
(10651910*10, 9, -8634, 793.001, 96.6508, 0, 0, 0),
(10651910*10, 10, -8635.94, 785.58, 96.6515, 0, 0, 0),
(10651910*10, 11, -8651.43, 775.162, 96.6714, 0, 0, 0),
(10651910*10, 12, -8661.39, 764.974, 96.6998, 0, 0, 0),
(10651910*10, 13, -8662.58, 758.134, 96.6947, 0, 0, 0),
(10651910*10, 14, -8647.73, 738.576, 96.6965, 0, 0, 0),
(10651910*10, 15, -8630.74, 726.606, 96.7377, 0, 0, 0),
(10651910*10, 16, -8618.88, 711.997, 96.7248, 0, 0, 0),
(10651910*10, 17, -8614.67, 709.545, 96.7549, 0, 0, 0),
(10651910*10, 18, -8606.13, 711.345, 96.7382, 0, 0, 0),
(10651910*10, 19, -8598.07, 712.945, 96.6746, 0, 0, 0),
(10651910*10, 20, -8588.25, 706.887, 97.0168, 0, 0, 0),
(10651910*10, 21, -8566.09, 678.512, 97.0168, 0, 0, 0),
(10651910*10, 22, -8561.86, 674.735, 97.0168, 0, 0, 0),
(10651910*10, 23, -8556.46, 676.784, 97.0168, 0, 0, 0),
(10651910*10, 24, -8542.79, 686.774, 97.6239, 0, 0, 0),
(10651910*10, 25, -8536.45, 686.854, 97.6775, 0, 0, 0),
(10651910*10, 26, -8531.64, 683.194, 98.4422, 0, 0, 0),
(10651910*10, 27, -8524.58, 673.178, 102.5, 0, 0, 0),
(10651910*10, 28, -8519.8, 666.4, 102.615, 0, 0, 0),
(10651910*10, 29, -8512.94, 656.648, 100.901, 0, 0, 0),
(10651910*10, 30, -8513.15, 648.714, 100.292, 0, 0, 0),
(10651910*10, 31, -8518.18, 642.361, 100.092, 0, 0, 0),
(10651910*10, 32, -8538.04, 630.723, 100.404, 0, 0, 0),
(10651910*10, 33, -8554.03, 617.81, 102.053, 0, 0, 0),
(10651910*10, 34, -8564.5, 613.48, 102.435, 0, 0, 0),
(10651910*10, 35, -8576.12, 601.799, 103.26, 0, 0, 0),
(10651910*10, 36, -8582.44, 589.572, 103.691, 0, 0, 0),
(10651910*10, 37, -8586.68, 575.605, 102.985, 0, 0, 0),
(10651910*10, 38, -8585.96, 565.941, 102.26, 0, 0, 0),
(10651910*10, 39, -8578.9, 545.988, 101.779, 0, 0, 0),
(10651910*10, 40, -8581.73, 541.012, 102.09, 0, 0, 0),
(10651910*10, 41, -8590.09, 533.912, 104.76, 0, 0, 0),
(10651910*10, 42, -8598.32, 527.164, 106.399, 0, 0, 0),
(10651910*10, 43, -8605.67, 520.882, 105.748, 0, 0, 0),
(10651910*10, 44, -8610.26, 515.735, 103.79, 0, 0, 0),
(10651910*10, 45, -8613.43, 514.684, 103.401, 0, 0, 0),
(10651910*10, 46, -8618.8, 518.794, 103.068, 0, 0, 0),
(10651910*10, 47, -8635.17, 535.152, 99.9833, 0, 0, 0),
(10651910*10, 48, -8647.39, 546.721, 97.8568, 0, 0, 0),
(10651910*10, 49, -8655.78, 552.938, 96.9435, 0, 0, 0),
(10651910*10, 50, -8671.86, 552.874, 97.2037, 0, 0, 0),
(10651910*10, 51, -8679.66, 549.654, 97.5031, 0, 0, 0),
(10651910*10, 52, -8689.63, 540.268, 97.828, 0, 0, 0),
(10651910*10, 53, -8698.98, 530.295, 97.7173, 0, 0, 0),
(10651910*10, 54, -8712.64, 520.242, 97.2398, 0, 0, 0),
(10651910*10, 55, -8715.24, 521.571, 97.4039, 0, 0, 0),
(10651910*10, 56, -8720.77, 528.729, 99.1496, 0, 0, 0),
(10651910*10, 57, -8729.84, 539.87, 101.105, 0, 0, 0),
(10651910*10, 58, -8735.95, 547.101, 100.845, 0, 0, 0),
(10651910*10, 59, -8745.79, 557.737, 97.7107, 0, 0, 0),
(10651910*10, 60, -8746.01, 564.915, 97.4001, 0, 0, 0),
(10651910*10, 61, -8729.92, 581.294, 97.6775, 0, 0, 0),
(10651910*10, 62, -8719.58, 591.033, 98.4713, 0, 0, 0),
(10651910*10, 63, -8712.04, 594.001, 98.6079, 0, 0, 0),
(10651910*10, 64, -8707.26, 600.676, 98.9982, 0, 0, 0),
(10651910*10, 65, -8704.46, 616.407, 100.215, 0, 0, 0),
(10651910*10, 66, -8705.6, 629.078, 100.477, 0, 0, 0),
(10651910*10, 67, -8708.67, 645.787, 99.9994, 0, 0, 0),
(10651910*10, 68, -8716.46, 666.585, 98.8681, 0, 0, 0),
(10651910*10, 69, -8724.09, 676.482, 98.6317, 0, 0, 0),
(10651910*10, 70, -8728.54, 684.167, 98.7324, 0, 0, 0),
(10651910*10, 71, -8733.47, 695.151, 98.723, 0, 0, 0),
(10651910*10, 72, -8743.6, 709.876, 98.2678, 0, 0, 0),
(10651910*10, 73, -8741.08, 714.561, 98.9815, 0, 0, 0),
(10651910*10, 74, -8734.46, 720.119, 101.647, 0, 0, 0),
(10651910*10, 75, -8726.79, 726.231, 100.924, 0, 0, 0),
(10651910*10, 76, -8718.09, 733.687, 97.9511, 0, 0, 0),
(10651910*10, 77, -8716.42, 737.269, 97.7782, 0, 0, 0),
(10651910*10, 78, -8721.01, 746.752, 97.9693, 0, 0, 0),
(10651910*10, 79, -8730.96, 759.107, 98.1572, 0, 0, 0),
(10651910*10, 80, -8731.99, 769.385, 98.0161, 0, 0, 0),
(10651910*10, 81, -8724.64, 778.108, 98.0604, 0, 0, 0),
(10651910*10, 82, -8717.55, 792.762, 97.1197, 0, 0, 0),
(10651910*10, 83, -8717.68, 798.804, 97.1792, 0, 0, 0),
(10651910*10, 84, -8728.3, 817.744, 96.9777, 0, 0, 0),
(10651910*10, 85, -8726.79, 830.504, 96.3102, 0, 0, 0),
(10651910*10, 86, -8723.42, 841.35, 96.0764, 0, 0, 0),
(10651910*10, 87, -8709.57, 857.779, 96.978, 0, 0, 0),
(10651910*10, 88, -8692.38, 870.557, 97.0284, 0, 0, 0),
(10651910*10, 89, -8679.35, 880.974, 97.0167, 0, 0, 0),
(10651910*10, 90, -8661.22, 896.239, 97.5968, 0, 0, 0),
(10651910*10, 91, -8643.7, 912.233, 98.9288, 0, 0, 0),
(10651910*10, 92, -8634.58, 918.926, 99.3551, 0, 0, 0);

DELETE FROM creature_formations WHERE leaderGUID = 10651910;
INSERT INTO creature_formations (leaderGUID, memberGUID, dist, angle, groupAI) VALUES
(10651910, 10651910, 0, 0, 515),
(10651910, 10651909, 3, 45, 515);

UPDATE creature_template SET AIName = "SmartAI" WHERE entry = 8666;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 8666 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(8666, 0, 0, 0, 1, 0, 100, 0, 1000, 15000, 150000, 180000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Lil Timmy - Out of Combat - Say Line 0");

DELETE FROM creature_text WHERE `CreatureID` = 8666;
INSERT INTO creature_text (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextID`, `TextRange`, `comment`) VALUES
(8666, 0, 0, "Don't worry, I'll find a good home for ya.", 12, 7, 100, 0, 0, 0, 4523, 0, "Lil Timmy"),
(8666, 0, 1, "Can anyone give my adorable, extra little kitty a home?", 12, 7, 100, 0, 0, 0, 4524, 0, "Lil Timmy"),
(8666, 0, 2, "I can't believe dad won't let me keep your sister.", 12, 7, 100, 0, 0, 0, 4525, 0, "Lil Timmy"),
(8666, 0, 3, "Kitten for sale, looking for a good home.", 12, 7, 100, 0, 0, 0, 4526, 0, "Lil Timmy"),
(8666, 0, 4, "What does allergic mean anyway? And what does it have to do with either of my kitties?", 12, 7, 100, 0, 0, 0, 4527, 0, "Lil Timmy"),
(8666, 0, 5, "Will someone please give my little kitten a good home?", 12, 7, 100, 0, 0, 0, 4528, 0, "Lil Timmy");

UPDATE creature SET modelid = 0 WHERE id = 51348;
UPDATE creature SET curhealth = 116235, spawntimesecs = 300 WHERE guid IN (10645750, 10645755);
DELETE FROM creature_addon WHERE guid IN (10645750, 188682, 188683, 188684, 188687, 188689, 188690, 189231, 189310, 10645755);
INSERT INTO creature_addon (guid, path_id, mount, bytes1, bytes2, emote) VALUES
(10645750, 0, 27913, 50331648, 257, 0),
(188682, 0, 27913, 50331648, 257, 0),
(188683, 0, 27913, 50331648, 257, 0),
(188684, 0, 27913, 50331648, 257, 0),
(188687, 0, 27913, 50331648, 257, 0),
(188689, 0, 27913, 50331648, 257, 0),
(188690, 0, 27913, 50331648, 257, 0),
(189231, 0, 27913, 50331648, 257, 0),
(189310, 0, 27913, 50331648, 257, 0),
(10645755, 0, 27913, 50331648, 257, 0);
