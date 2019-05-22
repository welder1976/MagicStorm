#include "ScriptMgr.h"
#include "AreaTrigger.h"
#include "AreaTriggerAI.h"


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

        void EnterEvadeMode(EvadeReason /why/) override
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

        void JustDied(Unit* /killer/) override
        {
             _JustDied();
             DoSendEventScenario(47080);
             if (Creature* weapon = instance->SummonCreature(105155, Position(-2747.18f, -328.5697f, 38.43f, 1.927426f)))
             weapon->SetDisplayId(68978);
             instance->SummonGameObject(248785, Position(-2747.18f, -328.5697f, 38.43f, 1.927426f), QuaternionData(), WEEK);
        }
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

void AddSC_scenario_artifact_dh()
{
  new caria_felsoul
}