DELETE FROM `gossip_menu_option` WHERE (`menuid`=102343 AND `OptionIndex`=0);
INSERT INTO `gossip_menu_option` (`menuid`, `OptionIndex`, `optionicon`, `optiontext`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild`) VALUES
(102343, 0, 0, '��� � ����� � ������ ��� ����������? � ����� ����� ���� ������ � ����� ����?', 0, 0, 0, 0);

UPDATE  `gossip_menu_option` SET `OptionType`=1, `OptionNpcFlag`=1, `VerifiedBuild`=26124 WHERE (`MenuId`=102343 AND `OptionIndex`=0);

UPDATE `creature_template` SET `gossip_menu_id`=102343, `npcflag` = 3, `minlevel`=98, `maxlevel`=110, `ScriptName`='npc_calia_102343' WHERE `entry`=102343;
UPDATE `creature_template` SET `minlevel`=98, `maxlevel`=110, `ScriptName`='npc_alonsus_faol1_102358' WHERE `entry`=102358;

SET @GROUP_ID=0;
SET @ID=0;
DELETE FROM `creature_text` WHERE (`CreatureID`=91866) OR (`CreatureID`=92460) OR (`CreatureID`=92909) OR (`CreatureID`=93532) OR (`CreatureID`=103092) OR (`CreatureID`=108692) OR (`CreatureID`=108693) OR (`CreatureID`=108783);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `comment`) VALUES
(102358, @GROUP_ID+0, @ID+0, '������ �� �����������- ������ �������� � ������� ����.�� ���� ����� ��� �������� �������� � ������ ������ � ���������� ������ �����, � ������������� ������ �����. �� � ������ ��� � ��������� ������', 12, 0, 100, 0, 0, 0, 0, 'to Player'),
(102358, @GROUP_ID+1, @ID+0, '������ ������������ ���������� ������ ������� � ������� �����������, ����� �� ������ ������, ��� ������� � ��������', 12, 0, 100, 0, 0, 0, 0, 'to Player'),
(102358, @GROUP_ID+2, @ID+0, '� ���� ����� �� ��������������� ������� ����, ������ �������� ���������� ����� �����. � ������� �� ��������� �� ��� ����� $n?', 12, 0, 100, 0, 0, 0, 0, 'to Player');