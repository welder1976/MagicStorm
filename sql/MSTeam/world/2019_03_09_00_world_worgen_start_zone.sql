UPDATE creature_text SET BroadcastTextId = 36124 WHERE CreatureID = 34850 AND GroupID = 0 AND ID = 0;
UPDATE creature_text SET BroadcastTextId = 36125 WHERE CreatureID = 34850 AND GroupID = 1 AND ID = 0;
UPDATE creature_text SET BroadcastTextId = 36126 WHERE CreatureID = 34850 AND GroupID = 2 AND ID = 0;

UPDATE creature_template SET flags_extra = 128 WHERE entry IN (35006, 35010, 35011);

UPDATE gameobject_template_addon SET flags = 36 WHERE entry = 195581;
UPDATE gameobject SET state = 1 WHERE guid = 51003233 AND id = 195581;
UPDATE creature SET spawndist = 1, MovementType = 1 WHERE guid IN (20556383, 20556395, 20556387, 20556391, 20556247, 20556388, 20556390);

UPDATE creature SET spawndist = 15, MovementType = 1 WHERE guid = 20556212;

UPDATE creature SET position_x = -1494.238, position_y = 1336.437, position_z = 58.767, orientation = 1.931 WHERE guid = 20556295 AND id = 34867;
UPDATE creature SET position_x = -1486.67, position_y = 1440.716, position_z = 59.125, orientation = 4.659 WHERE guid = 20556277 AND id = 34867;
UPDATE creature SET position_x = -1507.03, position_y = 1321.1, position_z = 73.285, orientation = 3.377 WHERE guid = 20556264 AND id = 34867;
UPDATE creature SET position_x = -1460.46, position_y = 1370.69, position_z = 64.475, orientation = 6.209 WHERE guid = 20556278 AND id = 34867;
UPDATE creature SET position_x = -1409.69, position_y = 1429.24, position_z = 59.063, orientation = 6.205 WHERE guid = 20556285 AND id = 34867;
UPDATE creature SET position_x = -1430.04, position_y = 1429.107, position_z = 59.057, orientation = 3.169 WHERE guid = 20556269 AND id = 34867;
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (20556292, 20556281, 20556264, 20556242, 20556259);

DELETE FROM creature_addon WHERE guid IN (20556295, 20556277, 20556292, 20556281, 20556264, 20556278, 20556242, 20556285, 20556259, 20556269);
INSERT INTO creature_addon (guid, path_id, bytes1, bytes2, emote, auras) VALUES
(20556295, 0, 8, 1, 0, "39662"),
(20556277, 0, 8, 1, 0, "39662"),
(20556292, 0, 0, 1, 0, "39662"),
(20556281, 0, 0, 1, 0, "39662"),
(20556264, 0, 0, 1, 0, "39662"),
(20556278, 0, 8, 1, 0, "39662"),
(20556242, 0, 0, 1, 0, "39662"),
(20556285, 0, 8, 1, 0, "39662"),
(20556259, 0, 0, 1, 0, "39662"),
(20556269, 0, 8, 1, 0, "39662");

DELETE FROM waypoints WHERE entry IN (348670, 348671, 348672, 348673, 348674);
INSERT INTO waypoints (entry, pointid, position_x, position_y, position_z, point_comment) VALUES
(348670, 1, -1446.26, 1450.723, 71.052, "Shadowy Figure"),
(348670, 2, -1442.63, 1450.802, 71.116, "Shadowy Figure"),
(348670, 3, -1440.51, 1450.681, 71.129, "Shadowy Figure"),
(348670, 4, -1420.51, 1451.143, 71.531, "Shadowy Figure"),
(348670, 5, -1420.19, 1426.869, 72.786, "Shadowy Figure"),
(348670, 6, -1420.51, 1451.143, 71.531, "Shadowy Figure"),
(348670, 7, -1409.75, 1451.397, 72.759, "Shadowy Figure"),
(348670, 8, -1387.74, 1393.781, 73.008, "Shadowy Figure"),
(348670, 9, -1387.15, 1461.186, 73.155, "Shadowy Figure"),
(348670, 10, -1386.907, 1452.008, 72.38, "Shadowy Figure"),
(348670, 11, -1439.54, 1450.702, 71.146, "Shadowy Figure"),
(348670, 12, -1442.58, 1450.784, 71.117, "Shadowy Figure"),
(348670, 13, -1473.61, 1451.046, 72.529, "Shadowy Figure"),
(348670, 14, -1513.61, 1450.771, 71.138, "Shadowy Figure"),
(348670, 15, -1516.12, 1450.739, 71.117, "Shadowy Figure"),
(348670, 16, -1542.02, 1450.936, 71.166, "Shadowy Figure"),
(348670, 17, -1546.29, 1450.326, 71.221, "Shadowy Figure"),
(348670, 18, -1549.32, 1449.374, 71.223, "Shadowy Figure"),
(348670, 19, -1553.46, 1447.001, 71.131, "Shadowy Figure"),
(348670, 20, -1557.25, 1442.885, 71.026, "Shadowy Figure"),
(348670, 21, -1560.56, 1437.244, 70.992, "Shadowy Figure"),
(348670, 22, -1562.48, 1430.272, 70.922, "Shadowy Figure"),
(348670, 23, -1562.16, 1409.398, 71.713, "Shadowy Figure"),
(348670, 24, -1594.14, 1408.106, 72.883, "Shadowy Figure"),
(348670, 25, -1562.22, 1409.616, 71.692, "Shadowy Figure"),
(348670, 26, -1562.39, 1431.084, 70.912, "Shadowy Figure"),
(348670, 27, -1560.53, 1437.465, 70.994, "Shadowy Figure"),
(348670, 28, -1556.66, 1443.712, 71.037, "Shadowy Figure"),
(348670, 29, -1553.18, 1447.33, 71.141, "Shadowy Figure"),
(348670, 30, -1549.11, 1449.37, 71.228, "Shadowy Figure"),
(348670, 31, -1546.61, 1450.35, 71.225, "Shadowy Figure"),
(348670, 32, -1515.92, 1450.72, 71.117, "Shadowy Figure"),
(348670, 33, -1513.99, 1450.71, 71.13, "Shadowy Figure"),
(348670, 34, -1483.33, 1451.37, 72.75, "Shadowy Figure"),
(348670, 35, -1462.11, 1450.82, 71.27, "Shadowy Figure"),

(348671, 1, -1509.6, 1370.527, 73.233, "Shadowy Figure"),
(348671, 2, -1475.03, 1370.223, 71.843, "Shadowy Figure"),
(348671, 3, -1497.173, 1370.632, 73.18, "Shadowy Figure"),
(348671, 4, -1531.706, 1370.083, 71.79, "Shadowy Figure"),

(348672, 1, -1564.72, 1305.238, 73.726, "Shadowy Figure"),
(348672, 2, -1503.081, 1322.301, 73.691, "Shadowy Figure"),
(348672, 3, -1475.027, 1330.954, 71.224, "Shadowy Figure"),
(348672, 4, -1472.984, 1331.376, 70.75, "Shadowy Figure"),
(348672, 5, -1453.404, 1336.731, 70.541, "Shadowy Figure"),
(348672, 6, -1472.984, 1331.376, 70.75, "Shadowy Figure"),
(348672, 7, -1475.027, 1330.954, 71.224, "Shadowy Figure"),
(348672, 8, -1494.097, 1325.942, 71.425, "Shadowy Figure"),
(348672, 9, -1509.355, 1320.472, 73.064, "Shadowy Figure"),

(348673, 1, -1421.93, 1401.452, 71.819, "Shadowy Figure"),
(348673, 2, -1421.27, 1416.098, 64.517, "Shadowy Figure"),
(348673, 3, -1421.18, 1411.319, 63.666, "Shadowy Figure"),
(348673, 4, -1421.408, 1378.319, 73.281, "Shadowy Figure"),
(348673, 5, -1421.539, 1386.362, 71.882, "Shadowy Figure"),

(348674, 1, -1562.106, 1409.314, 71.721, "Shadowy Figure"),
(348674, 2, -1562.505, 1424.309, 71.087, "Shadowy Figure"),
(348674, 3, -1550.897, 1424.948, 67.934, "Shadowy Figure"),
(348674, 4, -1554.607, 1424.859, 67.016, "Shadowy Figure"),
(348674, 5, -1562.159, 1409.213, 71.714, "Shadowy Figure"),
(348674, 6, -1583.951, 1408.604, 71.773, "Shadowy Figure"),
(348674, 7, -1583.977, 1406.82, 70.702, "Shadowy Figure"),
(348674, 8, -1583.578, 1388.626, 70.268, "Shadowy Figure"),
(348674, 9, -1583.587, 1386.165, 70.406, "Shadowy Figure"),
(348674, 10, -1583.345, 1378.357, 71.797, "Shadowy Figure"),
(348674, 11, -1593.268, 1378.017, 72.829, "Shadowy Figure"),
(348674, 12, -1560.139, 1377.728, 72.973, "Shadowy Figure"),
(348674, 13, -1536.4, 1370.865, 62.997, "Shadowy Figure"),
(348674, 14, -1536.334, 1383.368, 61.117, "Shadowy Figure"),
(348674, 15, -1536.364, 1381.037, 60.534, "Shadowy Figure"),
(348674, 16, -1546.873, 1370.736, 64.512, "Shadowy Figure"),
(348674, 17, -1583.402, 1378.333, 71.796, "Shadowy Figure"),
(348674, 18, -1583.493, 1386.039, 70.43, "Shadowy Figure"),
(348674, 19, -1583.528, 1388.739, 70.256, "Shadowy Figure"),
(348674, 20, -1583.938, 1408.432, 71.775, "Shadowy Figure"),
(348674, 21, -1567.825, 1409.405, 71.127, "Shadowy Figure");

UPDATE creature_template SET AIName = "SmartAI" WHERE entry = 34867;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (34867, -20556292, -20556281, -20556264, -20556242, -20556259) AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (34867*100, 34867*100+1, 34867*100+2, 34867*100+3, 34867*100+4, 34867*100+5, 34867*100+6, 34867*100+7, 34867*100+8, 34867*100+9, 34867*100+10) AND `source_type` = 9;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (34867*100+11, 34867*100+12, 34867*100+13, 34867*100+14, 34867*100+15, 34867*100+16, 34867*100+17, 34867*100+18, 34867*100+19) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(-20556292,0,0,0,25,0,100,0,0,0,0,0,53,1,348670,1,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Reset - Start Waypoint"),
(-20556292,0,1,2,40,0,100,0,7,348670,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 7 Reached - Pause Waypoint (2700 ms)"),
(-20556292,0,2,0,61,0,100,0,0,0,0,0,80,34867*100,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 7 Reached (Link) - Start Script"),
(-20556292,0,3,4,40,0,100,0,10,348670,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 10 Reached - Pause Waypoint (2700 ms)"),
(-20556292,0,4,0,61,0,100,0,0,0,0,0,80,34867*100+1,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 10 Reached (Link) - Start Script"),
(-20556292,0,5,6,40,0,100,0,13,348670,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 13 Reached - Pause Waypoint (2700 ms)"),
(-20556292,0,6,0,61,0,100,0,0,0,0,0,80,34867*100+2,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 13 Reached (Link) - Start Script"),
(-20556292,0,7,8,40,0,100,0,34,348670,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 34 Reached - Pause Waypoint (2700 ms)"),
(-20556292,0,8,0,61,0,100,0,0,0,0,0,80,34867*100+3,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 34 Reached (Link) - Start Script"),

(-20556281,0,0,0,25,0,100,0,0,0,0,0,53,1,348671,1,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Reset - Start Waypoint"),
(-20556281,0,1,2,40,0,100,0,1,348671,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 1 Reached - Pause Waypoint (2700 ms)"),
(-20556281,0,2,0,61,0,100,0,0,0,0,0,80,34867*100+4,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 1 Reached (Link) - Start Script"),
(-20556281,0,3,4,40,0,100,0,3,348671,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 3 Reached - Pause Waypoint (2700 ms)"),
(-20556281,0,4,0,61,0,100,0,0,0,0,0,80,34867*100+5,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 3 Reached (Link) - Start Script"),

(-20556264,0,0,0,25,0,100,0,0,0,0,0,53,1,348672,1,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Reset - Start Waypoint"),
(-20556264,0,1,2,40,0,100,0,2,348672,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 2 Reached - Pause Waypoint (2700 ms)"),
(-20556264,0,2,0,61,0,100,0,0,0,0,0,80,34867*100+6,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 2 Reached (Link) - Start Script"),
(-20556264,0,3,4,40,0,100,0,8,348672,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 8 Reached - Pause Waypoint (2700 ms)"),
(-20556264,0,4,0,61,0,100,0,0,0,0,0,80,34867*100+7,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 8 Reached (Link) - Start Script"),

(-20556242,0,0,0,25,0,100,0,0,0,0,0,53,0,348673,1,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Reset - Start Waypoint"),
(-20556242,0,1,2,40,0,100,0,1,348673,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 1 Reached - Pause Waypoint (2700 ms)"),
(-20556242,0,2,0,61,0,100,0,0,0,0,0,80,34867*100+8,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 1 Reached (Link) - Start Script"),
(-20556242,0,3,4,40,0,100,0,2,348673,0,0,54,6000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 2 Reached - Pause Waypoint (6000 ms)"),
(-20556242,0,4,0,61,0,100,0,0,0,0,0,80,34867*100+9,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 2 Reached (Link) - Start Script"),
(-20556242,0,5,6,40,0,100,0,3,348673,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 3 Reached - Pause Waypoint (2700 ms)"),
(-20556242,0,6,0,61,0,100,0,0,0,0,0,80,34867*100+10,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 3 Reached (Link) - Start Script"),
(-20556242,0,7,8,40,0,100,0,4,348673,0,0,54,6000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 4 Reached - Pause Waypoint (6000 ms)"),
(-20556242,0,8,0,61,0,100,0,0,0,0,0,80,34867*100+11,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 4 Reached (Link) - Start Script"),

(-20556259,0,0,0,25,0,100,0,0,0,0,0,53,0,348674,1,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Reset - Start Waypoint"),
(-20556259,0,1,2,40,0,100,0,2,348674,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 2 Reached - Pause Waypoint (2700 ms)"),
(-20556259,0,2,0,61,0,100,0,0,0,0,0,80,34867*100+12,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 2 Reached (Link) - Start Script"),
(-20556259,0,3,4,40,0,100,0,3,348674,0,0,54,14000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 3 Reached - Pause Waypoint (14000 ms)"),
(-20556259,0,4,0,61,0,100,0,0,0,0,0,80,34867*100+13,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 3 Reached (Link) - Start Script"),
(-20556259,0,5,6,40,0,100,0,4,348674,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 4 Reached - Pause Waypoint (2700 ms)"),
(-20556259,0,6,0,61,0,100,0,0,0,0,0,80,34867*100+14,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 4 Reached (Link) - Start Script"),
(-20556259,0,7,8,40,0,100,0,11,348674,0,0,54,14000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 11 Reached - Pause Waypoint (14000 ms)"),
(-20556259,0,8,0,61,0,100,0,0,0,0,0,80,34867*100+15,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 11 Reached (Link) - Start Script"),
(-20556259,0,9,10,40,0,100,0,12,348674,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 12 Reached - Pause Waypoint (2700 ms)"),
(-20556259,0,10,0,61,0,100,0,0,0,0,0,80,34867*100+16,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 12 Reached (Link) - Start Script"),
(-20556259,0,11,12,40,0,100,0,14,348674,0,0,54,14000,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 14 Reached - Pause Waypoint (14000 ms)"),
(-20556259,0,12,0,61,0,100,0,0,0,0,0,80,34867*100+17,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 14 Reached (Link) - Start Script"),
(-20556259,0,13,14,40,0,100,0,15,348674,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 15 Reached - Pause Waypoint (2700 ms)"),
(-20556259,0,14,0,61,0,100,0,0,0,0,0,80,34867*100+18,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 15 Reached (Link) - Start Script"),
(-20556259,0,15,16,40,0,100,0,16,348674,0,0,54,2700,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 16 Reached - Pause Waypoint (2700 ms)"),
(-20556259,0,16,0,61,0,100,0,0,0,0,0,80,34867*100+19,2,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Waypoint 16 Reached (Link) - Start Script"),

(34867*100,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100,9,1,0,0,0,100,0,0,0,0,0,97,20,20,0,0,0,0,1,0,0,0,-1387.127,1452.005,72.38,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+1,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+1,9,1,0,0,0,100,0,0,0,0,0,97,20,20,0,0,0,0,1,0,0,0,-1409.852,1451.495,72.74,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+1,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+2,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+2,9,1,0,0,0,100,0,0,0,0,0,97,20,10,0,0,0,0,1,0,0,0,-1483.831,1451.561,72.65,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+2,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+3,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+3,9,1,0,0,0,100,0,0,0,0,0,97,20,10,0,0,0,0,1,0,0,0,-1473.696,1451.069,72.56,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+3,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+4,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+4,9,1,0,0,0,100,0,0,0,0,0,97,20,10,0,0,0,0,1,0,0,0,-1497.173,1370.632,73.18,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+4,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+5,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+5,9,1,0,0,0,100,0,0,0,0,0,97,20,10,0,0,0,0,1,0,0,0,-1509.436,1370.704,73.279,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+5,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+6,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+6,9,1,0,0,0,100,0,0,0,0,0,97,20,10,0,0,0,0,1,0,0,0,-1494.099,1325.924,71.427,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+6,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+7,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+7,9,1,0,0,0,100,0,0,0,0,0,97,20,10,0,0,0,0,1,0,0,0,-1502.94,1322.25,73.7,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+7,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+8,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+8,9,1,0,0,0,100,0,0,0,0,0,97,20,15,0,0,0,0,1,0,0,0,-1421.19,1409.8,63.4,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+8,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+9,9,0,0,0,0,100,0,1000,1000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,1.559,"Shadowy Figure - On Script - Set Orientation (1.559)"),

(34867*100+10,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+10,9,1,0,0,0,100,0,0,0,0,0,97,20,15,0,0,0,0,1,0,0,0,-1421.94,1401.76,71.81,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+10,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+11,9,0,0,0,0,100,0,1000,1000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,4.783,"Shadowy Figure - On Script - Set Orientation (4.783)"),

(34867*100+12,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+12,9,1,0,0,0,100,0,0,0,0,0,97,20,10,0,0,0,0,1,0,0,0,-1557.18,1424.72,67.07,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+12,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+13,9,0,0,0,0,100,0,1000,1000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.0431,"Shadowy Figure - On Script - Set Orientation (0.0431)"),

(34867*100+14,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+14,9,1,0,0,0,100,0,0,0,0,0,97,20,10,0,0,0,0,1,0,0,0,-1562.2,1424.838,71.068,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+14,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+15,9,0,0,0,0,100,0,1000,1000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,3.1415,"Shadowy Figure - On Script - Set Orientation (3.1415)"),

(34867*100+16,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+16,9,1,0,0,0,100,0,0,0,0,0,97,20,10,0,0,0,0,1,0,0,0,-1546.94,1370.72,64.519,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+16,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+17,9,0,0,0,0,100,0,1000,1000,0,0,66,0,0,0,0,0,0,8,0,0,0,0,0,0,0.4868,"Shadowy Figure - On Script - Set Orientation (0.4868)"),

(34867*100+18,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+18,9,1,0,0,0,100,0,0,0,0,0,97,20,15,0,0,0,0,1,0,0,0,-1536.37,1370.77,62.998,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+18,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off"),

(34867*100+19,9,0,0,0,0,100,0,0,0,0,0,48,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active On"),
(34867*100+19,9,1,0,0,0,100,0,0,0,0,0,97,20,15,0,0,0,0,1,0,0,0,-1559.48,1377.65,73.239,0,"Shadowy Figure - On Script - Jump To Position"),
(34867*100+19,9,2,0,0,0,100,0,2500,2500,0,0,48,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Shadowy Figure - On Script - Set Active Off");

UPDATE `creature_template_addon` SET `bytes1` = 1 WHERE `entry` = 50260;
UPDATE `creature_addon` SET `bytes1` = 1 WHERE `guid` IN (20556260, 20556290, 20556256, 20556272, 20556280, 20556252, 20556267, 20556250);
UPDATE `creature_template` SET `ScriptName` = "npc_gilnean_crow", `InhabitType` = 4 WHERE `entry` = 50260;

DELETE FROM `creature_text` WHERE `CreatureID` IN (44086, 34864, 34851);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `comment`, `BroadcastTextID`) VALUES
(44086, 0, 0, "You can't do this to us!", 12, 0, 33, 5, 0, 0, 'Panicked Citizen OOC', 44377),
(44086, 0, 1, "My children are in Stormglen alone! I have to get to them!", 12, 0, 33, 5, 0, 0, 'Panicked Citizen OOC', 44378),
(44086, 0, 2, "What's going on? I thought I saw something on the rooftops.", 12, 0, 33, 6, 0, 0, 'Panicked Citizen OOC', 44379),
(44086, 0, 3, "We have to get out of here! The howling gets louder every night.", 12, 0, 33, 5, 0, 0, 'Panicked Citizen OOC', 44393),
(44086, 0, 4, "They told us Gilneas City was safe...", 12, 0, 33, 1, 0, 0, 'Panicked Citizen OOC', 44394),
(34864, 0, 0, "No one's allowed to leave. Prince Liam's orders.", 12, 0, 100, 1, 0, 0, 'Gilneas City Guard OOC', 44373),
(34864, 0, 1, "It isn't safe to leave, there's something lurking out there in the woods.", 12, 0, 100, 1, 0, 0, 'Gilneas City Guard OOC', 44374),
(34864, 0, 2, "Stay calm. We have everything under control.", 12, 0, 100, 25, 0, 0, 'Gilneas City Guard OOC', 44375),
(34851, 0, 0, "What... what are those things on the rooftops?", 12, 0, 100, 6, 0, 0, 'Panicked Citizen Quest', 39313),
(34851, 1, 0, "What... what are those things on the rooftops?", 12, 0, 50, 6, 0, 0, 'Panicked Citizen OOC', 39313),
(34851, 1, 1, "Somebody save me!", 12, 0, 50, 22, 0, 0, 'Panicked Citizen OOC', 29078),
(34851, 1, 2, "Spare my life! I will leave this place forever! Please!", 12, 0, 50, 22, 0, 0, 'Panicked Citizen OOC', 28765),
(34851, 1, 3, "You make my children orphans on this day, friend!", 12, 0, 50, 22, 0, 0, 'Panicked Citizen OOC', 28769),
(34851, 1, 4, "The... They promised that we'd be safe...", 12, 0, 50, 1, 0, 0, 'Panicked Citizen OOC', 29085),
(34851, 1, 5, "I'm too young to die!", 12, 0, 50, 22, 0, 0, 'Panicked Citizen OOC', 29086),
(34851, 1, 6, "Ask for anything and you shall have it! Just spare my life!", 12, 0, 50, 22, 0, 0, 'Panicked Citizen OOC', 29088),
(34851, 1, 7, "HELP! HELP!", 12, 0, 50, 22, 0, 0, 'Panicked Citizen OOC', 29074),
(34851, 2, 0, "I won't go down that easy!", 12, 0, 50, 0, 0, 0, 'Panicked Citizen Aggro', 28788),
(34851, 2, 1, "Die Monster dog!DIE!", 12, 0, 50, 0, 0, 0, 'Panicked Citizen Aggro', 28789),
(34851, 2, 2, "Is this the best the Scourge can do?", 12, 0, 50, 0, 0, 0, 'Panicked Citizen Aggro', 28791),
(34851, 2, 3, "You may take my life, but you won't take my freedom!", 12, 0, 50, 0, 0, 0, 'Panicked Citizen Aggro', 28792),
(34851, 2, 4, "DIE!", 12, 0, 50, 0, 0, 0, 'Panicked Citizen Aggro', 28793),
(34851, 2, 5, "You're not taking me without a fight!", 12, 0, 50, 0, 0, 0, 'Panicked Citizen Aggro', 28787);

UPDATE `creature` SET `position_x` = -1559.391602, `position_y` = 1409.633057, `position_z` = 35.955856, `orientation` = 0.015523, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 20556313 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1397.222900, `position_y` = 1402.700439, `position_z` = 35.960724, `orientation` = 1.985302, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 20556332 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1396.093384, `position_y` = 1361.986572, `position_z` = 35.556969, `orientation` = 2.679593, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 20556330 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1581.99939, `position_y` = 1361.849854, `position_z` = 35.99939, `orientation` = 2.079495, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 20556276 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1510.293457, `position_y` = 1371.880249, `position_z` = 35.998672, `orientation` = 4.001499, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 20556291 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1477.174072, `position_y` = 1379.595459, `position_z` = 36.0047, `orientation` = 2.410171, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 20556210 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1524.065918, `position_y` = 1406.494263, `position_z` = 35.555931, `orientation` = 5.669565, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 20556271 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1539.934204, `position_y` = 1440.324463, `position_z` = 35.976925, `orientation` = 3.941693, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 20556327 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1515.657471, `position_y` = 1412.916016, `position_z` = 35.556549, `orientation` = 0.339493, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 20556251 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1476.542114, `position_y` = 1439.608276, `position_z` = 35.961662, `orientation` = 4.066211, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 20556302 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1429.768921, `position_y` = 1439.704102, `position_z` = 35.968613, `orientation` = 2.530758, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 20556331 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1449.906982, `position_y` = 1369.512817, `position_z` = 35.603382, `orientation` = 5.28279, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 20556303 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1471.026123, `position_y` = 1412.337646, `position_z` = 35.556248, `orientation` = 2.424378, `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 20556207 AND `id` = 34851;

UPDATE `creature` SET `position_x` = -1502.160034, `position_y` = 1372.489990, `position_z` = 35.556000, `orientation` = 1.561972, `spawntimesecs` = 30, `spawndist` = 0, `MovementType` = 2 WHERE `guid` = 20556307 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1421.198730, `position_y` = 1414.781616, `position_z` = 36.004940, `orientation` = 1.629003, `spawntimesecs` = 30, `spawndist` = 0, `MovementType` = 2 WHERE `guid` = 20556305 AND `id` = 34851;
UPDATE `creature` SET `position_x` = -1529.618408, `position_y` = 1325.179199, `position_z` = 35.847145, `orientation` = 1.748103, `spawntimesecs` = 30, `spawndist` = 0, `MovementType` = 2 WHERE `guid` = 20556301 AND `id` = 34851;
UPDATE `creature` SET `spawndist` = 0, `MovementType` = 2 WHERE `guid` IN (20556329, 20556325) AND `id` = 34851;

DELETE FROM `creature_template_addon` WHERE `entry` = 34851;
DELETE FROM `creature_addon` WHERE `guid` IN (20556307, 20556305, 20556301, 20556329, 20556325, 20556313, 20556332, 20556330, 20556276, 20556291, 20556210, 20556271);
DELETE FROM `creature_addon` WHERE `guid` IN (20556327, 20556251, 20556302, 20556331, 20556303, 20556207, 20556205, 20556299, 20556312, 20556314, 20556333);
INSERT INTO `creature_addon` (`guid`, `path_id`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(20556307, 205563070, 0, 1, 0, "52385"),
(20556305, 205563050, 0, 1, 0, "52385"),
(20556301, 205563010, 0, 1, 0, "52385"),
(20556329, 205563290, 0, 1, 0, "52385"),
(20556325, 205563250, 0, 1, 0, "52385"),

(20556313, 0, 0, 1, 431, ""),
(20556332, 0, 0, 1, 431, ""),
(20556330, 0, 0, 1, 431, ""),
(20556276, 0, 0, 1, 431, ""),
(20556291, 0, 0, 1, 431, ""),
(20556210, 0, 0, 1, 431, ""),
(20556271, 0, 0, 1, 431, ""),
(20556327, 0, 0, 1, 431, ""),
(20556251, 0, 0, 1, 431, ""),
(20556302, 0, 0, 1, 431, ""),
(20556331, 0, 0, 1, 431, ""),
(20556303, 0, 0, 1, 431, ""),
(20556207, 0, 0, 1, 431, ""),
(20556205, 0, 0, 1, 431, ""),
(20556299, 0, 0, 1, 431, ""),
(20556312, 0, 0, 1, 431, ""),
(20556314, 0, 0, 1, 431, ""),
(20556333, 0, 0, 1, 431, "");

UPDATE `creature_template` SET `AIName` = "SmartAI" WHERE `entry` = 34851;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (34851, -20556307, -20556305, -20556301, -20556313, -20556332, -20556330, -20556276, -20556291, -20556210, -20556271) AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (-20556327, -20556251, -20556302, -20556331, -20556303, -20556207, -20556205, -20556299, -20556312, -20556314, -20556333) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(-20556307, 0, 0, 0, 34, 0, 100, 0, 2, 10, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Reach Point 10 - Despawn (2000 ms)"),
(-20556307, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556305, 0, 0, 0, 34, 0, 100, 0, 2, 6, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Reach Point 6 - Despawn (2000 ms)"),
(-20556305, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556301, 0, 0, 0, 34, 0, 100, 0, 2, 10, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Reach Point 10 - Despawn (2000 ms)"),
(-20556301, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556313, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556313, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556332, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556332, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556330, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556330, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556276, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556276, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556291, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556291, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556210, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556210, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556271, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556271, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556327, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556327, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556251, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556251, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556302, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556302, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556331, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556331, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556303, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556303, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556207, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556207, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556205, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556205, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556299, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556299, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556312, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556312, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556314, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556314, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2"),

(-20556333, 0, 0, 0, 1, 0, 60, 0, 10000, 120000, 60000, 240000, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Update - Say Text Line 1"),
(-20556333, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Panicked Citizen - On Aggro - Say Text Line 2");

DELETE FROM `waypoint_data` WHERE `id` IN (205563070, 205563050, 205563010, 205563290, 205563250);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`) VALUES
(205563070, 1, -1502.16, 1372.49, 35.556, 1.508, 0, 1),
(205563070, 2, -1502.47, 1391.86, 35.556, 1.586, 0, 1),
(205563070, 3, -1508.6, 1395.27, 35.556, 2.635, 0, 1),
(205563070, 4, -1548.69, 1396.63, 35.556, 3.114, 0, 1),
(205563070, 5, -1552.45, 1403.35, 35.556, 2.081, 0, 1),
(205563070, 6, -1552.08, 1420.15, 35.556, 1.559, 0, 1),
(205563070, 7, -1545.65, 1426.27, 35.556, 0.754, 0, 1),
(205563070, 8, -1444.34, 1425.76, 35.556, 6.244, 0, 1),
(205563070, 9, -1432.26, 1420.81, 35.556, 5.894, 0, 1),
(205563070, 10, -1421.06, 1420.63, 35.556, 6.267, 0, 1),
(205563070, 11, -1420.76, 1413.39, 36.519, 4.775, 4000, 1),

(205563050, 1, -1441.17, 1427.62, 35.556, 3.228, 0, 1),
(205563050, 2, -1544.72, 1426.79, 35.556, 3.142, 0, 1),
(205563050, 3, -1551.93, 1422.04, 35.556, 3.754, 0, 1),
(205563050, 4, -1553.96, 1369.56, 35.628, 4.701, 0, 1),
(205563050, 5, -1556.84, 1365.27, 35.623, 4.049, 0, 1),
(205563050, 6, -1583.88, 1364.09, 35.699, 3.22, 0, 1),
(205563050, 7, -1583.53, 1359.91, 36.168, 4.795, 4000, 1),

(205563010, 1, -1528.82, 1334.27, 35.557, 0.357, 0, 1),
(205563010, 2, -1505.82, 1341.25, 35.557, 0.306, 0, 1),
(205563010, 3, -1502.35, 1347.33, 35.557, 1.052, 0, 1),
(205563010, 4, -1504.18, 1423.11, 35.557, 1.567, 0, 1),
(205563010, 5, -1510.72, 1427.18, 35.557, 2.584, 0, 1),
(205563010, 6, -1549.44, 1427.19, 35.557, 3.142, 0, 1),
(205563010, 7, -1553.56, 1420.96, 35.557, 4.127, 0, 1),
(205563010, 8, -1554.42, 1393.65, 35.576, 4.681, 0, 1),
(205563010, 9, -1560.49, 1390.59, 35.758, 3.609, 0, 1),
(205563010, 10, -1568.86, 1391.21, 36.199, 3.067, 0, 1),
(205563010, 11, -1569.41, 1385.65, 36.578, 4.642, 4000, 1),

(205563290, 1, -1557.69, 1332.67, 35.5567, 0, 0, 1),
(205563290, 2, -1511.86, 1343.79, 35.5567, 0, 0, 1),
(205563290, 3, -1502.91, 1353.88, 35.5567, 0, 0, 1),
(205563290, 4, -1503.16, 1382.47, 35.5567, 0, 0, 1),
(205563290, 5, -1497.24, 1393.37, 35.5567, 0, 0, 1),
(205563290, 6, -1438.17, 1394.41, 35.5567, 0, 0, 1),
(205563290, 7, -1435.39, 1419.84, 35.5567, 0, 0, 1),
(205563290, 8, -1405.91, 1418.12, 35.5567, 0, 0, 1),
(205563290, 9, -1402.8, 1390.08, 35.5567, 0, 0, 1),
(205563290, 10, -1424.8, 1365.57, 35.5567, 0, 0, 1),
(205563290, 11, -1517.5, 1337.497, 35.5567, 0, 0, 1),
(205563290, 12, -1566.1, 1320.586, 35.5567, 0, 0, 1),

(205563250, 1, -1553.72, 1391.36, 35.5714, 0, 0, 1),
(205563250, 2, -1540.54, 1393.87, 35.5563, 0, 0, 1),
(205563250, 3, -1522.59, 1394.47, 35.5563, 0, 0, 1),
(205563250, 4, -1507.78, 1394.14, 35.5563, 0, 0, 1),
(205563250, 5, -1501.33, 1401.19, 35.5563, 0, 0, 1),
(205563250, 6, -1500.24, 1413.15, 35.5563, 0, 0, 1),
(205563250, 7, -1495.03, 1424.15, 35.5563, 0, 0, 1),
(205563250, 8, -1485.34, 1426.21, 35.5563, 0, 0, 1),
(205563250, 9, -1457.24, 1426.59, 35.5563, 0, 0, 1),
(205563250, 10, -1443.77, 1424.88, 35.5563, 0, 0, 1),
(205563250, 11, -1437.21, 1418.57, 35.5563, 0, 0, 1),
(205563250, 12, -1437.1, 1397.73, 35.5563, 0, 0, 1),
(205563250, 13, -1442.47, 1373, 35.5563, 0, 0, 1),
(205563250, 14, -1449.17, 1363.34, 35.5563, 0, 0, 1),
(205563250, 15, -1474.46, 1350.9, 35.5563, 0, 0, 1),
(205563250, 16, -1485.1, 1353.39, 35.5671, 0, 0, 1),
(205563250, 17, -1498.15, 1349.15, 35.5563, 0, 0, 1),
(205563250, 18, -1502.75, 1355.02, 35.5563, 0, 0, 1),
(205563250, 19, -1503.81, 1385.74, 35.5563, 0, 0, 1),
(205563250, 20, -1504.02, 1406.74, 35.5563, 0, 0, 1),
(205563250, 21, -1509.41, 1418.46, 35.5563, 0, 0, 1),
(205563250, 22, -1521.87, 1426.38, 35.5563, 0, 0, 1),
(205563250, 23, -1542.9, 1426.84, 35.5563, 0, 0, 1),
(205563250, 24, -1552.11, 1418.9, 35.5563, 0, 0, 1),
(205563250, 25, -1553.31, 1390.12, 35.5692, 0, 0, 1),
(205563250, 26, -1554.03, 1365.39, 35.5678, 0, 0, 1),
(205563250, 27, -1554.34, 1343.59, 35.5567, 0, 0, 1),
(205563250, 28, -1542.8, 1332.8, 35.5567, 0, 0, 1),
(205563250, 29, -1513.19, 1337.98, 35.5567, 0, 0, 1),
(205563250, 30, -1445.78, 1358.5, 35.5567, 0, 0, 1),
(205563250, 31, -1415.59, 1365.53, 35.5567, 0, 0, 1),
(205563250, 32, -1404.28, 1377.33, 35.5567, 0, 0, 1),
(205563250, 33, -1403.22, 1405.88, 35.5567, 0, 0, 1),
(205563250, 34, -1407.56, 1418.01, 35.5567, 0, 0, 1),
(205563250, 35, -1436.94, 1421.79, 35.5567, 0, 0, 1),
(205563250, 36, -1471.4, 1424.21, 35.5567, 0, 0, 1),
(205563250, 37, -1511.17, 1424.79, 35.5567, 0, 0, 1),
(205563250, 38, -1542.08, 1424.6, 35.5567, 0, 0, 1),
(205563250, 39, -1550.87, 1418.47, 35.5567, 0, 0, 1),
(205563250, 40, -1554.48, 1394.58, 35.5781, 0, 0, 1),
(205563250, 41, -1572.96, 1393.06, 36.4174, 0, 0, 1),
(205563250, 42, -1567.68, 1390.9, 36.1181, 0, 0, 1);

UPDATE creature_template SET AIName = "SmartAI" WHERE entry IN (35873, 35869, 44459, 44469);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (35873, 35869, 44459, 44469) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(35873, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 11, 13236, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Celestine of the Harvest - On Reset - Cast Cosmetic Spell"),
(35869, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 11, 45104, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Vitus Darkwalker - On Reset - Cast Cosmetic Spell"),
(44459, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 11, 13236, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Celestine of the Harvest - On Reset - Cast Cosmetic Spell"),
(44469, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 11, 45104, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Vitus Darkwalker - On Reset - Cast Cosmetic Spell");
