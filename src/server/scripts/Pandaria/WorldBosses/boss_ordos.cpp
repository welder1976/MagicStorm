/*
 * Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
 * Copyright (C) 2016 Firestorm Servers <https://firestorm-servers.com>
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
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "Spell.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "AreaTrigger.h"

#define BOSS_ORDOS 0

enum OrdosTexts
{
    SAY_AGGRO               = 0,
    SAY_INTRO_1             = 1,
    SAY_INTRO_2             = 2,
    SAY_INTRO_3             = 3,
    SAY_DEATH               = 4,
    SAY_SLAY                = 5,
    SAY_POOL_OF_FIRE        = 6,
    SAY_ANCIENT_FLAME       = 7,
    SAY_BURNING_SOUL        = 8,
    SAY_ETERNAL_AGONY       = 9
};

enum OrdosSpells
{
    SPELL_ANCIENT_FLAME_S   = 144695,
    SPELL_ANCIENT_FLAME_A   = 144691,
    SPELL_ANCIENT_FLAME_D   = 144699,
    SPELL_BURNING_SOUL      = 144689,
    SPELL_BURNING_SOUL_EX   = 144690,
    SPELL_MAGMA_CRUSH       = 144688,
    SPELL_POOL_OF_FIRE      = 144692,
    SPELL_POOL_OF_FIRE_A    = 144693,
    SPELL_ETERNAL_AGONY     = 144696
};

enum OrdosEvents
{
    EVENT_ANCIENT_FLAME     = 1,
    EVENT_BURNING_SOUL      = 2,
    EVENT_POOL_OF_FIRE      = 3,
    EVENT_MAGMA_CRUSH       = 4,

    EVENT_ETERNAL_AGONY     = 5
};

enum OrdosNpcs
{
    NPC_ANCIENT_FLAME       = 72059
};

class boss_ordos : public CreatureScript
{
    public:
        boss_ordos() : CreatureScript("boss_ordos") { }

        struct boss_ordosAI : public BossAI
        {
            boss_ordosAI(Creature* creature) : BossAI(creature, BOSS_ORDOS),  summons(me) { }

            EventMap _events;
            SummonList summons;

            void Reset() override
            {
                _events.Reset();
                summons.DespawnAll();

                _Reset();
            }

            void EnterCombat(Unit* /*attacker*/) override
            {
                Talk(SAY_AGGRO);

                _events.ScheduleEvent(EVENT_MAGMA_CRUSH, 10s, 13s);
                _events.ScheduleEvent(EVENT_ANCIENT_FLAME, 40s, 45s);
                _events.ScheduleEvent(EVENT_BURNING_SOUL, 20s, 30s);
                _events.ScheduleEvent(EVENT_POOL_OF_FIRE, 30s, 45s);

                _events.ScheduleEvent(EVENT_ETERNAL_AGONY, 300s);
            }

            void KilledUnit(Unit* who) override
            {
                if (who->ToPlayer())
                    Talk(SAY_SLAY);
            }

            void EnterEvade()
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void JustDied(Unit* /*killer*/) override
            {
                Talk(SAY_DEATH);
                summons.DespawnAll();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
                summon->setActive(true);

                if (me->IsInCombat())
                    summon->SetInCombatWithZone();

                if (summon->GetEntry() == NPC_ANCIENT_FLAME)
                {
                    summon->AddAura(SPELL_ANCIENT_FLAME_A, summon);
                    summon->SetReactState(REACT_PASSIVE);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MAGMA_CRUSH:
                            DoCastVictim(SPELL_MAGMA_CRUSH);
                            _events.ScheduleEvent(EVENT_MAGMA_CRUSH, 7s, 27s);
                            break;

                        case EVENT_ANCIENT_FLAME:
                            Talk(SAY_ANCIENT_FLAME);
                            //DoCast(me, SPELL_ANCIENT_FLAME);
                            _events.ScheduleEvent(EVENT_ANCIENT_FLAME, 40s, 45s);
                            break;

                        case EVENT_POOL_OF_FIRE:
                            Talk(SAY_POOL_OF_FIRE);
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                                DoCast(target, SPELL_POOL_OF_FIRE);
                            _events.ScheduleEvent(EVENT_POOL_OF_FIRE, 30s, 40s);
                            break;

                        case EVENT_BURNING_SOUL:
                            Talk(SAY_BURNING_SOUL);
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                                DoCast(target, SPELL_BURNING_SOUL);
                            _events.ScheduleEvent(EVENT_BURNING_SOUL, 10s, 25s);
                            break;

                        case EVENT_ETERNAL_AGONY:
                            Talk(SAY_ETERNAL_AGONY);
                            DoCast(me, SPELL_ETERNAL_AGONY);
                            _events.ScheduleEvent(EVENT_ETERNAL_AGONY, 20s, 25s);
                            break;

                        default: break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new boss_ordosAI(creature);
        }
};

// Ancient Flames 144691.
class spell_ancient_flames : public SpellScriptLoader
{
    public:
        spell_ancient_flames() : SpellScriptLoader("spell_ancient_flames") { }

    private:
        class spell_ancient_flames_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ancient_flames_AuraScript);

            void HandlePeriodicTick(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                //if (Unit* target = ObjectAccessor::GetUnit(*GetCaster(), GetCaster()->GetGuidValue(UNIT_FIELD_CHANNEL_DATA)))
                    //GetCaster()->CastSpell(target, GetSpellInfo()->Effects[EFFECT_0].TriggerSpell, true);
                
                //GetCaster()->CastSpell(target, GetSpellValue()->EffectTriggerSpell[EFFECT_0], true);
                
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_ancient_flames_AuraScript::HandlePeriodicTick, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ancient_flames_AuraScript();
        }
};

// Ordos Burning Soul 144689.
class spell_ordos_burning_soul_aura : public SpellScriptLoader
{
    public:
        spell_ordos_burning_soul_aura() : SpellScriptLoader("spell_ordos_burning_soul_aura") { }

        class spell_ordos_burning_soul_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ordos_burning_soul_aura_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                switch (GetTargetApplication()->GetRemoveMode())
                {
                    case AURA_REMOVE_BY_EXPIRE:
                        break;

                    default: return;
                }

                GetTarget()->CastSpell(GetTarget(), SPELL_BURNING_SOUL_EX, true);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_ordos_burning_soul_aura_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_FEAR, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ordos_burning_soul_aura_AuraScript();
        }
};

// Magma Crush - 144688
class spell_ordos_magma_crush : public SpellScript
{
    PrepareSpellScript(spell_ordos_magma_crush);

    bool Load()
    {
        _targets = 1;
        return true;
    }

    void CountTargets(std::list<WorldObject*>& targets)
    {
        _targets = targets.size();
    }

    void SplitDamage(SpellEffIndex /*eff*/)
    {
        SetHitDamage(int32(GetHitDamage() / _targets));
    }

    void Register()
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ordos_magma_crush::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        OnEffectHitTarget += SpellEffectFn(spell_ordos_magma_crush::SplitDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }

    private:
        uint8 _targets;
};

/*class spell_area_ordos_pool_of_fire : public SpellAreaTriggerScript
{
public:
    spell_area_ordos_pool_of_fire() : SpellAreaTriggerScript("spell_area_ordos_pool_of_fire") { }

    bool OnAddTarget(AreaTrigger* trigger, Unit* target)
    {
        if (!trigger->GetCaster())
            return false;

        if (!target->isTargetableForAttack())
            return false;

        trigger->GetCaster()->AddAura(144693, target);

        return true;
    }

    bool OnRemoveTarget(AreaTrigger* trigger, Unit* target)
    {
        target->RemoveAura(144693);

        return true;
    }
};*/

void AddSC_boss_ordos()
{
    new boss_ordos();
    new spell_ancient_flames();
    new spell_ordos_burning_soul_aura();
    RegisterSpellScript(spell_ordos_magma_crush);
    //new spell_area_ordos_pool_of_fire();
}
