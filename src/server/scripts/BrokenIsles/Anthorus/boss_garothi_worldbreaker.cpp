/*
ANTORUS_THE_BURNING_THRONE LYOSKY
*/
#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "antorus_the_burning_throne.h"


enum Spells
{
    SPELL_APOCALYPSE_DRIVE = 240277,//244152
    SPELL_CARNAGE = 244106,

    SPELL_ERADICATION = 244969,
    SPELL_FEL_BOMBARDMENT = 246220,
    SPELL_SEARING_BARRAGE = 244395,
    SPELL_EMPOWERED = 245237,

    //122778 cast    
    SPELL_ANNIHILATION = 244758,//244761,

    //122773 cast
    SPELL_DECIMATOR = 244410,

    SPELL_RIDE_SPELL = 244993,
    SPELL_HELMET = 246839,



    SPELL_ARCANE_BLAST = 196357,
    SPELL_NETHER_LINK = 196804,
    SPELL_OVERCHARGE = 196396,
    SPELL_OVERCHARGE_MANA = 196392,
    SPELL_VOLATILE_MAGIC = 196562,

    NPC_NIGHT_CRYSTAL = 98734,
    SPELL_CHARGED_BOLT = 220581,
};

enum Events
{
    // Intro
    EVENT_INTRO = 1,
    EVENT_APOCALYPSE_DRIVE = 2,
    EVENT_ANNIHILATION = 3,
    EVENT_DECIMATOR = 4,
    EVENT_AFTER_APOCALYPSE_DRIVE = 5,
    //the Annihilator or the Decimator.
};

enum Phases
{
    PHASE_NORMAL,
    PHASE_ANNIHILATION,
    PHASE_DECIMATOR,
    PHASE_APOCALYPSE_DRIVE,
    PHASE_INTRO
};

Position const pos1 = Position(-3302.531f, 9813.955f, -46.21415f, 4.729585f);
Position const pos2 = Position(-3285.261f, 9812.452f, -46.21415f, 4.729585f);

struct boss_garothi_worldbreaker : public BossAI
{
    boss_garothi_worldbreaker(Creature* creature) : BossAI(creature, DATA_GAROTHI_WORLDBREAKER) { Initialize(); }

    void Initialize()
    {
        PhaseStatus = Phases::PHASE_INTRO;
        hp60 = false;
        hp20 = false;
        if (!me->HasAura(SPELL_HELMET))
            DoCastSelf(SPELL_HELMET);
        annihilatorGUID = ObjectGuid::Empty;
        decimatorGUID = ObjectGuid::Empty;
        SetCombatMovement(false);
    }

    void Reset() override
    {
        _Reset();
        Initialize();
    }

    void JustDied(Unit* /*killer*/) override
    {
        _JustDied();
        Talk(9);
        me->Say(135268);
        if (instance)
            //instance->DoDelayedConversation(2000, 5519);
        me->SummonCreature(130137, Position(-3299.84f, 9782.797f, -63.40741f, 1.480368f));
    }

    void SpawnMobs()
    {
        if (Creature* decimator = me->GetSummonedCreatureByEntry(NPC_DECIMATOR))
            decimator->DespawnOrUnsummon();
        if (Creature* annihilator = me->GetSummonedCreatureByEntry(NPC_ANNIHILATOR))
            annihilator->DespawnOrUnsummon();

        me->GetScheduler().Schedule(Milliseconds(500), [](TaskContext context)
        {
            Creature* annihilator1 = GetContextCreature()->SummonCreature(NPC_ANNIHILATOR, pos1, TEMPSUMMON_MANUAL_DESPAWN);
            //annihilatorGUID = annihilator1->GetGUID();
            annihilator1->CastSpell(GetContextCreature(), SPELL_RIDE_SPELL, true);
        });

        me->GetScheduler().Schedule(Milliseconds(1000), [](TaskContext context)
        {

            Creature* decimator1 = GetContextCreature()->SummonCreature(NPC_DECIMATOR, pos2, TEMPSUMMON_MANUAL_DESPAWN);
            //annihilatorGUID = decimator1->GetGUID();
            decimator1->CastSpell(GetContextCreature(), SPELL_RIDE_SPELL, true);
        });
    }

    void ScheduleTasks() override
    {
        //talk
        me->Say(132426);
        Talk(1);
        if (instance)
            //instance->DoConversation(5514);
        PhaseStatus = Phases::PHASE_ANNIHILATION;
       
        events.ScheduleEvent(SPELL_ANNIHILATION, 1s);
        events.ScheduleEvent(EVENT_DECIMATOR, 30s);
        events.ScheduleEvent(SPELL_FEL_BOMBARDMENT, 10s);
        //if (instance)
            //instance->DoDelayedConversation(5000, 5515);
    }

    void DoAction(int32 param)
    {
        switch (param)
        {
        case 1:
            me->RemoveAurasDueToSpell(244152);
            me->CastStop(SPELL_APOCALYPSE_DRIVE);
            //talk ??
            Talk(7);
            me->Say(138536);
            DoCastSelf(SPELL_ERADICATION);
            Talk(8);
            me->Say(132434);
            events.ScheduleEvent(EVENT_ANNIHILATION, 5s);
            break;
        }
    }

    void ExecuteEvent(uint32 eventId) override
    {
        if (!hp60 && HealthBelowPct(60))
        {
            hp60 = true;
            events.Reset();
            PhaseStatus = Phases::PHASE_APOCALYPSE_DRIVE;
            events.ScheduleEvent(SPELL_APOCALYPSE_DRIVE, 1s);
        }

        if (!hp20 && HealthBelowPct(20))
        {
            hp20 = true;
            events.Reset();
            PhaseStatus = Phases::PHASE_APOCALYPSE_DRIVE;
            events.ScheduleEvent(SPELL_APOCALYPSE_DRIVE, 1s);
        }
        if (PhaseStatus == Phases::PHASE_ANNIHILATION)
        {
            switch (eventId)
            {
            case SPELL_ANNIHILATION:
            {
                Talk(2);
                me->Say(132431);
                if (Creature* annihilation = me->GetSummonedCreatureByEntry(NPC_ANNIHILATOR))
                    if (annihilation->IsAlive())
                    {
                        if (Unit* target1 = SelectTarget(SELECT_TARGET_RANDOM, 0.0, 0.0, true))
                            annihilation->CastSpell(target1, SPELL_ANNIHILATION, false);

                        if (Unit* target2 = SelectTarget(SELECT_TARGET_RANDOM, 0.0, 0.0, true))
                            annihilation->CastSpell(target2, SPELL_ANNIHILATION, false);
                    }
                events.Repeat(10s, 20s);
                break;
            }
            case EVENT_DECIMATOR:
            {
                PhaseStatus = Phases::PHASE_DECIMATOR;
                events.Reset();
                events.ScheduleEvent(SPELL_DECIMATOR, 1s);
                events.ScheduleEvent(SPELL_FEL_BOMBARDMENT, 10s);
                events.ScheduleEvent(EVENT_ANNIHILATION, 30s);
                break;
            }
            }
        }
        if (PhaseStatus == Phases::PHASE_DECIMATOR)
        {
            switch (eventId)
            {
            case SPELL_DECIMATOR:
            {
                Talk(4);
                me->Say(132430);
                if (Creature* decimator = me->GetSummonedCreatureByEntry(NPC_DECIMATOR))
                    if (decimator->IsAlive())
                    {
                        if (Unit* target1 = SelectTarget(SELECT_TARGET_RANDOM, 0.0, 0.0, true))
                            decimator->CastSpell(target1, PHASE_DECIMATOR, false);

                        if (Unit* target2 = SelectTarget(SELECT_TARGET_RANDOM, 0.0, 0.0, true))
                            decimator->CastSpell(target2, PHASE_DECIMATOR, false);
                    }
                events.Repeat(10s, 14s);
                break;
            }
            case EVENT_ANNIHILATION:
            {
                PhaseStatus = Phases::PHASE_ANNIHILATION;
                events.Reset();
                events.ScheduleEvent(SPELL_ANNIHILATION, 1s);
                events.ScheduleEvent(SPELL_FEL_BOMBARDMENT, 10s);
                events.ScheduleEvent(EVENT_DECIMATOR, 30s);
                break;
            }
            }
        }
        if (PhaseStatus == Phases::PHASE_APOCALYPSE_DRIVE)
        {
            switch (eventId)
            {
            case SPELL_APOCALYPSE_DRIVE:
            {
                Talk(5);
                me->Say(132631);
                if (Creature* annihilation = me->GetSummonedCreatureByEntry(NPC_ANNIHILATOR))
                    if (annihilation->IsAlive())
                      annihilation->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);

                if (Creature* decimator = me->GetSummonedCreatureByEntry(NPC_DECIMATOR))
                    if (decimator->IsAlive())
                        decimator->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);

                DoCastSelf(244152);
                DoCastSelf(SPELL_APOCALYPSE_DRIVE);

                events.ScheduleEvent(EVENT_AFTER_APOCALYPSE_DRIVE, 30s);
                break;
            }
            case EVENT_AFTER_APOCALYPSE_DRIVE:
            {
                Talk(6);
                me->Say(132433);
                if (Creature* annihilation = me->GetSummonedCreatureByEntry(NPC_ANNIHILATOR))
                    if (annihilation->IsAlive())
                        DoCastSelf(SPELL_EMPOWERED);

                if (Creature* decimator = me->GetSummonedCreatureByEntry(NPC_DECIMATOR))
                    if (decimator->IsAlive())
                        DoCastSelf(SPELL_SEARING_BARRAGE);
                DoAction(1);
                break;
            }
            case EVENT_ANNIHILATION:
            {
                if (Creature* annihilation = me->GetSummonedCreatureByEntry(NPC_ANNIHILATOR))
                    if (annihilation->IsAlive())
                        annihilation->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);

                if (Creature* decimator = me->GetSummonedCreatureByEntry(NPC_DECIMATOR))
                    if (decimator->IsAlive())
                        decimator->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                PhaseStatus = Phases::PHASE_ANNIHILATION;
                //talk
                events.Reset();
                events.ScheduleEvent(SPELL_ANNIHILATION, 1s);
                events.ScheduleEvent(SPELL_FEL_BOMBARDMENT, 10s);
                events.ScheduleEvent(EVENT_DECIMATOR, 30s);
                break;
            }
            }
        }
        switch (eventId)
        {
        case SPELL_FEL_BOMBARDMENT:
        {
            if (Unit* target1 = SelectTarget(SELECT_TARGET_RANDOM, 0.0, 0.0, true))
                me->CastSpell(target1, SPELL_FEL_BOMBARDMENT, false);
            events.Repeat(20s);
            break;
        }
        default:
            break;
        }
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (who->IsPlayer() && me->IsWithinDist(who, 50.0f, false) && PhaseStatus == Phases::PHASE_INTRO)
        {
            PhaseStatus = Phases::PHASE_NORMAL;

            SpawnMobs();

            me->GetScheduler().Schedule(Milliseconds(3000), [](TaskContext context)
            {
                GetContextCreature()->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            });
        }
    }
    uint8 PhaseStatus;
    bool hp60;
    bool hp20;
    ObjectGuid annihilatorGUID;
    ObjectGuid decimatorGUID;
};


void AddSC_boss_garothi_worldbreaker()
{
    RegisterCreatureAI(boss_garothi_worldbreaker);
}
