-- test - despawn josiah averey 35369 on quest complete 14159

UPDATE `creature_template` SET `npcflag`='3', `unit_flags`='294912', `AIName`='SmartAI' WHERE  `entry`=35369;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 35369 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(35369, '0', '0', '0', '20', '0', '100', '0', '14159', '0', '0', '0', '41', '1', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', 'Despawn on quest 14159');