/*
 * Copyright (C) 2018+ MagicStormProject <https://github.com/MagicStormTeam>
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
 
#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "AreaTriggerTemplate.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "antorus_the_burning_throne.h"
#include "Log.h"
 
enum Spells
{
    // Cannons
    SPELL_ANNIHILATION            = 244790,
    SPELL_DECIMATION              = 244410,
    SPELL_RESTORE_HEALTH          = 246012,
    SPELL_HAYWIRE                 = 246965, // Mythic only
 
    // Garothi
    SPELL_CARNAGE                 = 244106,
    SPELL_EMPOWERED               = 245237,
    SPELL_ERADICATION             = 244969,
    SPELL_SEARING_BARRAGE         = 244395,
    SPELL_SEARING_BARRAGE_DMG     = 244400,
    SPELL_APOCALYPSE_DRIVE_AURA   = 244152,
    SPELL_APOCALYPSE_DRIVE_CAST   = 240227,
    SPELL_FEL_BOMBARDMENT         = 246220,
    SPELL_SURGING_FEL             = 246655, // Heroic and Mythic
    SPELL_MELEE                   = 248230,
    SPELL_ANNIHILATOR_C_EJECT     = 245527,
    SPELL_APOCALYPSE_DRIVE        = 240277,
    SPELL_DECIMATOR               = 244410,
    SPELL_RIDE_SPELL              = 244993,
    SPELL_HELMET                  = 246839,
    SPELL_ARCANE_BLAST            = 196357,
    SPELL_NETHER_LINK             = 196804,
    SPELL_OVERCHARGE              = 196396,
    SPELL_OVERCHARGE_MANA         = 196392,
    SPELL_VOLATILE_MAGIC          = 196562,
    NPC_NIGHT_CRYSTAL             = 98734,
    SPELL_CHARGED_BOLT            = 220581,
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
 
enum Actions
{
    ACTION_CANNON_CAST_SPELL = 0,
    ACTION_ERADICATION,
};
 
enum Misc
{
    ONE_SHOT_ANIM = 13264
};
 
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
 
struct npc_garothi_annihilator : public ScriptedAI
{
    npc_garothi_annihilator(Creature* creature) : ScriptedAI(creature) { }
 
    void DoAction(int32 action) override
    {
        if (action == ACTION_CANNON_CAST_SPELL)
        {
            /* Things... */
        }
    }
 
    void JustDied(Unit* /*killer*/) override
    {
        if (Creature* garothi = ObjectAccessor::GetCreature(*me, instance->GetObjectGuid(DATA_GAROTHI_WORLDBREAKER)))
            garothi->AI()->DoAction(ACTION_ERADICATION);
    }
 
};
 
struct npc_garothi_decimator : public ScriptedAI
{
    npc_garothi_decimator(Creature* creature) : ScriptedAI(creature) { }
 
    void DoAction(int32 action) override
    {
        if (action == ACTION_CANNON_CAST_SPELL)
        {
            /* Things... */
        }
    }
 
    void JustDied(Unit* /*killer*/) override
    {
        if (Creature* garothi = ObjectAccessor::GetCreature(*me, instance->GetObjectGuid(DATA_GAROTHI_WORLDBREAKER)))
            garothi->AI()->DoAction(ACTION_ERADICATION);
    }
};
 
enum AnnihilationSpell
{
    SPELL_ANNIHILATION_AT  = 244795, // areatrigger
    SPELL_ANNIHILATION_CAST = 247572,
    SPELL_ANNIHILATION_AREA_NO_PLAYERS = 244799, // CASTED BY 122818
    SPELL_ANNIHILATION_EXPLODE = 244761, // TRIGGERED BY 244758
    SPELL_ANNIHILATION_EXPLODE_NO_PLAYERS = 244762, // Explosion with no players inside
    SPELL_ANNIHILATION_PROJECTILE = 244758, // PROJECTILE + EXPLOSION
};
 
struct npc_annihilation_worldtrigger : public ScriptedAI
{
    npc_annihilation_worldtrigger(Creature* creature) : ScriptedAI(creature) { }
 
    void Reset() override
    {
        DoCast(SPELL_ANNIHILATION_AREA_NO_PLAYERS);
        DoCastSelf(SPELL_ANNIHILATION_AT, true);
        DoCast(SPELL_ANNIHILATION_CAST); // This should be casted by Annihilator, but lets do this way for now
    }
};
 
void RemoveNoPlayerUnits(GuidUnorderedSet& G)
{
    auto it = G.begin();
    while (it != G.end())
    {
        if (!(*it).IsPlayer())
            it = G.erase(it);
        else
            it++;
    }
}
 
// 247572
class spell_annihilation : public SpellScript
{
    PrepareSpellScript(spell_annihilation);
 
    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
        {
            if (Creature* annihilator = caster->FindNearestCreature({ NPC_CANNON_ANNIHILATOR }, 200.f))
            {
                if (AreaTrigger* areatrigger = caster->GetAreaTrigger(SPELL_ANNIHILATION_AT))
                {
                    GuidUnorderedSet _units = areatrigger->GetInsideUnits();
                    RemoveNoPlayerUnits(_units);
                    if (_units.empty())
                        annihilator->CastSpell(caster->GetPosition(), SPELL_ANNIHILATION_EXPLODE_NO_PLAYERS, true);
                    else
                    {
                        annihilator->CastSpell(caster->GetPosition(), SPELL_ANNIHILATION_PROJECTILE, true); // ToDo: Split damage between targets?
                    }
                    caster->ToCreature()->DespawnOrUnsummon();
                }
            }
        }
    }
 
    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_annihilation::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};
 
// AT: 15496
// Spell: 244795
struct at_spell_annihilation : AreaTriggerAI
{
    at_spell_annihilation(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }
 
 
    void OnUnitEnter(Unit* unit) override
    {
        if (Unit* caster = at->GetCaster())
            if (unit->IsPlayer())
                if (caster->HasAura(SPELL_ANNIHILATION_AREA_NO_PLAYERS))
                    caster->RemoveAura(SPELL_ANNIHILATION_AREA_NO_PLAYERS);
    }
};
 
enum DecimationSpell {
    SPELL_DECIMATION_CAST = 245338, // useful
    SPELL_DECIMATION_PROJECTILE_AREA_EXPLODE = 244448, // Strange thing: It does the damage before projectile hits the ground.
    SPELL_DECIMATION_DAMAGE = 244449, // trigger by 244448
};
 
class spell_gen_antorus_decimation : public SpellScriptLoader
{
public:
    spell_gen_antorus_decimation() : SpellScriptLoader("spell_gen_antorus_decimation") { }

    class spell_gen_antorus_decimation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_antorus_decimation_AuraScript);

        bool Validate(SpellInfo const* /spellInfo/) override
        {
            return ValidateSpellInfo({ SPELL_DECIMATOR });
        }

        void OnRemove(AuraEffect const* /aurEff/, AuraEffectHandleModes /mode/)
        {
            if (Unit* caster = GetTarget())
                if (InstanceScript* instance = caster->GetInstanceScript())
                {
                    if (Creature* d = ObjectAccessor::GetCreature(caster, instance->GetGuidData(DATA_GAROTHI_WORLDBREAKER)))
                        if (d->IsAlive())
                            d->CastSpell(caster, 244449, true);
                }
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_gen_antorus_decimation_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript GetAuraScript() const override
    {
        return new spell_gen_antorus_decimation_AuraScript();
    }
};
 
enum FelBombardmentSpell
{
    SPELL_FEL_BOMBARDMENT_AURA = 244536,
    SPELL_FEL_BOMBARDMENT_MISSILE = 244533
};
 
// 246220
class spell_fel_bombardment_aura : public AuraScript
{
    PrepareAuraScript(spell_fel_bombardment_aura);
 
    void HandleRemove(AuraEffect const* /*aufEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(GetTarget(), SPELL_FEL_BOMBARDMENT_AURA, true);
    }
 
    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_fel_bombardment_aura::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};
 
// 244536
class spell_fel_bombardment_aura_missile : public AuraScript
{
    PrepareAuraScript(spell_fel_bombardment_aura_missile);
 
    void HandlePeriodic(AuraEffect const* /*aufEff*/)
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(GetTarget()->GetPosition(), SPELL_FEL_BOMBARDMENT_MISSILE);
    }
 
    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_fel_bombardment_aura_missile::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};
 
// 244395
class spell_searing_barrage : public AuraScript
{
    PrepareAuraScript(spell_searing_barrage);
 
    void HandlePeriodic(AuraEffect const* /*aufEff*/)
    {
        if (Unit* caster = GetCaster())
            if (!caster->IsPlayer()) // prevent crash with .cast or smth like that.
                if (Unit* target = caster->GetAI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 300.f, true))
                    caster->CastSpell(target, SPELL_SEARING_BARRAGE_DMG, true);
    }
 
    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_searing_barrage::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

void AddSC_boss_garothi_worldbreaker()
{
    RegisterCreatureAI(boss_garothi_worldbreaker);
    RegisterCreatureAI(npc_annihilation_worldtrigger);
    RegisterCreatureAI(npc_garothi_annihilator);
    RegisterCreatureAI(npc_garothi_decimator);
    RegisterSpellScript(spell_annihilation);
    RegisterAreaTriggerAI(at_spell_annihilation);
    new spell_gen_antorus_decimation();
    RegisterAuraScript(spell_fel_bombardment_aura);
    RegisterAuraScript(spell_fel_bombardment_aura_missile);
    RegisterAuraScript(spell_searing_barrage);
}
