DELETE FROM creature WHERE guid IN (3906, 10298, 10299, 10300, 302508) AND id = 100;
INSERT INTO creature (guid, id, map, zoneId, areaId, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(3906, 100, 0, 12, 60, -9945.86, 620.999, 37.2259, 4.03808, 28800, 10, 1),
(10298, 100, 0, 12, 60, -9981.18, 566.13, 37.6634, 1.37087, 28800, 10, 1),
(10299, 100, 0, 12, 60, -10067.1, 629.856, 39.2659, 0.67265, 28800, 10, 1),
(10300, 100, 0, 12, 60, -10017, 654.089, 37.149, 5.412, 28800, 10, 1),
(302508, 100, 0, 12, 60, -9930.88, 573.464, 37.0119, 2.35969, 28800, 10, 1);

DELETE FROM pool_template WHERE entry IN (460, 461, 463, 571);
INSERT INTO pool_template (entry, max_limit, description) VALUES
(460, 1, "Bayne"),
(461, 1, "Farmer Solliden"),
(463, 1, "Gruff Swiftbite"),
(571, 1, "Sha of Anger");

DELETE FROM pool_creature WHERE pool_entry IN (460, 461, 463, 571);
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

(10576396, 571, 0, "Sha of Anger (Spawn 1)"),
(10576395, 571, 0, "Sha of Anger (Spawn 2)"),
(10550815, 571, 0, "Sha of Anger (Spawn 3)"),
(10550814, 571, 0, "Sha of Anger (Spawn 4)");
