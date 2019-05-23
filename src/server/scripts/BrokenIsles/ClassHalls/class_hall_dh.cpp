/*
 * Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
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
#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "InstanceScript.h"

enum
{
    NPC_KORVAS_BLOODTHORN_99343 = 99343,

    ///DH Quest
    QUEST_CALL_OF_THE_ILLIDARI_39261 = 39261,
    QUEST_CALL_OF_THE_ILLIDARI_39047 = 39047,
};

struct npc_korvas_bloodthorn_99343 : public ScriptedAI
{
    npc_korvas_bloodthorn_99343(Creature* creature) : ScriptedAI(creature) { SayHi = false; }

    void MoveInLineOfSight(Unit* who) override
    {
        if (!who || !who->IsInWorld())
            return;
        if (!me->IsWithinDist(who, 25.0f, false))
            return;

        Player* player = who->GetCharmerOrOwnerPlayerOrPlayerItself();

        if (!player)
            return;
        me->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, me->GetFollowAngle());
        if (!SayHi)
        {
            SayHi = true;
            Talk(0, player);
        }
    }

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_CALL_OF_THE_ILLIDARI_39261 || quest->GetQuestId() == QUEST_CALL_OF_THE_ILLIDARI_39047)
        {
            Talk(1, player);
            me->DespawnOrUnsummon(5000);
        }
    }
private:
    bool SayHi;
};

enum Spells
{
    SPELL_CALL_OF_THE_WARBLADES       = 208464,
    SPELL_FELLSOUL_SLAM               = 216164,

    SPELL_SIGIL_OF_POWER              = 216228,

    SPELL_SOUL_CARVER                 = 216188

};

enum Events
{
    EVENT_CALL_OF_THE_WARBLADES         = 1,
    EVENT_FELLSOUL_SLAM                 = 2,
    EVENT_SIGIL_OF_POWER                = 3,
    EVENT_SOUL_CARVER                   = 4
};

Position const SummonPositions[2] =
{
    {-2747.18f, -328.5697f, 38.43f, 1.927426f},
    {-2747.18f, -328.5697f, 38.43f, 1.927426f},
};

class caria_felsoul : public CreatureScript
{
public:
    caria_felsoul() : CreatureScript("caria_felsoul") { }

    struct caria_felsoulAI : public WorldBossAI
    {
        caria_felsoulAI(Creature* creature) : WorldBossAI(creature) { }

        void Reset() override
        {
            _Reset();
            me->SetFullHealth();
            me->CombatStop(true);
            me->GetMotionMaster()->MoveTargetedHome();
            events.Reset();
        }

        void EnterEvadeMode(EvadeReason why) override
        {
            Reset();
        }

        void EnterCombat(Unit* who) override
        {
            if (!who)
                return;

            me->setActive(true);
            DoZoneInCombat();
            me->SetFullHealth();
            events.ScheduleEvent(EVENT_CALL_OF_THE_WARBLADES, 14000);
            events.ScheduleEvent(EVENT_FELLSOUL_SLAM, 28800);
            events.ScheduleEvent(EVENT_SIGIL_OF_POWER, 29000);
            events.ScheduleEvent(EVENT_SOUL_CARVER, 35000);
        }

        /*void JustDied(Unit* killer) override
        {
             _JustDied();
             DoSendEventScenario(47080);
             instance->SummonCreature(105155, SummonPositions[0]);
             weapon->SetDisplayId(68978);
             instance->SummonGameObject(248785, SummonPositions[1], QuaternionData(), WEEK);
        }*/
        void UpdateAI(uint32 diff) override
        {
            UpdateVictim();

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            if (me->IsInCombat())
            {
                if (events.Empty())
                {
                    events.ScheduleEvent(EVENT_CALL_OF_THE_WARBLADES, 14000);
                    events.ScheduleEvent(EVENT_FELLSOUL_SLAM, 28800);
                    events.ScheduleEvent(EVENT_SIGIL_OF_POWER, 29000);
                    events.ScheduleEvent(EVENT_SOUL_CARVER, 35000);
                }
            }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CALL_OF_THE_WARBLADES:
                {
                    DoCastAOE(SPELL_CALL_OF_THE_WARBLADES);
                    events.ScheduleEvent(EVENT_CALL_OF_THE_WARBLADES, 14000);
                    break;
                }
                case EVENT_FELLSOUL_SLAM:
                {
                    DoCastAOE(SPELL_FELLSOUL_SLAM, false);
                    events.ScheduleEvent(EVENT_FELLSOUL_SLAM, 28800);
                    break;
                }
                case EVENT_SIGIL_OF_POWER:
                {
                    Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0.0f, 10.0f, true);
                    if (!target)
                        break;
                    DoCast(target, SPELL_SIGIL_OF_POWER);
                    events.ScheduleEvent(EVENT_SIGIL_OF_POWER, 29000);
                    break;
                }
                case EVENT_SOUL_CARVER:
                {
                    Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0.0, 0.0, true);
                    if (!target)
                        break;
                    DoCast(target, SPELL_SOUL_CARVER, false);
                    events.ScheduleEvent(EVENT_SOUL_CARVER, 24400);
                    break;
                }
                }
            }

            DoMeleeAttackIfReady();
        }

    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new caria_felsoulAI(creature);
    }
};

void AddSC_class_hall_dh()
{
    RegisterCreatureAI(npc_korvas_bloodthorn_99343);
    new caria_felsoul();
}
