-- fix for quest 1252
DELETE FROM `smart_scripts` WHERE `entryorguid`=28565;
INSERT INTO `smart_scripts` (
`entryorguid`, `source_type`, `id`, `link`, 
`event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, 
`action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, 
`target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(28565, 0, 0, 0, 38, 0, 100, 512, 0, 1, 0, 0, '', 29, 1, 1, 28591, 0, 0, 0, 19, 28591, 15, 0, 0, 0, 0, 0, 'Decaying Ghoul - On Data Set 0 1 - Start Follow Closest Creature \'Ghoul Feeding KC Bunny\' (Phase 1) (No Repeat) (Dungeon)'),
(28565, 0, 1, 2, 65, 0, 100, 0, 0, 0, 0, 0, '', 11, 52030, 0, 0, 0, 0, 0, 21, 20, 0, 0, 0, 0, 0, 0, 'Decaying Ghoul - On Follow Complete - Cast \'Kill Credit\''),
(28565, 0, 2, 3, 61, 0, 100, 512, 0, 0, 0, 0, '', 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Decaying Ghoul - On Follow Complete - Despawn In 2000 ms'),
(28565, 0, 3, 0, 61, 0, 100, 512, 0, 0, 0, 0, '', 51, 0, 0, 0, 0, 0, 0, 11, 28591, 50, 0, 0, 0, 0, 0, 'Decaying Ghoul - On Follow Complete - Kill Target');
