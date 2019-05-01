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
 
#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "AreaTriggerTemplate.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "antorus.h"
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
 
};
 
enum Texts
{
    SAY_GAROTHI_INTRO = 0,        /* Enemy contact. Deploying arsenal. */
    SAY_GAROTHI_AGGRO,            /* Enemy combatants detected. Threat level nominal. */
    SAY_GAROTHI_DECIMATION_1,     /* Targets acquired for decimation. */
    SAY_GAROTHI_DECIMATION_2,     /* Weapon system primed, target locked. */
    SAY_GAROTHI_ANNIHILATION_1,   /* Targets acquired. Sound: 86971 */
    SAY_GAROTHI_ANNIHILATION_2,   /* Cannon primed for annihilation. */
    SAY_GAROTHI_APOCALYPSE_DRIVE, /* Apocalypse drive engaged. */
    SAY_GAROTHI_ERADICATION,      /* Systems re-engaged, rerouting power. Weapons primed. */
 
    SAY_GAROTHI_PLAYER_KILLED_0,  /* Exterminated. */
    SAY_GAROTHI_PLAYER_KILLED_1,  /* Threat terminated. */
    SAY_GAROTHI_PLAYER_KILLED_2,  /* Hostile destroyed. */
 
    SAY_GAROTHI_WIPE_0,           /* Targets cleared. */
    SAY_GAROTHI_WIPE_1,           /* Enemy combatants neutralized. */
    SAY_GAROTHI_WIPE_2,           /* No hostile targets remaining. Continuing patrol. */
 
    SAY_GAROTHI_DEATH_0,          /* Critical system failure... */
    SAY_GAROTHI_DEATH_1,          /* Power core... rupturing... */
    SAY_GAROTHI_DEATH_2,          /* Termination imminent... */
};
 
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
    public:
        boss_garothi_worldbreaker(Creature* creature) : BossAI(creature, DATA_GAROTHI_WORLDBREAKER) { }
 
        void EnterCombat(Unit* /*attacker*/) override
        {
            _EnterCombat();
            Talk(SAY_GAROTHI_AGGRO);
        }
 
        void ExecuteEvent(uint32 eventId) override
        {
            switch (eventId)
            {
                case SPELL_ANNIHILATION:
                    break;
                case SPELL_DECIMATION:
                    break;
                case SPELL_FEL_BOMBARDMENT:
                    break;
                case SPELL_SURGING_FEL:
                    break;
                default:
                    break;
            }
        }
 
        void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) override
        {
            if (!HealthAbovePct(IsHeroic() ? 65 : 60))
            {
                /* It has to do the next couple of things:
                   - Cast apocalypse drive
                   - Expose the cannons so players can kill them
                   - If players can't kill any cannon and cast is completed = wipe.
                   - If players kills one cannon, the other one should be set to full health and cast Empowered. Immediately garothi starts to cast Eradication
                   - Heroic/Mythic: meanwhile the cannons are exposed, garothi will cast Surging Fel.
                   - Mythic: Haywire the cannons. -> The destroyed cannon gets a boost in his abilities
                */
            }
 
            if (!HealthAbovePct(IsHeroic() ? 35 : 20))
            {
                
                /* It has to do the next couple of things:
                   - Cast apocalypse drive
                   - Expose the last cannon.
                   - If players can't kill the last cannon = wipe.
                   - If players kills the cannon, Garothi immediately starts to cast Eradication
                   - Heroic/Mythic: When both cannons gets destroyed, garothi will cast Surging Fel until he dies.
                   - Mythic: Haywire both cannons. -> The destroyed cannon gets a boost in his abilities
                */
            }
        }
 
        void DoAction(int32 action) override
        {
            if (action != ACTION_ERADICATION)
                return;
 
            /* Things...*/
        }
 
    private:
        /*Safe*/
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
 
// 244410
class spell_decimation_aura : public AuraScript
{
    PrepareAuraScript(spell_decimation_aura);
 
    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
            caster->CastSpell(GetTarget(), SPELL_DECIMATION_PROJECTILE_AREA_EXPLODE);
    }
 
    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_decimation_aura::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
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
    RegisterAuraScript(spell_decimation_aura);
    RegisterAuraScript(spell_fel_bombardment_aura);
    RegisterAuraScript(spell_fel_bombardment_aura_missile);
    RegisterAuraScript(spell_searing_barrage);
}
