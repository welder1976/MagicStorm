DELETE FROM pool_template WHERE entry IN (460, 461, 571);
INSERT INTO pool_template (entry, max_limit, description) VALUES
(460, 1, "Bayne"),
(461, 1, "Farmer Solliden"),
(571, 1, "Sha of Anger");
DELETE FROM pool_creature WHERE pool_entry IN (460, 461, 571);
INSERT INTO pool_creature (guid, pool_entry, chance, description) VALUES
(131458, 460, 0, "Bayne (Spawn 1)"),
(131459, 460, 0, "Bayne (Spawn 2)"),
(131457, 460, 0, "Bayne (Spawn 3)"),
(131456, 460, 0, "Bayne (Spawn 4)"),
(52641, 460, 0, "Bayne (Spawn 5)"),
(306498, 460, 0, "Bayne (Spawn 6)"),
(17528, 461, 0, "Farmer Solliden (Spawn 1)"),
(194009, 461, 0, "Farmer Solliden (Spawn 2)"),
(10576396, 461, 0, "Sha of Anger (Spawn 1)"),
(10576395, 461, 0, "Sha of Anger (Spawn 2)"),
(10550815, 461, 0, "Sha of Anger (Spawn 3)"),
(10550814, 461, 0, "Sha of Anger (Spawn 4)");
