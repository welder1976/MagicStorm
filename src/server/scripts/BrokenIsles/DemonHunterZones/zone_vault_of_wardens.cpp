/*
 * Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
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

enum eVaultOfWardens
{
    EVENT_FEL_ANNIHILATION        = 0,
    EVENT_CRUSHING_SHADOWS        = 1,
    EVENT_BLUR_OF_SHADOWS         = 2,

    SPELL_FEL_ANNIHILATION        = 200007,
    SPELL_CRUSHING_SHADOWS        = 200027,
    SPELL_BLUR_OF_SHADOWS         = 200002,
};

class npc_bastillax : public CreatureScript
{
public:
    npc_bastillax() : CreatureScript("npc_bastillax") { }

    struct npc_bastillaxAI : public ScriptedAI
    {
        npc_bastillaxAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            events.Reset();
        }

        void JustDied(Unit* killer) override
        {
            if (killer->GetTypeId() == TYPEID_PLAYER)
            {
                killer->ToPlayer()->KilledMonsterCredit(113812, ObjectGuid::Empty);
                killer->ToPlayer()->KilledMonsterCredit(106255, ObjectGuid::Empty);
            }
        }

        void EnterCombat(Unit* /*who*/) override
        {
            events.ScheduleEvent(EVENT_FEL_ANNIHILATION, 6s, 8s);
            events.ScheduleEvent(EVENT_CRUSHING_SHADOWS, 10s, 12s);
            events.ScheduleEvent(EVENT_BLUR_OF_SHADOWS, 3s);
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FEL_ANNIHILATION:
                        if (Unit* target = me->GetVictim())
                            me->CastSpell(me, SPELL_FEL_ANNIHILATION, true);

                        events.ScheduleEvent(EVENT_FEL_ANNIHILATION, 8s, 10s);
                        break;
                    case EVENT_CRUSHING_SHADOWS:
                        if (Unit* target = me->GetVictim())
                            me->CastSpell(target, SPELL_CRUSHING_SHADOWS, true);

                        events.ScheduleEvent(EVENT_CRUSHING_SHADOWS, 6s);
                        break;
                    case EVENT_BLUR_OF_SHADOWS:
                        if (Unit* target = me->GetVictim())
                            me->CastSpell(target, SPELL_BLUR_OF_SHADOWS, true);

                        events.ScheduleEvent(EVENT_BLUR_OF_SHADOWS, 6s);
                        break;
                }
            }

            if (UpdateVictim())
                DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_bastillaxAI(creature);
    }
};

void AddSC_zone_vault_of_wardens()
{
    new npc_bastillax();
}
