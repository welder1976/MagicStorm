-- fix - emotes on gnome survivors gnome starting area
DELETE FROM `creature_addon` WHERE `guid` in (167778, 168303, 168353, 168354, 168378, 168379, 168380, 168399, 168400, 168414, 168415, 168416, 
168417, 168837, 168838, 168839, 168841, 168842, 168843, 168898, 168900, 168901, 168928, 168937, 168939);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `auras`) VALUES
 (167778, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168303, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168353, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168354, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168378, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168379, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168380, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168399, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168400, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168414, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168415, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168416, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168417, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168837, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168838, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168839, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168841, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168842, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168843, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168898, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168900, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168901, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168928, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168937, 0, 0, 0, 0, 431, 0, 0, 0, 0),
 (168939, 0, 0, 0, 0, 431, 0, 0, 0, 0);
