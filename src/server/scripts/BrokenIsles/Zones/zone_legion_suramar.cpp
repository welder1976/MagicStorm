/*
* Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptMgr.h"

enum SPELLS
{
    QUEST_MASQUERADE = 42079,
    LEARN_MASQUERADE = 211086,
    SPELL_MASQUERADE = 211135,
};

class Player_Spell_Masquerade : public PlayerScript
{

public:
    Player_Spell_Masquerade() : PlayerScript("Player_Spell_Masquerade") { }

    void OnLogin(Player* player, bool /*firstLogin*/)
    {       
        UseSpellMasquerade(player, player->GetAreaId(), true);
    }

    void OnPlayerUpdateAreaAlternate(Player* player, uint32 newArea, uint32 /*oldArea*/)
    {
        UseSpellMasquerade(player, newArea, false);
    }

    void UseSpellMasquerade(Player* player, uint32 area, bool trigger)
    {        
        switch (area)
        {
        case 8149:
        case 8148:
        case 8150:
        case 7963:
        case 8351:
        case 8352:
        case 8353: 
        case 8355:
        case 8378: 
        case 8379: 
        case 8380: 
        case 8381: 
        case 8386: 
        case 8385: 
        case 8384: 
        case 8383: 
        case 8382: 
        case 8395: 
        case 8397: 
        case 8398: 
        case 8431: 
        case 8436: 
        case 8441:
        case 8434: 
        case 8345: 
        case 8461: 
        case 8487: 
        case 8496: 
            if (!player->HasAura(LEARN_MASQUERADE))
               return;
            if (player->GetQuestStatus(QUEST_MASQUERADE) != QUEST_STATUS_REWARDED)
                return;
            if(trigger)
                player->RemoveAurasDueToSpell(SPELL_MASQUERADE);
            if (!player->HasAura(SPELL_MASQUERADE))
                player->CastSpell(player, SPELL_MASQUERADE, true);
            player->GetQuestObjectiveCriteriaMgr();
            return;
            break;
        default:
            return;
            break;
        }
    }
};

void AddSC_suramar()
{

    new Player_Spell_Masquerade();
}
