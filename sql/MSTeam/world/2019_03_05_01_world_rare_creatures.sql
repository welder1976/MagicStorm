DELETE FROM creature WHERE guid IN (3906, 10298, 10299, 10300, 302508) AND id = 100;
DELETE FROM creature WHERE guid IN (5401, 10301, 10302, 10303, 10304, 53205, 74373, 74377) AND id = 61;
INSERT INTO creature (guid, id, map, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(3906, 100, 0, -9945.86, 620.999, 37.2259, 4.03808, 28800, 10, 1),
(10298, 100, 0, -9981.18, 566.13, 37.6634, 1.37087, 28800, 10, 1),
(10299, 100, 0, -10067.1, 629.856, 39.2659, 0.67265, 28800, 10, 1),
(10300, 100, 0, -10017, 654.089, 37.149, 5.412, 28800, 10, 1),
(302508, 100, 0, -9930.88, 573.464, 37.0119, 2.35969, 28800, 10, 1),

(5401, 61, 0, -9303.19, -292.231, 70.63, 3.22, 28800, 0, 2),
(10301, 61, 0, -9326.41, 543.283, 75.0137, 4.24072, 28800, 0, 0),
(10302, 61, 0, -9282.06, 520.888, 85.7024, 4.49441, 28800, 0, 0),
(10303, 61, 0, -9261.58, 465.959, 83.0718, 3.914, 28800, 0, 0),
(10304, 61, 0, -9859.07, -217.846, 36.3363, 3.8606, 28800, 0, 0),
(53205, 61, 0, -9862.97, -232.651, 35.444, 0.579991, 28800, 0, 0),
(74373, 61, 0, -9778.61, -1586.34, 41.3725, 4.474, 28800, 0, 0),
(74377, 61, 0, -9783.59, -1567.96, 41.8322, 0.732357, 28800, 0, 0);

DELETE FROM creature_template_addon WHERE entry = 61;
DELETE FROM `creature_addon` WHERE `guid` IN (5401, 10301, 10302, 10303, 10304, 53205, 74373, 74377);
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(5401, 5401*10, 0, 0, 1, 0, ""),
(10301, 0, 0, 1, 1, 0, ""),
(10302, 0, 0, 1, 1, 0, ""),
(10303, 0, 0, 1, 1, 0, ""),
(10304, 0, 0, 1, 1, 0, ""),
(53205, 0, 0, 1, 1, 0, ""),
(74373, 0, 0, 1, 1, 0, ""),
(74377, 0, 0, 1, 1, 0, "");

DELETE FROM waypoint_data WHERE id = 5401*10;
INSERT INTO waypoint_data (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
(5401*10, 1, -9303.19, -292.231, 70.63, 0, 120000, 0, 0, 100, 0),
(5401*10, 2, -9304.99, -292.831, 70.5517, 0, 0, 0, 0, 100, 0),
(5401*10, 3, -9313.18, -292.923, 70.0808, 0, 0, 0, 0, 100, 0),
(5401*10, 4, -9324.45, -294.622, 69.0239, 0, 20000, 0, 0, 100, 0),
(5401*10, 5, -9304.99, -292.831, 70.5517, 0, 0, 0, 0, 100, 0),
(5401*10, 6, -9303.19, -292.231, 70.63, 0, 0, 0, 0, 100, 0),
(5401*10, 7, -9301.44, -289.602, 70.782, 0, 0, 0, 0, 100, 0);

DELETE FROM pool_template WHERE entry IN (460, 461, 463, 464, 465, 571);
INSERT INTO pool_template (entry, max_limit, description) VALUES
(460, 1, "Bayne"),
(461, 1, "Farmer Solliden"),
(463, 1, "Gruff Swiftbite"),
(464, 1, "Thuros Lightfingers"),
(465, 1, "Ambassador Jerrikar"),
(571, 1, "Sha of Anger");

DELETE FROM pool_creature WHERE pool_entry IN (460, 461, 463, 464, 465, 571);
INSERT INTO pool_creature (guid, pool_entry, chance, description) VALUES
(131458, 460, 0, "Bayne (Spawn 1)"),
(131459, 460, 0, "Bayne (Spawn 2)"),
(131457, 460, 0, "Bayne (Spawn 3)"),
(131456, 460, 0, "Bayne (Spawn 4)"),
(52641, 460, 0, "Bayne (Spawn 5)"),
(306498, 460, 0, "Bayne (Spawn 6)"),

(17528, 461, 0, "Farmer Solliden (Spawn 1)"),
(194009, 461, 0, "Farmer Solliden (Spawn 2)"),

(3906, 463, 0, "Gruff Swiftbite (Spawn 1)"),
(10298, 463, 0, "Gruff Swiftbite (Spawn 2)"),
(10299, 463, 0, "Gruff Swiftbite (Spawn 3)"),
(10300, 463, 0, "Gruff Swiftbite (Spawn 4)"),
(302508, 463, 0, "Gruff Swiftbite (Spawn 5)"),

(5401, 464, 0, "Thuros Lightfingers (Spawn 1)"),
(10301, 464, 0, "Thuros Lightfingers (Spawn 2)"),
(10302, 464, 0, "Thuros Lightfingers (Spawn 3)"),
(10303, 464, 0, "Thuros Lightfingers (Spawn 4)"),
(10304, 464, 0, "Thuros Lightfingers (Spawn 5)"),
(53205, 464, 0, "Thuros Lightfingers (Spawn 6)"),
(74373, 464, 0, "Thuros Lightfingers (Spawn 7)"),
(74377, 464, 0, "Thuros Lightfingers (Spawn 8)"),

(126689, 465, 0, "Ambassador Jerrikar (Spawn 1)"),
(126690, 465, 0, "Ambassador Jerrikar (Spawn 2)"),
(131431, 465, 0, "Ambassador Jerrikar (Spawn 3)"),
(131432, 465, 0, "Ambassador Jerrikar (Spawn 4)"),
(131433, 465, 0, "Ambassador Jerrikar (Spawn 5)"),

(10576396, 571, 0, "Sha of Anger (Spawn 1)"),
(10576395, 571, 0, "Sha of Anger (Spawn 2)"),
(10550815, 571, 0, "Sha of Anger (Spawn 3)"),
(10550814, 571, 0, "Sha of Anger (Spawn 4)");
