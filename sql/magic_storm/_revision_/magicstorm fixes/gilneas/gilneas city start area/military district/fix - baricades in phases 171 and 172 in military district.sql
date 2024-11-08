--  fix - baricades in phases 171 and 172 in military district
DELETE FROM `gameobject` WHERE `guid`=20406557;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `SpawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) 
VALUES (20406557, 195466, 654, 4755, 4757, 1, 0, 171, 0, -1, -1805.29, 1399.5, 20.261, 1.26536, 0.0655289, 0.0184669, 0.591166, 0.803671, 120, 255, 1, 0, '', 25383);
DELETE FROM `gameobject` WHERE `guid`=20406556;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `SpawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) 
VALUES (20406556, 195465, 654, 4755, 4757, 1, 0, 171, 0, -1, -1794.13, 1402.51, 20.1318, 2.15548, -0.038096, -0.0252781, 0.880382, 0.472057, 120, 255, 1, 0, '', 25383);
DELETE FROM `gameobject` WHERE `guid`=20406555;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `SpawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) 
VALUES (20406555, 195454, 654, 4755, 4757, 1, 0, 171, 0, -1, -1764.93, 1415.87, 20.1221, 1.40499, 0, 0, 0.646124, 0.763233, 120, 255, 1, 0, '', 25383);
DELETE FROM `gameobject` WHERE `guid`=20406554;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `SpawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) 
VALUES (20406554, 195453, 654, 4755, 4757, 1, 0, 171, 0, -1, -1769.5, 1405.62, 20.0667, 5.89672, -0.0201735, 0.0687904, -0.189473, 0.979266, 120, 255, 1, 0, '', 25383);

DELETE FROM `gameobject` WHERE `guid`=20406561;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `SpawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) 
VALUES (20406561, 195466, 654, 4755, 4757, 1, 0, 172, 0, -1, -1805.29, 1399.5, 20.261, 1.26536, 0.0655289, 0.0184669, 0.591166, 0.803671, 120, 255, 1, 0, '', 25383);
DELETE FROM `gameobject` WHERE `guid`=20406562;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `SpawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) 
VALUES (20406562, 195465, 654, 4755, 4757, 1, 0, 172, 0, -1, -1794.13, 1402.51, 20.1318, 2.15548, -0.038096, -0.0252781, 0.880382, 0.472057, 120, 255, 1, 0, '', 25383);
DELETE FROM `gameobject` WHERE `guid`=20406563;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `SpawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) 
VALUES (20406563, 195454, 654, 4755, 4757, 1, 0, 172, 0, -1, -1764.93, 1415.87, 20.1221, 1.40499, 0, 0, 0.646124, 0.763233, 120, 255, 1, 0, '', 25383);
DELETE FROM `gameobject` WHERE `guid`=20406564;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `SpawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) 
VALUES (20406564, 195453, 654, 4755, 4757, 1, 0, 172, 0, -1, -1769.5, 1405.62, 20.0667, 5.89672, -0.0201735, 0.0687904, -0.189473, 0.979266, 120, 255, 1, 0, '', 25383);

DELETE FROM `gameobject_template_addon` WHERE `entry`=195466;
INSERT INTO `gameobject_template_addon` (`entry`, `faction`, `flags`, `mingold`, `maxgold`, `WorldEffectID`) 
VALUES (195466, 0, 4, 0, 0, 0);
DELETE FROM `gameobject_template_addon` WHERE `entry`=195465;
INSERT INTO `gameobject_template_addon` (`entry`, `faction`, `flags`, `mingold`, `maxgold`, `WorldEffectID`) 
VALUES (195465, 0, 4, 0, 0, 0);
DELETE FROM `gameobject_template_addon` WHERE `entry`=195464;
INSERT INTO `gameobject_template_addon` (`entry`, `faction`, `flags`, `mingold`, `maxgold`, `WorldEffectID`) 
VALUES (195464, 0, 4, 0, 0, 0);
DELETE FROM `gameobject_template_addon` WHERE `entry`=195463;
INSERT INTO `gameobject_template_addon` (`entry`, `faction`, `flags`, `mingold`, `maxgold`, `WorldEffectID`) 
VALUES (195463, 0, 4, 0, 0, 0);
