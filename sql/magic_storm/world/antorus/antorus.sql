DELETE FROM `instance_template` WHERE `map` = 1712;
INSERT INTO `instance_template` VALUES
(1712, 1669, 'instance_antorus', 1, 0);

DELETE INTO `areatrigger_teleport` WHERE `ID` IN (-48, -49);
INSERT INTO `areatrigger_teleport` VALUES 
(-48, 6161, 'Antorus entrance'),
(-49, 6162, 'Antorus exit');

DELETE FROM `areatrigger` WHERE `guid` IN (48,49)
INSERT INTO `areatrigger` VALUES
(48, 100001, 1669, 1, -3208.954834, 9415.329102, -173.882599, ''), -- Not sniff coords :P
(49, 100004, 1712, 245760, -3433.522217, 9523.628906, 11.270082, ''); -- Sniff coords

DELETE FROM `areatrigger_template` WHERE `Id` = 15496;
INSERT INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `VerifiedBuild`) VALUES
(15496, 0, 4, 4, 4, 0, 0, 0, 0, 26124);

DELETE FROM `spell_areatrigger` WHERE `AreaTriggerId` = 15496;
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(10662, 15496, 0, 0, 0, 0, 0, 0, 8000, 26124); -- SpellId : 244795

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_annihilation', 'spell_decimation_aura', 'spell_fel_bombardment_aura', 'spell_fel_bombardment_aura_missile', 'spell_searing_barrage');
INSERT INTO `spell_script_names` VALUES
(247572, 'spell_annihilation'),
(244410, 'spell_decimation_aura'),
(246220, 'spell_fel_bombardment_aura'),
(244536, 'spell_fel_bombardment_aura_missile'),
(244395, 'spell_searing_barrage');

UPDATE `creature_template` SET `ScriptName` = 'npc_annihilation_worldtrigger', `flags_extra` = 128 WHERE `entry` = 122818;
UPDATE `creature_template` SET `ScriptName` = 'npc_garothi_annihilator' WHERE `entry` = 122778;
UPDATE `areatrigger_template` SET `ScriptName` = 'at_spell_annihilation' WHERE `id` = 15496;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 247572;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 247572, 31, 3, 122818, 'Garothi Worldbreaker - Annihilation');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 4 AND `SourceEntry` = 244449;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 4, 244449, 31, 3, 124436, 'Garothi Worldbreaker - Decimation - Blightscale worm');