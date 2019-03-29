////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/*
 * Scripts for spells with SPELLFAMILY_DRUID and SPELLFAMILY_GENERIC spells used by druid players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dru_".
 */

#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Containers.h"
#include "HelperDefines.h"

/// last update : 6.2.3
/// Ysera's Gift - 145108
class spell_dru_yseras_gift: public SpellScriptLoader
{
    public:
        spell_dru_yseras_gift() : SpellScriptLoader("spell_dru_yseras_gift") { }

        class spell_dru_yseras_gift_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_yseras_gift_AuraScript);

            enum eSpells
            {
                YserasGiftHealCaster    = 145109,
                YserasGiftHealAlly      = 145110,
                EkowraithCreatorOfWorld = 210667
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                float l_Amount = p_AurEff->GetAmount();

                if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::EkowraithCreatorOfWorld, EFFECT_4))
                    AddPct(l_Amount, float(l_AuraEffect->GetAmount()));

                int32 l_HealAmount = l_Caster->CountPctFromMaxHealth(l_Amount);

                if (!l_Caster->IsFullHealth())
                {
                    SpellInfo const* l_YseraHeal = sSpellMgr->GetSpellInfo(eSpells::YserasGiftHealCaster);

                    l_Caster->HealBySpell(l_Caster, l_YseraHeal, l_HealAmount, false);
                }
                else
                {
                    float l_Radius = 30.0f;
                    std::list<Unit*> l_TargetList;
                    JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(l_Caster, l_Caster, l_Radius);
                    JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(l_Caster, l_TargetList, u_check);
                    l_Caster->VisitNearbyObject(l_Radius, searcher);

                    l_TargetList.remove_if([l_Caster](Unit* p_Unit) -> bool
                    {
                        if (p_Unit == nullptr)
                            return true;

                        if (!l_Caster->IsValidAssistTarget(p_Unit))
                            return true;

                        if (!l_Caster->IsInPartyWith(p_Unit) && !l_Caster->IsInRaidWith(p_Unit))
                            return true;

                        if (p_Unit->ToUnit()->IsFullHealth() || p_Unit->GetGUID() == l_Caster->GetGUID())
                            return true;

                        return false;
                    });

                    if (l_TargetList.size() > 1)
                    {
                        l_TargetList.sort(JadeCore::HealthPctOrderPred());
                        l_TargetList.resize(1);
                    }
                    if (l_TargetList.size() < 1)
                        return;

                    SpellInfo const* l_YseraHeal = sSpellMgr->GetSpellInfo(eSpells::YserasGiftHealAlly);

                    l_Caster->HealBySpell(l_TargetList.front(), l_YseraHeal, l_HealAmount, false);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_yseras_gift_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_yseras_gift_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Ysera's Gift (Ally heal) - 145110
class spell_dru_yseras_gift_ally_proc : public SpellScriptLoader
{
    public:
        spell_dru_yseras_gift_ally_proc() : SpellScriptLoader("spell_dru_yseras_gift_ally_proc") { }

        class spell_dru_yseras_gift_ally_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_yseras_gift_ally_proc_SpellScript);

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                Unit* l_Caster = GetCaster();

                p_Targets.remove_if([l_Caster](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || p_Object->ToUnit() == nullptr)
                        return true;

                    if (p_Object->ToUnit()->IsFullHealth() || p_Object->GetGUID() == l_Caster->GetGUID())
                        return true;

                    return false;
                });

                if (p_Targets.size() > 1)
                {
                    p_Targets.sort(JadeCore::HealthPctOrderPred());
                    p_Targets.resize(1);
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_yseras_gift_ally_proc_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_yseras_gift_ally_proc_SpellScript();
        }
};

/// Wrath - 5176
class spell_dru_wrath : public SpellScriptLoader
{
    public:
        spell_dru_wrath() : SpellScriptLoader("spell_dru_wrath") { }

        class spell_dru_wrath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wrath_SpellScript);

            enum eSpells
            {
                DreamOfCenariusRestoTalent  = 158504,
                DreamOfCenariusHeal         = 145153
            };

            void HandleBeforeHit()
            {
                /// Dream of Cenarius - 158504
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(eSpells::DreamOfCenariusRestoTalent))
                    {
                        std::list<Unit*> l_Party;

                        l_Caster->GetRaidMembers(l_Party);

                        if (l_Party.empty())
                            return;

                        if (l_Party.size() > 1)
                        {
                            l_Party.sort(JadeCore::HealthPctOrderPred());
                            l_Party.resize(1);
                        }

                        SpellInfo const* l_DreamOfCenariusSpellInfo = l_Caster->GetAura(eSpells::DreamOfCenariusRestoTalent)->GetSpellInfo();
                        SpellInfo const* l_DreamOfCenariusHealSpellInfo = sSpellMgr->GetSpellInfo(eSpells::DreamOfCenariusHeal);

                        if (l_DreamOfCenariusSpellInfo == nullptr || l_DreamOfCenariusHealSpellInfo == nullptr)
                            return;

                        int32 l_HealAmount = CalculatePct(GetHitDamage(), l_DreamOfCenariusSpellInfo->Effects[EFFECT_1].BasePoints);
                        l_Caster->HealBySpell(l_Party.front(), l_DreamOfCenariusHealSpellInfo, l_HealAmount, false);
                    }
                }
            }

            void Register() override
            {
                BeforeHit += SpellHitFn(spell_dru_wrath_SpellScript::HandleBeforeHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_wrath_SpellScript();
        }
};

/// Incarnation : Chosen of Elune - 102560
class spell_dru_incarnation_chosen_of_elune: public SpellScriptLoader
{
    public:
        spell_dru_incarnation_chosen_of_elune() : SpellScriptLoader("spell_dru_incarnation_chosen_of_elune") { }

        class spell_dru_incarnation_chosen_of_elune_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_incarnation_chosen_of_elune_AuraScript);

            enum eSpells
            {
                IncarnationChosenOfElune = 122114
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                    l_Player->CastSpell(l_Player, eSpells::IncarnationChosenOfElune, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                    l_Player->RemoveAura(eSpells::IncarnationChosenOfElune);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_incarnation_chosen_of_elune_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_chosen_of_elune_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_incarnation_chosen_of_elune_AuraScript();
        }
};

/// Last Update 7.0.3
/// Called by Incarnation : Chosen of Elune - 102560, Incarnation: Guardian of Ursoc - 102558 and Incarnation : King of the Jungle - 102543
/// Incarnation - Skins
class spell_dru_incarnation_skins : public SpellScriptLoader
{
    public:
        spell_dru_incarnation_skins() : SpellScriptLoader("spell_dru_incarnation_skins") { }

        class spell_dru_incarnation_skins_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_incarnation_skins_AuraScript);

            enum eSpells
            {
                GuardianofUrsoc             = 102558,
                BearForm                    = 5487,
                IncarnationKingOfTheJungle  = 102543,
                CatForm                     = 768,
                IncarnationChosenOfElune    = 102560,
                MoonkinForm                 = 24858,
                FeralInstinct               = 210631,
                FeralInstinctBuff           = 210649
            };

            enum eTraits
            {
                FeralInstinctPowerId = 1167
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                switch (GetSpellInfo()->Id)
                {
                    case eSpells::IncarnationKingOfTheJungle:
                    {
                        SpellInfo const* l_FeralInstinctSpellInfo = sSpellMgr->GetSpellInfo(eSpells::FeralInstinctBuff);
                        if (!l_FeralInstinctSpellInfo)
                            return;

                        int32 l_FeralInstinctBp0 = l_FeralInstinctSpellInfo->Effects[EFFECT_0].BasePoints * l_Player->GetRankOfArtifactPowerId(eTraits::FeralInstinctPowerId);
                        if (!l_Player->HasAura(eSpells::CatForm))
                            l_Player->CastSpell(l_Player, eSpells::CatForm, true);
                        if (l_Player->HasAura(eSpells::FeralInstinct))
                            l_Player->CastCustomSpell(l_Player, eSpells::FeralInstinctBuff, &l_FeralInstinctBp0, nullptr, nullptr, true);
                        break;
                    }
                    case eSpells::GuardianofUrsoc:
                        if (!l_Player->HasAura(eSpells::BearForm))
                            l_Player->CastSpell(l_Player, eSpells::BearForm, true);
                        break;
                    case eSpells::IncarnationChosenOfElune:
                        if (!l_Player->HasAura(eSpells::MoonkinForm))
                            l_Player->CastSpell(l_Player, eSpells::MoonkinForm, true);
                        break;
                }


                l_Player->SetDisplayId(l_Player->GetModelForForm(l_Player->GetShapeshiftForm()));
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                    l_Player->SetDisplayId(l_Player->GetModelForForm(l_Player->GetShapeshiftForm()));
            }

            void Register() override
            {
                switch (m_scriptSpellId)
                {
                    case 102543:// King of the Jungle
                        AfterEffectApply += AuraEffectApplyFn(spell_dru_incarnation_skins_AuraScript::OnApply, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
                        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_skins_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
                        break;
                    case 102558:///< Guardian of Ursoc
                        AfterEffectApply += AuraEffectApplyFn(spell_dru_incarnation_skins_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_skins_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                        break;
                    case 102560:// Chosen of Elune
                        AfterEffectApply += AuraEffectApplyFn(spell_dru_incarnation_skins_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_skins_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                        break;
                    default:
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_incarnation_skins_AuraScript();
        }
};

/// Item - PvP Feral 4P Bonus - 131537
class spell_dru_item_pvp_feral_4p: public SpellScriptLoader
{
    public:
        spell_dru_item_pvp_feral_4p() : SpellScriptLoader("spell_dru_item_pvp_feral_4p") { }

        class spell_dru_item_pvp_feral_4p_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_item_pvp_feral_4p_AuraScript);

            enum eSpells
            {
                Stampede                    = 81022,
                IncarnationKingOfTheJungle  = 102543
            };

            uint32 update;

            bool Validate(SpellInfo const* /*spell*/) override
            {
                update = 0;

                if (!sSpellMgr->GetSpellInfo(131537))
                    return false;
                return true;
            }

            void OnUpdate(uint32 diff, AuraEffect* /*aurEff*/)
            {
                if (!GetCaster())
                    return;

                if (GetCaster()->HasAura(eSpells::Stampede))
                    return;

                update += diff;

                if (GetCaster()->HasAura(eSpells::IncarnationKingOfTheJungle))
                    return;

                if (update >= 30 * IN_MILLISECONDS)
                {
                    if (Player* l_Player = GetCaster()->ToPlayer())
                        l_Player->CastSpell(l_Player, eSpells::Stampede, true);

                    update = 0;
                }
            }

            void Register() override
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_dru_item_pvp_feral_4p_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_item_pvp_feral_4p_AuraScript();
        }
};

/// Last Update 7.0.3
/// Trash - 77758
class spell_dru_thrash_bear : public SpellScriptLoader
{
    public:
        spell_dru_thrash_bear() : SpellScriptLoader("spell_dru_thrash_bear") { }

        class spell_dru_thrash_bear_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_thrash_bear_SpellScript);

            enum eSpells
            {
                TrashPeriodic           = 192090,
                TrashPulverizeMarker    = 158790,
                BloodFrenzy             = 203962,
                BloodFrenzyEnergy       = 203961
            };

            void HandleDamage(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Target->HasAura(eSpells::TrashPeriodic, l_Caster->GetGUID()))
                    l_Caster->CastSpell(l_Target, eSpells::TrashPulverizeMarker, true);

                l_Caster->CastSpell(l_Target, eSpells::TrashPeriodic, true);
                if (l_Caster->HasAura(eSpells::BloodFrenzy))
                    l_Caster->CastSpell(l_Caster, eSpells::BloodFrenzyEnergy, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_thrash_bear_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_thrash_bear_SpellScript();
        }
};

/// Last Update 7.0.3
/// Trash (Periodic) - 192090
class spell_dru_thrash_bear_periodic : public SpellScriptLoader
{
    public:
        spell_dru_thrash_bear_periodic() : SpellScriptLoader("spell_dru_thrash_bear_periodic") { }

        class spell_dru_thrash_bear_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_thrash_bear_periodic_AuraScript);

            enum eSpells
            {
                TrashPulverizeMarker    = 158790,
                BloodFrenzy             = 203962,
                BloodFrenzyEnergy       = 203961
            };

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::BloodFrenzy))
                    l_Caster->CastSpell(l_Caster, eSpells::BloodFrenzyEnergy, true);
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                l_Target->RemoveAura(eSpells::TrashPulverizeMarker, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_thrash_bear_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_thrash_bear_periodic_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_thrash_bear_periodic_AuraScript();
        }
};

/// Swipe - 106785
class spell_dru_swipe: public SpellScriptLoader
{
    public:
        spell_dru_swipe() : SpellScriptLoader("spell_dru_swipe") { }

        class spell_dru_swipe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_swipe_SpellScript);

            void HandleDamage(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                int32 l_Damage = GetHitDamage();

                /// Award 1 combot point
                l_Caster->AddComboPoints(GetSpellInfo()->Effects[EFFECT_0].BasePoints);

                /// Swipe and Maul deals 20% more damage if target is bleeding
                if (l_Target->HasAuraState(AURA_STATE_BLEEDING))
                {
                    AddPct(l_Damage, GetSpellInfo()->Effects[EFFECT_1].BasePoints);
                    SetHitDamage(l_Damage);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_swipe_SpellScript::HandleDamage, EFFECT_2, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_swipe_SpellScript();
        }
};

/// Maul - 6807
class spell_dru_maul: public SpellScriptLoader
{
    public:
        spell_dru_maul() : SpellScriptLoader("spell_dru_maul") { }

        class spell_dru_maul_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_maul_SpellScript);

            enum eSpells
            {
                T17Guardian4P   = 165423,
                PrimalMending   = 177969
            };

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        int32 l_Damage = GetHitDamage();

                        // Deals 20% more damage if target is bleeding
                        if (l_Target->HasAuraState(AURA_STATE_BLEEDING))
                        {
                            AddPct(l_Damage, GetSpellInfo()->Effects[EFFECT_3].BasePoints);
                            SetHitDamage(l_Damage);
                        }
                    }
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_maul_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_maul_SpellScript();
        }
};

/// Nature's Vigil - 124974
class spell_dru_natures_vigil: public SpellScriptLoader
{
    public:
        spell_dru_natures_vigil() : SpellScriptLoader("spell_dru_natures_vigil") { }

        class spell_dru_natures_vigil_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_natures_vigil_AuraScript);

            enum eSpells
            {
                NatureVigilHeal     = 124988,
                NatureVigilDamage   = 124991
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellProcInfo = p_EventInfo.GetDamageInfo()->GetSpellInfo();

                if (l_Caster == nullptr || l_SpellProcInfo == nullptr)
                    return;

                if (l_SpellProcInfo->Id == eSpells::NatureVigilHeal || l_SpellProcInfo->Id == eSpells::NatureVigilDamage)
                    return;

                bool l_SingleTarget = false;
                for (uint8 i = 0; i < l_SpellProcInfo->EffectCount; ++i)
                {
                    if ((l_SpellProcInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_TARGET_ALLY ||
                        l_SpellProcInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_TARGET_ENEMY) &&
                        l_SpellProcInfo->Effects[i].TargetB.GetTarget() == 0)
                        l_SingleTarget = true;
                }

                if (!l_SingleTarget)
                    return;

                int32 l_Bp = CalculatePct(p_EventInfo.GetDamageInfo()->GetAmount(), GetSpellInfo()->Effects[EFFECT_2].BasePoints);

                if (!l_SpellProcInfo->IsPositive())
                    l_Bp = CalculatePct(p_EventInfo.GetDamageInfo()->GetAmount(), GetSpellInfo()->Effects[EFFECT_3].BasePoints);

                l_Caster->CastCustomSpell(l_Caster, eSpells::NatureVigilHeal, &l_Bp, NULL, NULL, true);

                Player *l_Player = l_Caster->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->GetActiveSpecializationID() == SPEC_DRUID_RESTORATION && l_SpellProcInfo->IsPositive())
                    l_Caster->CastCustomSpell(l_Caster, eSpells::NatureVigilDamage, &l_Bp, NULL, NULL, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_natures_vigil_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_natures_vigil_AuraScript();
        }
};

/// Nature's Vigil (heal) - 124988, Nature's Vigil (damage) - 124991
class spell_dru_natures_vigil_proc : public SpellScriptLoader
{
    public:
        spell_dru_natures_vigil_proc() : SpellScriptLoader("spell_dru_natures_vigil_proc") { }

        class spell_dru_natures_vigil_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_natures_vigil_proc_SpellScript);

            enum eSpells
            {
                NatureVigilHeal     = 124988,
                NatureVigilDamage   = 124991
            };

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.size() > 1)
                {
                    p_Targets.sort(JadeCore::HealthPctOrderPred());
                    p_Targets.resize(1);
                }
            }

            void Register() override
            {
                switch (m_scriptSpellId)
                {
                    case eSpells::NatureVigilHeal:
                        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_natures_vigil_proc_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
                        break;
                    case eSpells::NatureVigilDamage:
                        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_natures_vigil_proc_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                        break;
                    default:
                        break;
                }
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_natures_vigil_proc_SpellScript();
        }
};

/// Dash - 1850
class spell_dru_dash: public SpellScriptLoader
{
    public:
        spell_dru_dash() : SpellScriptLoader("spell_dru_dash") { }

        class spell_dru_dash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_dash_SpellScript);

            enum eSpells
            {
                StampedingRoar = 106898
            };

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->HasAura(eSpells::StampedingRoar))
                        l_Player->RemoveAura(eSpells::StampedingRoar);

                    l_Player->RemoveMovementImpairingAuras();
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_dash_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_dash_SpellScript();
        }
};

/// last update : 6.2.3
/// Germination - 155777
class spell_dru_germination : public SpellScriptLoader
{
    public:
        spell_dru_germination() : SpellScriptLoader("spell_dru_germination") { }

        class spell_dru_germination_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_germination_AuraScript);

            enum eSpells
            {
                SoulOfTheForestResto = 114108
            };

            void HandleCalculateAmount(AuraEffect const* p_AurEff, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    ///If soul of the forest is activated we increase the heal by 100%
                    if (l_Caster->HasAura(eSpells::SoulOfTheForestResto))
                    {
                        amount *= 2;
                        l_Caster->AddDelayedEvent([=]() -> void {l_Caster->RemoveAura(eSpells::SoulOfTheForestResto); }, 10);
                    }
                }
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_germination_AuraScript::HandleCalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_germination_AuraScript();
        }
};

/// last update 7.0.3 Build 225221
/// Called by Predatory Swiftness - 16974
class spell_dru_predatory_swiftness : public SpellScriptLoader
{
    public:
        spell_dru_predatory_swiftness() : SpellScriptLoader("spell_dru_predatory_swiftness") { }

        class spell_dru_predatory_swiftness_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_predatory_swiftness_AuraScript);

            enum eSpells
            {
                PredatorySwiftnessProc = 69369,

                Maim1                  = 168877,
                Maim2                  = 168878,
                Maim3                  = 168879,
                Maim4                  = 168880,
                Maim5                  = 168881
            };

            uint32 m_ComboPointsUsed = 0;

            void HandleOnProc(ProcEventInfo& p_ProcEventInfo)
            {
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                DamageInfo* l_DamageInfo = p_ProcEventInfo.GetDamageInfo();
                if (!l_SpellInfo || !l_DamageInfo)
                    return;

                Unit* l_Caster = l_DamageInfo->GetActor();
                SpellInfo const* l_ProcSpellInfo = l_DamageInfo->GetSpellInfo();
                if (!l_Caster || !l_ProcSpellInfo)
                    return;

                if (!l_ProcSpellInfo->IsFinishingMove() && !IsMaimSpell(l_ProcSpellInfo))
                    return;

                if (!roll_chance_i(m_ComboPointsUsed * l_SpellInfo->Effects[EFFECT_2].BasePoints))
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::PredatorySwiftnessProc, true);
            }

            void SetGuid(uint32 p_Index, uint64 p_Value) override
            {
                if (p_Index != 1)
                    return;

                m_ComboPointsUsed = p_Value;
            }

            void Register() override
            {
                OnProc += AuraProcFn(spell_dru_predatory_swiftness_AuraScript::HandleOnProc);
            }

        private:

            bool IsMaimSpell(SpellInfo const* p_SpellInfo) const
            {
                switch (p_SpellInfo->Id)
                {
                    case eSpells::Maim1:
                    case eSpells::Maim2:
                    case eSpells::Maim3:
                    case eSpells::Maim4:
                    case eSpells::Maim5:
                        return true;
                }

                return false;
            }

        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_predatory_swiftness_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Rejuvenation - 774
class spell_dru_rejuvenation : public SpellScriptLoader
{
    public:
        spell_dru_rejuvenation() : SpellScriptLoader("spell_dru_rejuvenation") { }

        class spell_dru_rejuvenation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_rejuvenation_SpellScript);

            enum eSpells
            {
                Rejuvenation       = 774,
                Germination        = 155777,
                GerminationPassive = 155675
            };

            int32 m_RejuvenationAura = 0;
            int32 m_RejuvenationAuraAmount = 0;

            void HandleBeforeHit()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Unit* l_Target = GetHitUnit();
                if (!l_Target)
                    return;

                ///Germination
                if (l_Caster->HasAura(eSpells::GerminationPassive) && l_Target->HasAura(eSpells::Rejuvenation, l_Caster->GetGUID()))
                {
                    Aura* l_RejuvenationAura = l_Target->GetAura(eSpells::Rejuvenation, l_Caster->GetGUID());
                    if (!l_RejuvenationAura)
                        return;

                    if (!l_Target->HasAura(eSpells::Germination, l_Caster->GetGUID()))
                    {
                        l_Caster->CastSpell(l_Target, eSpells::Germination, true);
                        m_RejuvenationAura = l_RejuvenationAura->GetDuration();
                    }
                    else
                    {
                        Aura* l_GerminationAura = l_Target->GetAura(eSpells::Germination, l_Caster->GetGUID());
                        Aura* l_RejuvenationAura = l_Target->GetAura(eSpells::Rejuvenation, l_Caster->GetGUID());
                        if (l_GerminationAura && l_RejuvenationAura)
                        {
                            int32 l_GerminationDuration = l_GerminationAura->GetDuration();
                            int32 l_RejuvenationDuration = l_RejuvenationAura->GetDuration();
                            if (l_GerminationDuration > l_RejuvenationDuration)
                            {
                                l_Caster->AddAura(eSpells::Rejuvenation, l_Target);
                            }
                            else
                            {
                                l_Caster->CastSpell(l_Target, eSpells::Germination, true);
                                m_RejuvenationAura = l_RejuvenationDuration;
                            }
                        }
                    }
                }
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Unit* l_Target = GetHitUnit();
                if (!l_Target)
                    return;

                Aura* l_RejuvenationAura = l_Target->GetAura(eSpells::Rejuvenation, l_Caster->GetGUID());

                if (l_RejuvenationAura && m_RejuvenationAura > 0)
                    l_RejuvenationAura->SetDuration(m_RejuvenationAura);
            }

            void Register() override
            {
                if (m_scriptSpellId == eSpells::Rejuvenation)
                {
                    BeforeHit += SpellHitFn(spell_dru_rejuvenation_SpellScript::HandleBeforeHit);
                    AfterHit += SpellHitFn(spell_dru_rejuvenation_SpellScript::HandleAfterHit);
                }
            }
        };

        class spell_dru_rejuvenation_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_rejuvenation_AuraScript);

            enum eSpells
            {
                Abundance                 = 207383,
                AbundanceAura             = 207640,
                Cultivation               = 200390,
                CultivationAura           = 200389,
                Rejuvenation              = 774
            };

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (l_Caster->HasAura(eSpells::Abundance))
                    l_Caster->CastSpell(l_Caster, eSpells::AbundanceAura, true);
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Aura* l_Aura = l_Caster->GetAura(eSpells::AbundanceAura);
                if (l_Aura == nullptr)
                    return;

                l_Aura->DropStack();
            }

            void OnTick(AuraEffect const* p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();
                if (!l_Caster || !l_Target)
                    return;

                if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::Cultivation, EFFECT_0))
                {
                    if (l_Target->GetHealthPct() < l_AuraEffect->GetAmount())
                        l_Caster->CastSpell(l_Target, eSpells::CultivationAura, true);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_rejuvenation_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                AfterEffectApply += AuraEffectApplyFn(spell_dru_rejuvenation_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_rejuvenation_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_rejuvenation_AuraScript();
        }

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_rejuvenation_SpellScript();
        }
};

///Legion 7.1.5 - build - 23420
///Called by Regrowth - 8936
class spell_dru_regrowth : public SpellScriptLoader
{
    public:
        spell_dru_regrowth() : SpellScriptLoader("spell_dru_regrowth") { }

        class spell_dru_regrowth_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_regrowth_SpellScript);

            enum eSpells
            {
                Clearcastering              = 16870,
                MomentOfClarity             = 155577,
                ProtectorOfTheGrove         = 209730,
                ProtectorOfTheGroveEffect   = 209731,
                BloodtalonsAura             = 155672,
                BloodtalonsProc             = 145152,
                PowerOfElune                = 208284
            };

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (l_Caster->HasAura(eSpells::BloodtalonsAura))
                    l_Caster->CastSpell(l_Caster, eSpells::BloodtalonsProc, true);

                if (!l_Caster->HasAura(eSpells::Clearcastering))
                    return;
            }

            void HandleOnCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                SpellInfo const* l_PowerOfEluneInfo = sSpellMgr->GetSpellInfo(eSpells::PowerOfElune);
                if (!l_Caster || !l_Target || !l_PowerOfEluneInfo)
                    return;

                if (l_Caster->HasAura(eSpells::ProtectorOfTheGrove) && l_Target != l_Caster)
                    l_Caster->CastSpell(l_Caster, eSpells::ProtectorOfTheGroveEffect, true);

                l_Caster->m_SpellHelper.GetUint64(eSpellHelpers::Regrowth) = l_PowerOfEluneInfo->Effects[EFFECT_0].BasePoints * l_Caster->GetAuraCount(eSpells::PowerOfElune);
                Aura* l_Aura = l_Caster->GetAura(eSpells::Clearcastering);
                if (!l_Aura)
                    l_Caster->m_SpellHelper.GetUint32(eSpellHelpers::ClearCastering) = 0;
                else
                    l_Caster->m_SpellHelper.GetUint32(eSpellHelpers::ClearCastering) = l_Aura->GetStackAmount();
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                if (!l_Caster || !l_SpellInfo)
                    return;

                int32 l_HealAmount = GetHitHeal();
                if (l_Caster->HasAura(eSpells::PowerOfElune))
                    SetHitHeal(AddPct(l_HealAmount, l_Caster->m_SpellHelper.GetUint64(eSpellHelpers::Regrowth)));

                Aura* l_Aura = l_Caster->GetAura(eSpells::Clearcastering);
                if (!l_Aura)
                    return;

                l_Aura->SetStackAmount(l_Aura->GetStackAmount() - 1);
                if (l_Aura->GetStackAmount() <= 0)
                    l_Aura->Remove();

                SpellInfo const* l_MomentOfClarityInfo = sSpellMgr->GetSpellInfo(eSpells::MomentOfClarity);
                if (l_Caster->HasAura(eSpells::MomentOfClarity) && l_MomentOfClarityInfo)
                    SetHitHeal(AddPct(l_HealAmount, l_MomentOfClarityInfo->Effects[EFFECT_2].BasePoints));

            }

            void Register() override
            {
                AfterHit += SpellHitFn(spell_dru_regrowth_SpellScript::HandleAfterHit);
                OnCast += SpellCastFn(spell_dru_regrowth_SpellScript::HandleOnCast);
                OnHit += SpellHitFn(spell_dru_regrowth_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_regrowth_SpellScript();
        }

        class spell_dru_regrowth_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_regrowth_AuraScript);

            enum eSpells
            {
                MomentOfClarity = 155577,
                PowerOfElune    = 208284
            };

            void HandleAfterApply(AuraEffect const* p_AuraEffect, AuraEffectHandleModes p_Modes)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_PowerOfEluneInfo = sSpellMgr->GetSpellInfo(eSpells::PowerOfElune);
                if (!l_Caster || !l_PowerOfEluneInfo)
                    return;

                int32 l_HealingAmount = p_AuraEffect->GetAmount();
                AddPct(l_HealingAmount, l_Caster->m_SpellHelper.GetUint64(eSpellHelpers::Regrowth));
                const_cast<AuraEffect*>(p_AuraEffect)->SetAmount(l_HealingAmount);
                Aura* l_Aura = l_Caster->GetAura(eSpells::MomentOfClarity);
                if (!l_Aura || l_Caster->m_SpellHelper.GetUint32(eSpellHelpers::ClearCastering) == 0)
                    return;

                AuraEffect* l_AuraEffect = l_Aura->GetEffect(EFFECT_2);
                if (!l_AuraEffect)
                    return;

                const_cast<AuraEffect*>(p_AuraEffect)->SetAmount(p_AuraEffect->GetAmount() + (p_AuraEffect->GetAmount() * l_AuraEffect->GetAmount() / 100));
            }

            void HandleAfterRemove(AuraEffect const* /* p_AuraEffect */, AuraEffectHandleModes /* p_Modes */)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                l_Caster->m_SpellHelper.GetUint64(eSpellHelpers::Regrowth) = 0;
            }

            void HandleOnUpdate(AuraEffect* p_AuraEffect)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_PowerOfEluneInfo = sSpellMgr->GetSpellInfo(eSpells::PowerOfElune);
                if (!l_Caster || !l_PowerOfEluneInfo)
                    return;

                int32 l_HealingAmount = p_AuraEffect->GetAmount();
                AddPct(l_HealingAmount, l_Caster->m_SpellHelper.GetUint64(eSpellHelpers::Regrowth));
                p_AuraEffect->SetAmount(l_HealingAmount);
                l_Caster->m_SpellHelper.GetUint64(eSpellHelpers::Regrowth) = 0;
            }

            void Register() override
            {
                OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_dru_regrowth_AuraScript::HandleOnUpdate, EFFECT_1, SPELL_AURA_PERIODIC_HEAL);
                AfterEffectApply += AuraEffectApplyFn(spell_dru_regrowth_AuraScript::HandleAfterApply, EFFECT_1, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_regrowth_AuraScript::HandleAfterApply, EFFECT_1, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_regrowth_AuraScript();
        }
};

/// Last Update 6.2.3
/// Wild growth - 48438
class spell_dru_wild_growth : public SpellScriptLoader
{
    public:
        spell_dru_wild_growth() : SpellScriptLoader("spell_dru_wild_growth") { }

        class spell_dru_wild_growth_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wild_growth_SpellScript);

            enum eSpells
            {
                TreeOfLife              = 33891,
                NaturesEssence          = 189787,
                MatureEssenceHeal       = 189800,
                Dreamwalker             = 189849,
                TearstoneOfElune        = 207932,
                Rejuvination            = 774,
                PowerOfTheArchdruid     = 189870,
                PowerOfTheArchdruidProc = 213121,
            };

            bool m_HasSoulOfTheForest = false;

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                uint8 l_MaxTargets = GetSpellInfo()->Effects[EFFECT_2].BasePoints + 1; ///< +1 = Main Target
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::TreeOfLife))
                    l_MaxTargets += 2;

                if (p_Targets.size() > l_MaxTargets)
                {
                    p_Targets.sort(JadeCore::HealthPctOrderPred());
                    p_Targets.resize(l_MaxTargets);
                }
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                if (!l_Caster || !l_Target)
                    return;

                if (l_Caster->HasAura(eSpells::NaturesEssence))
                    l_Caster->CastSpell(l_Target, eSpells::MatureEssenceHeal, true);

                if (l_Caster->HasAura(eSpells::TearstoneOfElune))
                {
                    SpellInfo const* l_SpellInfoElune = sSpellMgr->GetSpellInfo(eSpells::TearstoneOfElune);
                    if (!l_SpellInfoElune)
                        return;

                    if (roll_chance_i(l_SpellInfoElune->Effects[EFFECT_0].BasePoints))
                        l_Caster->CastSpell(l_Target, eSpells::Rejuvination, true);
                }
            }

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                SpellInfo const* l_DreamWalker = sSpellMgr->GetSpellInfo(eSpells::Dreamwalker);
                if (l_DreamWalker != nullptr && l_Caster->HasAura(eSpells::Dreamwalker) && roll_chance_i(l_DreamWalker->ProcChance))
                    l_Caster->CastSpell(l_Caster, l_DreamWalker->Effects[EFFECT_0].TriggerSpell, true);

                SpellInfo const* l_PowerOfTheArchdruid = sSpellMgr->GetSpellInfo(eSpells::PowerOfTheArchdruid);
                if (l_PowerOfTheArchdruid && l_Caster->HasAura(eSpells::PowerOfTheArchdruid) && roll_chance_i(l_PowerOfTheArchdruid->ProcChance))
                    l_Caster->CastSpell(l_Caster, l_PowerOfTheArchdruid->Effects[EFFECT_0].TriggerSpell, true);
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_wild_growth_SpellScript::HandleAfterCast);
                AfterHit += SpellHitFn(spell_dru_wild_growth_SpellScript::HandleAfterHit);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_growth_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_wild_growth_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        class spell_dru_wild_growth_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_wild_growth_AuraScript);

            enum eSpells
            {
                T10Resto2PBonus = 70658
            };

            uint32 m_TooltipAmount;

            void HandleCalculateAmountOnTick(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Owner = GetUnitOwner();
                if (!l_Caster || !l_Owner)
                    return;

                m_TooltipAmount = 7 * p_Amount; ///< The base healing is split among the ticks with the first tick getting (6%+1/7) of the tooltip heal
                p_Amount += CalculatePct(m_TooltipAmount, 6);
                l_Caster->m_SpellHelper.m_Uint64Map[eSpellHelpers::WildGrowth][l_Owner->GetGUID()] = m_TooltipAmount;
            }

            void DecreaseHealOnTick(AuraEffect* p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                float l_SetMod = 1.0f;

                // Item - Druid T10 Restoration 2P Bonus
                if (AuraEffect* l_T10Resto2PBonus = l_Caster->GetAuraEffect(eSpells::T10Resto2PBonus, EFFECT_0))
                    l_SetMod = 1.0f - l_T10Resto2PBonus->GetAmount() / 100.0f;

                int32 l_Amount = p_AurEff->GetAmount();

                l_Amount -= l_SetMod * CalculatePct(m_TooltipAmount, 2);  ///< "each successive tick losing 2% of the tooltip heal" http://wowwiki.wikia.com/Wild_Growth
                p_AurEff->SetAmount(l_Amount);
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_wild_growth_AuraScript::HandleCalculateAmountOnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_dru_wild_growth_AuraScript::DecreaseHealOnTick,  EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_wild_growth_SpellScript();
        }

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_wild_growth_AuraScript();
        }
};

///Last Update Legion 7.1.5 build 23420
/// Lifebloom - 33763
class spell_dru_lifebloom : public SpellScriptLoader
{
    public:
        spell_dru_lifebloom() : SpellScriptLoader("spell_dru_lifebloom") { }

        class spell_dru_lifebloom_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_lifebloom_AuraScript);

            enum eSpells
            {
                LifebloomFinalHeal      = 33778,
                OmenOfClarity           = 113043,
                MomentOfClarityTalent   = 155577,
                ClearCasting            = 16870,
                TheDarkTitansAdvice     = 207271
            };

            void AfterRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                // Final heal only on duration end
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                    return;

                Unit* l_Target = GetUnitOwner();
                Unit* l_Caster = GetCaster();

                if (!l_Caster || !l_Target)
                    return;

                SpellInfo const* l_LifebloomFinalHeal = sSpellMgr->GetSpellInfo(eSpells::LifebloomFinalHeal);
                SpellInfo const* l_TheDarkTitansAdviceBonus = sSpellMgr->GetSpellInfo(eSpells::TheDarkTitansAdvice);

                // final heal
                int32 l_SpellPower = 0;
                int32 l_HealAmount = 0;

                l_SpellPower = l_Caster->SpellHealingBonusDone(l_Target, GetSpellInfo(), l_SpellPower, p_AurEff->GetEffIndex(), HEAL);
                l_SpellPower = l_Target->SpellHealingBonusTaken(l_Caster, GetSpellInfo(), l_SpellPower, HEAL);

                if (l_LifebloomFinalHeal != nullptr)
                    l_HealAmount = l_SpellPower * l_LifebloomFinalHeal->Effects[EFFECT_0].BonusMultiplier;

                if (l_Caster->HasAura(eSpells::TheDarkTitansAdvice))
                    AddPct(l_HealAmount, l_TheDarkTitansAdviceBonus->Effects[EFFECT_0].BasePoints);

                l_Caster->CastCustomSpell(l_Target, eSpells::LifebloomFinalHeal, &l_HealAmount, NULL, NULL, true, NULL, p_AurEff, GetCasterGUID());
            }

            void HandleDispel(DispelInfo* p_DispelInfo)
            {
                if (AuraEffect const* aurEff = GetEffect(EFFECT_0))
                {
                    Unit* l_Target = GetUnitOwner();
                    Unit* l_Caster = GetCaster();

                    if (!l_Caster || !l_Target)
                        return;

                    SpellInfo const* l_LifebloomFinalHeal = sSpellMgr->GetSpellInfo(eSpells::LifebloomFinalHeal);
                    SpellInfo const* l_TheDarkTitansAdviceBonus = sSpellMgr->GetSpellInfo(eSpells::TheDarkTitansAdvice);

                    // final heal
                    int32 l_SpellPower = 0;
                    int32 l_HealAmount = 0;

                    l_SpellPower = l_Caster->SpellHealingBonusDone(l_Target, GetSpellInfo(), l_SpellPower, aurEff->GetEffIndex(), HEAL, p_DispelInfo->GetRemovedCharges());
                    l_SpellPower = l_Target->SpellHealingBonusTaken(l_Caster, GetSpellInfo(), l_SpellPower, HEAL, p_DispelInfo->GetRemovedCharges());

                    if (l_LifebloomFinalHeal != nullptr)
                        l_HealAmount = l_SpellPower * l_LifebloomFinalHeal->Effects[EFFECT_0].BonusMultiplier;

                    if (l_Caster->HasAura(eSpells::TheDarkTitansAdvice))
                        AddPct(l_HealAmount, l_TheDarkTitansAdviceBonus->Effects[EFFECT_0].BasePoints);

                    l_Caster->CastCustomSpell(l_Target, eSpells::LifebloomFinalHeal, &l_HealAmount, NULL, NULL, true, NULL, aurEff, GetCasterGUID());
                }
            }

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster && l_Caster->HasAura(eSpells::OmenOfClarity) && roll_chance_i(4))
                    l_Caster->CastSpell(l_Caster, eSpells::ClearCasting, true);
            }

            void HandleAfterReapply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();
                if (!l_Caster || !l_Target)
                    return;

                Aura* l_Aura = GetAura();
                if (!l_Aura || l_Aura->GetDuration() > 5000)
                    return;

                SpellInfo const* l_LifebloomFinalHeal = sSpellMgr->GetSpellInfo(eSpells::LifebloomFinalHeal);
                SpellInfo const* l_TheDarkTitansAdviceBonus = sSpellMgr->GetSpellInfo(eSpells::TheDarkTitansAdvice);

                // final heal
                int32 l_SpellPower = 0;
                int32 l_HealAmount = 0;

                l_SpellPower = l_Caster->SpellHealingBonusDone(l_Target, GetSpellInfo(), l_SpellPower, p_AurEff->GetEffIndex(), HEAL);
                l_SpellPower = l_Target->SpellHealingBonusTaken(l_Caster, GetSpellInfo(), l_SpellPower, HEAL);

                if (l_LifebloomFinalHeal != nullptr)
                    l_HealAmount = l_SpellPower * l_LifebloomFinalHeal->Effects[EFFECT_0].BonusMultiplier;

                if (l_Caster->HasAura(eSpells::TheDarkTitansAdvice))
                    AddPct(l_HealAmount, l_TheDarkTitansAdviceBonus->Effects[EFFECT_0].BasePoints);

                l_Caster->CastCustomSpell(l_Target, eSpells::LifebloomFinalHeal, &l_HealAmount, NULL, NULL, true, NULL, p_AurEff, GetCasterGUID());
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_lifebloom_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                OnDispel += AuraDispelFn(spell_dru_lifebloom_AuraScript::HandleDispel);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_lifebloom_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                AfterEffectApply += AuraEffectApplyFn(spell_dru_lifebloom_AuraScript::HandleAfterReapply, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAPPLY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_lifebloom_AuraScript();
        }
};

/// Last Updatr 6.2.3
/// Called by Cat Form - 768 and Bear Form - 5487
/// Killer Instinct - 108299
class spell_dru_killer_instinct: public SpellScriptLoader
{
    public:
        spell_dru_killer_instinct() : SpellScriptLoader("spell_dru_killer_instinct") { }

        class spell_dru_killer_instinct_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_killer_instinct_AuraScript);

            enum eSpells
            {
                KillerInstinct          = 108299,
                KillerInstinctModStat   = 108300
            };

            void AfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*m_Mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                {
                    if (l_Player->HasAura(eSpells::KillerInstinct))
                    {
                        int32 l_Bp = l_Player->GetStat(Stats::STAT_INTELLECT);

                        int32 l_IntellectFromWeapon = 0;
                        if (MS::Artifact::Manager* l_Manager = l_Player->GetCurrentlyEquippedArtifact())
                        {
                            if (Item* l_Item = l_Manager->ToItem())
                            {
                                for (uint8 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
                                {
                                    if (l_Item->GetItemStatType(i) != static_cast<int32>(ItemModType::INTELLECT))
                                        continue;

                                    l_IntellectFromWeapon += std::max(0, l_Item->GetItemStatValue(i, l_Player, l_Item->GetScaleIlvl()));
                                }
                            }
                        }

                        l_Bp -= l_IntellectFromWeapon;

                        l_Player->CastCustomSpell(l_Player, eSpells::KillerInstinctModStat, &l_Bp, nullptr, nullptr, true);
                    }
                }
            }

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*m_Mode*/)
            {
                if (Player* l_Player = GetTarget()->ToPlayer())
                {
                    if (l_Player->HasAura(eSpells::KillerInstinct))
                        l_Player->RemoveAura(eSpells::KillerInstinctModStat);
                }
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_killer_instinct_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_killer_instinct_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_killer_instinct_AuraScript();
        }
};

/// Nature's Cure - 88423 and Remove Corruption - 2782
class spell_dru_natures_cure: public SpellScriptLoader
{
    public:
        spell_dru_natures_cure() : SpellScriptLoader("spell_dru_natures_cure") { }

        class spell_dru_natures_cure_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_natures_cure_SpellScript);

            SpellCastResult CheckCleansing()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                if (!l_Caster || !l_Target)
                    return SPELL_FAILED_ERROR;

                DispelChargesList l_DispelList;

                // Create dispel mask by dispel type
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                for (uint8 i = 0; i < l_SpellInfo->EffectCount; ++i)
                {
                    if (!l_SpellInfo->Effects[i].IsEffect())
                        break;

                    uint32 l_DispelType = l_SpellInfo->Effects[i].MiscValue;
                    uint32 l_DispelMask = l_SpellInfo->GetDispelMask(DispelType(l_DispelType));

                    l_Target->GetDispellableAuraList(l_Caster, l_DispelMask, l_DispelList);
                }

                if (l_DispelList.empty())
                    return SPELL_FAILED_NOTHING_TO_DISPEL;

                return SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_natures_cure_SpellScript::CheckCleansing);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_natures_cure_SpellScript();
        }
};

/// Last Update 6.2.3
/// Called by Prowl - 5215, Displacer Beast - 102280 and Dash - 1850
/// Should activate the cat form if not in cat form
class spell_dru_activate_cat_form : public SpellScriptLoader
{
    public:
        spell_dru_activate_cat_form() : SpellScriptLoader("spell_dru_activate_cat_form") { }

        class spell_dru_activate_cat_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_activate_cat_form_SpellScript);

            enum eSpells
            {
                CatForm                 = 768,
                IncarnationTreeofLife   = 33891
            };

            void HandleBeforeHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasAura(eSpells::IncarnationTreeofLife)) ///< Prevent it to be remove by default
                    l_Player->RemoveAura(eSpells::IncarnationTreeofLife, 0, 0, AURA_REMOVE_BY_CANCEL);
                if (l_Player && l_Player->GetShapeshiftForm() != FORM_CAT_FORM)
                    l_Player->CastSpell(l_Player, eSpells::CatForm, true);
            }

            void Register() override
            {
                BeforeHit += SpellHitFn(spell_dru_activate_cat_form_SpellScript::HandleBeforeHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_activate_cat_form_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Cat Form - 768, Cat Form (Claws of Shirvallah) - 171746
class spell_dru_cat_form: public SpellScriptLoader
{
    public:
        spell_dru_cat_form() : SpellScriptLoader("spell_dru_cat_form") { }

        class spell_dru_cat_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_cat_form_SpellScript);

            enum eSpells
            {
                SavageRoar = 52610
            };

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                l_Player->RemoveMovementImpairingAuras();

                /// Fix Berserk - amount of energy after reshift
                l_Player->UpdateMaxPower(POWER_ENERGY);
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_cat_form_SpellScript::HandleOnHit);
            }
        };

        class spell_dru_cat_form_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_cat_form_AuraScript);

            enum eSpells
            {
                Dash                = 1850,
                Prowl               = 5215,
                DesplacerBeast      = 137452,
                BurningEssence      = 138927,
                BurningEssenceModel = 38150
            };

            enum eDatas
            {
                FandralsFlamescythe         = 69897,
                FandralsFlamescytheHeroic   = 71466
            };

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();

                SpellInfo const* l_Dash = sSpellMgr->GetSpellInfo(eSpells::Dash);
                if (!l_Dash)
                    return;

                /// If we have dash, we should restore it to increase movement speed
                if (AuraEffect* l_DashAura = l_Target->GetAuraEffect(eSpells::Dash, EFFECT_0))
                    l_DashAura->ChangeAmount(l_Dash->Effects[EFFECT_0].BasePoints);
            }

            void AfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                Item const* l_Weapon = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

                if (l_Player->HasAura(eSpells::BurningEssence) || (l_Weapon && l_Weapon->GetTemplate() && (l_Weapon->GetTemplate()->ItemId == eDatas::FandralsFlamescythe || l_Weapon->GetTemplate()->ItemId == eDatas::FandralsFlamescytheHeroic)))
                    l_Player->SetDisplayId(eSpells::BurningEssenceModel);
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();

                if (l_Target->HasAura(eSpells::Prowl))
                    l_Target->RemoveAura(eSpells::Prowl);

                /// When we remove cat form dash shouldn't increase movement speed
                if (AuraEffect* l_DashAura = l_Target->GetAuraEffect(eSpells::Dash, EFFECT_0))
                    l_DashAura->ChangeAmount(0);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_cat_form_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                OnEffectApply += AuraEffectApplyFn(spell_dru_cat_form_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_cat_form_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_cat_form_SpellScript();
        }

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_cat_form_AuraScript();
        }
};

enum SkullBashSpells
{
    SPELL_DRUID_SKULL_BASH_INTERUPT = 93985
};

/// Skull Bash - 106839
class spell_dru_skull_bash: public SpellScriptLoader
{
    public:
        spell_dru_skull_bash() : SpellScriptLoader("spell_dru_skull_bash") { }

        class spell_dru_skull_bash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_skull_bash_SpellScript);

            enum eSpells
            {
                SkullBashInterrupt  = 93985,
                SkullBashCharge     = 221514
            };

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                    {
                        l_Player->CastSpell(target, eSpells::SkullBashInterrupt, true);
                        l_Player->CastSpell(target, eSpells::SkullBashCharge, true);
                    }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_skull_bash_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_skull_bash_SpellScript();
        }
};

/// Efflorescence - 145205
class spell_dru_efflorescence : public SpellScriptLoader
{
    public:
        spell_dru_efflorescence() : SpellScriptLoader("spell_dru_efflorescence") { }

        class spell_dru_efflorescence_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_efflorescence_SpellScript);

            enum eNPC
            {
                Efflorence = 47649,
            };

            void HandleSummon(SpellEffIndex p_EffIndex)
            {
                PreventHitDefaultEffect(p_EffIndex);

                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                std::list<Creature*> l_Mushroomlist;

                l_Player->GetCreatureListWithEntryInGrid(l_Mushroomlist, eNPC::Efflorence, 200.0f);

                /// Remove other player Efflorescence
                for (std::list<Creature*>::iterator l_Itr = l_Mushroomlist.begin(); l_Itr != l_Mushroomlist.end(); ++l_Itr)
                {
                    Unit* l_Owner = (*l_Itr)->GetOwner();
                    if (l_Owner && l_Owner == l_Player && (*l_Itr)->isSummon())
                        continue;

                    l_Itr = l_Mushroomlist.erase(l_Itr);
                }

                if (static_cast<int32>(l_Mushroomlist.size()) >= GetSpellInfo()->Effects[p_EffIndex].BasePoints)
                    l_Mushroomlist.back()->ToTempSummon()->UnSummon();

                Position l_Pos;
                GetExplTargetDest()->GetPosition(&l_Pos);
                TempSummon* l_Summon = l_Player->SummonCreature(GetSpellInfo()->Effects[p_EffIndex].MiscValue, l_Pos, TEMPSUMMON_TIMED_DESPAWN, GetSpellInfo()->GetDuration());
                l_Summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                if (l_Summon)
                {
                    l_Summon->SetGuidValue(UNIT_FIELD_SUMMONED_BY, l_Player->GetGUID());
                    l_Summon->setFaction(l_Player->getFaction());
                    l_Summon->SetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL, GetSpellInfo()->Id);
                }
            }

            void Register() override
            {
                OnEffectHit += SpellEffectFn(spell_dru_efflorescence_SpellScript::HandleSummon, EFFECT_1, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_efflorescence_SpellScript();
        }
};

/// Efflorescence (heal) - 81262
class spell_dru_efflorescence_heal : public SpellScriptLoader
{
    public:
        spell_dru_efflorescence_heal() : SpellScriptLoader("spell_dru_efflorescence_heal") { }

        class spell_dru_efflorescence_heal_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_efflorescence_heal_AuraScript);

            enum eSpells
            {
                Efflorescence = 81269,
            };

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                Unit* l_Mushroom = GetCaster();
                Unit* l_Owner = l_Mushroom->GetOwner();

                if (l_Mushroom == nullptr || l_Owner == nullptr)
                    return;

                l_Owner->CastSpell(l_Mushroom, eSpells::Efflorescence, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_efflorescence_heal_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_efflorescence_heal_AuraScript();
        }
};

/// Efflorescence proc (heal) - 81269
class spell_dru_efflorescence_heal_proc : public SpellScriptLoader
{
    public:
        spell_dru_efflorescence_heal_proc() : SpellScriptLoader("spell_dru_efflorescence_heal_proc") { }

        class spell_dru_efflorescence_heal_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_efflorescence_heal_proc_SpellScript);

            enum eSpells
            {
                T15RestorationBonus     = 138284,
                SpringBlossoms          = 207385,
                SpringBlossomsAura      = 207386,
                Disentanglement         = 233673,
                DisentanglementDispel   = 233674
            };

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::T15RestorationBonus);
                uint8 l_MaxTargets = 3;

                if (l_SpellInfo != nullptr && l_Caster->HasAura(eSpells::T15RestorationBonus))
                    l_MaxTargets = l_SpellInfo->Effects[EFFECT_0].BasePoints;

                if (p_Targets.size() > 1)
                {
                    p_Targets.sort(JadeCore::HealthPctOrderPred());
                    p_Targets.resize(l_MaxTargets);
                }
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::SpringBlossoms))
                    l_Caster->CastSpell(l_Target, eSpells::SpringBlossomsAura, true);

                if (l_Caster->HasAura(eSpells::Disentanglement))
                    l_Caster->CastSpell(l_Target, eSpells::DisentanglementDispel, true);
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_efflorescence_heal_proc_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
                OnHit += SpellHitFn(spell_dru_efflorescence_heal_proc_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_efflorescence_heal_proc_SpellScript();
        }
};

/// Stampeding Roar (cat) - 77764 and Stampeding Roar(bear) - 77761
class spell_dru_stampeding_roar: public SpellScriptLoader
{
    public:
        spell_dru_stampeding_roar() : SpellScriptLoader("spell_dru_stampeding_roar") { }

        class spell_dru_stampeding_roar_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_stampeding_roar_SpellScript);

            enum eSpells
            {
                BearForm = 5487
            };

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->GetShapeshiftForm() != FORM_CAT_FORM && l_Player->GetShapeshiftForm() != FORM_BEAR_FORM)
                        l_Player->CastSpell(l_Player, eSpells::BearForm, true);

                    if (Unit* l_Target = GetHitUnit())
                        l_Target->RemoveMovementImpairingAuras();
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_stampeding_roar_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_stampeding_roar_SpellScript();
        }
};

/// Last Update 6.2.3
/// Druid of the flames - 138927
class spell_dru_druid_flames : public SpellScriptLoader
{
    public:
        spell_dru_druid_flames() : SpellScriptLoader("spell_dru_druid_flames") { }

        class spell_dru_druid_flames_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_druid_flames_AuraScript);

            enum eSpells
            {
                DruidOfFlames = 138927
            };

            enum eDatas
            {
                DruidOfFlamesModel = 38150,
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->GetShapeshiftForm() == FORM_CAT_FORM)
                    l_Player->SetDisplayId(eDatas::DruidOfFlamesModel);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->GetShapeshiftForm() == FORM_CAT_FORM)
                    l_Player->RestoreDisplayId();
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_druid_flames_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dru_druid_flames_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_druid_flames_AuraScript();
        }
};

/// Last Update 6.2.1
/// Moonfire - 8921
class spell_dru_moonfire : public SpellScriptLoader
{
    public:
        spell_dru_moonfire() : SpellScriptLoader("spell_dru_moonfire") { }

        class spell_dru_moonfire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_moonfire_SpellScript);

            enum eSpells
            {
                MoonfireDamage  = 164812
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr || l_Target == l_Caster)
                    return;

                l_Caster->CastSpell(l_Target, eSpells::MoonfireDamage, true);
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_moonfire_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_moonfire_SpellScript();
        }
};

/// Last Update 6.2.1
/// Sunfire - 93402
class spell_dru_sunfire : public SpellScriptLoader
{
    public:
        spell_dru_sunfire() : SpellScriptLoader("spell_dru_sunfire") { }

        class spell_dru_sunfire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_sunfire_SpellScript);

            enum eSpells
            {
                SolarPeak       = 171744,
                DreamOfCenarius = 108373,
                SunfireDamage   = 164815
            };

            void HandleOnHit(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                l_Caster->CastSpell(l_Target, eSpells::SunfireDamage, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_sunfire_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_sunfire_SpellScript();
        }
};

/// Last Update 7.0.3
/// Moonfire - 164812, Sunfire - 164815
class spell_dru_moonfire_sunfire_damage : public SpellScriptLoader
{
    public:
        spell_dru_moonfire_sunfire_damage() : SpellScriptLoader("spell_dru_moonfire_sunfire_damage") { }

        class spell_dru_moonfire_sunfire_damage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_moonfire_sunfire_damage_AuraScript);

            enum eSpells
            {
                ShootingStars       = 202342,
                ShootingStarsDamage = 202497
            };

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();

                if (l_Caster == nullptr)
                    return;

                if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::ShootingStars, EFFECT_0))
                {
                    if (!roll_chance_i(l_AuraEffect->GetAmount()))
                        return;

                    l_Caster->CastSpell(l_Target, eSpells::ShootingStarsDamage, true);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_moonfire_sunfire_damage_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_moonfire_sunfire_damage_AuraScript();
        }

        class spell_dru_moonfire_sunfire_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_moonfire_sunfire_damage_SpellScript);

            enum eSpells
            {
                GalacticGuardian    = 213708,
                Moonfire            = 164812,
                CrescentBurn        = 200567
            };

            void HandleBeforeHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                if (!l_Caster || !l_Target)
                    return;

                m_IsMoonfireApplied = l_Target && l_Target->HasAura(eSpells::Moonfire, l_Caster->GetGUID());
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Spell* l_Spell = GetSpell();
                if (!l_Caster || !l_Spell)
                    return;

                SpellInfo const* l_MoonfireInfo = l_Spell->GetSpellInfo();

                if (!l_MoonfireInfo)
                    return;

                if (l_MoonfireInfo->Id == eSpells::Moonfire)
                {
                    int32 l_Damages = GetHitDamage();

                    if (l_Caster->HasAura(eSpells::GalacticGuardian))
                    {
                        if (SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::GalacticGuardian))
                        {
                            AddPct(l_Damages, l_SpellInfo->Effects[EFFECT_2].BasePoints);
                            l_Caster->ModifyPower(Powers::POWER_RAGE, l_SpellInfo->Effects[EFFECT_0].BasePoints);
                        }
                    }

                    if (AuraEffect const* l_AurEff = l_Caster->GetAuraEffect(eSpells::CrescentBurn, EFFECT_0))
                    {
                        if (m_IsMoonfireApplied)
                            AddPct(l_Damages, l_AurEff->GetAmount());
                    }

                    SetHitDamage(l_Damages);
                }
            }

            void Register() override
            {
                BeforeHit += SpellHitFn(spell_dru_moonfire_sunfire_damage_SpellScript::HandleBeforeHit);
                OnHit += SpellHitFn(spell_dru_moonfire_sunfire_damage_SpellScript::HandleOnHit);
            }

        private:

            bool m_IsMoonfireApplied = false;
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_moonfire_sunfire_damage_SpellScript();
        }
};

/// T10 Restoration 4P - 70691
class spell_dru_t10_restoration_4p_bonus: public SpellScriptLoader
{
    public:
        spell_dru_t10_restoration_4p_bonus() : SpellScriptLoader("spell_dru_t10_restoration_4p_bonus") { }

        class spell_dru_t10_restoration_4p_bonus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_t10_restoration_4p_bonus_SpellScript);

            bool Load() override
            {
                return GetCaster()->IsPlayer();
            }

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                if (!GetCaster()->ToPlayer()->GetGroup())
                {
                    p_Targets.clear();
                    p_Targets.push_back(GetCaster());
                }
                else
                {
                    p_Targets.remove(GetExplTargetUnit());
                    std::list<Unit*> l_TempTargets;
                    for (std::list<WorldObject*>::const_iterator l_Itr = p_Targets.begin(); l_Itr != p_Targets.end(); ++l_Itr)
                        if ((*l_Itr)->IsPlayer() && GetCaster()->IsInRaidWith((*l_Itr)->ToUnit()))
                            l_TempTargets.push_back((*l_Itr)->ToUnit());

                    if (l_TempTargets.empty())
                    {
                        p_Targets.clear();
                        FinishCast(SPELL_FAILED_DONT_REPORT);
                        return;
                    }

                    Unit* l_Targets = JadeCore::Containers::SelectRandomContainerElement(l_TempTargets);
                    p_Targets.clear();
                    p_Targets.push_back(l_Targets);
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dru_t10_restoration_4p_bonus_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_t10_restoration_4p_bonus_SpellScript();
        }
};

/// Savage Roar - 52610
class spell_dru_savage_roar: public SpellScriptLoader
{
    public:
        spell_dru_savage_roar() : SpellScriptLoader("spell_dru_savage_roar") { }

        class spell_dru_savage_roar_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_savage_roar_SpellScript);

            enum eSpells
            {
                PredatorySwiftness = 16974,
                PredatorySwiftnessProc = 69369
            };

            int32 m_ComboPoint = 0;
            int32 m_OldDuration = 0;

            SpellCastResult CheckCast()
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster->GetShapeshiftForm() != FORM_CAT_FORM)
                    return SPELL_FAILED_ONLY_SHAPESHIFT;

                return SPELL_CAST_OK;
            }

            void HandleOnPrepare()
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                m_ComboPoint = l_Caster->GetPower(Powers::POWER_COMBO_POINT);

                if (Aura* l_Aura = l_Caster->GetAura(eSpells::PredatorySwiftness))
                    l_Aura->SetScriptGuid(1, m_ComboPoint);

                if (Aura* l_SavageRoar = l_Caster->GetAura(GetSpellInfo()->Id))
                    m_OldDuration = l_SavageRoar->GetDuration();
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                int32 l_BonusDuration = m_ComboPoint * 4 * IN_MILLISECONDS;
                int32 l_MaxDuration = GetSpellInfo()->GetMaxDuration();
                int32 l_NewDuration = m_OldDuration + GetSpellInfo()->GetDuration() + l_BonusDuration;

                if (Aura* l_SavageRoar = l_Caster->GetAura(GetSpellInfo()->Id))
                {
                    /// Can't be more then 130% of max duration
                    if (l_NewDuration > l_MaxDuration)
                        l_NewDuration = l_MaxDuration;

                    l_SavageRoar->SetDuration(l_NewDuration);
                }

                /// Handle Predatory Swiftness here, we can't do it in script x)
                if (AuraEffect* const l_Predatory = l_Caster->GetAuraEffect(eSpells::PredatorySwiftness, EFFECT_2))
                {
                    if (roll_chance_i(m_ComboPoint * l_Predatory->GetBaseAmount()))
                        l_Caster->CastSpell(l_Caster, eSpells::PredatorySwiftnessProc, true);
                }

                l_Caster->SetPower(Powers::POWER_COMBO_POINT, 0);
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_savage_roar_SpellScript::CheckCast);
                OnPrepare += SpellOnPrepareFn(spell_dru_savage_roar_SpellScript::HandleOnPrepare);
                AfterHit += SpellHitFn(spell_dru_savage_roar_SpellScript::HandleAfterHit);
            }
        };

        class spell_dru_savage_roar_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_savage_roar_AuraScript);

            enum eSpells
            {
                SavageRoarCast = 62071
            };

            bool Validate(SpellInfo const* /*spell*/) override
            {
                if (!sSpellMgr->GetSpellInfo(eSpells::SavageRoarCast))
                    return false;
                return true;
            }

            void AfterApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, eSpells::SavageRoarCast, true, NULL, p_AurEff, GetCasterGUID());
            }

            void OnUpdate(uint32 /*p_Diff*/, AuraEffect* p_AurEff)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->GetShapeshiftForm() == FORM_CAT_FORM && !l_Caster->HasAura(eSpells::SavageRoarCast))
                    l_Caster->CastSpell(l_Caster, eSpells::SavageRoarCast, true, NULL, p_AurEff, GetCasterGUID());
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->RemoveAurasDueToSpell(eSpells::SavageRoarCast);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_savage_roar_AuraScript::AfterApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectUpdate += AuraEffectUpdateFn(spell_dru_savage_roar_AuraScript::OnUpdate, EFFECT_1, SPELL_AURA_DUMMY);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_savage_roar_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_savage_roar_SpellScript();
        }

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_savage_roar_AuraScript();
        }
};

/// Survival Instincts - 61336
class spell_dru_survival_instincts: public SpellScriptLoader
{
    public:
        spell_dru_survival_instincts() : SpellScriptLoader("spell_dru_survival_instincts") { }

        class spell_dru_survival_instincts_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_survival_instincts_AuraScript);

            enum eSpells
            {
                SurvivalInstincts = 50322
            };

            void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, eSpells::SurvivalInstincts, true);
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->RemoveAurasDueToSpell(eSpells::SurvivalInstincts);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_survival_instincts_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_survival_instincts_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_survival_instincts_AuraScript();
        }
};

/// Last Update 6.2.3
/// Swiftmend - 18562
class spell_dru_swiftmend: public SpellScriptLoader
{
    public:
        spell_dru_swiftmend() : SpellScriptLoader("spell_dru_swiftmend") { }

        class spell_dru_swiftmend_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_swiftmend_SpellScript);

            enum eSpells
            {
                Rejuvenation                = 774
            };

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    int32 l_HealAmount = GetHitHeal() + CalculatePct(l_Caster->SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_ALL), GetSpellInfo()->Effects[EFFECT_0].BasePoints);
                    SetHitHeal(l_HealAmount);
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_swiftmend_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_swiftmend_SpellScript();
        }
};

/// Called by the cancel of Aquatic Form - 1066, Swift Flight Form - 40120
/// Flight Form - 33943 and Stag Form - 165961
/// Shapeshift removal handling
class spell_dru_travel_form_removal : public SpellScriptLoader
{
    public:
        spell_dru_travel_form_removal() : SpellScriptLoader("spell_dru_travel_form_removal") { }

        class spell_dru_travel_form_removal_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_travel_form_removal_AuraScript);

            enum eSpells
            {
                TravelForm = 783
            };

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                if (l_Target == nullptr)
                    return;

                /// This should only be called by client, if player cancel aura itself
                /// Or if client send CMSG_CANCEL_AURA when casting incompatible spell with Shapeshift
                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_CANCEL)
                    l_Target->RemoveAura(eSpells::TravelForm);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_travel_form_removal_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_travel_form_removal_AuraScript();
        }
};

/// Travel form - 783
class spell_dru_travel_form: public SpellScriptLoader
{
    public:
        spell_dru_travel_form() : SpellScriptLoader("spell_dru_travel_form") { }

        class spell_dru_travel_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_travel_form_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (!l_Player->GetMap()->IsOutdoors(l_Player->GetPositionX(), l_Player->GetPositionY(), l_Player->GetPositionZ()))
                        return SPELL_FAILED_ONLY_OUTDOORS;
                }

                return SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_travel_form_SpellScript::CheckCast);
            }
        };

        class spell_dru_travel_form_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_travel_form_AuraScript);

            void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                // Call the player script "spell_dru_travel_form_playerscript" below to avoid code duplication
                if (GetTarget()->IsPlayer())
                    sScriptMgr->OnPlayerUpdateMovement(GetTarget()->ToPlayer());
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_CANCEL)
                    l_Target->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_travel_form_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_travel_form_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_travel_form_SpellScript();
        }

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_travel_form_AuraScript();
        }
};

class spell_dru_travel_form_playerscript: public PlayerScript
{
    public:
        spell_dru_travel_form_playerscript() : PlayerScript("spell_dru_travel_form_playerscript") {}

        enum eSpells
        {
            TravelForm              = 783,
            AquaticForm             = 1066,
            FlightForm              = 33943,
            SwiftFlightForm         = 40120,
            StagForm                = 165961,
            ColdWeatherFlying       = 54197,
            FlightMastersLicense    = 90267,
            WisdomOfTheFourWinds    = 115913,
            DraenorPathfinder       = 191645
        };

        bool CheckIfCanFlyInLoc(Player* p_Player)
        {
            bool l_CanFly = false;

            /// First check by map...
            uint32 l_Map = GetVirtualMapForMapAndZone(p_Player->GetMapId(), p_Player->GetZoneId());
            switch (l_Map)
            {
                case 530: ///< Outland
                    l_CanFly = true;
                    break;
                case 571: ///< Northrend
                {
                    if (p_Player->HasSpell(eSpells::ColdWeatherFlying))
                        l_CanFly = true;
                    break;
                }
                case 0:   ///< Eastern Kingdoms
                case 1:   ///< Kalimdor
                case 646: ///< Deepholm
                case 1190: ///< BlastedLands Dark portal
                {
                    if (p_Player->HasSpell(eSpells::FlightMastersLicense))
                        l_CanFly = true;
                    break;
                }
                case 870: ///< Pandaria
                {
                    if (p_Player->HasSpell(eSpells::WisdomOfTheFourWinds))
                        l_CanFly = true;
                    break;
                }
                case 1116: ///< Draenor
                {
                   if (p_Player->HasSpell(eSpells::DraenorPathfinder))
                        l_CanFly = true;
                    break;
                }
            }

            /// ...and then by area
            AreaTableEntry const* l_Area = sAreaStore.LookupEntry(p_Player->GetAreaId());
            if (!l_Area || l_Area->Flags & AREA_FLAG_NO_FLY_ZONE)
                l_CanFly = false;

            return l_CanFly;
        }

        void OnUpdateMovement(Player* p_Player)
        {
            if (!p_Player || p_Player->getClass() != CLASS_DRUID || !p_Player->HasAura(eSpells::TravelForm))
                return;

            if (!p_Player->IsOutdoors())
            {
                p_Player->RemoveAura(eSpells::TravelForm);
                return;
            }

            if (p_Player->IsInWater() && p_Player->GetShapeshiftForm() != FORM_AQUATIC_FORM)
                p_Player->CastSpell(p_Player, eSpells::AquaticForm, true);
            else if (!p_Player->IsInWater() && p_Player->getLevel() >= 71 && CheckIfCanFlyInLoc(p_Player) && !p_Player->isInCombat() &&
                     p_Player->GetShapeshiftForm() != FORM_FLIGHT_FORM_EPIC)
                p_Player->CastSpell(p_Player, eSpells::SwiftFlightForm, true);
            else if (!p_Player->IsInWater() && p_Player->getLevel() >= 60 && CheckIfCanFlyInLoc(p_Player) && !p_Player->isInCombat() &&
                     p_Player->GetShapeshiftForm() != FORM_FLIGHT_FORM_EPIC && p_Player->GetShapeshiftForm() != FORM_FLIGHT_FORM )
                p_Player->CastSpell(p_Player, eSpells::FlightForm, true);
            else if (!p_Player->IsInWater() && !p_Player->IsFlying() && p_Player->GetShapeshiftForm() != FORM_FLIGHT_FORM_EPIC &&
                     p_Player->GetShapeshiftForm() != FORM_FLIGHT_FORM && p_Player->GetShapeshiftForm() != FORM_TRAVEL_FORM)
                p_Player->CastSpell(p_Player, eSpells::StagForm, true);
        }

        void OnChangeShapeshift(Player* p_Player, ShapeshiftForm p_Form)
        {
            if (!p_Player || p_Player->getClass() != CLASS_DRUID)
                return;

            if (p_Player->HasAura(eSpells::TravelForm) && !p_Player->IsTravelForm(p_Form))
                p_Player->RemoveAura(eSpells::TravelForm);
        }
};

/// 40121 - Swift Flight Form (Passive)
class spell_dru_swift_flight_passive: public SpellScriptLoader
{
    public:
        spell_dru_swift_flight_passive() : SpellScriptLoader("spell_dru_swift_flight_passive") { }

        class spell_dru_swift_flight_passive_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_swift_flight_passive_AuraScript);

            enum eSpells
            {
                MasterFlying = 90265
            };

            bool Load() override
            {
                return GetCaster()->IsPlayer();
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & p_Amount, bool & /*canBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                if (l_Caster->HasAura(eSpells::MasterFlying))
                    p_Amount = 310; // 310% instead of 280% by default
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_swift_flight_passive_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_swift_flight_passive_AuraScript();
        }
};

/// Last Update : 7.1.5 Build 23420
/// Rake - 1822
class spell_dru_rake: public SpellScriptLoader
{
    public:
        spell_dru_rake() : SpellScriptLoader("spell_dru_rake") { }

        class spell_dru_rake_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_rake_SpellScript);

            enum eSpells
            {
                Prowl           = 5215,
                KingOfTheJungle = 102543,
                SavageRoar      = 52610,
                RakeEffect      = 155722,
                RakeStun        = 163505,
                Bloodtalons     = 145152
            };

            void HandleOnPrepare()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (l_Caster->HasStealthAura())
                    l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidSteath) = true;

                if (l_Caster->HasAura(eSpells::KingOfTheJungle))
                    l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidKingOfTheJungle) = true;
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                int32 l_Damage = GetHitDamage();
                if (!l_Caster || !l_Target || !l_SpellInfo)
                    return;

                l_Caster->CastSpell(l_Target, eSpells::RakeEffect);

                bool l_Stealth = l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidSteath);
                bool l_King = l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidKingOfTheJungle);
                if (l_King || l_Stealth)
                {
                    if (l_Stealth)
                        l_Caster->CastSpell(l_Target, eSpells::RakeStun, true);

                    AddPct(l_Damage, l_SpellInfo->Effects[EFFECT_3].BasePoints);
                }

                if (Aura* l_BloodTalons = l_Caster->GetAura(eSpells::Bloodtalons)) ///< Charge is drop on Rake periodic damage
                    AddPct(l_Damage, l_BloodTalons->GetEffect(EFFECT_0)->GetAmount());

                SetHitDamage(l_Damage);

                l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidSteath) = false;
                l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidKingOfTheJungle) = false;
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_rake_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnPrepare += SpellOnPrepareFn(spell_dru_rake_SpellScript::HandleOnPrepare);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_rake_SpellScript();
        }
};

/// Last Update : 7.1.5 Build 23420
/// Rake (triggered) - 155722
class spell_dru_rake_triggered : public SpellScriptLoader
{
    public:
        spell_dru_rake_triggered() : SpellScriptLoader("spell_dru_rake_triggered") { }

        class spell_dru_rake_triggered_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_rake_triggered_AuraScript);

            enum eSpells
            {
                Bloodtalons = 145152,
                Rake        = 1822
            };

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::Rake);
                if (!l_Caster || !l_SpellInfo)
                    return;

                if (l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidSteath) || l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidKingOfTheJungle))
                    AddPct(p_Amount, l_SpellInfo->Effects[EFFECT_3].BasePoints);

                if (Aura* l_BloodTalons = l_Caster->GetAura(eSpells::Bloodtalons))
                {
                    p_Amount += CalculatePct(p_Amount, l_BloodTalons->GetEffect(EFFECT_0)->GetAmount());
                    l_Caster->AddDelayedEvent([=]() -> void
                    {
                        Aura* l_BloodTalons = l_Caster->GetAura(eSpells::Bloodtalons);
                        if (l_BloodTalons)
                            l_BloodTalons->DropCharge();
                    }, 10);
                }
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rake_triggered_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_rake_triggered_AuraScript();
        }
};

/// Last Update 7.0.3
/// Barkskin - 22812
class spell_dru_barkskin : public SpellScriptLoader
{
    public:
        spell_dru_barkskin() : SpellScriptLoader("spell_dru_barkskin") { }

        class spell_dru_barkskin_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_barkskin_AuraScript);

            enum eSpells
            {
                Brambles        = 203953,
                BramblesDamage  = 213709
            };

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::Brambles))
                    l_Caster->CastSpell(l_Caster, eSpells::BramblesDamage, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_barkskin_AuraScript::OnTick, EFFECT_4, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_barkskin_AuraScript();
        }
};

/// Last Update : 7.1.5 Build 23420
/// Shred - 5221
class spell_dru_shred: public SpellScriptLoader
{
    public:
        spell_dru_shred() : SpellScriptLoader("spell_dru_shred") { }

        class spell_dru_shred_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_shred_SpellScript);

            enum eSpells
            {
                KingOfTheJungle = 102543,
                Swipe           = 106785,
                SavageRoar      = 62071
            };

            void HandleOnPrepare()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (l_Caster->HasStealthAura())
                    l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidSteath) = true;

                if (l_Caster->HasAura(eSpells::KingOfTheJungle))
                    l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidKingOfTheJungle) = true;
            }

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                SpellInfo const* l_SpellInfoShred = GetSpellInfo();
                SpellInfo const* l_SpellInfoSwipe = sSpellMgr->GetSpellInfo(eSpells::Swipe);
                int32 l_Damage = GetHitDamage();
                if (!l_Caster || !l_Target || !l_SpellInfoShred || !l_SpellInfoSwipe)
                    return;

                if (l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidSteath) || l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidKingOfTheJungle))
                    AddPct(l_Damage, l_SpellInfoShred->Effects[EFFECT_3].BasePoints);

                if (l_Target->HasAuraState(AURA_STATE_BLEEDING))
                    AddPct(l_Damage, l_SpellInfoSwipe->Effects[EFFECT_1].BasePoints);

                SetHitDamage(l_Damage);

                l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidSteath) = false;
                l_Caster->m_SpellHelper.GetBool(eSpellHelpers::DruidKingOfTheJungle) = false;
            }

            void Register() override
            {
                OnPrepare += SpellOnPrepareFn(spell_dru_shred_SpellScript::HandleOnPrepare);
                OnEffectHitTarget += SpellEffectFn(spell_dru_shred_SpellScript::HandleDamage, EFFECT_2, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_shred_SpellScript();
        }
};

/// Last Update 7.0.3
/// Ferocious Bite - 22568
class spell_dru_ferocious_bite: public SpellScriptLoader
{
    public:
        spell_dru_ferocious_bite() : SpellScriptLoader("spell_dru_ferocious_bite") { }

        class spell_dru_ferocious_bite_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_ferocious_bite_SpellScript);

            enum eSpells
            {
                Sabertooth              = 202031,
                Berserk                 = 106951,
                Rip                     = 1079,
                PredatorySwiftness      = 16974,
                FerociousWound          = 236020,
                FerociousWoundDebuff    = 236021
            };

            int32 m_SpellCost = 25;
            int32 m_EnergyConsumedExtra = 0;
            bool m_IsFreeCost = false;

            void HandleOnPrepare()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::Berserk))
                    m_SpellCost = 12;

                if (l_Caster->HasAura(135700))
                    m_IsFreeCost = true;
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                int32 l_Damage = GetHitDamage();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::FerociousWound);

                /// Prevent double call
                if (!l_Caster || !l_Target || l_Damage == 0 || !l_SpellInfo)
                    return;

                uint8 l_CombotPoint = l_Caster->GetPower(Powers::POWER_COMBO_POINT);

                if (Aura* l_Aura = l_Caster->GetAura(eSpells::PredatorySwiftness))
                    l_Aura->SetScriptGuid(1, l_CombotPoint);

                if (l_Caster->IsPlayer())
                    l_Damage = (l_Damage / 5) * (l_CombotPoint + 1);

                /// converts each extra point of energy ( up to 25 energy ) into additional damage
                int32 l_EnergyConsumed = l_Caster->GetPower(Powers::POWER_ENERGY) > m_SpellCost ? m_SpellCost : l_Caster->GetPower(Powers::POWER_ENERGY);

                AddPct(l_Damage, l_EnergyConsumed * (100 / m_SpellCost));
                SetHitDamage(l_Damage);

                m_EnergyConsumedExtra = l_EnergyConsumed;

                /// if target is under 25% of life, also reset rake duration
                if ((l_Target->GetHealthPct() <= 25.0f) || l_Caster->HasAura(eSpells::Sabertooth))
                if (Aura* l_Rip = l_Target->GetAura(eSpells::Rip))
                    l_Rip->RefreshDuration();

                if (l_CombotPoint >= l_SpellInfo->Effects[EFFECT_1].BasePoints && l_Caster->HasAura(eSpells::FerociousWound))
                {
                    l_Target->RemoveAura(eSpells::FerociousWoundDebuff);
                    l_Caster->CastSpell(l_Target, eSpells::FerociousWoundDebuff, true);
                }
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                l_Caster->SetPower(Powers::POWER_COMBO_POINT, 0);
                if (m_IsFreeCost)
                {
                    l_Caster->SetPower(Powers::POWER_ENERGY, l_Caster->GetPower(Powers::POWER_ENERGY) + m_EnergyConsumedExtra);
                    m_IsFreeCost = false;
                }
            }

            void Register() override
            {
                OnPrepare += SpellOnPrepareFn(spell_dru_ferocious_bite_SpellScript::HandleOnPrepare);
                OnHit += SpellHitFn(spell_dru_ferocious_bite_SpellScript::HandleOnHit);
                AfterHit += SpellHitFn(spell_dru_ferocious_bite_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_ferocious_bite_SpellScript();
        }
};

/// last update 7.1.5 Build 23420
/// Called by Ferocious Wound - 236021
class spell_dru_ferocious_wound : public SpellScriptLoader
{
    public:
        spell_dru_ferocious_wound() : SpellScriptLoader("spell_dru_ferocious_wound") { }

        class spell_dru_ferocious_wound_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ferocious_wound_AuraScript);

            enum eSpells
            {
                FerociousWoundDebuff = 236021
            };

            void HandleAfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();
                if (!l_Caster || !l_Target)
                    return;

                uint64& l_PrevTargetGUID = l_Caster->m_SpellHelper.GetUint64(eSpellHelpers::FerociousWound);

                Unit* l_PrevTarget = sObjectAccessor->GetUnit(*l_Caster, l_PrevTargetGUID);

                if (l_PrevTarget && l_PrevTarget != l_Target)
                    l_PrevTarget->RemoveAura(eSpells::FerociousWoundDebuff, l_Caster->GetGUID());

                l_Caster->m_SpellHelper.GetUint64(eSpellHelpers::FerociousWound) = l_Target->GetGUID();
            }

            void HandleAfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                l_Caster->m_SpellHelper.GetUint64(eSpellHelpers::FerociousWound) = 0;
            }

            void Register() override
            {
               AfterEffectApply += AuraEffectApplyFn(spell_dru_ferocious_wound_AuraScript::HandleAfterApply, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT, AURA_EFFECT_HANDLE_REAL);
               AfterEffectRemove += AuraEffectRemoveFn(spell_dru_ferocious_wound_AuraScript::HandleAfterApply, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_ferocious_wound_AuraScript();
        }
};

/// Last Update 7.1.5 23420
/// Rip - 1079
class spell_dru_rip: public SpellScriptLoader
{
    public:
        spell_dru_rip() : SpellScriptLoader("spell_dru_rip") { }

        class spell_dru_rip_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_rip_AuraScript);

            enum eSpells
            {
                Bloodtalons            = 145152,
                PredatorySwiftness     = 16974,
                PredatorySwiftnessProc = 69369,
                KingOfTheJungle        = 203052,
                KingOfTheJungleProc    = 203059,
                OpenWounds             = 210666,
                OpenWoundsDebuff       = 210670
            };

            uint32 m_PreviousTick = 0;

            void HandleKingOfJungle(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (!l_Caster->HasAura(eSpells::KingOfTheJungle))
                    return;

                std::set<uint64>& l_RipApplyTargets = l_Caster->m_SpellHelper.m_Uint64Set[eSpellHelpers::RipApplyTargets];
                bool l_AddKingOfTheJungle = l_RipApplyTargets.find(l_Target->GetGUID()) == l_RipApplyTargets.end();

                /// If the target is already on the list we just refresh aura duration
                if (!l_AddKingOfTheJungle)
                {
                    if (Aura* l_KingOfTheJungleAura = l_Caster->GetAura(eSpells::KingOfTheJungleProc))
                        l_KingOfTheJungleAura->RefreshDuration();
                }
                else
                    l_Caster->DelayedCastSpell(l_Caster, eSpells::KingOfTheJungleProc, true, 10);

                l_Caster->m_SpellHelper.m_Uint64Set[eSpellHelpers::RipApplyTargets].insert(l_Target->GetGUID());
            }

            void AfterReApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                /// On re-apply
                /// if (AuraEffect* l_AurEff = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0, l_Caster->GetGUID()))
                ///    m_PreviousTick = ((l_AurEff->GetAmount() * (l_AurEff->GetBase()->GetDuration() / l_AurEff->GetAmplitude())) / (p_AurEff->GetBase()->GetMaxDuration() / p_AurEff->GetAmplitude()));
                /// https://trello.com/c/ewjER9ad/1475-druid-rip

                if (AuraEffect* l_AurEff = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0, l_Caster->GetGUID()))
                    l_AurEff->SetAmount(l_AurEff->GetAmount() + m_PreviousTick);
            }

            void HandleApplyOpenWounds(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Caster = GetCaster();
                if (!l_Caster || !l_Target)
                    return;

                if (l_Caster->HasAura(eSpells::OpenWounds))
                    l_Caster->CastSpell(l_Target, eSpells::OpenWoundsDebuff, true);
            }

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                int32 l_Combo = l_Caster->GetPower(Powers::POWER_COMBO_POINT);
                if (l_Combo > 0)
                    p_Amount *= l_Combo;

                if (Aura* l_BloodTalons = l_Caster->GetAura(eSpells::Bloodtalons))
                    p_Amount += CalculatePct(p_Amount, l_BloodTalons->GetEffect(EFFECT_0)->GetAmount());

                l_Caster->AddDelayedEvent([=]() -> void
                {
                    if (Aura* l_BloodTalons = l_Caster->GetAura(eSpells::Bloodtalons))
                        l_BloodTalons->DropCharge();

                    /// Handle Predatory Swiftness here, we can't do it in script x)
                    if (AuraEffect* const l_Predatory = l_Caster->GetAuraEffect(eSpells::PredatorySwiftness, EFFECT_2))
                    {
                        if (roll_chance_i(l_Combo * l_Predatory->GetBaseAmount()))
                            l_Caster->CastSpell(l_Caster, eSpells::PredatorySwiftnessProc, true);
                    }

                    l_Caster->SetPower(Powers::POWER_COMBO_POINT, 0);
                }, 10);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                std::set<uint64>& l_RipApplyTargets = l_Caster->m_SpellHelper.m_Uint64Set[eSpellHelpers::RipApplyTargets];
                l_Caster->m_SpellHelper.m_Uint64Set[eSpellHelpers::RipApplyTargets].erase(l_Target->GetGUID());

                if (l_Target)
                    l_Target->RemoveAura(eSpells::OpenWoundsDebuff);
            }

            void Register() override
            {
                AfterEffectApply   += AuraEffectApplyFn(spell_dru_rip_AuraScript::HandleKingOfJungle, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                AfterEffectApply   += AuraEffectApplyFn(spell_dru_rip_AuraScript::AfterReApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAPPLY);
                AfterEffectApply   += AuraEffectApplyFn(spell_dru_rip_AuraScript::HandleApplyOpenWounds, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rip_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                AfterEffectRemove  += AuraEffectRemoveFn(spell_dru_rip_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_rip_AuraScript();
        }

        class spell_dru_rip_SpelScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_rip_SpelScript);

            enum eSpells
            {
                PredatorySwiftness = 16974
            };

            void HandleBeforeCast()
            {
                uint32 l_PowerCost = 0;

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                l_PowerCost = l_Caster->GetPower(POWER_COMBO_POINT);

                Aura* l_Aura = l_Caster->GetAura(eSpells::PredatorySwiftness);
                if (!l_Aura)
                    return;

                l_Aura->SetScriptGuid(1, l_PowerCost);
            }

            void Register() override
            {
                BeforeCast += SpellCastFn(spell_dru_rip_SpelScript::HandleBeforeCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_rip_SpelScript();
        }
};

/// last update 7.0.3 Build 22522
/// Called by Maim - 22570
class spell_dru_maim: public SpellScriptLoader
{
    public:
        spell_dru_maim() : SpellScriptLoader("spell_dru_maim") { }

        class spell_dru_maim_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_maim_SpellScript);

            enum eSpells
            {
                PredatorySwiftness  = 16974,
                Maim1               = 168877,
                Maim2               = 168878,
                Maim3               = 168879,
                Maim4               = 168880,
                Maim5               = 168881,
                FieryRedMaimers     = 236757,
            };

            uint8 m_PowerCost = 0;
            void HandleBeforeCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                if (l_Target == nullptr)
                    return;

                m_PowerCost = l_Caster->GetPower(POWER_COMBO_POINT);

                Aura* l_Aura = l_Caster->GetAura(eSpells::PredatorySwiftness);
                if (l_Aura != nullptr)
                    l_Aura->SetScriptGuid(1, m_PowerCost);
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                l_Caster->SetPower(POWER_COMBO_POINT, 0);

                l_Caster->RemoveAura(eSpells::FieryRedMaimers);
            }

            void HandleDummy(SpellEffIndex effIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                if (!l_Caster || !l_Target)
                    return;

                uint32 damageSpell = 0;
                switch (m_PowerCost)
                {
                    case 1: damageSpell = eSpells::Maim1; break;
                    case 2: damageSpell = eSpells::Maim2; break;
                    case 3: damageSpell = eSpells::Maim3; break;
                    case 4: damageSpell = eSpells::Maim4; break;
                    case 5: damageSpell = eSpells::Maim5; break;
                }

                if (!damageSpell)
                    return;

                TriggerCastFlags triggerFlags = TriggerCastFlags(TriggerCastFlags::TRIGGERED_FULL_MASK & ~TriggerCastFlags::TRIGGERED_DISALLOW_PROC_EVENTS);
                l_Caster->CastSpell(l_Target, damageSpell, triggerFlags);

                if (l_Caster->HasAura(eSpells::FieryRedMaimers))
                {
                    HandleFieryRedMaimers(l_Caster, l_Target, damageSpell);
                }
            }

            void HandleDamage(SpellEffIndex effIndex)
            {
                // Prevent damage EFFECT_1 and EFFECT_2
                SetHitDamage(0);
            }

            void Register() override
            {
                BeforeCast += SpellCastFn(spell_dru_maim_SpellScript::HandleBeforeCast);
                OnEffectHitTarget += SpellEffectFn(spell_dru_maim_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnEffectHitTarget += SpellEffectFn(spell_dru_maim_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_WEAPON_DAMAGE);
                OnEffectHitTarget += SpellEffectFn(spell_dru_maim_SpellScript::HandleDamage, EFFECT_2, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
                AfterHit += SpellHitFn(spell_dru_maim_SpellScript::HandleAfterHit);
            }

        private:

            void HandleFieryRedMaimers(Unit* l_Caster, Unit* l_Target, uint32 damageSpell)
            {
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::FieryRedMaimers);
                if (!l_SpellInfo)
                    return;

                float l_Radius = l_SpellInfo->Effects[EFFECT_0].BasePoints;
                int32 l_MaxTargets = l_SpellInfo->Effects[EFFECT_0].BasePoints;

                std::list<Unit*> l_MaimEnemies;
                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck u_check(l_Target, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> searcher(l_Target, l_MaimEnemies, u_check);
                l_Target->VisitNearbyObject(l_Radius, searcher);
                for (auto l_MaimTarget : l_MaimEnemies)
                {
                    if (l_MaimTarget != l_Target && l_MaxTargets > 0)
                    {
                        l_Caster->CastSpell(l_MaimTarget, damageSpell, true);
                        --l_MaxTargets;
                    }
                }
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_maim_SpellScript();
        }
};

/// last update 7.1.5 Build 22522
/// Called by Enraged Maim - 236025
class spell_dru_enraged_maim : public SpellScriptLoader
{
    public:
        spell_dru_enraged_maim() : SpellScriptLoader("spell_dru_enraged_maim") { }

        class spell_dru_enraged_maim_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_enraged_maim_AuraScript);

            enum eSpells
            {
                RakeDot     = 155722,
                Rip         = 1079,
                ThrashDot   = 106830
            };

            void HandleAfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (!GetCaster() || !GetUnitOwner())
                    return;

                GetUnitOwner()->RemoveAura(eSpells::RakeDot, GetCaster()->GetGUID());
                GetUnitOwner()->RemoveAura(eSpells::Rip, GetCaster()->GetGUID());
                GetUnitOwner()->RemoveAura(eSpells::ThrashDot, GetCaster()->GetGUID());
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_enraged_maim_AuraScript::HandleAfterApply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_enraged_maim_AuraScript();
        }
};

/// Dream of Cenarius - 158501
class spell_dru_dream_of_cenarius: public SpellScriptLoader
{
    public:
        spell_dru_dream_of_cenarius() : SpellScriptLoader("spell_dru_dream_of_cenarius") { }

        class spell_dru_dream_of_cenarius_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_dream_of_cenarius_AuraScript);

            enum eSpells
            {
                Mangle              = 33917,
                DreamOfCenariusProc = 145162
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (p_EventInfo.GetActor()->GetGUID() != l_Caster->GetGUID())
                    return;

                if (!p_EventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != eSpells::Mangle)
                    return;

                if (!(p_EventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT))
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::DreamOfCenariusProc, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_dream_of_cenarius_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_dream_of_cenarius_AuraScript();
        }
};

/// Primal Fury - 16961 159286
class spell_dru_primal_fury: public SpellScriptLoader
{
    public:
        spell_dru_primal_fury() : SpellScriptLoader("spell_dru_primal_fury") { }

        class spell_dru_primal_fury_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_primal_fury_AuraScript);

            enum eSpells
            {
                PrimalFuryCP        = 159286,
                PrimalFuryCPProc    = 16953,
                PrimalFuryRage      = 16961,
                PrimalFuryRageProc  = 16959
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                Unit* l_Actor = p_EventInfo.GetActor();
                uint32 l_HitMask = p_EventInfo.GetHitMask();
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                if (!l_Caster || !l_Actor || (l_Caster->GetGUID() != l_Actor->GetGUID()) || !(l_HitMask & PROC_EX_CRITICAL_HIT) || !l_SpellInfo)
                    return;

                DamageInfo* l_DamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_DamageInfo)
                    return;

                SpellInfo const* l_ProcSpellInfo = l_DamageInfo->GetSpellInfo();
                if (!l_ProcSpellInfo)
                    return;

                if (l_SpellInfo->Id == eSpells::PrimalFuryRage && l_Caster->GetShapeshiftForm() == FORM_BEAR_FORM)
                    l_Caster->CastSpell(l_Caster, eSpells::PrimalFuryRageProc, true);
                else if (l_SpellInfo->Id == eSpells::PrimalFuryCP && l_Caster->GetShapeshiftForm() == FORM_CAT_FORM)
                {
                    if (l_ProcSpellInfo->HasEffect(SPELL_EFFECT_ADD_COMBO_POINTS))
                        l_Caster->CastSpell(l_Caster, eSpells::PrimalFuryCPProc, true);
                    else if (l_ProcSpellInfo->HasEffect(SPELL_EFFECT_ENERGIZE))
                    {
                        for (SpellEffectInfo l_Effect : l_ProcSpellInfo->Effects._effects)
                        {
                            if (l_Effect.Effect == SPELL_EFFECT_ENERGIZE && l_Effect.MiscValue == POWER_COMBO_POINT)
                            {
                                l_Caster->CastSpell(l_Caster, eSpells::PrimalFuryCPProc, true);
                                break;
                            }
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_primal_fury_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_primal_fury_AuraScript();
        }
};

/// Call by Starfire - 2912, Wrath - 5176
/// Entangling Energy - 164991
class spell_dru_entangling_energy : public SpellScriptLoader
{
    public:
        spell_dru_entangling_energy() : SpellScriptLoader("spell_dru_entangling_energy") { }

        class spell_dru_entangling_energy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_entangling_energy_SpellScript);

            enum eSpells
            {
                T17Balance4P        = 166677,
                MoonfireDamage      = 164812,
                SunfireDamage       = 164815,
                BalanceOfPower      = 152220,
                Starfire            = 2912,
                Wrath               = 5176
            };

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                /// Balance of Power - 152220
                if (l_Caster->HasAura(eSpells::BalanceOfPower))
                {
                    SpellInfo const* l_BalanceOfPowerSpells = sSpellMgr->GetSpellInfo(eSpells::BalanceOfPower);
                    /// Your Starfires extend the duration of Moonfire by 6 sec.
                    if (GetSpellInfo()->Id == eSpells::Starfire)
                    {
                        if (Aura* l_Moonfire = l_Target->GetAura(eSpells::MoonfireDamage, l_Caster->GetGUID()))
                        {
                            int32 l_CurrentDuration = l_Moonfire->GetDuration();
                            int32 l_MaxDuration = l_Moonfire->GetMaxDuration();
                            int32 l_NewDuration = l_CurrentDuration + l_BalanceOfPowerSpells->Effects[0].BasePoints * IN_MILLISECONDS;

                            if (l_NewDuration > l_MaxDuration)
                                l_NewDuration = l_MaxDuration;

                            l_Moonfire->SetDuration(l_NewDuration);
                        }
                    }
                    /// Your Wraths extend the duration of Sunfire by 4 sec.
                    else if (GetSpellInfo()->Id == eSpells::Wrath)
                    {
                        if (Aura* l_Sunfire = l_Target->GetAura(eSpells::SunfireDamage, l_Caster->GetGUID()))
                        {
                            int32 l_CurrentDuration = l_Sunfire->GetDuration();
                            int32 l_MaxDuration = l_Sunfire->GetMaxDuration();
                            int32 l_NewDuration = l_CurrentDuration + l_BalanceOfPowerSpells->Effects[1].BasePoints * IN_MILLISECONDS;

                            if (l_NewDuration > l_MaxDuration)
                                l_NewDuration = l_MaxDuration;

                            l_Sunfire->SetDuration(l_NewDuration);
                        }
                    }
                }
            }

            void Register() override
            {
                AfterHit += SpellHitFn(spell_dru_entangling_energy_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_entangling_energy_SpellScript();
        }
};

/// Ursa Major (aura) - 159233
class spell_dru_ursa_major_aura : public SpellScriptLoader
{
    public:
        spell_dru_ursa_major_aura() : SpellScriptLoader("spell_dru_ursa_major_aura") { }

        class spell_dru_ursa_major_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ursa_major_aura_AuraScript);

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                l_Caster->AddToStackOnDuration(GetSpellInfo()->Id, GetSpellInfo()->GetMaxDuration(), GetSpellInfo()->Effects[EFFECT_0].BasePoints);

                StackOnDuration* l_Stack = l_Caster->GetStackOnDuration(GetSpellInfo()->Id);

                if (l_Stack == nullptr)
                    return;

                GetEffect(EFFECT_0)->SetAmount(l_Stack->GetTotalAmount());
            }

            void OnUpdate(uint32 /*p_Diff*/, AuraEffect* p_AurEff)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                StackOnDuration* l_Stack = l_Caster->GetStackOnDuration(GetSpellInfo()->Id);

                if (l_Stack == nullptr)
                    return;

                if (p_AurEff->GetAmount() > l_Stack->GetTotalAmount())
                {
                    float l_Percent = l_Caster->GetHealthPct();
                    l_Caster->HandleStatModifier(UNIT_MOD_HEALTH, TOTAL_PCT, p_AurEff->GetAmount(), false);
                    l_Caster->HandleStatModifier(UNIT_MOD_HEALTH, TOTAL_PCT, l_Stack->GetTotalAmount(), true);
                    if (l_Caster->isAlive())
                        l_Caster->SetHealth(l_Caster->CountPctFromMaxHealth(int32(l_Percent)));
                }

                p_AurEff->SetAmount(l_Stack->GetTotalAmount());
            }

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                l_Caster->RemoveStackOnDuration(GetSpellInfo()->Id);
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_ursa_major_aura_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT, AuraEffectHandleModes(AURA_EFFECT_HANDLE_REAL | AURA_EFFECT_HANDLE_REAPPLY));
                OnEffectUpdate += AuraEffectUpdateFn(spell_dru_ursa_major_aura_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_ursa_major_aura_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_ursa_major_aura_AuraScript();
        }
};

/// Last Uptade 6.1.2
/// Healing Touch - 5185
class spell_dru_healing_touch: public SpellScriptLoader
{
    public:
        spell_dru_healing_touch() : SpellScriptLoader("spell_dru_healing_touch") { }

        class spell_dru_healing_touch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_healing_touch_SpellScript);

            enum eSpells
            {
                BearForm                    = 5487,
                SoulOfTheForestResto        = 114108,
                Bloodtalons                 = 145152,
                BlootalonsTalent            = 155672,
                PrimalVitalityAura          = 202812
            };

            void HandleOnPrepare()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                /// Healing Touch no longer cancels Bear Form for Guardian Druids.
                if (l_Player->HasAura(eSpells::BearForm) && l_Player->GetActiveSpecializationID() != SPEC_DRUID_GUARDIAN)
                    l_Player->RemoveAura(eSpells::BearForm);
            }

            void HandleOnCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target;
                if (!l_Caster)
                    return;

                l_Target = GetExplTargetUnit();
                if (l_Caster->HasAura(eSpells::BlootalonsTalent))
                    l_Caster->CastSpell(l_Caster, eSpells::Bloodtalons, true);
            }

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (l_Caster->HasAura(eSpells::SoulOfTheForestResto))
                    l_Caster->RemoveAura(eSpells::SoulOfTheForestResto);

                if (l_Caster->HasAura(eSpells::PrimalVitalityAura))
                    l_Caster->RemoveAura(eSpells::PrimalVitalityAura);
            }

            void Register() override
            {
                OnPrepare += SpellOnPrepareFn(spell_dru_healing_touch_SpellScript::HandleOnPrepare);
                OnCast += SpellCastFn(spell_dru_healing_touch_SpellScript::HandleOnCast);
                AfterCast += SpellCastFn(spell_dru_healing_touch_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_healing_touch_SpellScript();
        }
};

/// Last Update - 6.1.2
/// Dream of Cenarius (feral) - 158497
/// Called by Healing Touch (5185) and Rejuvenation (774)
class spell_dru_dream_of_cenarius_feral : public SpellScriptLoader
{
    public:
        spell_dru_dream_of_cenarius_feral() : SpellScriptLoader("spell_dru_dream_of_cenarius_feral") { }

        class spell_dru_dream_of_cenarius_feral_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_dream_of_cenarius_feral_SpellScript);

            enum eSpells
            {
                Rejuvenation            = 774,
                DreamOfCenariusFeral    = 158497,
                HealingTouch            = 5185
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                if (GetSpellInfo()->Id == eSpells::HealingTouch)
                {
                    if (l_Caster != l_Target && l_Caster->HasAura(eSpells::DreamOfCenariusFeral))
                    {
                        uint32 l_HealAmount = GetHitHeal();

                        l_Caster->HealBySpell(l_Caster, GetSpellInfo(), l_HealAmount, GetSpell()->IsCritForTarget(l_Target));
                    }
                }
                else if (GetSpellInfo()->Id == eSpells::Rejuvenation)
                {
                    if (l_Caster != l_Target && l_Caster->HasAura(eSpells::DreamOfCenariusFeral))
                        l_Caster->AddAura(eSpells::Rejuvenation, l_Caster);
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_dream_of_cenarius_feral_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_dream_of_cenarius_feral_SpellScript();
        }
};

/// Last Update - 6.1.2
/// Item - Druid WoD PvP Restoration 2P Bonus - 170853
/// Called by Barskin (22812) and Ironbark (102342)
class spell_dru_wod_pvp_2p_restoration : public SpellScriptLoader
{
    public:
        spell_dru_wod_pvp_2p_restoration() : SpellScriptLoader("spell_dru_wod_pvp_2p_restoration") { }

        class spell_dru_wod_pvp_2p_restoration_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_wod_pvp_2p_restoration_SpellScript);

            enum eSpells
            {
                WodPvP2PBonus       = 170853,
                WodPvP2PBonusEffect = 170856
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::WodPvP2PBonus))
                    l_Caster->CastSpell(l_Target, eSpells::WodPvP2PBonusEffect, true);
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_wod_pvp_2p_restoration_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_wod_pvp_2p_restoration_SpellScript();
        }
};

/// Last Update 7.0.3
/// Pulverize - 80313
class spell_dru_pulverize : public SpellScriptLoader
{
    public:
        spell_dru_pulverize() : SpellScriptLoader("spell_dru_pulverize") { }

        class spell_dru_pulverize_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_pulverize_SpellScript);

            enum eSpells
            {
                Trash                   = 192090,
                PulverizeAura           = 158792,
                TrashPulverizeMarker    = 158790,
            };

            SpellCastResult CheckCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();

                if (!l_Caster || !l_Target)
                    return SPELL_FAILED_ERROR;

                if (Aura* l_Trash = l_Target->GetAura(eSpells::Trash, l_Caster->GetGUID()))
                {
                    if (l_Trash->GetStackAmount() >= 2)
                        return SPELL_CAST_OK;
                }

                return SPELL_FAILED_ERROR;
            }

            void HandleDamage(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (Aura* l_Lacerate = l_Target->GetAura(eSpells::Trash, l_Caster->GetGUID()))
                    l_Lacerate->ModStackAmount(-2);

                l_Caster->CastSpell(l_Caster, eSpells::PulverizeAura, true);
                l_Target->RemoveAura(eSpells::TrashPulverizeMarker);
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_pulverize_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_dru_pulverize_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_pulverize_SpellScript();
        }
};

/// Last Update 6.2.3
/// WoD PvP Balance 4P Bonus - 180717
class spell_dru_WodPvpBalance4pBonus : public SpellScriptLoader
{
    public:
        spell_dru_WodPvpBalance4pBonus() : SpellScriptLoader("spell_dru_WodPvpBalance4pBonus") { }

        class spell_dru_WodPvpBalance4pBonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_WodPvpBalance4pBonus_AuraScript);

            enum eSpells
            {
                CelestialFury   = 180719,
                Starsurge       = 78674
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    Unit* l_Victim = p_EventInfo.GetDamageInfo()->GetTarget();
                    SpellInfo const* l_SpellInfo = p_EventInfo.GetDamageInfo()->GetSpellInfo();

                    if (l_SpellInfo == nullptr || l_Victim == nullptr)
                        return;

                    if (l_SpellInfo->Id != eSpells::Starsurge)
                        return;

                    if (l_Victim->IsPlayer())
                        l_Caster->CastSpell(l_Caster, eSpells::CelestialFury, true);
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_WodPvpBalance4pBonus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_WodPvpBalance4pBonus_AuraScript();
        }
};

/// Item - Druid WoD PvP Balance 2P Bonus - 165701
/// Called by Entangling Roots - 339
class spell_dru_wod_pvp_balance_2p : public SpellScriptLoader
{
    public:
        spell_dru_wod_pvp_balance_2p() : SpellScriptLoader("spell_dru_wod_pvp_balance_2p") { }

        class spell_dru_wod_pvp_balance_2p_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_wod_pvp_balance_2p_AuraScript);

            enum eSpells
            {
                Starsurge               = 78674,
                WoDPvPBalance2PBonus    = 165701
            };

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                if (!l_Player->HasAura(eSpells::WoDPvPBalance2PBonus))
                    return;

                /// When Entangling Roots is dispelled or broken by damage, you gain 1 charge of Starsurge.
                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE || GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                    return;

                if (SpellInfo const* l_Starsurge = sSpellMgr->GetSpellInfo(eSpells::Starsurge))
                    l_Player->RestoreCharge(l_Starsurge->ChargeCategoryEntry);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectApplyFn(spell_dru_wod_pvp_balance_2p_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_ROOT_2, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_wod_pvp_balance_2p_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Lunar Inspiration - 155580
class spell_dru_lunar_inspiration : public SpellScriptLoader
{
    public:
        spell_dru_lunar_inspiration() : SpellScriptLoader("spell_dru_lunar_inspiration") { }

        class spell_dru_lunar_inspiration_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_lunar_inspiration_AuraScript);

            enum eSpells
            {
                LunarInspirationOverride = 155627
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                l_Caster->AddAura(eSpells::LunarInspirationOverride, l_Caster);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                l_Caster->RemoveAura(eSpells::LunarInspirationOverride);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_lunar_inspiration_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_lunar_inspiration_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_lunar_inspiration_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Incarnation: Tree of Life - 33891
class spell_dru_incarnation_tree_of_life : public SpellScriptLoader
{
    public:
        spell_dru_incarnation_tree_of_life() : SpellScriptLoader("spell_dru_incarnation_tree_of_life") { }

        class spell_dru_incarnation_tree_of_life_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_incarnation_tree_of_life_AuraScript);

            enum eSpells
            {
                Incarnation = 117679
            };

            void OnApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                if (Aura* l_Aura = l_Target->GetAura(eSpells::Incarnation))
                {
                    p_AurEff->GetBase()->SetDuration(l_Aura->GetDuration());
                    l_Target->RemoveAura(eSpells::Incarnation);
                }
            }

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();

                AuraRemoveMode RemoveMod = GetTargetApplication()->GetRemoveMode();

                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_CANCEL)
                    return;

                if (p_AurEff->GetBase()->GetDuration())
                    l_Target->CastSpell(l_Target, eSpells::Incarnation, true);

                if (Aura* l_Aura = l_Target->GetAura(eSpells::Incarnation))
                    l_Aura->SetDuration(p_AurEff->GetBase()->GetDuration());
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_incarnation_tree_of_life_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_incarnation_tree_of_life_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_incarnation_tree_of_life_AuraScript();
        }
};

/// last update : 7.0.3
/// Soul of the forest - 158476
class PlayerScript_soul_of_the_forest : public PlayerScript
{
    public:
        PlayerScript_soul_of_the_forest() : PlayerScript("PlayerScript_soul_of_the_forest") { }

        enum eSpells
        {
            SoulOfTheForestAura     = 158476,
            SoulOfTheForestTrigger  = 114113
        };

        void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen, bool p_After)
        {
            if (!p_After)
                return;

            if (p_Regen || p_Power != POWER_COMBO_POINT || p_Player->getClass() != CLASS_DRUID || !p_Player->HasAura(eSpells::SoulOfTheForestAura))
                return;

            if (p_Player->GetActiveSpecializationID() != SPEC_DRUID_FERAL)
                return;

            /// Get the power earn (if > 0 ) or consum (if < 0)
            int32 l_DiffVal = p_NewValue - p_OldValue;

            if (l_DiffVal < 0)
            {
                AuraEffect* l_AuraEffect = p_Player->GetAuraEffect(eSpells::SoulOfTheForestAura, EFFECT_0);
                if (l_AuraEffect == nullptr)
                    return;

                int32 l_Bp = l_AuraEffect->GetAmount() * abs(l_DiffVal);
                p_Player->EnergizeBySpell(p_Player, eSpells::SoulOfTheForestTrigger, l_Bp, POWER_ENERGY);
            }
        }
};

/// last update : 6.1.2 19802
/// Moonkin Form - 24858, Incarnation: Chosen of Elune 102560
class spell_dru_astral_form : public SpellScriptLoader
{
    public:
        spell_dru_astral_form() : SpellScriptLoader("spell_dru_astral_form") { }

        class spell_dru_astral_form_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_astral_form_AuraScript);

            enum eSpells
            {
                GlyphOfStars    = 114301,
                MoonkinForm     = 24858,
                ChosenofElune   = 102560,
                OwlkinFrenzy    = 157228
            };

            void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::GlyphOfStars);

                if (l_Target->HasAura(eSpells::GlyphOfStars) && l_SpellInfo != nullptr)
                {
                    l_Target->SetDisplayId(l_Target->GetNativeDisplayId());
                    l_Target->CastSpell(l_Target, l_SpellInfo->Effects[EFFECT_0].BasePoints, true);
                }
            }

            void AfterRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::GlyphOfStars);

                if (l_Target->HasAura(eSpells::MoonkinForm) || l_Target->HasAura(eSpells::ChosenofElune))
                    return;

                if (l_SpellInfo != nullptr && l_Target->HasAura(l_SpellInfo->Effects[EFFECT_0].BasePoints))
                    l_Target->RemoveAura(l_SpellInfo->Effects[EFFECT_0].BasePoints);
            }

            void HandleOnProc(ProcEventInfo& p_ProcEventInfo)
            {
                DamageInfo* l_DamageInfo = p_ProcEventInfo.GetDamageInfo();
                Unit* l_Caster = GetCaster();
                if (!l_DamageInfo)
                    return;

                Spell const* l_Spell = l_DamageInfo->GetSpell();
                if (l_Spell && !l_Spell->IsSingleEnnemyTargetSpell())
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::OwlkinFrenzy, true);
            }

            void Register() override
            {
                switch (m_scriptSpellId)
                {
                    case eSpells::MoonkinForm:
                        AfterEffectApply += AuraEffectApplyFn(spell_dru_astral_form_AuraScript::AfterApply, EFFECT_1, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_astral_form_AuraScript::AfterRemove, EFFECT_1, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                        OnProc += AuraProcFn(spell_dru_astral_form_AuraScript::HandleOnProc);
                        break;
                    case eSpells::ChosenofElune:
                        AfterEffectApply += AuraEffectApplyFn(spell_dru_astral_form_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_astral_form_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                        break;
                    default:
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_astral_form_AuraScript();
        }
};

/// last update : 7.0.3
/// Guardian of Elune - 155578
class spell_dru_guardian_of_elune : public SpellScriptLoader
{
    public:
        spell_dru_guardian_of_elune() : SpellScriptLoader("spell_dru_guardian_of_elune") { }

        class spell_dru_guardian_of_elune_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_guardian_of_elune_AuraScript);

            enum eSpells
            {
                Mangle              = 33917,
                GuardianOfEluneAura = 213680
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                SpellInfo const* l_ProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (l_ProcSpell == nullptr|| l_ProcSpell->Id != eSpells::Mangle)
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::GuardianOfEluneAura, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_guardian_of_elune_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_guardian_of_elune_AuraScript();
        }
};

/// last update : 7.0.3
/// Guardian of Elune - 213680
class spell_dru_guardian_of_elune_aura : public SpellScriptLoader
{
    public:
        spell_dru_guardian_of_elune_aura() : SpellScriptLoader("spell_dru_guardian_of_elune_aura") { }

        class spell_dru_guardian_of_elune_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_guardian_of_elune_aura_AuraScript);

            enum eSpells
            {
                Mangle              = 33917,
                GuardianOfEluneAura = 213680,
                IronFur             = 192081,
                MarkofUrsol         = 192083
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                SpellInfo const* l_ProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (l_ProcSpell == nullptr || (l_ProcSpell->Id != eSpells::IronFur && l_ProcSpell->Id != eSpells::MarkofUrsol))
                    return;

                l_Caster->RemoveAura(GetSpellInfo()->Id);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_guardian_of_elune_aura_AuraScript::OnProc, EFFECT_1, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_guardian_of_elune_aura_AuraScript();
        }
};

/// Item - Druid T17 Feral 2P Bonus - 165431
class spell_dru_item_t17_feral_2p_bonus : public SpellScriptLoader
{
    public:
        spell_dru_item_t17_feral_2p_bonus() : SpellScriptLoader("spell_dru_item_t17_feral_2p_bonus") { }

        class spell_dru_item_t17_feral_2p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_item_t17_feral_2p_bonus_AuraScript);

            enum eSpell
            {
                ScentOfBlood = 169752
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                /// Dealing bleed damage generates 1 Energy.
                SpellInfo const* l_ProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (!l_ProcSpell || l_ProcSpell->Mechanic != Mechanics::MECHANIC_BLEED)
                    return;

                l_Caster->CastSpell(l_Caster, eSpell::ScentOfBlood, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_item_t17_feral_2p_bonus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_item_t17_feral_2p_bonus_AuraScript();
        }
};

/// Called by Berserk (for Cat Form) - 106951
/// Item - Druid T17 Feral 4P Bonus - 165432
class spell_dru_item_t17_feral_4p_bonus : public SpellScriptLoader
{
    public:
        spell_dru_item_t17_feral_4p_bonus() : SpellScriptLoader("spell_dru_item_t17_feral_4p_bonus") { }

        class spell_dru_item_t17_feral_4p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_item_t17_feral_4p_bonus_AuraScript);

            enum eSpells
            {
                T17Feral4P              = 165432,
                T17Feral4PProcDriver    = 166639,
                FeralInstinct           = 210631,
                FeralInstinctBuff       = 210649
            };

            enum eTraits
            {
                FeralInstinctPowerId = 1167
            };

            void AfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_FeralInstinctSpellInfo = sSpellMgr->GetSpellInfo(eSpells::FeralInstinctBuff);
                if (!l_Caster || !l_FeralInstinctSpellInfo)
                    return;

                if (l_Caster->HasAura(eSpells::T17Feral4P))
                    l_Caster->CastSpell(l_Caster, eSpells::T17Feral4PProcDriver, true);

                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player)
                    return;

                int32 l_FeralInstinctBp0 = l_FeralInstinctSpellInfo->Effects[EFFECT_0].BasePoints * l_Player->GetRankOfArtifactPowerId(eTraits::FeralInstinctPowerId);
                if (l_Player->HasAura(eSpells::FeralInstinct))
                    l_Player->CastCustomSpell(l_Player, eSpells::FeralInstinctBuff, &l_FeralInstinctBp0, nullptr, nullptr, true);
            }

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                l_Caster->RemoveAura(eSpells::T17Feral4PProcDriver);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_item_t17_feral_4p_bonus_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectApplyFn(spell_dru_item_t17_feral_4p_bonus_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_item_t17_feral_4p_bonus_AuraScript();
        }
};

/// Item - Druid T17 Feral 2P Bonus Proc Driver - 166639
class spell_dru_item_t17_feral_4p_bonus_proc_driver : public SpellScriptLoader
{
    public:
        spell_dru_item_t17_feral_4p_bonus_proc_driver() : SpellScriptLoader("spell_dru_item_t17_feral_4p_bonus_proc_driver") { }

        class spell_dru_item_t17_feral_4p_bonus_proc_driver_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_item_t17_feral_4p_bonus_proc_driver_AuraScript);

            enum eSpells
            {
                T17Feral4P      = 165432,
                GushingWound    = 166638
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                /// While Berserk is active, special attacks deal an additional 30% bleed damage to the target over 6 sec.
                AuraEffect* l_AurEff = l_Caster->GetAuraEffect(eSpells::T17Feral4P, EFFECT_0);
                if (l_AurEff == nullptr)
                {
                    p_AurEff->GetBase()->Remove();
                    return;
                }

                Unit* l_Target = p_EventInfo.GetActionTarget();
                if (l_Target == nullptr || l_Target == l_Caster)
                    return;

                SpellInfo const* l_Trigger  = sSpellMgr->GetSpellInfo(eSpells::GushingWound);
                int32 l_Damage              = CalculatePct(p_EventInfo.GetDamageInfo()->GetAmount(), l_AurEff->GetAmount());
                int32 l_PreviousTotalDamage = 0;

                if (AuraEffect* l_OldWounds = l_Target->GetAuraEffect(eSpells::GushingWound, EFFECT_0, l_Caster->GetGUID()))
                {
                    int32 l_PeriodicDamage  = l_OldWounds->GetAmount();
                    int32 l_Duration        = l_OldWounds->GetBase()->GetDuration();
                    int32 l_Amplitude       = l_OldWounds->GetAmplitude();

                    if (l_Amplitude)
                        l_PreviousTotalDamage = l_PeriodicDamage * (l_Duration / l_Amplitude);

                    l_PreviousTotalDamage /= (l_Trigger->GetMaxDuration() / l_Trigger->Effects[EFFECT_0].Amplitude);
                }

                if (l_Trigger->Effects[EFFECT_0].Amplitude)
                    l_Damage /= (l_Trigger->GetMaxDuration() / l_Trigger->Effects[EFFECT_0].Amplitude);

                l_Damage += l_PreviousTotalDamage;

                if (Aura* l_Wounds = l_Target->GetAura(eSpells::GushingWound, l_Caster->GetGUID()))
                    l_Wounds->RefreshDuration();
                else
                    l_Caster->CastSpell(l_Target, eSpells::GushingWound, true);

                if (AuraEffect* l_NewWounds = l_Target->GetAuraEffect(eSpells::GushingWound, EFFECT_0, l_Caster->GetGUID()))
                    l_NewWounds->SetAmount(l_Damage);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_item_t17_feral_4p_bonus_proc_driver_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_item_t17_feral_4p_bonus_proc_driver_AuraScript();
        }
};

/// Item - Druid T17 Restoration 4P Bonus - 167714
class spell_dru_item_t17_restoration_4p_bonus : public SpellScriptLoader
{
    public:
        spell_dru_item_t17_restoration_4p_bonus() : SpellScriptLoader("spell_dru_item_t17_restoration_4p_bonus") { }

        class spell_dru_item_t17_restoration_4p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_item_t17_restoration_4p_bonus_AuraScript);

            enum eSpells
            {
                HealingTouch    = 5185,
                NaturesWisdom   = 167715
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                /// When you cast 2 sequential Healing Touch spells, the mana cost of your next Wild Growth is reduced by 25%.
                SpellInfo const* l_ProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (!l_ProcSpell)
                    return;

                uint32 l_FlagsNot = ProcFlagsExLegacy::PROC_EX_INTERNAL_DOT | ProcFlagsExLegacy::PROC_EX_INTERNAL_HOT;
                l_FlagsNot |= ProcFlagsExLegacy::PROC_EX_INTERNAL_TRIGGERED | ProcFlagsExLegacy::PROC_EX_INTERNAL_MULTISTRIKE;

                if (p_EventInfo.GetHitMask() & l_FlagsNot)
                    return;

                if (AuraEffect* l_AurEff = p_AurEff->GetBase()->GetEffect(EFFECT_0))
                {
                    if (l_ProcSpell->Id != eSpells::HealingTouch)
                    {
                        l_AurEff->ChangeAmount(2);
                        return;
                    }

                    l_AurEff->ChangeAmount(l_AurEff->GetAmount() - 1);

                    if (l_AurEff->GetAmount() <= 0)
                    {
                        l_AurEff->ChangeAmount(2);
                        l_Caster->CastSpell(l_Caster, eSpells::NaturesWisdom, true);
                    }
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_item_t17_restoration_4p_bonus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_item_t17_restoration_4p_bonus_AuraScript();
        }
};

/// Last Update 6.2.3
/// Living Seed - 48500
class spell_dru_living_seed : public SpellScriptLoader
{
    public:
        spell_dru_living_seed() : SpellScriptLoader("spell_dru_living_seed") { }

        class spell_dru_living_seed_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_living_seed_AuraScript);

            enum eSpells
            {
                LivingSeedAura = 48504
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();

                if (p_EventInfo.GetDamageInfo() == nullptr || l_Caster == nullptr)
                    return;

                int32 l_HealAmount = p_EventInfo.GetDamageInfo()->GetAmount();

                Unit* l_Target = p_EventInfo.GetDamageInfo()->GetTarget();

                if (!l_HealAmount || l_Target == nullptr)
                    return;

                l_HealAmount = CalculatePct(l_HealAmount, p_AurEff->GetAmount());
                if (AuraEffect* l_LivingSeed = l_Caster->GetAuraEffect(eSpells::LivingSeedAura, EFFECT_0))
                    l_HealAmount += l_LivingSeed->GetAmount();

                l_Caster->CastCustomSpell(l_Target, eSpells::LivingSeedAura, &l_HealAmount, NULL, NULL, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_living_seed_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_living_seed_AuraScript();
        }
};

/// Last Update 6.2.3
/// One With Nature - 147420
class spell_dru_one_with_nature : public SpellScriptLoader
{
public:
    spell_dru_one_with_nature() : SpellScriptLoader("spell_dru_one_with_nature") { }

    class spell_dru_one_with_nature_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_one_with_nature_SpellScript);

        enum eSpells
        {
            OneWithNature = 147420
        };

        void HandleAfterHit()
        {
            Player* l_Player = GetCaster()->ToPlayer();

            if (l_Player == nullptr)
                return;

            int ZoneId[8] = { 85, 4, 493, 148, 331, 3711, 5805, 5785 };
            uint8 l_nbrDest = 0;
            uint32 l_minLevel = 0;
            for (uint8 i = 0; i < 8; ++i)
            {
                if (WorldMapAreaEntry const* l_wma = sWorldMapAreaStore.LookupEntry(ZoneId[i]))
                {
                    switch (ZoneId[i])
                    {
                        case 4: ///< minRecommendedLevel for Blasted land is 0
                            l_minLevel = 55;
                            break;
                        case 493: ///< minRecommendedLevel for Moonglade is 55
                            l_minLevel = 15;
                            break;
                        default:
                            l_minLevel = l_wma->minRecommendedLevel;
                            break;
                    }
                    if (l_Player->getLevel() >= l_minLevel)
                        ++l_nbrDest;
                }
            }

            uint8 l_dest = urand(0, l_nbrDest-1);

            switch (l_dest)
            {
                case 0: ///< Tirisfal Glades
                    l_Player->TeleportTo(0, 1800.0f, 2239.0f, 148.88f, 0.0f);
                    break;
                case 1: ///< Darkshore
                    l_Player->TeleportTo(1, 4987.0f, 143.0f, 50.32f, 4.86f);
                    break;
                case 2: ///< Moonglade
                    l_Player->TeleportTo(1, 7372.0f, -2630.0f, 464.67f, 6.17f);
                    break;
                case 3: ///< Ashenvale
                    l_Player->TeleportTo(1, 3824.0f, 88.0f, 13.1f, 1.6f);
                    break;
                case 4: ///< Blasted Lands
                    l_Player->TeleportTo(0, -12338.0f, -2376.0f, 21.79f, 3.38f);
                    break;
                case 5: ///< Sholazar Basin
                    l_Player->TeleportTo(571, 6338.0f, 5189.0f, -75.7f, 2.78f);
                    break;
                case 6: ///< The Jade Forest
                    l_Player->TeleportTo(870, 262.0f, 1981.0f, 162.67f, 5.45f);
                    break;
                case 7: ///< Valley Of The Four Winds
                    l_Player->TeleportTo(870, 1104.0f, -1896.0f, 132.96f, 3.11f);
                    break;
                default:
                    break;
            }
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_dru_one_with_nature_SpellScript::HandleAfterHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_one_with_nature_SpellScript();
    }
};

/// Charm Woodland Creature - 127757
class spell_dru_charm_woodland_creature : public SpellScriptLoader
{
    public:
        spell_dru_charm_woodland_creature() : SpellScriptLoader("spell_dru_charm_woodland_creature") { }

        class spell_dru_charm_woodland_creature_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_charm_woodland_creature_SpellScript);

            void HandleDummy()
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (l_Player == nullptr)
                    return;

                Unit* l_Target = l_Player->GetSelectedUnit();   ///< Woodland Creature
                if (l_Target == nullptr)
                    return;

                l_Target->GetMotionMaster()->MoveFollow(l_Player, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE, MOTION_SLOT_CONTROLLED);
            }

            void Register() override
            {
                AfterHit += SpellHitFn(spell_dru_charm_woodland_creature_SpellScript::HandleDummy);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_charm_woodland_creature_SpellScript();
        }
};

/// Last Update 7.0.3 - 22045
class PlayerScript_predator : public PlayerScript
{
    public:

        PlayerScript_predator() :PlayerScript("PlayerScript_predator") { }

        enum eSpells
        {
            Predator = 202021,
            TigersFury = 5217
        };

        void OnKill(Player* p_Player, Unit* p_Victim, bool p_BeforeAurasRemoved) override
        {
            if (p_Victim == nullptr || p_BeforeAurasRemoved)
                return;

            Unit::AuraEffectList const& l_Periodic = p_Victim->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
            for (auto l_AuraEffect : l_Periodic)
            {
                if (l_AuraEffect->GetSpellInfo()->GetEffectMechanicMask(l_AuraEffect->GetEffIndex()) & (1LL << MECHANIC_BLEED))
                {
                    Unit* l_Caster = l_AuraEffect->GetCaster();

                    if (l_Caster == nullptr)
                        continue;

                    Player* l_Player = l_Caster->ToPlayer();

                    if (l_Player == nullptr)
                        continue;

                    if (l_Player->GetActiveSpecializationID() != SPEC_DRUID_FERAL || !l_Player->HasAura(eSpells::Predator))
                        continue;

                    l_Player->RemoveSpellCooldown(eSpells::TigersFury);
                }
            }
        }
};

/// Last Update 7.0.3
/// Balance Affinity - 197488, Balance Affinity - 197632
class spell_dru_balance_affinity : public SpellScriptLoader
{
    public:
        spell_dru_balance_affinity() : SpellScriptLoader("spell_dru_balance_affinity") { }

        enum eSpells
        {
            BalanceAffinity1 = 197488,
            BalanceAffinity2 = 197632
        };

        enum eLearningSpells
        {
            AstralInfluence = 197524,
            MoonkinForm     = 197625,
            Starsurge       = 197626,
            LunarStrike     = 197628,
            SolarWrath      = 197629,
            Sunfire         = 197630
        };

        class spell_dru_balance_affinity_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_balance_affinity_AuraScript);

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                switch (GetSpellInfo()->Id)
                {
                    case BalanceAffinity1:
                    {
                        l_Player->learnSpell(eLearningSpells::AstralInfluence, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::LunarStrike, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::MoonkinForm, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::SolarWrath, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Starsurge, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Sunfire, false, false, false, 0, true);
                        break;
                    }
                    case BalanceAffinity2:
                    {
                        l_Player->learnSpell(eLearningSpells::AstralInfluence, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::MoonkinForm, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Starsurge, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::LunarStrike, false, false, false, 0, true);
                        break;
                    }
                    default:
                        break;
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                switch (GetSpellInfo()->Id)
                {
                case BalanceAffinity1:
                {
                    l_Player->removeSpell(eLearningSpells::AstralInfluence, false, false);
                    l_Player->removeSpell(eLearningSpells::LunarStrike, false, false);
                    l_Player->removeSpell(eLearningSpells::MoonkinForm, false, false);
                    l_Player->removeSpell(eLearningSpells::SolarWrath, false, false);
                    l_Player->removeSpell(eLearningSpells::Starsurge, false, false);
                    l_Player->removeSpell(eLearningSpells::Sunfire, false, false);
                    break;
                }
                case BalanceAffinity2:
                {
                    l_Player->removeSpell(eLearningSpells::AstralInfluence, false, false);
                    l_Player->removeSpell(eLearningSpells::MoonkinForm, false, false);
                    l_Player->removeSpell(eLearningSpells::Starsurge, false, false);
                    l_Player->removeSpell(eLearningSpells::LunarStrike, false, false);
                    break;
                }
                default:
                    break;
                }
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_balance_affinity_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_balance_affinity_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_balance_affinity_AuraScript();
        }
};

/// Last Update 7.0.3
/// Guardian Affinity - 197491, Guardian Affinity - 217615
class spell_dru_guardian_affinity : public SpellScriptLoader
{
    public:
        spell_dru_guardian_affinity() : SpellScriptLoader("spell_dru_guardian_affinity") { }

        enum eSpells
        {
            GuardianAffinity1 = 197491,
            GuardianAffinity2 = 217615
        };

        enum eLearningSpells
        {
            Mangle                  = 33917,
            Thrash                  = 106832,
            Ironfur                 = 192081,
            FrenziedRegeneration    = 22842,
            ThickHide               = 16931
        };

        class spell_dru_guardian_affinity_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_guardian_affinity_AuraScript);

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                switch (GetSpellInfo()->Id)
                {
                    case GuardianAffinity1:
                    {
                        l_Player->learnSpell(eLearningSpells::ThickHide, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Mangle, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Thrash, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Ironfur, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::FrenziedRegeneration, false, false, false, 0, true);
                        break;
                    }
                    case GuardianAffinity2:
                    {
                        l_Player->learnSpell(eLearningSpells::ThickHide, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Mangle, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Ironfur, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::FrenziedRegeneration, false, false, false, 0, true);
                        break;
                    }
                    default:
                        break;
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                switch (GetSpellInfo()->Id)
                {
                    case GuardianAffinity1:
                    {
                        l_Player->removeSpell(eLearningSpells::ThickHide, false, false);
                        l_Player->removeSpell(eLearningSpells::Mangle, false, false);
                        l_Player->removeSpell(eLearningSpells::Thrash, false, false);
                        l_Player->removeSpell(eLearningSpells::Ironfur, false, false);
                        l_Player->removeSpell(eLearningSpells::FrenziedRegeneration, false, false);
                        break;
                    }
                    case GuardianAffinity2:
                    {
                        l_Player->removeSpell(eLearningSpells::ThickHide, false, false);
                        l_Player->removeSpell(eLearningSpells::Mangle, false, false);
                        l_Player->removeSpell(eLearningSpells::Ironfur, false, false);
                        l_Player->removeSpell(eLearningSpells::FrenziedRegeneration, false, false);
                        break;
                    }
                    default:
                        break;
                }
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_guardian_affinity_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_guardian_affinity_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_guardian_affinity_AuraScript();
        }
};

/// Last Update 7.0.3
/// Restoration Affinity - 197492
class spell_dru_restoration_affinity : public SpellScriptLoader
{
    public:
        spell_dru_restoration_affinity() : SpellScriptLoader("spell_dru_restoration_affinity") { }

        enum eLearningSpells
        {
            YserasGift      = 145108,
            Rejuvenation    = 774,
            HealingTouch    = 5185,
            Swiftmend       = 18562
        };

        class spell_dru_restoration_affinity_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_restoration_affinity_AuraScript);

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                l_Player->learnSpell(eLearningSpells::YserasGift, false, false, false, 0, true);
                l_Player->learnSpell(eLearningSpells::Rejuvenation, false, false, false, 0, true);
                l_Player->learnSpell(eLearningSpells::HealingTouch, false, false, false, 0, true);
                l_Player->learnSpell(eLearningSpells::Swiftmend, false, false, false, 0, true);
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                l_Player->removeSpell(eLearningSpells::YserasGift, false, false);
                l_Player->removeSpell(eLearningSpells::Rejuvenation, false, false);
                l_Player->removeSpell(eLearningSpells::HealingTouch, false, false);
                l_Player->removeSpell(eLearningSpells::Swiftmend, false, false);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_restoration_affinity_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_restoration_affinity_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_restoration_affinity_AuraScript();
        }
};

/// Last Update 7.0.3
/// Feral Affinity - 197490, Feral Affinity - 202155, Feral Affinity - 202157
class spell_dru_feral_affinity : public SpellScriptLoader
{
    public:
        spell_dru_feral_affinity() : SpellScriptLoader("spell_dru_feral_affinity") { }

        enum eSpells
        {
            FeralAffinity1 = 197490,
            FeralAffinity2 = 202155,
            FeralAffinity3 = 202157
        };

        enum eLearningSpells
        {
            FelineSwiftness = 131768,
            Shred = 5221,
            FerociousBite = 22568,
            Swipe = 106785,
            Rake = 1822,
            Rip = 1079
        };

        class spell_dru_feral_affinity_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_feral_affinity_AuraScript);

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                switch (GetSpellInfo()->Id)
                {
                    case FeralAffinity1:
                    case FeralAffinity3:
                    {
                        l_Player->learnSpell(eLearningSpells::FelineSwiftness, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Shred, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::FerociousBite, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Swipe, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Rip, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Rake, false, false, false, 0, true);
                        break;
                    }
                    case FeralAffinity2:
                    {
                        l_Player->learnSpell(eLearningSpells::FelineSwiftness, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Shred, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::FerociousBite, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Rip, false, false, false, 0, true);
                        l_Player->learnSpell(eLearningSpells::Rake, false, false, false, 0, true);
                        break;
                    }
                    default:
                        break;
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                switch (GetSpellInfo()->Id)
                {
                    case FeralAffinity1:
                    case FeralAffinity3:
                    {
                        l_Player->removeSpell(eLearningSpells::FelineSwiftness, false, false);
                        l_Player->removeSpell(eLearningSpells::Shred, false, false);
                        l_Player->removeSpell(eLearningSpells::FerociousBite, false, false);
                        l_Player->removeSpell(eLearningSpells::Swipe, false, false);
                        l_Player->removeSpell(eLearningSpells::Rip, false, false);
                        l_Player->removeSpell(eLearningSpells::Rake, false, false);
                        break;
                    }
                    case FeralAffinity2:
                    {
                        l_Player->removeSpell(eLearningSpells::FelineSwiftness, false, false);
                        l_Player->removeSpell(eLearningSpells::Shred, false, false);
                        l_Player->removeSpell(eLearningSpells::FerociousBite, false, false);
                        l_Player->removeSpell(eLearningSpells::Rip, false, false);
                        l_Player->removeSpell(eLearningSpells::Rake, false, false);
                        break;
                    }
                    default:
                        break;
                }
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_feral_affinity_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_feral_affinity_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_feral_affinity_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Bristling Fur - 155835
class spell_dru_bristling_fur : public SpellScriptLoader
{
    public:
        spell_dru_bristling_fur() : SpellScriptLoader("spell_dru_bristling_fur") { }

        class spell_dru_bristling_fur_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_bristling_fur_AuraScript);

            enum eSpells
            {
                BristlingFurBuff = 204031
            };

            void HandleOnProc(ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster || l_Caster->GetShapeshiftForm() != FORM_BEAR_FORM)
                    return;

                DamageInfo* l_DamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_DamageInfo)
                    return;

                int32 l_Bp = (l_DamageInfo->GetAmount() / (l_Caster->GetMaxHealth() / 100.0f)) * l_Caster->GetPowerCoeff(POWER_RAGE);

                l_Caster->CastCustomSpell(l_Caster, eSpells::BristlingFurBuff, &l_Bp, nullptr, nullptr, true);
            }

            void Register() override
            {
                OnProc += AuraProcFn(spell_dru_bristling_fur_AuraScript::HandleOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_bristling_fur_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Brambles - 203953
class spell_dru_brambles : public SpellScriptLoader
{
    public:
        spell_dru_brambles() : SpellScriptLoader("spell_dru_brambles") { }

        class spell_dru_brambles_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_brambles_AuraScript);

            enum eSpells
            {
                BramblesDamage  = 203958
            };

            void OnAbsorb(AuraEffect* /*p_AurEff*/, DamageInfo& p_DmgInfo, uint32& p_AbsorbAmount)
            {
                Unit* l_Attacker = p_DmgInfo.GetActor();
                Unit* l_Victim = p_DmgInfo.GetTarget();

                if (!l_Attacker || !l_Victim || l_Attacker == l_Victim)
                    return;

                int32 absorb = l_Victim->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * 0.24f;

                p_AbsorbAmount = absorb;

                // Prevent from freezing
                if (!UpdateDamageCooldown())
                    return;

                int32 damage = RoundToInterval(absorb, 0, int32(p_DmgInfo.GetAmount()));

                l_Victim->CastCustomSpell(eSpells::BramblesDamage, SpellValueMod::SPELLVALUE_BASE_POINT0, damage, l_Attacker, true);
            }

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                p_Amount = -1; ///< Initialize to infinite Absorb
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_brambles_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_brambles_AuraScript::OnAbsorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }

        private:

            bool UpdateDamageCooldown()
            {
                if (m_DamageCooldown > time(NULL))
                    return false;

                m_DamageCooldown = time(NULL) + 1;

                return true;
            }

        private:

            time_t m_DamageCooldown = 0;
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_brambles_AuraScript();
        }
};

/// Last Update 7.1.5 23420
/// Earthwarden (Periodic) - 203974
class spell_dru_earthwarden : public SpellScriptLoader
{
    public:
        spell_dru_earthwarden() : SpellScriptLoader("spell_dru_earthwarden") { }

        class spell_dru_earthwarden_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_earthwarden_AuraScript);

            enum eSpells
            {
                EarthWarden     = 203974,
                EarthwardenBuff = 203975
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (Player* l_Player = l_Caster->ToPlayer())
                {
                    if (l_Player->HasSpellCooldown(eSpells::EarthwardenBuff))
                        return;

                    l_Player->CastSpell(l_Player, eSpells::EarthwardenBuff, true);

                    l_Player->AddSpellCooldown(eSpells::EarthwardenBuff, 0, 1 * IN_MILLISECONDS);
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Owner = GetUnitOwner();
                if (!l_Owner)
                    return;

                l_Owner->RemoveAurasDueToSpell(eSpells::EarthwardenBuff);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_earthwarden_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_earthwarden_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_earthwarden_AuraScript();
        }
};

/// Last Update 7.1.5 23420
/// Earthwarden - 203975
class spell_dru_earthwarden_absorb : public SpellScriptLoader
{
    public:
        spell_dru_earthwarden_absorb() : SpellScriptLoader("spell_dru_earthwarden_absorb") { }

        class spell_dru_earthwarden_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_earthwarden_absorb_AuraScript);

            enum eSpells
            {
                Earthwarden = 203974
            };

            void OnAbsorb(AuraEffect* p_AurEff, DamageInfo& p_DmgInfo, uint32& p_AbsorbAmount)
            {
                Unit* l_Victim = p_DmgInfo.GetTarget();

                if (l_Victim == nullptr)
                    return;

                AuraEffect* l_AuraEffect = l_Victim->GetAuraEffect(eSpells::Earthwarden, EFFECT_0);

                if (l_AuraEffect == nullptr)
                    return;

                if (p_DmgInfo.GetSpell() != nullptr || p_DmgInfo.GetSchoolMask() != SPELL_SCHOOL_MASK_NORMAL)
                {
                    p_AbsorbAmount = 0;
                    return;
                }

                p_AbsorbAmount = CalculatePct(p_DmgInfo.GetAmount(), l_AuraEffect->GetAmount());
                p_AurEff->GetBase()->DropStack();
            }

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                p_Amount = -1; ///< Initialize to infinite Absorb
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_earthwarden_absorb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_earthwarden_absorb_AuraScript::OnAbsorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_earthwarden_absorb_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Galactic Guardian - 203964
class spell_dru_galactic_guardian : public SpellScriptLoader
{
    public:
        spell_dru_galactic_guardian() : SpellScriptLoader("spell_dru_galactic_guardian") { }

        class spell_dru_galactic_guardian_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_galactic_guardian_AuraScript);

            enum eSpells
            {
                GalacticGuardianAura    = 213708,
                Moonfire                = 8921
            };

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();
                DamageInfo* l_DamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_DamageInfo || l_DamageInfo->GetDamageType() == DamageEffectType::DOT)
                    return;

                Unit* l_Victim = l_DamageInfo->GetTarget();
                Unit* l_Caster = GetCaster();
                if (!l_Caster || !l_Victim)
                    return;

                l_Caster->CastSpell(l_Victim, eSpells::Moonfire, true);
                l_Caster->CastSpell(l_Caster, eSpells::GalacticGuardianAura, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_galactic_guardian_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_galactic_guardian_AuraScript();
        }
};

/// Last Update 7.0.3
/// Rend and tear - 204053
class spell_dru_rend_and_tear : public SpellScriptLoader
{
    public:
        spell_dru_rend_and_tear() : SpellScriptLoader("spell_dru_rend_and_tear") { }

        class spell_dru_rend_and_tear_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_rend_and_tear_AuraScript);

            enum eSpells
            {
                Trash = 192090
            };

            void OnAbsorb(AuraEffect* p_AurEff, DamageInfo& p_DmgInfo, uint32& p_AbsorbAmount)
            {
                Unit* l_Victim = p_DmgInfo.GetTarget();
                Unit* l_Attacker = p_DmgInfo.GetActor();

                if (l_Victim == nullptr || l_Attacker == nullptr)
                    return;

                if (l_Victim->GetShapeshiftForm() != FORM_BEAR_FORM)
                    return;

                Aura* l_Aura = l_Attacker->GetAura(eSpells::Trash, l_Victim->GetGUID());

                if (l_Aura == nullptr)
                {
                    p_AbsorbAmount = 0;
                    return;
                }

                AuraEffect* l_AuraEffect = p_AurEff->GetBase()->GetEffect(EFFECT_1);

                if (l_AuraEffect == nullptr)
                {
                    p_AbsorbAmount = 0;
                    return;
                }

                uint8 l_Pct = l_Aura->GetStackAmount() * l_AuraEffect->GetAmount();

                p_AbsorbAmount = CalculatePct(p_DmgInfo.GetAmount(), l_Pct);
            }

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                p_Amount = -1; ///< Initialize to infinite Absorb
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rend_and_tear_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_rend_and_tear_AuraScript::OnAbsorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_rend_and_tear_AuraScript();
        }
};

/// Last Update 7.0.3
/// Innervate - 29166
class spell_dru_innervate : public SpellScriptLoader
{
    public:
        spell_dru_innervate() : SpellScriptLoader("spell_dru_innervate") { }

        class spell_dru_innervate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_innervate_SpellScript);

            enum eSpells
            {
                RapidInnervationBuff    = 202842,
                RapidInnervation        = 202890
            };

            bool IsHealer(SpecIndex p_Spec)
            {
                switch (p_Spec)
                {
                    case SPEC_DRUID_RESTORATION:
                    case SPEC_PRIEST_HOLY:
                    case SPEC_PRIEST_DISCIPLINE:
                    case SPEC_PALADIN_HOLY:
                    case SPEC_SHAMAN_RESTORATION:
                    case SPEC_MONK_MISTWEAVER:
                        return true;
                        break;
                    default:
                        break;
                }
                return false;
            }

            SpellCastResult CheckTarget()
            {
                Unit* l_Target = GetExplTargetUnit();
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return SPELL_FAILED_DONT_REPORT;

                Player* l_PlayerTarget = l_Target->ToPlayer();

                if (l_PlayerTarget && !IsHealer((SpecIndex)l_PlayerTarget->GetActiveSpecializationID()))
                    return SPELL_FAILED_BAD_TARGETS;

                return SPELL_CAST_OK;
            }

            void HandleRapidInnervation()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                if (!l_Caster || !l_Target)
                    return;

                if (!l_Caster->HasAura(eSpells::RapidInnervation))
                    return;

                l_Target->CastSpell(l_Target, eSpells::RapidInnervationBuff, true);
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_innervate_SpellScript::CheckTarget);
                AfterCast += SpellCastFn(spell_dru_innervate_SpellScript::HandleRapidInnervation);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_innervate_SpellScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Solar Empowerment - 164545 and Lunar Empowerment - 164547
/// Starlord - 202345
class spell_dru_starlord : public SpellScriptLoader
{
    public:
        spell_dru_starlord() : SpellScriptLoader("spell_dru_starlord") { }

        class spell_dru_starlord_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_starlord_AuraScript);

            enum eSpells
            {
                SolarEmpowerment = 164545,
                LunarEmpowerment = 164547,
                Starlord        = 202345,
                StarlordSolar   = 202416,
                StarlordLunar   = 202423
            };

            uint64 m_Amount = 0;

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();

                if (!l_Target->HasAura(eSpells::Starlord))
                    return;

                if (GetSpellInfo()->Id == eSpells::SolarEmpowerment)
                    l_Target->CastSpell(l_Target, eSpells::StarlordSolar, true);
                else
                    l_Target->CastSpell(l_Target, eSpells::StarlordLunar, true);
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();

                if (GetSpellInfo()->Id == eSpells::SolarEmpowerment)
                    l_Target->RemoveAura(eSpells::StarlordSolar);
                else
                    l_Target->RemoveAura(eSpells::StarlordLunar);
            }

            void HandleOnApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (!m_Amount)
                    m_Amount = p_AurEff->GetAmount();
                else
                    const_cast<AuraEffect*>(p_AurEff)->SetAmount(m_Amount);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_starlord_AuraScript::HandleOnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                AfterEffectApply += AuraEffectApplyFn(spell_dru_starlord_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_starlord_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_starlord_AuraScript();
        }
};

/// Last Update 7.0.3
/// Mangle - 93622
class spell_dru_mangle : public SpellScriptLoader
{
    public:
        spell_dru_mangle() : SpellScriptLoader("spell_dru_mangle") { }

        class spell_dru_mangle_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_mangle_SpellScript);

            enum eSpells
            {
                Mangle          = 33917,
                EnragedMangle   = 202085
            };

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    l_Player->RemoveSpellCooldown(eSpells::Mangle, true);
                    l_Player->RemoveSpellCooldown(eSpells::EnragedMangle, true);
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_mangle_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_mangle_SpellScript();
        }
};

/// Last Update 7.0.3
/// Blessing of the Ancients - 202360
class spell_dru_blessing_of_the_ancients : public SpellScriptLoader
{
    public:
        spell_dru_blessing_of_the_ancients() : SpellScriptLoader("spell_dru_blessing_of_the_ancients") { }

        class spell_dru_blessing_of_the_ancients_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_blessing_of_the_ancients_SpellScript);

            enum eSpells
            {
                BlessingOfElune = 202737,
                BlessingOfAnshe = 202739
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::BlessingOfElune))
                {
                    l_Caster->RemoveAura(eSpells::BlessingOfElune);
                    l_Caster->CastSpell(l_Caster, eSpells::BlessingOfAnshe, true);
                }
                else
                {
                    l_Caster->RemoveAura(eSpells::BlessingOfAnshe);
                    l_Caster->CastSpell(l_Caster, eSpells::BlessingOfElune, true);
                }
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_blessing_of_the_ancients_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_blessing_of_the_ancients_SpellScript();
        }
};

class PlayerScript_blessing_of_the_ancients : public PlayerScript
{
    public:

        PlayerScript_blessing_of_the_ancients() :PlayerScript("PlayerScript_blessing_of_the_ancients") { }

        enum eSpells
        {
            AstralCommunion = 202359,
            ShootingStars   = 202342,
            BlessingOfElune = 202737,
            BlessingOfAnshe = 202739
        };

        void OnSpellLearned(Player* p_Player, uint32 p_SpellId) override
        {
            if (p_Player->GetActiveSpecializationID() == SPEC_DRUID_BALANCE && (p_SpellId == eSpells::ShootingStars || p_SpellId == eSpells::AstralCommunion))
            {
                p_Player->RemoveAura(eSpells::BlessingOfElune);
                p_Player->RemoveAura(eSpells::BlessingOfAnshe);
            }
        }
};

/// Last Update 7.1.5 Build 23420
/// Call by Lunar Strike - 194153 and Solar Wrath - 190984
/// Nature's Balance - 202430
class spell_dru_nature_balance : public SpellScriptLoader
{
    public:
        spell_dru_nature_balance() : SpellScriptLoader("spell_dru_nature_balance") { }

        class spell_dru_nature_balance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_nature_balance_SpellScript);

            enum eSpells
            {
                LunarStrike     = 194153,
                SolarWrath      = 190984,
                Moonfire        = 164812,
                Sunfire         = 164815,
                NatureBalance   = 202430,
                WarriorOfElune  = 202425,
                SunfireDamage   = 164815
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (!l_Caster->HasAura(eSpells::NatureBalance))
                    return;

                if (l_Target == nullptr)
                    return;

                if (GetSpellInfo()->Id == eSpells::LunarStrike)
                {
                    Aura* l_Aura = l_Target->GetAura(eSpells::Moonfire, l_Caster->GetGUID());
                    if (l_Aura == nullptr)
                        return;

                    int32 maxDuration = l_Aura->GetMaxDuration();
                    int32 newDuration = l_Aura->GetDuration() + (l_Aura->GetSpellInfo()->GetMaxDuration() / 3);

                    l_Aura->SetDuration(std::min(newDuration, maxDuration));
                }
                else if(GetSpellInfo()->Id == eSpells::SolarWrath)
                {
                    SpellInfo const* l_SunFireDamageInfo = sSpellMgr->GetSpellInfo(eSpells::SunfireDamage);
                    if (!l_SunFireDamageInfo)
                        return;

                    float l_Radius = l_SunFireDamageInfo->Effects[EFFECT_1].CalcRadius();

                    std::list<Unit*> l_TargetList;
                    SolarDamageTargetsCheck l_Check(l_Caster, l_Target, l_Radius);
                    JadeCore::UnitListSearcher<SolarDamageTargetsCheck> l_Searcher(l_Target, l_TargetList, l_Check);
                    l_Target->VisitNearbyObject(l_Radius, l_Searcher);

                    for (auto l_Target : l_TargetList)
                    {
                        if (Aura* l_Aura = l_Target->GetAura(eSpells::Sunfire, l_Caster->GetGUID()))
                        {
                            int32 maxDuration = l_Aura->GetMaxDuration();
                            int32 newDuration = l_Aura->GetDuration() + (l_Aura->GetSpellInfo()->GetMaxDuration() / 3);


                            l_Aura->SetDuration(std::min(newDuration, maxDuration));
                        }
                    }
                }
            }

            void HandleOnPrepare()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Aura* l_WarriorOfElune = l_Caster->GetAura(eSpells::WarriorOfElune);
                if (!l_WarriorOfElune)
                    return;

                if (GetSpell()->m_appliedMods.find(l_WarriorOfElune) != GetSpell()->m_appliedMods.end())
                    l_WarriorOfElune->DropStack();
            }

            void Register() override
            {
                if (m_scriptSpellId == eSpells::LunarStrike)
                    OnPrepare += SpellOnPrepareFn(spell_dru_nature_balance_SpellScript::HandleOnPrepare);
                OnHit += SpellHitFn(spell_dru_nature_balance_SpellScript::HandleOnHit);
            }

        private:

            class SolarDamageTargetsCheck
            {
                public:
                    SolarDamageTargetsCheck(Unit const* p_Caster, Unit const* p_Searcher, float range)
                        : i_Caster(p_Caster), i_Searcher(p_Searcher), i_range(range) {}

                    bool operator()(const Unit* u)
                    {
                        return u->isAlive()
                            && i_Searcher->IsWithinDistInMap(u, i_range)
                            && !i_Caster->IsFriendlyTo(u)
                            && i_Caster->IsValidAttackTarget(u)
                            && i_Caster->canSeeOrDetect(u)
                            && u->HasAura(eSpells::SunfireDamage, i_Caster->GetGUID());
                    }
                private:
                    Unit const* i_Caster;
                    Unit const* i_Searcher;
                    float i_range;
            };
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_nature_balance_SpellScript();
        }
};

/// Last Update 7.0.3 Build 22522
/// Called By Warrior Of Elune - 202425
class spell_dru_warrior_of_elune: public SpellScriptLoader
{
    public:
        spell_dru_warrior_of_elune() : SpellScriptLoader("spell_dru_warrior_of_elune") { }

        class spell_dru_warrior_of_elune_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_warrior_of_elune_SpellScript);

            enum eSpells
            {
                WarriorOfElune = 202425,
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Aura* l_WarriorOfElune = l_Caster->GetAura(eSpells::WarriorOfElune);
                if (!l_WarriorOfElune)
                    return;

                l_WarriorOfElune->SetStackAmount(3);
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_warrior_of_elune_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_warrior_of_elune_SpellScript();
        }
};

/// Last Update 7.0.3
/// Tranquility - 157982
class spell_dru_tranquility : public SpellScriptLoader
{
    public:
        spell_dru_tranquility() : SpellScriptLoader("spell_dru_tranquility") { }

        class spell_dru_tranquility_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_tranquility_SpellScript);

            enum eSpells
            {
                Tranquility         = 740,
                EssenceOfInfusion   = 208191
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::Tranquility);
                int32 l_HealAmount = GetHitHeal();
                if (!l_Caster || !l_Target || !l_SpellInfo || !l_HealAmount)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player)
                    return;

                if (!l_Player->GetGroup())
                {
                    AddPct(l_HealAmount, l_SpellInfo->Effects[EFFECT_2].BasePoints);
                    SetHitHeal(l_HealAmount);
                }

                if (l_Caster->HasAura(eSpells::EssenceOfInfusion))
                {
                    l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::EssenceOfInfusion);
                    if (!l_SpellInfo || l_Target->GetHealthPct() >= l_SpellInfo->Effects[EFFECT_0].BasePoints)
                        return;

                    AddPct(l_HealAmount, l_SpellInfo->Effects[EFFECT_1].BasePoints);
                    SetHitHeal(l_HealAmount);
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_tranquility_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_tranquility_SpellScript();
        }
};

/// Last Update 7.0.3
/// Ashamane's Frenzy (Artifact) - 210722
class spell_dru_ashamanes_frenzy  : public SpellScriptLoader
{
    public:
        spell_dru_ashamanes_frenzy() : SpellScriptLoader("spell_dru_ashamanes_frenzy") { }

        class spell_dru_ashamanes_frenzy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ashamanes_frenzy_AuraScript);

            enum eSpells
            {
                AshamanesFrenzyDamage = 210723
            };

            uint64 m_TargetGUID = 0;

            void OnTick(AuraEffect const* p_AurEff)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                Unit* l_Target = ObjectAccessor::FindUnit(m_TargetGUID);

                if (l_Target == nullptr)
                    return;

                l_Caster->CastSpell(l_Target, eSpells::AshamanesFrenzyDamage, true);
            }

            void SetGuid(uint32 index, uint64 guid) override
            {
                if (index == 0)
                    m_TargetGUID = guid;
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_ashamanes_frenzy_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        class spell_dru_ashamanes_frenzy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_ashamanes_frenzy_SpellScript);

            uint64 m_TargetGUID = 0;
            void HandleOnCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();

                if (l_Target == nullptr)
                    return;

                if (!l_Caster->IsValidAttackTarget(l_Target))
                    return;

                m_TargetGUID = l_Target->GetGUID();
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();

                Aura* l_Aura = l_Caster->GetAura(GetSpellInfo()->Id);
                if (l_Aura == nullptr)
                    return;

                l_Aura->SetScriptGuid(0, m_TargetGUID);
            }

            void Register() override
            {
                OnCast += SpellCastFn(spell_dru_ashamanes_frenzy_SpellScript::HandleOnCast);
                AfterHit += SpellHitFn(spell_dru_ashamanes_frenzy_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_ashamanes_frenzy_SpellScript();
        }

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_ashamanes_frenzy_AuraScript();
        }
};

/// Ashamane's Frenzy Damage - 210723
/// Updated as of 7.0.3 - 22522
class spell_dru_ashamanes_frenzy_dmg : public SpellScriptLoader
{
    public:
        spell_dru_ashamanes_frenzy_dmg() : SpellScriptLoader("spell_dru_ashamanes_frenzy_dmg") { }

        class spell_impl : public SpellScript
        {
            PrepareSpellScript(spell_impl);

            void HandleDamageAmount(SpellEffIndex /*effIndex*/)
            {
                SetHitDamage(GetHitDamage() * m_Multiplier);
            }

            void Register() override
            {
                OnEffectHit += SpellEffectFn(spell_impl::HandleDamageAmount, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_impl();
        }

        class aura_impl : public AuraScript
        {
            PrepareAuraScript(aura_impl);

            void HandleCalculateAmount(AuraEffect const* p_AurEff, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount *= m_Multiplier;
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(aura_impl::HandleCalculateAmount, EFFECT_2, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new aura_impl();
        }

    private:
        static uint8 const m_Multiplier = 15;
};

/// Last Update 7.0.3
/// Power of Goldrinn - 202996
class spell_dru_power_of_goldrinn : public SpellScriptLoader
{
    public:
        spell_dru_power_of_goldrinn() : SpellScriptLoader("spell_dru_power_of_goldrinn") { }

        class spell_dru_power_of_goldrinn_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_power_of_goldrinn_AuraScript);

            enum eSpells
            {
                StarSurge = 78674
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                Unit* l_Victim = p_EventInfo.GetDamageInfo()->GetTarget();

                if (l_Caster == nullptr || l_Victim == nullptr)
                    return;

                if (!p_EventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != eSpells::StarSurge)
                    return;

                l_Caster->CastSpell(l_Victim, p_AurEff->GetTriggerSpell(), true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_power_of_goldrinn_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_power_of_goldrinn_AuraScript();
        }
};

/// Last Update 7.1.5
/// Lunar Strike - 194153
class spell_druid_lunar_strike : public SpellScriptLoader
{
    public:
        spell_druid_lunar_strike() : SpellScriptLoader("spell_druid_lunar_strike") { }

        class spell_druid_lunar_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_druid_lunar_strike_SpellScript);

            enum eSpells
            {
                LunarEmpowerment    = 164547,
                OwlkinFrenzy        = 157228,
                PromiseOfElune      = 208283
            };

            uint64 m_MainTargetGUID = 0;
            uint32 m_StackAmount = 0;

            void HandleOnPrepare()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                l_Caster->RemoveAura(eSpells::OwlkinFrenzy);
            }

            void HandleOnCast()
            {
                Unit* l_MainTarget = GetExplTargetUnit();

                if (l_MainTarget == nullptr)
                    return;

                m_MainTargetGUID = l_MainTarget->GetGUID();
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Target->GetGUID() == m_MainTargetGUID)
                    return;
                float l_Coeff = (float)GetSpellInfo()->Effects[EFFECT_3].BasePoints / 100.0f;
                SetHitDamage(GetHitDamage() * l_Coeff);
            }

            void HandleBeforeHit()
            {
                Unit* l_Caster = GetCaster();
                Aura* l_Aura = l_Caster->GetAura(eSpells::LunarEmpowerment);
                if (l_Aura)
                    l_Aura->SetStackAmount(m_StackAmount);
            }

            void HandleBeforeCast()
            {
                Unit* l_Caster = GetCaster();
                Aura* l_Aura = l_Caster->GetAura(eSpells::LunarEmpowerment);

                if (l_Aura && (m_StackAmount = l_Aura->GetStackAmount()) >= 2)
                    l_Aura->SetStackAmount(1);
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Aura* l_Aura = l_Caster->GetAura(eSpells::LunarEmpowerment);

                if (l_Aura)
                    l_Aura->DropStack();
            }

            void Register() override
            {
                OnPrepare += SpellOnPrepareFn(spell_druid_lunar_strike_SpellScript::HandleOnPrepare);
                BeforeCast += SpellCastFn(spell_druid_lunar_strike_SpellScript::HandleBeforeCast);
                OnCast += SpellCastFn(spell_druid_lunar_strike_SpellScript::HandleOnCast);
                OnHit += SpellHitFn(spell_druid_lunar_strike_SpellScript::HandleOnHit);
                BeforeHit += SpellHitFn(spell_druid_lunar_strike_SpellScript::HandleBeforeHit);
                AfterHit += SpellHitFn(spell_druid_lunar_strike_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_druid_lunar_strike_SpellScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Wrath - 190984
class spell_dru_solar_wrath : public SpellScriptLoader
{
    public:
        spell_dru_solar_wrath() : SpellScriptLoader("spell_dru_solar_wrath") { }

        class spell_dru_solar_wrath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_solar_wrath_SpellScript);

            enum eSpells
            {
                SolarEmpowerment = 164545
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (Aura* l_Aura = l_Caster->GetAura(eSpells::SolarEmpowerment))
                    l_Aura->DropStack();
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_solar_wrath_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_solar_wrath_SpellScript();
        }
};

/// Last Update 7.0.3
/// Rage of the Sleeper - 200851
class spell_dru_rage_of_the_sleeper : public SpellScriptLoader
{
    public:
        spell_dru_rage_of_the_sleeper() : SpellScriptLoader("spell_dru_rage_of_the_sleeper") { }

        class spell_dru_rage_of_the_sleeper_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_rage_of_the_sleeper_AuraScript);

            enum eSpells
            {
                RageoftheSleeperDamage  = 219432,
                BloodMirror             = 221847,
                SpiritLinkDamage        = 204314
            };

            void OnAbsorb(AuraEffect* p_AurEff, DamageInfo& p_DmgInfo, uint32& p_AbsorbAmount)
            {
                p_AbsorbAmount = 0;

                Unit* l_Attacker = p_DmgInfo.GetActor();
                Unit* l_Victim = p_DmgInfo.GetTarget();
                uint32 l_Damage = p_DmgInfo.GetAmount();
                AuraEffect* l_AuraEffect = p_AurEff->GetBase()->GetEffect(EFFECT_0);

                if (l_AuraEffect == nullptr || l_Victim == nullptr || l_Attacker == nullptr)
                    return;

                p_AbsorbAmount = CalculatePct(l_Damage, l_AuraEffect->GetAmount());

                /// Prevent infinite loop
                if (p_DmgInfo.GetSpellInfo())
                {
                    switch (p_DmgInfo.GetSpellInfo()->Id)
                    {
                        case eSpells::RageoftheSleeperDamage:
                        case eSpells::BloodMirror:
                        case eSpells::SpiritLinkDamage:
                            return;
                        default:
                            break;
                    }
                }

                l_Victim->CastSpell(l_Attacker, eSpells::RageoftheSleeperDamage, true);
            }

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                p_Amount = -1; ///< Initialize to infinite Absorb
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_rage_of_the_sleeper_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_rage_of_the_sleeper_AuraScript::OnAbsorb, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        class spell_dru_rage_of_the_sleeper_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_rage_of_the_sleeper_SpellScript);

            enum eSpells
            {
                EmbraceoftheNightmare = 200855
            };

            void HandleEmbraceOfTheNightmare(SpellEffIndex p_EffIndex)
            {
                Unit* l_Target = GetHitUnit();
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                if (!l_SpellInfo || !l_Target || l_Target->HasAura(eSpells::EmbraceoftheNightmare))
                    return;

                Aura* l_RageOfTheSleeper = l_Target->GetAura(l_SpellInfo->Id);
                if (!l_RageOfTheSleeper)
                    return;

                AuraEffect* l_AuraEffect = l_RageOfTheSleeper->GetEffect(p_EffIndex);
                if (!l_AuraEffect)
                    return;

                l_AuraEffect->SetAmount(0);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_rage_of_the_sleeper_SpellScript::HandleEmbraceOfTheNightmare, EFFECT_2, SPELL_EFFECT_APPLY_AURA);
                OnEffectHitTarget += SpellEffectFn(spell_dru_rage_of_the_sleeper_SpellScript::HandleEmbraceOfTheNightmare, EFFECT_3, SPELL_EFFECT_APPLY_AURA);
                OnEffectHitTarget += SpellEffectFn(spell_dru_rage_of_the_sleeper_SpellScript::HandleEmbraceOfTheNightmare, EFFECT_4, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_rage_of_the_sleeper_SpellScript();
        }

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_rage_of_the_sleeper_AuraScript();
        }
};

struct DamageHistoryEntry
{
    DamageHistoryEntry(uint32 p_Damage, time_t p_Timestamp)
    {
        Damage = p_Damage;
        Timestamp = p_Timestamp;
    }

    uint32           Damage;
    uint32           Timestamp;
};

typedef std::list<DamageHistoryEntry>                DamagesHistory;
typedef std::map<uint64/*PlayerGUID*/, DamagesHistory> PlayerHistory;

class PlayerScript_Frenzied_Regenation : public PlayerScript
{
    private:

        const uint32 k_CycleInSecs = 10;
        static PlayerHistory m_HistoryDamagesPlayers;

    public:
        PlayerScript_Frenzied_Regenation() : PlayerScript("PlayerScript_Frenzied_Regenation") {}

        enum eSpells
        {
            FrenziedRegenerationAura = 22842
        };

        void OnLogout(Player* p_Player) override
        {
            if (m_HistoryDamagesPlayers.find(p_Player->GetGUID()) != m_HistoryDamagesPlayers.end())
                m_HistoryDamagesPlayers.erase(p_Player->GetGUID());
        }

        void OnTakeDamage(Player* p_Player, Unit*, DamageEffectType p_DamageEffectType, uint32 p_Damage, SpellSchoolMask p_SchoolMask, CleanDamage const* p_CleanDamage) override
        {
            if (p_Player->GetActiveSpecializationID() != SpecIndex::SPEC_DRUID_GUARDIAN &&
                !p_Player->HasAura(197491) && ///< Guardian Affinity
                !p_Player->HasAura(217615))   ///< Guardian Affinity
                return;

            if (p_DamageEffectType == DamageEffectType::SELF_DAMAGE
                || p_DamageEffectType == DamageEffectType::HEAL)
                return;

            auto& l_DamagesHistory = m_HistoryDamagesPlayers[p_Player->GetGUID()];
            l_DamagesHistory.push_back(DamageHistoryEntry(p_Damage, time(nullptr)));
        }

        void OnUpdate(Player* p_Player, uint32 p_Diff) override
        {
            if (p_Player->GetActiveSpecializationID() != SpecIndex::SPEC_DRUID_GUARDIAN &&
                !p_Player->HasAura(197491) && ///< Guardian Affinity
                !p_Player->HasAura(217615))   ///< Guardian Affinity
                return;

            auto& l_DamagesHistory = m_HistoryDamagesPlayers[p_Player->GetGUID()];

            /// - Remove old damage log (> 10 secs)
            uint32 l_Timestamp = time(nullptr);
            auto& l_History = m_HistoryDamagesPlayers[p_Player->GetGUID()];
            l_History.remove_if([&](DamageHistoryEntry const& p_DamageLog)
            {
                if ((p_DamageLog.Timestamp + k_CycleInSecs) < l_Timestamp)
                    return true;

                return false;
            });
        }

        static uint32 GetDamagesTakenInLastPeriod(uint64 p_PlayerGUID, uint32 p_Seconds)
        {
            auto& l_DamagesHistory = m_HistoryDamagesPlayers[p_PlayerGUID];
            uint32 l_Damages = 0;

            uint32 l_Timestamp = time(nullptr);

            for (auto l_Itr : l_DamagesHistory)
            {
                if (l_Timestamp < l_Itr.Timestamp + p_Seconds)
                    l_Damages += l_Itr.Damage;
            }

            return l_Damages;
        }
};

PlayerHistory PlayerScript_Frenzied_Regenation::m_HistoryDamagesPlayers;

/// last update : 7.0.3
/// Frenzied Regeneration - 22842
class spell_dru_frenzied_regeneration : public SpellScriptLoader
{
    public:
        spell_dru_frenzied_regeneration() : SpellScriptLoader("spell_dru_frenzied_regeneration") { }

        class spell_dru_frenzied_regeneration_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_frenzied_regeneration_AuraScript);

            enum eSpells
            {
                SoulOfTheForestResto = 114108
            };

            void HandleCalculateAmount(AuraEffect const* p_AurEff, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (l_Player == nullptr)
                    return;

                uint32 l_HealAmount = PlayerScript_Frenzied_Regenation::GetDamagesTakenInLastPeriod(l_Player->GetGUID(), GetSpellInfo()->Effects[EFFECT_2].BasePoints);
                l_HealAmount = std::max(uint64(l_HealAmount), CalculatePct(l_Player->GetMaxHealth(), GetSpellInfo()->Effects[EFFECT_3].BasePoints));
                p_Amount = l_HealAmount / p_AurEff->GetTotalTicks();
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_frenzied_regeneration_AuraScript::HandleCalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_frenzied_regeneration_AuraScript();
        }
};

/// Last Update 7.1.5
/// Call by Full Moon -  202771, Half Moon - 202768, New Moon - 202767
class spell_dru_moon_artifact : public SpellScriptLoader
{
    public:
        spell_dru_moon_artifact() : SpellScriptLoader("spell_dru_moon_artifact") { }

        class spell_dru_moon_artifact_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_moon_artifact_SpellScript);

            enum eSpells
            {
                NewMoon             = 202767,
                NewMoonOverride     = 202787,
                HalfMoon            = 202768,
                HalfMoonOverride    = 202788,
                FullMoon            = 202771
            };

            SpellCastResult CheckShapeshift()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->GetShapeshiftForm() == FORM_FLIGHT_FORM || l_Caster->GetShapeshiftForm() == FORM_FLIGHT_FORM_EPIC)
                    return SPELL_FAILED_NOT_SHAPESHIFT;

                return SPELL_CAST_OK;
            }

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                switch (GetSpellInfo()->Id)
                {
                    case eSpells::NewMoon:
                        l_Caster->CastSpell(l_Caster, eSpells::NewMoonOverride, true);
                        l_Caster->EnergizeBySpell(l_Caster, GetSpellInfo()->Id, GetSpellInfo()->Effects[EFFECT_2].BasePoints, (Powers)GetSpellInfo()->Effects[EFFECT_2].MiscValue);
                        break;
                    case eSpells::HalfMoon:
                        l_Caster->RemoveAura(eSpells::NewMoonOverride);
                        l_Caster->CastSpell(l_Caster, eSpells::HalfMoonOverride, true);
                        l_Caster->EnergizeBySpell(l_Caster, GetSpellInfo()->Id, GetSpellInfo()->Effects[EFFECT_2].BasePoints, (Powers)GetSpellInfo()->Effects[EFFECT_2].MiscValue);
                        break;
                    case eSpells::FullMoon:
                        l_Caster->RemoveAura(eSpells::NewMoonOverride);
                        l_Caster->RemoveAura(eSpells::HalfMoonOverride);
                        l_Caster->EnergizeBySpell(l_Caster, GetSpellInfo()->Id, GetSpellInfo()->Effects[EFFECT_1].BasePoints, (Powers)GetSpellInfo()->Effects[EFFECT_1].MiscValue);
                        break;
                }
            }

            void HandleAndPreventEffect(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_moon_artifact_SpellScript::CheckShapeshift);
                AfterCast += SpellCastFn(spell_dru_moon_artifact_SpellScript::HandleAfterCast);
                if (m_scriptSpellId == eSpells::NewMoon || m_scriptSpellId == eSpells::HalfMoon)
                {
                    OnEffectLaunch += SpellEffectFn(spell_dru_moon_artifact_SpellScript::HandleAndPreventEffect, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
                    OnEffectHitTarget += SpellEffectFn(spell_dru_moon_artifact_SpellScript::HandleAndPreventEffect, EFFECT_2, SPELL_EFFECT_ENERGIZE);
                }
                else if (m_scriptSpellId == eSpells::FullMoon)
                {
                    OnEffectHitTarget += SpellEffectFn(spell_dru_moon_artifact_SpellScript::HandleAndPreventEffect, EFFECT_1, SPELL_EFFECT_ENERGIZE);
                    OnEffectHitTarget += SpellEffectFn(spell_dru_moon_artifact_SpellScript::HandleAndPreventEffect, EFFECT_2, SPELL_EFFECT_REMOVE_AURA);
                    OnEffectHitTarget += SpellEffectFn(spell_dru_moon_artifact_SpellScript::HandleAndPreventEffect, EFFECT_3, SPELL_EFFECT_REMOVE_AURA);
                }
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_moon_artifact_SpellScript();
        }
};

/// Last Update 7.0.3
/// Dreamwalker - 189854
class spell_dru_dreamwalker_trigger : public SpellScriptLoader
{
    public:
        spell_dru_dreamwalker_trigger() : SpellScriptLoader("spell_dru_dreamwalker_trigger") { }

        class spell_dru_dreamwalker_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_dreamwalker_trigger_SpellScript);

            enum eSpells
            {
                Rejuvenation    = 774,
                DreamwalkerHeal = 189853
            };

            void HandleDummy(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Target->HasAura(eSpells::Rejuvenation))
                    l_Caster->CastSpell(l_Target, eSpells::DreamwalkerHeal, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_dreamwalker_trigger_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_dreamwalker_trigger_SpellScript();
        }
};

/// Fury of Elune - 202770
class spell_dru_fury_of_elune : public SpellScriptLoader
{
    public:
        spell_dru_fury_of_elune() : SpellScriptLoader("spell_dru_fury_of_elune") { }

        class spell_dru_fury_of_elune_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_fury_of_elune_AuraScript);

            enum eSpells
            {
                FuryOfEluneDmg = 211545
            };

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                AreaTrigger* l_AreaTrigger = l_Caster->GetAreaTrigger(GetSpellInfo()->Id);
                if (l_AreaTrigger == nullptr)
                    return;

                l_Caster->CastSpell(l_AreaTrigger->GetPositionX(), l_AreaTrigger->GetPositionY(), l_AreaTrigger->GetPositionZ(), eSpells::FuryOfEluneDmg, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                AreaTrigger* l_AreaTrigger = l_Caster->GetAreaTrigger(GetSpellInfo()->Id);
                if (l_AreaTrigger == nullptr)
                    return;

                l_AreaTrigger->Remove();
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_fury_of_elune_AuraScript::OnTick, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_fury_of_elune_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_fury_of_elune_AuraScript();
        }
};

/// Fury of Elune - 211547
class spell_dru_fury_of_elune_move : public SpellScriptLoader
{
    public:
        spell_dru_fury_of_elune_move() : SpellScriptLoader("spell_dru_fury_of_elune_move") { }

        class spell_dru_fury_of_elune_move_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_fury_of_elune_move_SpellScript);

            enum eSpells
            {
                FuryOfElune = 202770
            };

            void HandleMoveAreatrigger()
            {
                Unit* l_Caster = GetCaster();
                WorldLocation const* l_Dest = GetExplTargetDest();
                if (!l_Caster || !l_Dest)
                    return;

                AreaTrigger* l_AreaTrigger = l_Caster->GetAreaTrigger(eSpells::FuryOfElune);
                if (!l_AreaTrigger)
                    return;

                l_AreaTrigger->MoveAreaTrigger(l_Dest, 1);
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_fury_of_elune_move_SpellScript::HandleMoveAreatrigger);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_fury_of_elune_move_SpellScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Touch of the Moon Artifact Trait - 203018
class spell_dru_touch_of_the_moon : public SpellScriptLoader
{
    public:
        spell_dru_touch_of_the_moon() : SpellScriptLoader("spell_dru_touch_of_the_moon") { }

        class aura_impl : public AuraScript
        {
            PrepareAuraScript(aura_impl);

            enum eSpellIds
            {
                SPELL_DRUID_TOUCH_OF_THE_MOON_HEAL  = 213672
            };

            enum eArtifactPowerId
            {
                TouchOfTheMoon = 1035
            };

            void HandleOnProc(ProcEventInfo& /*p_ProcEventInfo*/)
            {
                if (!GetCaster())
                    return;

                PreventDefaultAction();
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->HasSpellCooldown(GetSpellInfo()->Id))
                        return;

                    if (uint8 l_Rank = l_Player->GetRankOfArtifactPowerId(eArtifactPowerId::TouchOfTheMoon))
                    {
                        int32 l_Pct = 5 * l_Rank; ///< give 5% for each rank

                        /// heal
                        if (roll_chance_i(l_Pct))
                        {
                            l_Player->CastSpell(l_Player, eSpellIds::SPELL_DRUID_TOUCH_OF_THE_MOON_HEAL, true);
                            l_Player->AddSpellCooldown(GetSpellInfo()->Id, 0, 20 * IN_MILLISECONDS);
                        }
                    }
                }
            }

            void Register() override
            {
                OnProc += AuraProcFn(aura_impl::HandleOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new aura_impl();
        }
};

/// Last Update 7.0.3 Build 22522
/// Called by Rip and Tear - 203242
class spell_dru_rip_and_tear : public SpellScriptLoader
{
    public:
        spell_dru_rip_and_tear() : SpellScriptLoader("spell_dru_rip_and_tear") { }

        class spell_dru_rip_and_tear_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_rip_and_tear_SpellScript);

            enum eSpells
            {
                Rake = 1822,
                Rip = 1079
            };

            void HandleAfterHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();

                if (!l_Player || !l_Target || l_Player->GetShapeshiftForm() != FORM_CAT_FORM)
                    return;

                l_Player->CastSpell(l_Target, eSpells::Rake, true);
                l_Player->CastSpell(l_Target, eSpells::Rip, true);
            }

            void Register() override
            {
                AfterHit += SpellHitFn(spell_dru_rip_and_tear_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_rip_and_tear_SpellScript();
        }
};

/// Last Update 7.0.3 Build 22522
/// Called by Prowl - 5215
/// Called For Primal Vitality - 202808
class spell_dru_primal_vitality : public SpellScriptLoader
{
    public:
        spell_dru_primal_vitality() : SpellScriptLoader("spell_dru_primal_vitality") { }

        class spell_dru_primal_vitality_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_primal_vitality_SpellScript);

            enum eSpells
            {
                PrimalVitality      = 202808,
                PrimalVitalityAura  = 202812
            };

            void HandleBeforeHit()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::PrimalVitality))
                    l_Caster->CastSpell(l_Caster, eSpells::PrimalVitalityAura, true);
            }

            void Register() override
            {
                BeforeHit += SpellHitFn(spell_dru_primal_vitality_SpellScript::HandleBeforeHit);
            }
        };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_primal_vitality_SpellScript();
    }
};

/// Gore - 210706
/// Updated as of 7.0.3 - 22522
class spell_dru_gore : public SpellScriptLoader
{
    public:
        spell_dru_gore() : SpellScriptLoader("spell_dru_gore") { }

        class spell_dru_gore_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_gore_AuraScript);

            enum eSpells
            {
                MangleBuff = 93622 ///< Mangle!
            };

            void OnProc(AuraEffect const* /*p_AuraEffect*/, ProcEventInfo& /*p_ProcEventInfo*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::MangleBuff, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_gore_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_gore_AuraScript();
        }
};

/// Last Update - 7.0.3
/// Overgrowth - 203651
class spell_dru_overgrowth : public SpellScriptLoader
{
    public:
        spell_dru_overgrowth() : SpellScriptLoader("spell_dru_overgrowth") { }

        class spell_dru_overgrowth_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_overgrowth_SpellScript);

            enum eSpells
            {
                Rejevunation            = 774,
                WildGrowth              = 48438,
                Lifebloom               = 33763,
                Regrowth                = 8936,
                RejuvenationGermination = 155777,
                Germination             = 155675
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();

                if (!l_Caster || !l_Target)
                    return;

                l_Caster->CastSpell(l_Target, eSpells::Rejevunation, true);
                l_Caster->CastSpell(l_Target, eSpells::WildGrowth, true);
                l_Caster->CastSpell(l_Target, eSpells::Lifebloom, true);
                l_Caster->CastSpell(l_Target, eSpells::Regrowth, true);
                if (l_Caster->HasAura(eSpells::Germination))
                    l_Caster->CastSpell(l_Target, eSpells::RejuvenationGermination, true);
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_overgrowth_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_overgrowth_SpellScript();
        }
};

/// Last Update 7.0.3 Build 22522
/// Called by Rake - 1822
/// Called for Pouncing Strikes - 202790
class spell_dru_pouncing_strikes : public SpellScriptLoader
{
    public:
        spell_dru_pouncing_strikes() : SpellScriptLoader("spell_dru_pouncing_strikes") { }

        class spell_dru_pouncing_strikes_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_pouncing_strikes_SpellScript);

            enum eSpells
            {
                PouncingStrikes             = 202790,
                PouncingStrikesChargeEffect = 202794
            };

            void HandleBeforeHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                if (!l_Caster || !l_Target || !l_Caster->HasAura(eSpells::PouncingStrikes))
                    return;

                l_Caster->CastSpell(l_Target, eSpells::PouncingStrikesChargeEffect, true);
            }

            void Register() override
            {
                BeforeHit += SpellHitFn(spell_dru_pouncing_strikes_SpellScript::HandleBeforeHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_pouncing_strikes_SpellScript();
        }
};

/// Last Update 7.1.5 Build 22522
/// Called by Bear Form - 5487 - Cat Form - 768 - Travel Form - 783
/// Called for Shapemender - 200308
class spell_dru_shapemender : public SpellScriptLoader
{
    public:
        spell_dru_shapemender() : SpellScriptLoader("spell_dru_shapemender") { }

        class spell_dru_shapemender_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_shapemender_AuraScript);

            enum eSpells
            {
                Shapemender         = 200308,
                ShapemenderEffect   = 200310,
                BearForm            = 5487,
                CatForm             = 768,
                TravelForm          = 783
            };

            void AfterApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* l_Player = GetCaster()->ToPlayer();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::ShapemenderEffect);
                SpellInfo const* l_SpellGCD = sSpellMgr->GetSpellInfo(eSpells::Shapemender);
                uint32 l_gcd = l_SpellGCD->Effects[EFFECT_1].BasePoints * 1000;
                if (!l_Player || !l_SpellInfo || !l_SpellGCD || !l_gcd)
                    return;

                if (l_Player->HasAura(eSpells::Shapemender))
                {
                    l_Player->CastSpell(l_Player, eSpells::ShapemenderEffect, true);
                    if (l_Player->HasSpellCooldown(eSpells::ShapemenderEffect))
                        return;
                    l_Player->AddSpellCooldown(l_SpellInfo->Id, 0, l_gcd);
                }
            }

            void Register() override
            {
                switch (m_scriptSpellId)
                {
                    case eSpells::TravelForm:
                        AfterEffectApply += AuraEffectApplyFn(spell_dru_shapemender_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                        break;
                    default:
                        AfterEffectApply += AuraEffectApplyFn(spell_dru_shapemender_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_shapemender_AuraScript();
        }
};

/// Called by Ironbark - 102342
/// Called for Fortified Bark - 203454
class spell_dru_fortified_bark : public SpellScriptLoader
{
    public:
        spell_dru_fortified_bark() : SpellScriptLoader("spell_dru_fortified_bark") { }

        class spell_dru_fortified_bark_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_fortified_bark_SpellScript);

            enum eSpells
            {
                FortifiedBark       = 203454,
                FortifiedBarkEffect = 203453
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();

                if (!l_Caster || !l_Target)
                    return;

                if (l_Caster->HasAura(eSpells::FortifiedBark))
                    l_Target->CastSpell(l_Target, eSpells::FortifiedBarkEffect, true);
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_fortified_bark_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_fortified_bark_SpellScript();
        }
};

/// Last Update 7.0.3 Build 22522
/// Called by Swiftmend - 18562 - Regrowth - 8936 - Wild Growth - 48438
/// Called for Soul of the Forest - 158478
class spell_dru_soul_of_the_forest_resto : public SpellScriptLoader
{
    public:
        spell_dru_soul_of_the_forest_resto() : SpellScriptLoader("spell_dru_soul_of_the_forest_resto") { }

        class spell_dru_soul_of_the_forest_resto_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_soul_of_the_forest_resto_SpellScript);

            enum eSpells
            {
                SoulOfTheForestResto        = 114108,
                SoulOfTheForestRestoTalent  = 158478,
                Swiftmend                   = 18562,
                Regrowth                    = 8936,
                WildGrowth                  = 48438
            };

            std::set<uint64> m_TargetGUIDs;

            void HandleSwiftmend()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster || !l_Caster->HasAura(eSpells::SoulOfTheForestRestoTalent))
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::SoulOfTheForestResto, false);
            }

            void HandleRegrowth()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster || !l_Caster->HasAura(eSpells::SoulOfTheForestResto))
                    return;

                int32 l_AmountPct = (sSpellMgr->GetSpellInfo(eSpells::SoulOfTheForestResto))->Effects[EFFECT_1].BasePoints;
                int32 l_HitHealAmount = GetHitHeal();
                if (l_AmountPct || l_HitHealAmount)
                {
                    int32 l_Heal = AddPct(l_HitHealAmount, l_AmountPct);
                    SetHitHeal(l_Heal);
                }
            }

            void HandleWildGrowth()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                Spell* l_Spell = GetSpell();
                if (!l_Caster || !l_Target || !l_Caster->HasAura(eSpells::SoulOfTheForestResto) || !l_Spell)
                    return;

                if (AuraEffect* l_AuraEffet = l_Target->GetAuraEffect(eSpells::WildGrowth, EFFECT_0))
                {
                    int32 l_AmountPct = (sSpellMgr->GetSpellInfo(eSpells::SoulOfTheForestResto))->Effects[EFFECT_2].BasePoints;
                    int32 l_HitHealAmount = l_AuraEffet->GetAmount();
                    l_AuraEffet->SetAmount(AddPct(l_HitHealAmount, l_AmountPct));
                }

                m_TargetGUIDs.insert(l_Target->GetGUID());

                if (m_TargetGUIDs.size() >= l_Spell->GetUnitTargetCount())
                    l_Caster->RemoveAurasDueToSpell(eSpells::SoulOfTheForestResto);
            }

            void Register() override
            {
                switch (m_scriptSpellId)
                {
                    case eSpells::WildGrowth:
                        AfterHit += SpellHitFn(spell_dru_soul_of_the_forest_resto_SpellScript::HandleWildGrowth);
                        break;
                    case eSpells::Swiftmend:
                        AfterHit += SpellHitFn(spell_dru_soul_of_the_forest_resto_SpellScript::HandleSwiftmend);
                        break;
                    case eSpells::Regrowth:
                        BeforeHit += SpellHitFn(spell_dru_soul_of_the_forest_resto_SpellScript::HandleRegrowth);
                        break;
                    default:
                        break;
                }
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_soul_of_the_forest_resto_SpellScript();
        }
};

/// Last Update 7.0.3 Build 22522
/// Called by Regrowth - 8936 - Rejuvenation - 774
/// Called for Soul of the Forest - 158478
class spell_dru_soul_of_the_forest_resto_aura : public SpellScriptLoader
{
    public:
        spell_dru_soul_of_the_forest_resto_aura() : SpellScriptLoader("spell_dru_soul_of_the_forest_resto_aura") { }

        class spell_dru_soul_of_the_forest_resto_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_soul_of_the_forest_resto_aura_AuraScript);

            enum eSpells
            {
                SoulOfTheForestResto        = 114108,
                SoulOfTheForestRestoTalent  = 158478,
                Regrowth                    = 8936,
                Rejuvenation                = 774
            };

            void HandleCalcAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();
                int32 l_AmountPct = (sSpellMgr->GetSpellInfo(eSpells::SoulOfTheForestResto))->Effects[EFFECT_1].BasePoints;
                if (!l_Caster || !l_Caster->HasAura(eSpells::SoulOfTheForestRestoTalent) || !l_AmountPct)
                    return;

                if (l_Caster->HasAura(eSpells::SoulOfTheForestResto))
                {
                    int32 l_Heal = AddPct(p_Amount, l_AmountPct);
                    p_Amount = l_Heal;
                    l_Caster->AddDelayedEvent([=]() -> void {l_Caster->RemoveAura(eSpells::SoulOfTheForestResto); }, 10);
                }
            }

            void Register() override
            {
                switch (m_scriptSpellId)
                {
                    case eSpells::Regrowth:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_soul_of_the_forest_resto_aura_AuraScript::HandleCalcAmount, EFFECT_1, SPELL_AURA_PERIODIC_HEAL);
                        break;
                    case eSpells::Rejuvenation:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_soul_of_the_forest_resto_aura_AuraScript::HandleCalcAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                        break;
                    default:
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_soul_of_the_forest_resto_aura_AuraScript();
        }
};

/// Last Update 7.0.3 Build 22522
/// Called by Growl - 6795
/// Called for Alpha Challenge - 207017
class spell_dru_alpha_challenge : public SpellScriptLoader
{
    public:
        spell_dru_alpha_challenge() : SpellScriptLoader("spell_dru_alpha_challenge") { }

        class spell_dru_alpha_challenge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_alpha_challenge_SpellScript);

            enum eSpells
            {
                AlphaChallenge  = 207017,
                Intimidated     = 206891,
                Growl           = 6795
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                if (!l_Caster || !l_Target || !l_Caster->HasAura(eSpells::AlphaChallenge))
                    return;

                l_Caster->CastSpell(l_Target, eSpells::Intimidated, true);
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_alpha_challenge_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_alpha_challenge_SpellScript();
        }
};

/// Last Update 7.0.3 Build 22522
/// Spell ID 209753
class spell_dru_cyclone : public SpellScriptLoader
{
public:
    spell_dru_cyclone() : SpellScriptLoader("spell_dru_cyclone") { }

    class spell_dru_cyclone_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dru_cyclone_SpellScript);

        enum eSpells
        {
            Cyclone = 33786
        };

        void HandleOnCast()
        {
            Unit *l_Caster = GetCaster();
            Unit *l_Target = GetExplTargetUnit();

            GetSpell()->cancel();
            l_Caster->CastSpell(l_Target, Cyclone, true);
        }

        void Register() override
        {
            BeforeCast += SpellCastFn(spell_dru_cyclone_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_dru_cyclone_SpellScript();
    }
};

/// Last Update 7.0.3 Build 22522
/// Called by Lifebloom - 33763
/// Called for Focused Growth - 203553
class spell_dru_focused_growth : public SpellScriptLoader
{
    public:
        spell_dru_focused_growth() : SpellScriptLoader("spell_dru_focused_growth") { }

        class spell_dru_focused_growth_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_focused_growth_AuraScript);

            enum eSpells
            {
                FocusedGrowth = 203553,
                FocusedGrowthEffect = 203554
            };

            void HandleOnEffectApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();
                if (!l_Caster || !l_Target || !l_Caster->HasAura(eSpells::FocusedGrowth, l_Caster->GetGUID()))
                    return;

                l_Caster->CastSpell(l_Target, eSpells::FocusedGrowthEffect, true);
            }

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();
                if (!l_Target)
                    return;

                l_Target->RemoveAurasDueToSpell(eSpells::FocusedGrowthEffect);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_focused_growth_AuraScript::HandleOnEffectApply, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_focused_growth_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_focused_growth_AuraScript();
        }
};

/// Last Update 7.0.3 Build 22522
/// Called by Rejuvenation - 774
/// Called for Revitalize - 203399
class spell_dru_revitalize : public SpellScriptLoader
{
    public:
        spell_dru_revitalize() : SpellScriptLoader("spell_dru_revitalize") { }

        class spell_dru_revitalize_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_revitalize_SpellScript);

            enum eSpells
            {
                Revitalize      = 203399,
                RevitalizeBuff  = 203407
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                if (!l_Caster || !l_Target || !l_Caster->HasAura(eSpells::Revitalize))
                    return;

                l_Caster->CastSpell(l_Target, eSpells::RevitalizeBuff, true);
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_revitalize_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_revitalize_SpellScript();
        }
};

/// Last Update 7.0.3 Build 22522
/// Called By Revitalize - 203407
class spell_dru_revitalize_proc : public SpellScriptLoader
{
    public:
        spell_dru_revitalize_proc() : SpellScriptLoader("spell_dru_revitalize_proc") { }

        class spell_dru_revitalize_proc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_revitalize_proc_AuraScript);

            enum eSpells
            {
                Rejuvenation    = 774,
                RevitalizeHeal  = 203406
            };

            void HandleOnProc(ProcEventInfo& p_ProcEventInfo)
            {
                DamageInfo* l_DamageInfo = p_ProcEventInfo.GetDamageInfo();
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();
                if (!l_DamageInfo || !l_Caster || !l_Target)
                    return;

                l_Caster->CastSpell(l_Target, eSpells::RevitalizeHeal, true);

                Aura* l_Aura = l_Target->GetAura(eSpells::Rejuvenation, l_Caster->GetGUID());
                if (!l_Aura)
                    return;

                l_Aura->RefreshDuration();
            }

            void Register() override
            {
                OnProc += AuraProcFn(spell_dru_revitalize_proc_AuraScript::HandleOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_revitalize_proc_AuraScript();
        }
};

/// Called by Druid Of The Claw - 209690
class spell_dru_druid_of_the_claw : public SpellScriptLoader
{
    public:
        spell_dru_druid_of_the_claw() : SpellScriptLoader("spell_dru_druid_of_the_claw") { }

        class spell_dru_druid_of_the_claw_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_druid_of_the_claw_AuraScript);

            enum eSpells
            {
                Rejuvenation    = 774,
                BearForm        = 5487
            };

            bool HandleOnProc(ProcEventInfo& /*p_ProcEventInfo*/)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                if (!l_Caster || !l_SpellInfo)
                    return false;

                if (!l_Caster->HasAura(eSpells::BearForm))
                    return false;

                if (!roll_chance_f(l_SpellInfo->ProcChance))
                    return false;

                l_Caster->DelayedCastSpell(l_Caster, eSpells::Rejuvenation, true, 1);
                return false;
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_druid_of_the_claw_AuraScript::HandleOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_druid_of_the_claw_AuraScript();
        }
};

class PlayerScript_Killer_Instinct : public PlayerScript
{
    public:
        PlayerScript_Killer_Instinct() : PlayerScript("PlayerScript_Killer_Instinct") {}

        enum eSpells
        {
            KillerInstinct = 108300
        };

        void OnModifySpec(Player* p_Player, int32 /*p_OldSpec*/, int32 p_NewSpec)
        {
            if (p_NewSpec == SPEC_DRUID_GUARDIAN || p_NewSpec == SPEC_DRUID_FERAL)
                p_Player->RemoveAurasDueToSpell(eSpells::KillerInstinct);
        }
};

/// Last Update 7.1.5 Build 23420
/// Blessings of the Ancients - 202360
class PlayerScript_dru_blessings_of_the_ancients : public PlayerScript
{
    public:
        PlayerScript_dru_blessings_of_the_ancients() : PlayerScript("PlayerScript_dru_blessings_of_the_ancients") {}

        enum eSpells
        {
            BlessingOfElune = 202737,
            BlessingOfAnshe = 202739
        };

        void OnSpellRemoved(Player* p_Player, uint32 p_SpellID) override
        {
            p_Player->RemoveAura(eSpells::BlessingOfElune);
            p_Player->RemoveAura(eSpells::BlessingOfAnshe);
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Moon and Stars - 202941
class spell_dru_moon_and_stars : public SpellScriptLoader
{
    public:
        spell_dru_moon_and_stars() : SpellScriptLoader("spell_dru_moon_and_stars") { }

        class spell_dru_moon_and_stars_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_moon_and_stars_AuraScript);

            enum eSpells
            {
                IncarnationChosenOfElune    = 102560,
                CelestialAlignment          = 194223,
                FuryOfElune                 = 211545,
                StarPower                   = 202942
            };

            Guid128 m_LastProcGUID;

            bool HandleCheckProc(ProcEventInfo& p_ProcEventInfo)
            {
                Unit* l_Caster = GetCaster();
                DamageInfo* l_DamageInfo = p_ProcEventInfo.GetDamageInfo();
                if (!l_Caster || !l_DamageInfo)
                    return false;

                SpellInfo const* l_SpellInfo = l_DamageInfo->GetSpellInfo();
                Spell const* l_Spell = l_DamageInfo->GetSpell();
                if (!l_SpellInfo || !l_Spell)
                    return false;

                if (l_Spell->getTriggerCastFlags() != TriggerCastFlags::TRIGGERED_NONE)
                    return false;

                Guid128 l_CastGUID = l_Spell->GetCastGuid();
                if (l_CastGUID == m_LastProcGUID)
                    return false;

                m_LastProcGUID = l_CastGUID;

                switch (l_DamageInfo->GetDamageType())
                {
                    case DamageEffectType::DIRECT_DAMAGE:
                    case DamageEffectType::SPELL_DIRECT_DAMAGE:
                        break;
                    default:
                        return false;
                }

                if ((!l_Caster->HasAura(eSpells::IncarnationChosenOfElune) && !l_Caster->HasAura(eSpells::CelestialAlignment)) || l_SpellInfo->Id == eSpells::FuryOfElune)
                    return false;

                return true;
            }

            void HandleTriggerSpell(AuraEffect const*, ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster && l_Caster->HasAura(eSpells::IncarnationChosenOfElune))
                    PreventDefaultAction();
            }

            void HandleOnProc(ProcEventInfo& p_ProcEventInfo)
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster || !l_Caster->HasAura(eSpells::IncarnationChosenOfElune))
                    return;

                int32 l_HastIncrease = 1;
                l_Caster->CastCustomSpell(l_Caster, eSpells::StarPower, &l_HastIncrease, nullptr, nullptr, true);
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_moon_and_stars_AuraScript::HandleCheckProc);
                OnEffectProc += AuraEffectProcFn(spell_dru_moon_and_stars_AuraScript::HandleTriggerSpell, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
                OnProc += AuraProcFn(spell_dru_moon_and_stars_AuraScript::HandleOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_moon_and_stars_AuraScript();
        }
};

///Legion 7.1.5 - build 23420
///Called by  clearcasting  16870 135700
class spell_dru_clearcasting : public SpellScriptLoader
{
    public:
        spell_dru_clearcasting() : SpellScriptLoader("spell_dru_clearcasting") { }

        enum eSpells
        {
            MomentOfClarity = 155577,
            MomentOfClarityFeral = 236068,
            ClearCasting = 16870,
            ClearCastingFeral = 135700
        };

        class spell_dru_clearcasting_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_clearcasting_SpellScript);

            void HandleBeforeCast()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (l_Caster->HasAura(eSpells::MomentOfClarity) || l_Caster->HasAura(eSpells::MomentOfClarityFeral))
                {
                    GetSpell()->SetSpellValue(SpellValueMod::SPELLVALUE_AURA_STACK, 3);
                }
            }

            void Register() override
            {
                BeforeCast += SpellCastFn(spell_dru_clearcasting_SpellScript::HandleBeforeCast);
            }
        };

        class spell_dru_clearcasting_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_clearcasting_AuraScript);

            void HandleCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
            {
                // HACK: spellmods should have value with 1 stack if the aura has 3 stacks
                if (GetStackAmount() > 1 && spellMod->value != 0)
                {
                    spellMod->value /= GetStackAmount();
                }
            }

            void Register() override
            {
                if (m_scriptSpellId == eSpells::ClearCasting)
                {
                    DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_dru_clearcasting_AuraScript::HandleCalcSpellMod, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER);
                    DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_dru_clearcasting_AuraScript::HandleCalcSpellMod, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER);
                }
                else if (m_scriptSpellId == eSpells::ClearCastingFeral)
                {
                    DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_dru_clearcasting_AuraScript::HandleCalcSpellMod, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER);
                    DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_dru_clearcasting_AuraScript::HandleCalcSpellMod, EFFECT_2, SPELL_AURA_ADD_PCT_MODIFIER);
                    DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_dru_clearcasting_AuraScript::HandleCalcSpellMod, EFFECT_3, SPELL_AURA_ADD_PCT_MODIFIER);
                }
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_clearcasting_SpellScript();
        }

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_clearcasting_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Moonkin Aura - 209740
class spell_dru_moonkin_aura : public SpellScriptLoader
{
    public:
        spell_dru_moonkin_aura() : SpellScriptLoader("spell_dru_moonkin_aura") { }

        class spell_dru_moonkin_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_moonkin_aura_AuraScript);

            enum eSpells
            {
                Starsurge = 78674
            };

            bool HandleCheckProc(ProcEventInfo& p_ProcEventInfo)
            {
                DamageInfo* l_DamageInfo = p_ProcEventInfo.GetDamageInfo();
                if (!l_DamageInfo)
                    return false;

                SpellInfo const* l_SpellInfo = l_DamageInfo->GetSpellInfo();
                if (!l_SpellInfo || l_SpellInfo->Id != eSpells::Starsurge)
                    return false;

                return true;
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_moonkin_aura_AuraScript::HandleCheckProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_moonkin_aura_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Moonkin Aura - 209746
class spell_dru_moonkin_aura_buff : public SpellScriptLoader
{
    public:
        spell_dru_moonkin_aura_buff() : SpellScriptLoader("spell_dru_moonkin_aura_buff") { }

        class spell_dru_moonkin_aura_buff_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_moonkin_aura_buff_SpellScript);

            enum eSpells
            {
                MoonkinAuraBuff = 209746
            };

            void HandleBeforeHit()
            {
                Unit* l_Target = GetHitUnit();
                if (!l_Target)
                    return;

                l_Target->RemoveAura(eSpells::MoonkinAuraBuff);
            }

            void Register() override
            {
                BeforeHit += SpellHitFn(spell_dru_moonkin_aura_buff_SpellScript::HandleBeforeHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_moonkin_aura_buff_SpellScript();
        }
};

/// Last Update 7.1.5 - 23420
/// Called by Den Mother - 236181
class spell_dru_den_mother_check : public SpellScriptLoader
{
    public:
        spell_dru_den_mother_check() : SpellScriptLoader("spell_dru_den_mother_check") { }

        class spell_dru_den_mother_check_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_den_mother_check_AuraScript);

            enum eSpells
            {
                DenMother       = 236180,
                DenMotherBuff   = 236181
            };

            bool Load() override
            {
                if (SpellInfo const* l_spellInfo = sSpellMgr->GetSpellInfo(eSpells::DenMother))
                    m_Radius = (float)l_spellInfo->Effects[EFFECT_0].BasePoints;

                return true;
            }

            void HandleUpdate(uint32 p_Diff)
            {
                if (m_UpdateTimer <= p_Diff)
                {
                    m_UpdateTimer = 5 * IN_MILLISECONDS;

                    Unit* l_Caster = GetCaster();
                    Unit* l_Target = GetUnitOwner();

                    if (!l_Target)
                        return;

                    if (!l_Caster)
                    {
                        Remove();
                        return;
                    }

                    if (!l_Caster->HasAura(eSpells::DenMother) || !l_Caster->IsWithinDistInMap(l_Target, m_Radius) || !l_Caster->IsValidAssistTarget(l_Target))
                    {
                        if (AreaTrigger* l_AreaTrigger = l_Caster->GetAreaTrigger(eSpells::DenMother))
                            l_AreaTrigger->GetAffectedPlayers()->erase(l_Target->GetGUID());

                        Remove();
                    }
                }
                else
                {
                    m_UpdateTimer -= p_Diff;
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_dru_den_mother_check_AuraScript::HandleUpdate);
            }

        private:

            float m_Radius = 15.f;
            uint32 m_UpdateTimer = 5 * IN_MILLISECONDS;
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_den_mother_check_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Overrun - 202249
class spell_dru_overrun : public SpellScriptLoader
{
    public:
        spell_dru_overrun() : SpellScriptLoader("spell_dru_overrun") { }

        class spell_dru_overrun_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_overrun_SpellScript);

            enum eSpells
            {
                OverrunStun = 202244
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                if (!l_Caster || !l_Target)
                    return;

                l_Caster->CastSpell(l_Target, eSpells::OverrunStun, true);
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_overrun_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_overrun_SpellScript();
        }
};

/// Called by Skysec's Hold - 208219
/// Called for Skysec's Hold (item) - 137025
class spell_dru_skysecs_hold : public SpellScriptLoader
{
    public:
        spell_dru_skysecs_hold() : SpellScriptLoader("spell_dru_skysecs_hold") { }

        class spell_dru_skysecs_hold_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_skysecs_hold_AuraScript);

            enum eSpells
            {
                FrenziedRegeneration = 22842
            };

            bool HandleCheckProc(ProcEventInfo& p_ProcEventInfo)
            {
                DamageInfo* l_DamageInfo = p_ProcEventInfo.GetDamageInfo();
                if (!l_DamageInfo)
                    return false;

                SpellInfo const* l_SpellInfo = l_DamageInfo->GetSpellInfo();
                if (!l_SpellInfo || l_SpellInfo->Id != eSpells::FrenziedRegeneration)
                    return false;

                return true;
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_skysecs_hold_AuraScript::HandleCheckProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_skysecs_hold_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by The Emerald Dreamcatcher - 208190
/// Called for The Emerald Dreamcatcher (item) - 137062
class spell_dru_the_emerald_dreamcatcher : public SpellScriptLoader
{
    public:
        spell_dru_the_emerald_dreamcatcher() : SpellScriptLoader("spell_dru_the_emerald_dreamcatcher") { }

        class spell_dru_the_emerald_dreamcatcher_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_the_emerald_dreamcatcher_AuraScript);

            enum eSpells
            {
                Starsurge = 78674
            };

            bool HandleCheckProc(ProcEventInfo& p_ProcEventInfo)
            {
                DamageInfo* l_DamageInfo = p_ProcEventInfo.GetDamageInfo();
                if (!l_DamageInfo)
                    return false;

                SpellInfo const* l_SpellInfo = l_DamageInfo->GetSpellInfo();
                if (!l_SpellInfo || l_SpellInfo->Id != eSpells::Starsurge)
                    return false;

                return true;
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_the_emerald_dreamcatcher_AuraScript::HandleCheckProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_the_emerald_dreamcatcher_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Oneth's Intuition - 209405
/// Called for Oneth's Intuition (item) - 137092
class spell_dru_oneths_intuition : public SpellScriptLoader
{
    public:
        spell_dru_oneths_intuition() : SpellScriptLoader("spell_dru_oneths_intuition") { }

        class spell_dru_oneths_intuition_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_oneths_intuition_AuraScript);

            enum eSpells
            {
                Starsurge                   = 78674,
                Starfall                    = 191034,
                OnethsIntuitionStarfall     = 209407,
                OnethsIntuitionStarsurge    = 209406
            };

            bool HandleCheckProc(ProcEventInfo& p_ProcEventInfo)
            {
                DamageInfo* l_DamageInfo = p_ProcEventInfo.GetDamageInfo();
                if (!l_DamageInfo)
                    return false;

                SpellInfo const* l_SpellInfo = l_DamageInfo->GetSpellInfo();
                if (!l_SpellInfo || (l_SpellInfo->Id != eSpells::Starsurge && l_SpellInfo->Id != eSpells::Starfall))
                    return false;

                return true;
            }

            void HandleOnProc(ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                DamageInfo* l_DamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_Caster || !l_DamageInfo)
                    return;

                SpellInfo const* l_SpellInfo = l_DamageInfo->GetSpellInfo();
                if (!l_SpellInfo)
                    return;

                if (l_SpellInfo->Id == eSpells::Starsurge)
                    l_Caster->CastSpell(l_Caster, eSpells::OnethsIntuitionStarfall, true);
                else
                    l_Caster->CastSpell(l_Caster, eSpells::OnethsIntuitionStarsurge, true);
            }

            void HandleAfterEffectRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                l_Caster->RemoveAura(eSpells::OnethsIntuitionStarfall);
                l_Caster->RemoveAura(eSpells::OnethsIntuitionStarsurge);
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_oneths_intuition_AuraScript::HandleCheckProc);
                OnProc += AuraProcFn(spell_dru_oneths_intuition_AuraScript::HandleOnProc);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_oneths_intuition_AuraScript::HandleAfterEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_oneths_intuition_AuraScript();
        }
};

/// Called by Impeccable Fel Essence - 208199
class spell_dru_impeccable_fel_essence : public SpellScriptLoader
{
    public:
        spell_dru_impeccable_fel_essence() : SpellScriptLoader("spell_dru_impeccable_fel_essence") { }

        class spell_dru_impeccable_fel_essence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_impeccable_fel_essence_AuraScript);

            enum eSpells
            {
                Incarnation         = 102560,
                CelestialAlignment  = 194223,
                FuryOfEluneTick     = 211545,
                FuryOfElune         = 202770
            };

            uint32 m_PowerCost = 0;

            void HandleOnProc(ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                SpellInfo const* l_FuryOfElune = sSpellMgr->GetSpellInfo(eSpells::FuryOfElune);
                DamageInfo *l_DamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_Caster || !l_SpellInfo || !l_DamageInfo || !l_FuryOfElune)
                    return;

                SpellInfo const* l_ProcSpellInfo = l_DamageInfo->GetSpellInfo();
                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player || !l_ProcSpellInfo)
                    return;

                SpellPowerEntry const* l_SpellPower = (l_ProcSpellInfo->Id == eSpells::FuryOfEluneTick ? l_FuryOfElune : l_ProcSpellInfo)->GetSpellPower(Powers::POWER_LUNAR_POWER);
                if (!l_SpellPower || (l_ProcSpellInfo->Id != eSpells::FuryOfEluneTick && l_ProcSpellInfo->Id != eSpells::FuryOfElune))
                    return;

                uint32 l_PowerCost = l_SpellPower->Cost;
                if (l_ProcSpellInfo->Id == eSpells::FuryOfEluneTick)
                    l_PowerCost = l_SpellPower->CostPerSecond;

                m_PowerCost = l_Caster->m_SpellHelper.GetUint32(eSpellHelpers::FelEssence);
                m_PowerCost += (l_PowerCost / l_Player->GetPowerCoeff(Powers::POWER_LUNAR_POWER));
                while (m_PowerCost > l_SpellInfo->Effects[EFFECT_1].BasePoints)
                {
                    l_Player->ReduceSpellCooldown(l_Player->HasSpell(eSpells::Incarnation) ? eSpells::Incarnation : eSpells::CelestialAlignment, l_SpellInfo->Effects[EFFECT_0].BasePoints * IN_MILLISECONDS);
                    m_PowerCost -= l_SpellInfo->Effects[EFFECT_1].BasePoints;
                }
                l_Caster->m_SpellHelper.GetUint32(eSpellHelpers::FelEssence) = m_PowerCost;
            }

            void Register() override
            {
                OnProc += AuraProcFn(spell_dru_impeccable_fel_essence_AuraScript::HandleOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_impeccable_fel_essence_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called for Impeccable Fel Essence - 208199
/// Called for Impeccable Fel Essence - Starsurge 78674 - Starfall 191034 - StellarFlare 202347
class spell_dru_impeccable_fel_essence_reset : public SpellScriptLoader
{
    public:
        spell_dru_impeccable_fel_essence_reset() : SpellScriptLoader("spell_dru_impeccable_fel_essence_reset") { }

        class spell_dru_impeccable_fel_essence_reset_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_impeccable_fel_essence_reset_SpellScript);

            enum eSpells
            {
                Incarnation         = 102560,
                CelestialAlignment  = 194223,
                FelEssence          = 208199
            };

            uint32 m_PowerCost = 0;

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                if (!l_Caster || !l_SpellInfo || !l_Caster->HasAura(eSpells::FelEssence))
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player)
                    return;

                SpellPowerEntry const* l_SpellPower = l_SpellInfo->GetSpellPower(Powers::POWER_LUNAR_POWER);
                SpellInfo const* l_FelEssence = sSpellMgr->GetSpellInfo(eSpells::FelEssence);
                if (!l_SpellPower || !l_FelEssence)
                    return;

                m_PowerCost = l_Caster->m_SpellHelper.GetUint32(eSpellHelpers::FelEssence);
                uint32 l_PowerCost = l_SpellPower->Cost;
                m_PowerCost += (l_PowerCost / l_Player->GetPowerCoeff(Powers::POWER_LUNAR_POWER));
                while (m_PowerCost > l_FelEssence->Effects[EFFECT_1].BasePoints)
                {
                    l_Player->ReduceSpellCooldown(l_Player->HasSpell(eSpells::Incarnation) ? eSpells::Incarnation : eSpells::CelestialAlignment, l_FelEssence->Effects[EFFECT_0].BasePoints * IN_MILLISECONDS);
                    m_PowerCost -= l_FelEssence->Effects[EFFECT_1].BasePoints;
                }
                l_Caster->m_SpellHelper.GetUint32(eSpellHelpers::FelEssence) = m_PowerCost;
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_impeccable_fel_essence_reset_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_impeccable_fel_essence_reset_SpellScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Starfall - 191034
class spell_dru_starfall : public SpellScriptLoader
{
    public:
        spell_dru_starfall() : SpellScriptLoader("spell_dru_starfall") { }

        class spell_dru_starfall_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_starfall_SpellScript);

            enum eSpells
            {
                OnethsIntuitionStarfall = 209407,
                ImpeccableFelEssence    = 208199,
                Incarnation             = 102560,
                CelestialAlignment      = 194223
            };

            uint32 m_PowerCost = 0;

            void HandleOnTakePower(Powers p_PowerType, int32& p_PowerCost)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster || !l_Caster->HasAura(eSpells::OnethsIntuitionStarfall) || p_PowerType != Powers::POWER_LUNAR_POWER)
                    return;

                l_Caster->RemoveAura(eSpells::OnethsIntuitionStarfall);
                p_PowerCost = 0;
            }

            void Register() override
            {
                OnTakePowers += SpellTakePowersFn(spell_dru_starfall_SpellScript::HandleOnTakePower);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_starfall_SpellScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Starsurge - 78674
class spell_dru_starsurge : public SpellScriptLoader
{
    public:
        spell_dru_starsurge() : SpellScriptLoader("spell_dru_starsurge") { }

        class spell_dru_starsurge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_starsurge_SpellScript);

            enum eSpells
            {
                OnethsIntuitionStarsurge = 209406
            };


            void HandleOnTakePower(Powers p_PowerType, int32& p_PowerCost)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster || !l_Caster->HasAura(eSpells::OnethsIntuitionStarsurge) || p_PowerType != Powers::POWER_LUNAR_POWER)
                    return;

                l_Caster->RemoveAura(eSpells::OnethsIntuitionStarsurge);
                p_PowerCost = 0;
            }

            void Register() override
            {
                OnTakePowers += SpellTakePowersFn(spell_dru_starsurge_SpellScript::HandleOnTakePower);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_starsurge_SpellScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by The Wildshaper's Clutch - 208319
class spell_dru_the_wildshapers_clutch : public SpellScriptLoader
{
    public:
        spell_dru_the_wildshapers_clutch() : SpellScriptLoader("spell_dru_the_wildshapers_clutch") { }

        class spell_dru_the_wildshapers_clutch_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_the_wildshapers_clutch_AuraScript);

            bool HandleCheckProc(ProcEventInfo& p_ProcEventInfo)
            {
                DamageInfo* l_DamageInfo = p_ProcEventInfo.GetDamageInfo();
                if (!l_DamageInfo)
                    return false;

                SpellInfo const* l_SpellInfo = l_DamageInfo->GetSpellInfo();
                if (!l_SpellInfo || l_SpellInfo->Mechanic != Mechanics::MECHANIC_BLEED || !(p_ProcEventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT))
                    return false;

                return true;
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_the_wildshapers_clutch_AuraScript::HandleCheckProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_the_wildshapers_clutch_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Promise of Elune, the Moon Goddess - 208283
class spell_dru_promise_of_elune : public SpellScriptLoader
{
    public:
        spell_dru_promise_of_elune() : SpellScriptLoader("spell_dru_promise_of_elune") { }

        class spell_dru_promise_of_elune_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_promise_of_elune_AuraScript);

            enum eSpells
            {
                SolarWrath  = 190984,
                LunarStrike = 194153
            };

            Guid128 m_LastProcGUID;

            bool CheckOnProc(ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                DamageInfo* l_ProcDamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_ProcDamageInfo || !l_Caster)
                    return false;

                SpellInfo const* l_ProcSpellInfo = l_ProcDamageInfo->GetSpellInfo();
                Spell const* l_Spell = l_ProcDamageInfo->GetSpell();
                if (!l_ProcSpellInfo || !l_Spell || (l_ProcSpellInfo->Id != eSpells::SolarWrath && l_ProcSpellInfo->Id != eSpells::LunarStrike))
                    return false;

                Guid128 l_CastGUID = l_Spell->GetCastGuid();
                if (l_CastGUID == m_LastProcGUID)
                    return false;

                m_LastProcGUID = l_CastGUID;
                return true;
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_promise_of_elune_AuraScript::CheckOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_promise_of_elune_AuraScript();
        }
};

/// Called by Aman'Thul's Wisdom - 208220
/// Called for Aman'Thul's Wisdom (item) - 137072
class spell_dru_amanthuls_wisdom : public SpellScriptLoader
{
    public:
        spell_dru_amanthuls_wisdom() : SpellScriptLoader("spell_dru_amanthuls_wisdom") { }

        class spell_dru_amanthuls_wisdom_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_amanthuls_wisdom_AuraScript);

            enum eSpells
            {
                Rejuvenation              = 774,
                Germination               = 155777,
                AmanthulsWisdomTargetAura = 214062
            };

            bool HandleCheckProc(ProcEventInfo& p_ProcEventInfo)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();
                DamageInfo* l_DamageInfo = p_ProcEventInfo.GetDamageInfo();
                if (!l_Caster || !l_Target || !l_DamageInfo)
                    return false;

                SpellInfo const* l_SpellInfo = l_DamageInfo->GetSpellInfo();
                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player || !l_SpellInfo || l_SpellInfo->Id != eSpells::Rejuvenation || l_Player->GetActiveSpecializationID() != SPEC_DRUID_RESTORATION)
                    return false;

                if (!l_Target->IsFullHealth())
                    return false;

                if (l_Target->HasAura(eSpells::Germination))
                    return false;

                return true;
            }

            void HandleOnProc(ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = p_EventInfo.GetProcTarget();
                if (!l_Caster || !l_Target)
                    return;

                l_Caster->CastSpell(l_Target, eSpells::AmanthulsWisdomTargetAura, true);
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_amanthuls_wisdom_AuraScript::HandleCheckProc);
                OnProc += AuraProcFn(spell_dru_amanthuls_wisdom_AuraScript::HandleOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_amanthuls_wisdom_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Promise of Elune, the Moon Goddess - 208284
class spell_dru_promise_of_elune_healing : public SpellScriptLoader
{
    public:
        spell_dru_promise_of_elune_healing() : SpellScriptLoader("spell_dru_promise_of_elune_healing") { }

        class spell_dru_promise_of_elune_healing_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_promise_of_elune_healing_AuraScript);

            enum eSpells
            {
                Regrowth        = 8936,
                PromiseofElune  = 208284
            };

            bool CheckOnProc(ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                DamageInfo* l_ProcDamageInfo = p_EventInfo.GetDamageInfo();
                Aura* l_Aura = GetAura();
                if (!l_ProcDamageInfo || !l_Caster || !l_Aura)
                    return false;

                SpellInfo const* l_ProcSpellInfo = l_ProcDamageInfo->GetSpellInfo();
                if (!l_ProcSpellInfo && l_ProcSpellInfo->Id != eSpells::Regrowth)
                    return false;

                return true;
            }

            void HandleOnProc(ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                Aura* l_Aura = GetAura();

                if (!l_Caster || !l_Aura)
                    return;

                l_Caster->RemoveAura(l_Aura);
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_promise_of_elune_healing_AuraScript::CheckOnProc);
                OnProc += AuraProcFn(spell_dru_promise_of_elune_healing_AuraScript::HandleOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_promise_of_elune_healing_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Aman'Thul's Wisdom - 214062
/// Called for Aman'Thul's Wisdom (item) - 137072
class spell_dru_amanthuls_wisdom_target_aura : public SpellScriptLoader
{
    public:
        spell_dru_amanthuls_wisdom_target_aura() : SpellScriptLoader("spell_dru_amanthuls_wisdom_target_aura") { }

        class spell_dru_amanthuls_wisdom_target_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_amanthuls_wisdom_target_aura_AuraScript);

            enum eSpells
            {
                AmanthulsWisdomTargetAura = 214062
            };

            bool HandleCheckProc(ProcEventInfo& p_ProcEventInfo)
            {
                DamageInfo* l_DamageInfo = p_ProcEventInfo.GetDamageInfo();
                if (!l_DamageInfo || l_DamageInfo->GetAmount() == 0)
                    return false;

                return true;
            }

            void HandleOnProc(ProcEventInfo& p_EventInfo)
            {
                Unit* l_Owner = GetUnitOwner();
                if (!l_Owner)
                    return;

                l_Owner->RemoveAura(eSpells::AmanthulsWisdomTargetAura);
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_amanthuls_wisdom_target_aura_AuraScript::HandleCheckProc);
                OnProc += AuraProcFn(spell_dru_amanthuls_wisdom_target_aura_AuraScript::HandleOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_amanthuls_wisdom_target_aura_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Oakheart's Puny Quods- 236478
class spell_dru_oakhearts_puny_quods : public SpellScriptLoader
{
    public:
        spell_dru_oakhearts_puny_quods() : SpellScriptLoader("spell_dru_oakhearts_puny_quods") { }

        class spell_dru_oakhearts_puny_quods_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_oakhearts_puny_quods_AuraScript);

            enum eSpells
            {
                Barkskin = 22812
            };

            bool OnCheckProc(ProcEventInfo& p_EventInfo)
            {
                DamageInfo* l_ProcDamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_ProcDamageInfo)
                    return false;

                SpellInfo const* l_ProcSpellInfo = l_ProcDamageInfo->GetSpellInfo();
                if (!l_ProcSpellInfo || l_ProcSpellInfo->Id != eSpells::Barkskin)
                    return false;

                return true;
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_oakhearts_puny_quods_AuraScript::OnCheckProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_oakhearts_puny_quods_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Lady and the Child - 200818
class spell_dru_lady_and_the_child : public SpellScriptLoader
{
    public:
        spell_dru_lady_and_the_child() : SpellScriptLoader("spell_dru_lady_and_the_child") { }

        class spell_dru_lady_and_the_child_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_lady_and_the_child_AuraScript);

            enum eSpells
            {
                Moonfire        = 8921,
                MoonfireAura    = 164812,
            };

            void HandleOnProc(ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                DamageInfo* l_DamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_Caster || !l_SpellInfo || !l_DamageInfo)
                    return;

                SpellInfo const* l_ProcSpellInfo = l_DamageInfo->GetSpellInfo();
                Unit* l_ProcTarget = l_DamageInfo->GetTarget();
                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player || !l_ProcSpellInfo || !l_ProcTarget)
                    return;

                if (l_ProcSpellInfo->Id != eSpells::Moonfire)
                    return;

                float l_Radius = l_SpellInfo->Effects[EFFECT_1].BasePoints;
                std::list<Unit*> l_TouchEnemies;
                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck u_check(l_ProcTarget, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> searcher(l_ProcTarget, l_TouchEnemies, u_check);
                l_ProcTarget->VisitNearbyObject(l_Radius, searcher);
                for (auto l_Target : l_TouchEnemies)
                    if (l_Target != l_ProcTarget && !l_Target->HasAura(eSpells::MoonfireAura))
                    {
                        l_Caster->CastSpell(l_Target, eSpells::Moonfire, true);
                        return;
                    }
            }

            void Register() override
            {
                OnProc += AuraProcFn(spell_dru_lady_and_the_child_AuraScript::HandleOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_lady_and_the_child_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Edraith, Bonds of Aglaya - 207943
class spell_dru_edraith_bonds_of_aglaya : public SpellScriptLoader
{
    public:
        spell_dru_edraith_bonds_of_aglaya() : SpellScriptLoader("spell_dru_edraith_bonds_of_aglaya") { }

        class spell_dru_edraith_bonds_of_aglaya_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_edraith_bonds_of_aglaya_AuraScript);

            enum eSpells
            {
                Swiftmend = 18562
            };

            bool CheckOnProc(ProcEventInfo& p_EventInfo)
            {
                DamageInfo* l_ProcDamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_ProcDamageInfo)
                    return false;

                SpellInfo const* l_ProcSpellInfo = l_ProcDamageInfo->GetSpellInfo();
                if (!l_ProcSpellInfo || l_ProcSpellInfo->Id != eSpells::Swiftmend)
                    return false;

                return true;
            }

            void HandleOnProc(ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                DamageInfo* l_ProcDamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_ProcDamageInfo || !l_Caster)
                    return;

                Unit* l_Target = l_ProcDamageInfo->GetTarget();
                if (!l_Target)
                    return;

                int32 l_Duration = GetSpellInfo()->Effects[EFFECT_0].BasePoints * IN_MILLISECONDS;
                Unit::AuraEffectList const &mPeriodic = l_Target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_HEAL);
                for (Unit::AuraEffectList::const_iterator i = mPeriodic.begin(); i != mPeriodic.end(); ++i)
                {
                    if ((*i)->GetCasterGUID() == l_Caster->GetGUID())
                    {
                        if (Aura* l_Aura = (*i)->GetBase())
                        {
                            int32 modDur = l_Aura->GetDuration() + l_Duration;
                            if (l_Aura->GetMaxDuration() < modDur)
                                l_Aura->SetMaxDuration(modDur);
                            l_Aura->SetDuration(modDur);
                        }
                    }
                }
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_edraith_bonds_of_aglaya_AuraScript::CheckOnProc);
                OnProc += AuraProcFn(spell_dru_edraith_bonds_of_aglaya_AuraScript::HandleOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_edraith_bonds_of_aglaya_AuraScript();
        }
};

/// Last Update Legion 7.1.5 build 23420
/// Lifebloom final heal - 33778
class spell_dru_lifebloom_final : public SpellScriptLoader
{
    public:
        spell_dru_lifebloom_final() : SpellScriptLoader("spell_dru_lifebloom_final") { }

        class spell_dru_lifebloom_final_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_lifebloom_final_SpellScript);

            enum eSpells
            {
                TheDarkTitansAdvice = 207271,
                BloomsOfGHanir      = 186393
            };

            void HandleOnCast()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                SpellInfo const* l_TheDarkTitansAdviceInfo = sSpellMgr->GetSpellInfo(eSpells::TheDarkTitansAdvice);
                SpellInfo const* l_BloomsOfGHanirInfo = sSpellMgr->GetSpellInfo(eSpells::BloomsOfGHanir);
                if (!l_TheDarkTitansAdviceInfo || !l_BloomsOfGHanirInfo)
                    return;

                int32 l_Healing = GetHitHeal();

                if (l_Caster->CanApplyPvPSpellModifiers()) ///< Lifebloom final heals 150% in pvp
                    AddPct(l_Healing, 50.0f);

                if (l_Caster->HasAura(eSpells::TheDarkTitansAdvice))
                    AddPct(l_Healing, l_TheDarkTitansAdviceInfo->Effects[EFFECT_0].BasePoints);

                if (l_Caster->HasAura(eSpells::BloomsOfGHanir))
                    AddPct(l_Healing, l_BloomsOfGHanirInfo->Effects[EFFECT_0].BasePoints);

                SetHitHeal(l_Healing);
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_lifebloom_final_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_lifebloom_final_SpellScript();
        }

};

/// Last Update 7.1.5 Build 23420
/// Called by Ironbark - 102342
class spell_dru_ironbark : public SpellScriptLoader
{
    public:
        spell_dru_ironbark() : SpellScriptLoader("spell_dru_ironbark") { }

        class spell_dru_ironbark_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ironbark_AuraScript);

            void HandleAfterEffectApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();
                if (!l_Caster || !l_Target)
                    return;

                l_Caster->m_SpellHelper.GetUint64(eSpellHelpers::IronbarkTarget) = l_Target->GetGUID();
            }

            void HandleAfterEffectRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                l_Caster->m_SpellHelper.GetUint64(eSpellHelpers::IronbarkTarget) = 0;
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_ironbark_AuraScript::HandleAfterEffectApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_ironbark_AuraScript::HandleAfterEffectRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_ironbark_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Fiery Red Maimers - 212875
class spell_dru_fiery_red_maimers : public SpellScriptLoader
{
    public:
        spell_dru_fiery_red_maimers() : SpellScriptLoader("spell_dru_fiery_red_maimers") { }

        class spell_dru_fiery_red_maimers_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_fiery_red_maimers_AuraScript);

            enum eSpells
            {
                AshamanesFrenzy = 210722
            };

            bool CheckOnProc(ProcEventInfo& p_EventInfo)
            {
                DamageInfo* l_ProcDamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_ProcDamageInfo)
                    return false;

                SpellInfo const* l_ProcSpellInfo = l_ProcDamageInfo->GetSpellInfo();
                if (!l_ProcSpellInfo || l_ProcSpellInfo->Id != eSpells::AshamanesFrenzy)
                    return false;

                return true;
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_fiery_red_maimers_AuraScript::CheckOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {

            return new spell_dru_fiery_red_maimers_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Fiery Red Maimers - 236757
class spell_dru_fiery_red_maimers_damage : public SpellScriptLoader
{
    public:
        spell_dru_fiery_red_maimers_damage() : SpellScriptLoader("spell_dru_fiery_red_maimers_damage") { }

        class spell_dru_fiery_red_maimers_damage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_fiery_red_maimers_damage_AuraScript);

            enum eSpells
            {
                Maim = 22570
            };

            bool CheckOnProc(ProcEventInfo& p_EventInfo)
            {
                DamageInfo* l_ProcDamageInfo = p_EventInfo.GetDamageInfo();
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                if (!l_ProcDamageInfo || !l_SpellInfo)
                    return false;

                SpellInfo const* l_ProcSpellInfo = l_ProcDamageInfo->GetSpellInfo();
                if (!l_ProcSpellInfo || l_ProcSpellInfo->Id != eSpells::Maim)
                    return false;

                return true;
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_fiery_red_maimers_damage_AuraScript::CheckOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_fiery_red_maimers_damage_AuraScript();
        }
};

/// Ekowraith, Creator of Worlds (210667) - 7.1.5 23420
/// Called by Thick Hide - 16931, Feline Swiftness - 131768, Astral Influence - 197524 and aura itself - 210667
class spell_dru_ekowraith_creator_of_worlds : public SpellScriptLoader
{
    public:
        spell_dru_ekowraith_creator_of_worlds() : SpellScriptLoader("spell_dru_ekowraith_creator_of_worlds") { }

        class spell_dru_ekowraith_creator_of_worlds_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ekowraith_creator_of_worlds_AuraScript);

            enum eSpells
            {
                EkowraithCreatorOfWorlds    = 210667,

                ThickHide                   = 16931,
                FelineSwiftness             = 131768,
                AstralInfluence             = 197524
            };

            void HandleCalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (AuraEffect const* l_AurEff = l_Caster->GetAuraEffect(eSpells::EkowraithCreatorOfWorlds, EFFECT_4))
                        AddPct(p_Amount, l_AurEff->GetAmount());
                }
            }

            void AfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                /// Must be delayed, in case those auras aren't applied yet
                if (Unit* l_Caster = GetCaster())
                {
                    int32 l_BasePoints = GetSpellInfo()->Effects[EFFECT_4].BasePoints;
                    l_Caster->m_Functions.AddFunction([=]() -> void
                    {
                        if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::ThickHide, EFFECT_0))
                        {
                            int32 l_Amount = l_AuraEffect->GetBaseAmount();

                            AddPct(l_Amount, l_BasePoints);

                            l_AuraEffect->ChangeAmount(l_Amount);
                        }

                        if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::FelineSwiftness, EFFECT_0))
                        {
                            int32 l_Amount = l_AuraEffect->GetBaseAmount();

                            AddPct(l_Amount, l_BasePoints);

                            l_AuraEffect->ChangeAmount(l_Amount);
                        }

                        for (uint8 l_I = EFFECT_0; l_I < EFFECT_4; ++l_I)
                        {
                            if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::AstralInfluence, SpellEffIndex(l_I)))
                            {
                                int32 l_Amount = l_AuraEffect->GetBaseAmount();

                                AddPct(l_Amount, l_BasePoints);

                                l_AuraEffect->ChangeAmount(l_Amount, true, true);
                            }
                        }
                    }, l_Caster->m_Functions.CalculateTime(10));
                }
            }

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                /// Can be done immediately
                if (Unit* l_Caster = GetCaster())
                {
                    if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::ThickHide, EFFECT_0))
                        l_AuraEffect->ChangeAmount(l_AuraEffect->GetBaseAmount());

                    if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::FelineSwiftness, EFFECT_0))
                        l_AuraEffect->ChangeAmount(l_AuraEffect->GetBaseAmount());

                    for (uint8 l_I = EFFECT_0; l_I < EFFECT_4; ++l_I)
                    {
                        if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::AstralInfluence, SpellEffIndex(l_I)))
                            l_AuraEffect->ChangeAmount(l_AuraEffect->GetBaseAmount(), true, true);
                    }
                }
            }

            void Register() override
            {
                switch (m_scriptSpellId)
                {
                    case eSpells::ThickHide:
                    {
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_ekowraith_creator_of_worlds_AuraScript::HandleCalculateAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
                        break;
                    }
                    case eSpells::FelineSwiftness:
                    {
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_ekowraith_creator_of_worlds_AuraScript::HandleCalculateAmount, EFFECT_0, SPELL_AURA_MOD_SPEED_ALWAYS);
                        break;
                    }
                    case eSpells::AstralInfluence:
                    {
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_ekowraith_creator_of_worlds_AuraScript::HandleCalculateAmount, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_ekowraith_creator_of_worlds_AuraScript::HandleCalculateAmount, EFFECT_1, SPELL_AURA_153);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_ekowraith_creator_of_worlds_AuraScript::HandleCalculateAmount, EFFECT_2, SPELL_AURA_ADD_FLAT_MODIFIER);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_ekowraith_creator_of_worlds_AuraScript::HandleCalculateAmount, EFFECT_3, SPELL_AURA_MOD_ATTACKER_RANGED_CRIT_CHANCE);
                        break;
                    }
                    case eSpells::EkowraithCreatorOfWorlds:
                    {
                        AfterEffectApply += AuraEffectApplyFn(spell_dru_ekowraith_creator_of_worlds_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_ekowraith_creator_of_worlds_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_ADD_FLAT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_ekowraith_creator_of_worlds_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Rejuvenation - 774
/// Called for X'oni's Caress - 235039
class spell_dru_xonis_caress : public SpellScriptLoader
{
    public:
        spell_dru_xonis_caress() : SpellScriptLoader("spell_dru_xonis_caress") { }

        class spell_dru_xonis_caress_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_xonis_caress_AuraScript);

            enum eSpells
            {
                XonisCaress     = 235039,
                XonisCaressHeal = 235040
            };

            void HandleAfterEffectPeriodic(AuraEffect const* p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (!l_Caster->HasAura(eSpells::XonisCaress) || !l_Caster->m_SpellHelper.GetUint64(eSpellHelpers::IronbarkTarget))
                    return;

                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::XonisCaress);
                Unit* l_TargetOfXonisHeal = sObjectAccessor->GetUnit(*l_Caster, l_Caster->m_SpellHelper.GetUint64(eSpellHelpers::IronbarkTarget));
                int32 l_HealAmount = GetPeriodicHealDone();
                if (!l_SpellInfo || !l_TargetOfXonisHeal || !l_HealAmount)
                    return;

                l_HealAmount = CalculatePct(l_HealAmount, l_SpellInfo->Effects[EFFECT_0].BasePoints);
                l_Caster->CastCustomSpell(l_TargetOfXonisHeal, eSpells::XonisCaressHeal, &l_HealAmount, nullptr, nullptr, true);
            }

            void Register() override
            {
                AfterEffectPeriodic += AuraEffectPeriodicFn(spell_dru_xonis_caress_AuraScript::HandleAfterEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_xonis_caress_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Rejuvenation - 774
/// Called for Aman'Thul's Wisdom - 208220
class spell_dru_amanthuls_wisdom_cooldown : public SpellScriptLoader
{
    public:
        spell_dru_amanthuls_wisdom_cooldown() : SpellScriptLoader("spell_dru_amanthuls_wisdom_cooldown") { }

        class spell_dru_amanthuls_wisdom_cooldown_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_amanthuls_wisdom_cooldown_AuraScript);

            enum eSpells
            {
                Rejuvenation                = 774,
                AmanthulsWisdomTargetAura   = 214062
            };

            int32 m_MaxDuration = 0;

            void OnApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                Aura* l_Aura = p_AurEff->GetBase();
                if (!l_Aura)
                    return;

                m_MaxDuration = l_Aura->GetMaxDuration();
            }

            void HandleOnEffectPeriodic(AuraEffect const* p_AurEff)
            {
                Unit* l_Target = GetTarget();
                if (!l_Target)
                    return;

                if (l_Target->HasAura(eSpells::AmanthulsWisdomTargetAura))
                {
                    Aura* l_Aura = l_Target->GetAura(eSpells::Rejuvenation);
                    if (!l_Aura)
                        return;

                    l_Aura->SetDuration(m_MaxDuration);
                }
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_amanthuls_wisdom_cooldown_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_amanthuls_wisdom_cooldown_AuraScript::HandleOnEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_amanthuls_wisdom_cooldown_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Regrowth - 8936 - Rejuvenation - 774
class spell_dru_power_of_the_archdruid : public SpellScriptLoader
{
    public:
        spell_dru_power_of_the_archdruid() : SpellScriptLoader("spell_dru_power_of_the_archdruid") { }

        class spell_dru_power_of_the_archdruid_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_power_of_the_archdruid_AuraScript);

            enum eSpells
            {
                Regrowth                    = 8936,
                Rejuvenation                = 774,
                PowerOfTheArchdruidBuff     = 189877,
                PowerOfTheArchdruidArea     = 213121
            };

            void HandleApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes)
            {
                Unit* l_Target = GetUnitOwner();
                Unit* l_Caster = GetCaster();
                if (!l_Target || !l_Caster || !l_Caster->HasAura(eSpells::PowerOfTheArchdruidBuff))
                    return;

                SpellInfo const* l_PowerOfTheArchdruid = sSpellMgr->GetSpellInfo(eSpells::PowerOfTheArchdruidArea);
                if (!l_PowerOfTheArchdruid || !l_PowerOfTheArchdruid->Effects[0].RadiusEntry)
                    return;

                float l_Radius = l_PowerOfTheArchdruid->Effects[0].RadiusEntry->RadiusMax;
                std::list<Unit*> l_TargetList;
                JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(l_Target, l_Target, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(l_Target, l_TargetList, u_check);
                l_Target->VisitNearbyObject(l_Radius, searcher);

                l_TargetList.sort(JadeCore::DistanceCompareOrderPred(l_Target));
                l_TargetList.remove(l_Target);
                l_TargetList.resize(l_PowerOfTheArchdruid->Effects[0].BasePoints);

                l_Caster->RemoveAurasDueToSpell(eSpells::PowerOfTheArchdruidBuff);

                for (Unit* l_Propagate : l_TargetList)
                    l_Caster->CastSpell(l_Propagate, m_scriptSpellId, true);
            }

            void Register() override
            {
                switch (m_scriptSpellId)
                {
                case eSpells::Regrowth:
                    AfterEffectApply += AuraEffectApplyFn(spell_dru_power_of_the_archdruid_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                    break;
                case eSpells::Rejuvenation:
                    AfterEffectApply += AuraEffectApplyFn(spell_dru_power_of_the_archdruid_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                    break;
                default:
                    return;
                }
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_power_of_the_archdruid_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Ashamane's Bite - 210702
class spell_dru_ashamanes_bite : public SpellScriptLoader
{
public:
    spell_dru_ashamanes_bite() : SpellScriptLoader("spell_dru_ashamanes_bite") { }

    class spell_dru_ashamanes_bite_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dru_ashamanes_bite_AuraScript);

        enum eSpells
        {
            Rip = 1079,
            AshamanesRip = 210705
        };

        bool HandleCheckProc(ProcEventInfo& p_EventInfo)
        {
            DamageInfo* l_ProcDamageInfo = p_EventInfo.GetDamageInfo();
            if (!l_ProcDamageInfo)
                return false;

            SpellInfo const* l_ProcSpellInfo = l_ProcDamageInfo->GetSpellInfo();
            if (!l_ProcSpellInfo || !l_ProcSpellInfo->HasEffect(SPELL_EFFECT_ENERGIZE))
                return false;

            for (uint8 l_I = 0; l_I < l_ProcSpellInfo->EffectCount; l_I++)
            {
                if (l_ProcSpellInfo->Effects[l_I].MiscValue == POWER_COMBO_POINT && l_ProcSpellInfo->Effects[l_I].IsEffect(SPELL_EFFECT_ENERGIZE))
                    return true;
            }

            return false;
        }

        void HandleOnProc(ProcEventInfo& p_EventInfo)
        {
            Unit* l_Caster = GetCaster();
            DamageInfo* l_ProcDamageInfo = p_EventInfo.GetDamageInfo();
            if (!l_ProcDamageInfo || !l_Caster)
                return;

            Unit* l_Target = l_ProcDamageInfo->GetTarget();
            SpellInfo const* l_ProcSpellInfo = l_ProcDamageInfo->GetSpellInfo();
            if (!l_Target || !l_ProcSpellInfo)
                return;

            Aura* l_AuraRip = l_Target->GetAura(eSpells::Rip);
            if (!l_AuraRip)
                return;

            Aura* l_AuraAshamanesRip = l_Caster->AddAura(eSpells::AshamanesRip, l_Target);
            AuraEffect* l_EffectRip = l_AuraRip->GetEffect(EFFECT_0);
            if (!l_AuraAshamanesRip || !l_EffectRip)
                return;

            AuraEffect* l_EffectAshamanesRip = l_AuraAshamanesRip->GetEffect(EFFECT_0);
            if (!l_EffectAshamanesRip)
                return;

            l_AuraAshamanesRip->SetDuration(l_AuraRip->GetDuration());
            l_EffectAshamanesRip->SetAmount(l_EffectRip->GetAmount());
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_dru_ashamanes_bite_AuraScript::HandleCheckProc);
            OnProc += AuraProcFn(spell_dru_ashamanes_bite_AuraScript::HandleOnProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_dru_ashamanes_bite_AuraScript();
    }
};

/// Last Update Legion 7.1.5
/// Tiger's Fury - 5217
class spell_dru_tigers_fury : public SpellScriptLoader
{
    public:
        spell_dru_tigers_fury() : SpellScriptLoader("spell_dru_tigers_fury") { }

        class spell_dru_tigers_fury_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_tigers_fury_SpellScript);

            enum eSpells
            {
                AshamanesEnergy     = 210579,
                AshamanesEnergyProc = 210583
            };

            enum eArtifactPowers
            {
                AshamanesEnergyArtifact = 1164
            };

            void HandleOnCast()
            {
                Unit* l_Caster = GetCaster();
                Player* l_Player = nullptr;

                if (!l_Caster || !(l_Player = l_Caster->ToPlayer()))
                    return;

                if (AuraEffect* l_Ashamane = l_Caster->GetAuraEffect(eSpells::AshamanesEnergy, EFFECT_0))
                {
                    uint32 l_TraitRank = l_Player->GetRankOfArtifactPowerId(eArtifactPowers::AshamanesEnergyArtifact);
                    int32 l_Bp0 = l_Ashamane->GetBaseAmount() * l_TraitRank;

                    l_Caster->CastCustomSpell(l_Caster, eSpells::AshamanesEnergyProc, &l_Bp0, nullptr, nullptr, true);
                }
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_tigers_fury_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_tigers_fury_SpellScript();
        }
};

/// Update to Legion 7.1.5 build 23420
/// Called by Ailuro Pouncers 208209
class spell_dru_ailuro_pouncers : public SpellScriptLoader
{
    public:
        spell_dru_ailuro_pouncers() : SpellScriptLoader("spell_dru_ailuro_pouncers") { }

        class spell_dru_ailuro_pouncers_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ailuro_pouncers_AuraScript);

            enum eSpells
            {
                PredatorSwiftness = 69369
            };

            void HandleOnTick(AuraEffect const* /* p_AurEff */)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::PredatorSwiftness, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_ailuro_pouncers_AuraScript::HandleOnTick, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_ailuro_pouncers_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called By Light of the Sun - 202918
class spell_dru_light_of_the_sun : public SpellScriptLoader
{
    public:
        spell_dru_light_of_the_sun() : SpellScriptLoader("spell_dru_light_of_the_sun") { }

        class spell_dru_light_of_the_sun_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_light_of_the_sun_AuraScript);

            enum eSpells
            {
                SolarBeam = 78675
            };

            void HandleOnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (Player* l_Player = l_Caster->ToPlayer())
                {
                    uint32 reduceTime = p_AurEff->GetAmount() * IN_MILLISECONDS;
                    // Delay cooldown reducing because 'SolarBeam' is not casted completely at this moment
                    l_Player->AddDelayedEvent([=]() -> void
                    {
                        l_Player->ReduceSpellCooldown(eSpells::SolarBeam, reduceTime);
                    }, 10);
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_light_of_the_sun_AuraScript::HandleOnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_light_of_the_sun_AuraScript();
        }
};

/// Last Update : 7.1.5 23420
/// Called for Protection of Ashmane 210650
/// Called by Cat Form - 768, Cat Form (Claws of Shirvallah) - 171746
class spell_dru_protection_of_ashmane : public SpellScriptLoader
{
    public:
        spell_dru_protection_of_ashmane() : SpellScriptLoader("spell_dru_protection_of_ashmane") { }

        class spell_dru_protection_of_ashmane_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_protection_of_ashmane_AuraScript);

            enum eSpells
            {
                ProtectionOfAshmane     = 210650,
                ProtectionOfAshmaneBuff = 210655,
                ProtectionOfAshmaneCD   = 214274
            };

            void AfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                if (l_Target == nullptr)
                    return;

                l_Target->RemoveAura(eSpells::ProtectionOfAshmaneBuff);
            }

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                if (!l_Target)
                    return;

                if (l_Target->HasAura(eSpells::ProtectionOfAshmane) && !l_Target->HasAura(eSpells::ProtectionOfAshmaneCD))
                {
                    l_Target->CastSpell(l_Target, eSpells::ProtectionOfAshmaneBuff, true);
                    l_Target->CastSpell(l_Target, eSpells::ProtectionOfAshmaneCD, true);
                }
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_protection_of_ashmane_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_protection_of_ashmane_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_protection_of_ashmane_AuraScript();
        }
};

/// Update to Legion 7.1.5 build 23420
/// Called for Ailuro Pouncers 208209
/// Called by Entangling Roots 339 - Regrowth 8936 - Rebirth 20484
class spell_dru_predator_swiftness_proc : public SpellScriptLoader
{
    public:
        spell_dru_predator_swiftness_proc() : SpellScriptLoader("spell_dru_predator_swiftness_proc") { }

        class spell_dru_predator_swiftness_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_predator_swiftness_proc_SpellScript);

            enum eSpells
            {
                PredatorSwiftnessProc = 69369
            };

            void HandleAfterCaster()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (Aura* l_Aura = l_Caster->GetAura(eSpells::PredatorSwiftnessProc))
                {
                    if (l_Aura->GetStackAmount() >= 1)
                        l_Aura->SetStackAmount(l_Aura->GetStackAmount() - 1);
                    else
                        l_Aura->Remove();
                }
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_predator_swiftness_proc_SpellScript::HandleAfterCaster);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_predator_swiftness_proc_SpellScript();
        }
};

/// Update to Legion 7.1.5 build 23420
/// Called by Infected Wounds - 48484
class spell_dru_infected_wounds : public SpellScriptLoader
{
    public:
        spell_dru_infected_wounds() : SpellScriptLoader("spell_dru_infected_wounds") { }

        class spell_dru_infected_wounds_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_infected_wounds_AuraScript);

            enum eSpells
            {
                Rake = 1822
            };

            bool CheckOnProc(ProcEventInfo& p_EventInfo)
            {
                DamageInfo* l_ProcDamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_ProcDamageInfo)
                    return false;

                SpellInfo const* l_ProcSpellInfo = l_ProcDamageInfo->GetSpellInfo();
                if (!l_ProcSpellInfo || l_ProcSpellInfo->Id != eSpells::Rake)
                    return false;

                return true;
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_infected_wounds_AuraScript::CheckOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_infected_wounds_AuraScript();
        }
};

/// Update to Legion 7.1.5 build 23420
/// Called by Infected Wounds (snare) - 58180
class spell_dru_infected_wounds_snare : public SpellScriptLoader
{
    public:
        spell_dru_infected_wounds_snare() : SpellScriptLoader("spell_dru_infected_wounds_snare") { }

        class spell_dru_infected_wounds_snare_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_infected_wounds_snare_AuraScript);

            void HandleOnEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (l_Caster->CanApplyPvPSpellModifiers()) ///< Infected Wound's snare is reduced by 40% in PvP
                    const_cast<AuraEffect*>(aurEff)->SetAmount(CalculatePct(aurEff->GetAmount(), 60.0f));
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_dru_infected_wounds_snare_AuraScript::HandleOnEffectApply, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_infected_wounds_snare_AuraScript();
        }
};

/// Last Update 7.1.5 23420
/// Call by Solar Beam - 78675
class spell_dru_solar_beam : public SpellScriptLoader
{
    public:
        spell_dru_solar_beam() : SpellScriptLoader("spell_dru_solar_beam") { }

        class spell_dru_solar_beam_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_solar_beam_SpellScript);

            enum eSpells
            {
                SolarBeamInterrupt  = 97547
            };

            void HandleLaunch(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                if (!l_Caster || !l_Target)
                    return;

                l_Caster->CastSpell(l_Target, eSpells::SolarBeamInterrupt, true);
            }

            void Register() override
            {
                OnEffectLaunch += SpellEffectFn(spell_dru_solar_beam_SpellScript::HandleLaunch, EFFECT_0, SPELL_EFFECT_TRIGGER_SPELL);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_solar_beam_SpellScript();
        }
};

/// Update to Legion 7.1.5 build 23420
/// Called by Shadow Thrash- 210676
class spell_dru_shadow_trash : public SpellScriptLoader
{
    public:
        spell_dru_shadow_trash() : SpellScriptLoader("spell_dru_shadow_trash") { }

        class spell_dru_shadow_trash_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_shadow_trash_AuraScript);

            enum eSpells
            {
                ShadowTrashDamages = 210687,
                Thrash             = 77758
            };

            Guid128 m_CastGuid;

            bool HandleCheckProc(ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                DamageInfo const* l_DamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_Caster || !l_DamageInfo)
                    return false;

                SpellInfo const* l_SpellInfo = l_DamageInfo->GetSpellInfo();
                if (!l_SpellInfo || l_SpellInfo->Id != eSpells::Thrash || m_CastGuid == l_DamageInfo->GetSpell()->GetCastGuid())
                    return false;

                return true;
            }

            void HandleOnProc(ProcEventInfo&  p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                DamageInfo const* l_DamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_Caster || !l_DamageInfo)
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::ShadowTrashDamages, true);
                m_CastGuid = l_DamageInfo->GetSpell()->GetCastGuid();
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_shadow_trash_AuraScript::HandleCheckProc);
                OnProc += AuraProcFn(spell_dru_shadow_trash_AuraScript::HandleOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_shadow_trash_AuraScript();
        }
};

/// Update to Legion 7.1.5 build 23420
/// Called by Adaptive Fur - 200850
class spell_dru_adaptive_fur : public SpellScriptLoader
{
    public:
        spell_dru_adaptive_fur() : SpellScriptLoader("spell_dru_adaptive_fur") { }

        class spell_dru_adaptive_fur_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_adaptive_fur_AuraScript);

            enum eSpells
            {
                AdaptiveFurArcane   = 200940,
                AdaptiveFurShadow   = 200941,
                AdaptiveFurFrost    = 200942,
                AdaptiveFurNature   = 200943,
                AdaptiveFurFire     = 200944,
                AdaptiveFurHoly     = 200945
            };

            bool HandleCheckProc(ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                DamageInfo const* l_DamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_Caster || !l_DamageInfo)
                    return false;

                if (l_DamageInfo->GetSchoolMask() & (SpellSchoolMask::SPELL_SCHOOL_MASK_SPELL | SpellSchoolMask::SPELL_SCHOOL_MASK_HOLY))
                    return true;

                return false;
            }

            void HandleOnProc(ProcEventInfo& p_EventInfo)
            {
                Unit* l_Caster = GetCaster();
                DamageInfo const* l_DamageInfo = p_EventInfo.GetDamageInfo();
                if (!l_Caster || !l_DamageInfo)
                    return;

                switch (l_DamageInfo->GetSchoolMask())
                {
                    case SpellSchoolMask::SPELL_SCHOOL_MASK_ARCANE:
                        l_Caster->CastSpell(l_Caster, eSpells::AdaptiveFurArcane, true);
                        break;
                    case SpellSchoolMask::SPELL_SCHOOL_MASK_SHADOW:
                        l_Caster->CastSpell(l_Caster, eSpells::AdaptiveFurShadow, true);
                        break;
                    case SpellSchoolMask::SPELL_SCHOOL_MASK_FROST:
                        l_Caster->CastSpell(l_Caster, eSpells::AdaptiveFurFrost, true);
                        break;
                    case SpellSchoolMask::SPELL_SCHOOL_MASK_NATURE:
                        l_Caster->CastSpell(l_Caster, eSpells::AdaptiveFurNature, true);
                        break;
                    case SpellSchoolMask::SPELL_SCHOOL_MASK_FIRE:
                        l_Caster->CastSpell(l_Caster, eSpells::AdaptiveFurFire, true);
                        break;
                    case SpellSchoolMask::SPELL_SCHOOL_MASK_HOLY:
                        l_Caster->CastSpell(l_Caster, eSpells::AdaptiveFurHoly, true);
                        break;
                    default:
                        break;
                }
            }

            void Register() override
            {
                DoCheckProc += AuraCheckProcFn(spell_dru_adaptive_fur_AuraScript::HandleCheckProc);
                OnProc += AuraProcFn(spell_dru_adaptive_fur_AuraScript::HandleOnProc);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_adaptive_fur_AuraScript();
        }
};

/// Update to Legion 7.1.5 build 23420
/// Called by Essence of G'Hanir - 208253
class spell_dru_essence_of_ghanir : public SpellScriptLoader
{
    public:
        spell_dru_essence_of_ghanir() : SpellScriptLoader("spell_dru_essence_of_ghanir") { }

        class spell_dru_essence_of_ghanir_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_essence_of_ghanir_AuraScript);

            void HandleCalcSpellMod(AuraEffect const* p_AurEff, SpellModifier*& p_SpellMod)
            {
                Unit* l_Owner = GetUnitOwner();
                if (!l_Owner)
                    return;

                Unit::AuraEffectList const& l_EffectList = l_Owner->GetAuraEffectsByType(SPELL_AURA_PERIODIC_HEAL);

                for (AuraEffect* l_Effect : l_EffectList)
                {
                    int32 l_Amplitude = l_Effect->GetAmplitude();

                    l_Amplitude = CalculatePct(l_Amplitude, abs(p_SpellMod->value));
                    l_Effect->SetAmplitude(l_Amplitude);
                }

                PreventDefaultAction();
            }

            void Register() override
            {
                DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_dru_essence_of_ghanir_AuraScript::HandleCalcSpellMod, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_essence_of_ghanir_AuraScript();
        }
};

/// last update 7.1.5
/// Ironfur - 192081
class spell_dru_ironfur : public SpellScriptLoader
{
    public:
        spell_dru_ironfur() : SpellScriptLoader("spell_dru_ironfur") { }

        class spell_dru_ironfur_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_ironfur_AuraScript);

            enum eSpells
            {
                GuardianOfElune = 213680,
                UrsocEndurance  = 200399
            };

            enum eArtifactTraits
            {
                UrsocEnduranceTrait = 949
            };

            std::queue<uint32> m_Timers;
            uint32 m_LiveTime;

            void HandleOnApply(AuraEffect const* p_AuraEffect, AuraEffectHandleModes p_Mode)
            {
                if (!(p_Mode & AURA_EFFECT_HANDLE_REAPPLY))
                    m_LiveTime = 0;

                SpellInfo const* l_SpellInfo = GetSpellInfo();
                Aura* l_Aura = GetAura();
                Unit* l_Caster = GetCaster();
                if (!l_SpellInfo || !l_Aura || !l_Caster)
                    return;

                if (l_Aura->GetStackAmount() > 6)
                    l_Aura->DropStack();

                SpellInfo const* l_UrsocEnduranceInfo = sSpellMgr->GetSpellInfo(eSpells::UrsocEndurance);
                SpellInfo const* l_GuardianOfEluneInfo = sSpellMgr->GetSpellInfo(eSpells::GuardianOfElune);
                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player || !l_GuardianOfEluneInfo || !l_UrsocEnduranceInfo)
                    return;

                uint32 l_TimeBonus = (l_Player->GetRankOfArtifactPowerId(eArtifactTraits::UrsocEnduranceTrait) * l_UrsocEnduranceInfo->Effects[EFFECT_0].BasePoints) / 2;
                l_TimeBonus += l_Player->HasAura(eSpells::GuardianOfElune) ? l_UrsocEnduranceInfo->Effects[EFFECT_0].BasePoints * 2 : 0;
                m_Timers.push(m_LiveTime + l_SpellInfo->GetMaxDuration() + l_TimeBonus);
            }

            void OnUpdate(uint32 const p_Diff)
            {
                m_LiveTime += p_Diff;

                Aura* l_Aura = GetAura();
                if (!l_Aura)
                    return;

                while (!m_Timers.empty() && m_LiveTime > m_Timers.front())
                {
                    l_Aura->DropStack();
                    m_Timers.pop();
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_dru_ironfur_AuraScript::OnUpdate);
                OnEffectApply += AuraEffectApplyFn(spell_dru_ironfur_AuraScript::HandleOnApply, EFFECT_0, SPELL_AURA_MOD_BASE_RESISTANCE_PCT, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_ironfur_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Bear Form - 5487
class spell_dru_bear_form : public SpellScriptLoader
{
    public:
        spell_dru_bear_form() : SpellScriptLoader("spell_dru_bear_form") { }

        class spell_dru_bear_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_bear_form_SpellScript);

            enum eSpells
            {
                BearFormRage    = 17057
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                l_Caster->SetPower(Powers::POWER_RAGE, 0);
                l_Caster->CastSpell(l_Caster, eSpells::BearFormRage, true);
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_dru_bear_form_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_bear_form_SpellScript();
        }
};

/// Last Update 7.1.5 23420
/// Call by Thorns - 203728
class spell_dru_thorns : public SpellScriptLoader
{
    public:
        spell_dru_thorns() : SpellScriptLoader("spell_dru_thorns") { }

        class spell_dru_thorns_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_thorns_SpellScript);

            void HandleHit()
            {
                Unit* l_Target = GetHitUnit();
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                if (!l_Target || !l_SpellInfo)
                    return;

                int32 l_Health = l_Target->GetMaxHealth();
                SetHitDamage(CalculatePct(l_Health, l_SpellInfo->Effects[0].BasePoints));
            }

            void Register() override
            {
                BeforeHit += SpellHitFn(spell_dru_thorns_SpellScript::HandleHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_thorns_SpellScript();
        }
};

/// Last Update 7.1.5 23420
/// Call by Enraged Mangle - 202085
class spell_dru_enraged_mangle : public SpellScriptLoader
{
    public:
        spell_dru_enraged_mangle() : SpellScriptLoader("spell_dru_enraged_mangle") { }

        class spell_dru_enraged_mangle_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_enraged_mangle_SpellScript);

            SpellCastResult FixAbilityRemovedFromGame()
            {
                Unit* l_Caster = GetCaster();

                if (Player* l_Player = l_Caster->ToPlayer())
                    l_Player->removeSpell(m_scriptSpellId);

                return SpellCastResult::SPELL_FAILED_DONT_REPORT;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_dru_enraged_mangle_SpellScript::FixAbilityRemovedFromGame);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_enraged_mangle_SpellScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Bear Form - 5487
/// Called for Celestial Guardian - 233754
class spell_dru_celestial_guardian : public SpellScriptLoader
{
    public:
        spell_dru_celestial_guardian() : SpellScriptLoader("spell_dru_celestial_guardian") { }

        class spell_dru_celestial_guardian_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_celestial_guardian_AuraScript);

            enum eSpells
            {
                CelestialGuardianAura = 233754,
                CelestialGuardianBuff = 234081
            };

            void HandleAfterEffectApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster || !l_Caster->HasAura(eSpells::CelestialGuardianAura))
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::CelestialGuardianBuff, true);
            }

            void HandleAfterEffectRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster || !l_Caster->HasAura(eSpells::CelestialGuardianAura))
                    return;

                l_Caster->RemoveAura(eSpells::CelestialGuardianBuff);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_celestial_guardian_AuraScript::HandleAfterEffectApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_celestial_guardian_AuraScript::HandleAfterEffectRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_celestial_guardian_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Healing Touch - 5185
class spell_dru_nourish : public SpellScriptLoader
{
    public:
        spell_dru_nourish() : SpellScriptLoader("spell_dru_nourish") { }

        class spell_dru_nourish_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_nourish_SpellScript);

            enum eSpells
            {
                Nourish                     = 203374,
                Rejuvenation                = 774,
                WildGrowth                  = 48438,
                Lifebloom                   = 33763,
                Regrowth                    = 8936,
                RejuvenationGermination     = 155777,
                Germination                 = 155675
            };

            int32 m_SpellToCast = 0;

            void HandleOnPrepare()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                Spell* l_Spell = GetSpell();

                m_SpellToCast = 0;

                if (!l_Caster || !l_Spell || !l_Target || !l_Caster->HasAura(eSpells::Nourish))
                    return;

                eSpells m_HotHeals[4] =
                {
                        Regrowth,
                        Rejuvenation,
                        WildGrowth,
                        Lifebloom
                };

                for (eSpells l_SpellId : m_HotHeals)
                {
                    if (!l_Target->HasAura(l_SpellId))
                    {
                        m_SpellToCast = l_SpellId;
                        return;
                    }
                }

                if (l_Caster->HasAura(eSpells::Germination))
                {
                    if (!l_Target->HasAura(eSpells::RejuvenationGermination))
                    {
                        m_SpellToCast = eSpells::RejuvenationGermination;
                        return;
                    }
                }

                l_Spell->SetCustomCritChance(100);
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();

                if (!l_Caster || !l_Target || !m_SpellToCast)
                    return;

                l_Caster->m_SpellHelper.GetBool(eSpellHelpers::Nourish) = true;
                l_Caster->CastSpell(l_Target, m_SpellToCast, true);
                l_Caster->m_SpellHelper.GetBool(eSpellHelpers::Nourish) = false;
            }

            void Register() override
            {
                OnPrepare += SpellOnPrepareFn(spell_dru_nourish_SpellScript::HandleOnPrepare);
                AfterHit += SpellHitFn(spell_dru_nourish_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_nourish_SpellScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Regrowth - 8936
class spell_dru_nourish_regrowth : public SpellScriptLoader
{
    public:
        spell_dru_nourish_regrowth() : SpellScriptLoader("spell_dru_nourish_regrowth") { }

        class spell_dru_nourish_regrowth_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_nourish_regrowth_SpellScript);

            void HandleBeforeHit()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster && l_Caster->m_SpellHelper.GetBool(eSpellHelpers::Nourish))
                    PreventHitHeal();
            }

            void Register() override
            {
                BeforeHit += SpellHitFn(spell_dru_nourish_regrowth_SpellScript::HandleBeforeHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_nourish_regrowth_SpellScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Starfall - 191037
/// Called for Echoing Stars - 214508
class spell_dru_echoing_stars : public SpellScriptLoader
{
    public:
        spell_dru_echoing_stars() : SpellScriptLoader("spell_dru_echoing_stars") { }

        class spell_dru_echoing_stars_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_echoing_stars_SpellScript);

            enum eSpells
            {
                EchoingStars        = 214508,
                EchoingStarsDmg     = 226104
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                SpellInfo const* l_EchoingStarInfo = sSpellMgr->GetSpellInfo(eSpells::EchoingStars);
                if (!l_Target || !l_Caster || !l_EchoingStarInfo || !l_Caster->HasAura(eSpells::EchoingStars))
                    return;

                l_Target = l_Target->SelectNearbyAlly(l_Target, l_EchoingStarInfo->Effects[EFFECT_0].BasePoints);
                if (l_Target && l_Caster->IsValidAttackTarget(l_Target))
                    l_Caster->CastSpell(l_Target, eSpells::EchoingStarsDmg, true);

            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_echoing_stars_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_echoing_stars_SpellScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Bear Form - 5487
/// Called for Tooth and Claw - 236019
class spell_dru_tooth_and_claw : public SpellScriptLoader
{
    public:
        spell_dru_tooth_and_claw() : SpellScriptLoader("spell_dru_tooth_and_claw") { }

        class spell_dru_tooth_and_claw_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_tooth_and_claw_AuraScript);

            enum eSpells
            {
                ToothAndClawAura    = 236019,
                ToothAndClawBuff    = 236440,
                EnragedMaul         = 236716
            };

            void HandleAfterEffectApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster || !l_Caster->HasAura(eSpells::ToothAndClawAura))
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player)
                    return;

                l_Player->CastSpell(l_Player, eSpells::ToothAndClawBuff, true);
            }

            void HandleAfterEffectRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player)
                    return;

                l_Player->RemoveAura(eSpells::ToothAndClawBuff);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_tooth_and_claw_AuraScript::HandleAfterEffectApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_tooth_and_claw_AuraScript::HandleAfterEffectRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_tooth_and_claw_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Tooth and Claw - 236019
class spell_dru_tooth_and_claw_remove : public SpellScriptLoader
{
    public:
        spell_dru_tooth_and_claw_remove() : SpellScriptLoader("spell_dru_tooth_and_claw_remove") { }

        class spell_dru_tooth_and_claw_remove_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_tooth_and_claw_remove_AuraScript);

            enum eSpells
            {
                ToothAndClawBuff    = 236440,
                EnragedMaul         = 236716
            };

            void HandleAfterEffectApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player)
                    return;

                if (l_Caster->GetShapeshiftForm() == FORM_BEAR_FORM)
                    l_Player->CastSpell(l_Player, eSpells::ToothAndClawBuff, true);

                l_Player->learnSpell(eSpells::EnragedMaul, false, false, false, 0, true);
            }

            void HandleAfterEffectRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player)
                    return;

                l_Player->RemoveAura(eSpells::ToothAndClawBuff);
                l_Player->removeSpell(eSpells::EnragedMaul, false, false);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dru_tooth_and_claw_remove_AuraScript::HandleAfterEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_dru_tooth_and_claw_remove_AuraScript::HandleAfterEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_tooth_and_claw_remove_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Rejuvenation - 774, WildGrowth - 48438, Lifebloom - 33763, Regrowth - 8936, Germination's Rejuvenation - 155777
class spell_dru_flourish_targets : public SpellScriptLoader
{
    public:
        spell_dru_flourish_targets() : SpellScriptLoader("spell_dru_flourish_targets") { }

        class spell_dru_flourish_targets_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_flourish_targets_AuraScript);

            enum eSpells
            {
                Regrowth = 8936
            };

            void HandleAfterEffectApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetUnitOwner();

                if (l_Caster && l_Target)
                {
                    std::map<uint64, uint64>& l_Targets = l_Caster->m_SpellHelper.m_Uint64Map[eSpellHelpers::Flourish];

                    if (l_Targets.find(l_Target->GetGUID()) == l_Targets.end())
                        l_Targets[l_Target->GetGUID()] = 0;

                    l_Targets[l_Target->GetGUID()]++;
                }
            }

            void HandleAfterEffectRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetUnitOwner();

                if (l_Caster && l_Target)
                {
                    std::map<uint64, uint64>& l_Targets = l_Caster->m_SpellHelper.m_Uint64Map[eSpellHelpers::Flourish];

                    if (l_Targets.find(l_Target->GetGUID()) == l_Targets.end())
                        l_Targets[l_Target->GetGUID()] = 0;

                    if (l_Targets[l_Target->GetGUID()] > 0)
                        l_Targets[l_Target->GetGUID()]--;
                }
            }

            void Register() override
            {
                if (m_scriptSpellId == eSpells::Regrowth)
                {
                    AfterEffectApply += AuraEffectApplyFn(spell_dru_flourish_targets_AuraScript::HandleAfterEffectApply, EFFECT_1, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                    AfterEffectRemove += AuraEffectRemoveFn(spell_dru_flourish_targets_AuraScript::HandleAfterEffectRemove, EFFECT_1, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                }
                else
                {
                    AfterEffectApply += AuraEffectApplyFn(spell_dru_flourish_targets_AuraScript::HandleAfterEffectApply, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                    AfterEffectRemove += AuraEffectRemoveFn(spell_dru_flourish_targets_AuraScript::HandleAfterEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_flourish_targets_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Flourish - 197721
class spell_druid_flourish : public SpellScriptLoader
{
    public:
        spell_druid_flourish() : SpellScriptLoader("spell_druid_flourish") { }

        class spell_druid_flourish_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_druid_flourish_SpellScript);

            enum eSpells
            {
                WildGrowth = 48438
            };

            void HandleScript(SpellEffIndex /*p_EffIndex*/)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellInfo = GetSpellInfo();

                if (!l_Caster || !l_SpellInfo)
                    return;

                const SpellRadiusEntry* l_RadiusEntry = l_SpellInfo->Effects[EFFECT_0].RadiusEntry;
                std::map<uint64, uint64> l_Targets = l_Caster->m_SpellHelper.m_Uint64Map[eSpellHelpers::Flourish];

                if (!l_RadiusEntry)
                    return;

                for (auto l_TargetPair : l_Targets)
                {
                    if (l_TargetPair.second == 0)
                        continue;;

                    Unit* l_Target = sObjectAccessor->GetUnit(*l_Caster, l_TargetPair.first);

                    if (!l_Target || !l_Target->IsInRange(l_Caster, 0, l_RadiusEntry->RadiusMax))
                        continue;

                    std::list<AuraEffect*> l_EffectList = l_Target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_HEAL);

                    for (AuraEffect* l_Effect : l_EffectList)
                    {
                        if (l_Effect->GetCasterGUID() != l_Caster->GetGUID())
                            continue;

                        Aura* l_Aura = l_Effect->GetBase();

                        if (!l_Aura)
                            continue;

                        if (l_Aura->GetSpellInfo() && l_Aura->GetSpellInfo()->Id == eSpells::WildGrowth)
                        {
                            std::map<uint64, uint64> l_WildGrowthOriginalAmountMap = l_Caster->m_SpellHelper.m_Uint64Map[eSpellHelpers::WildGrowth];

                            if (l_WildGrowthOriginalAmountMap.find(l_Target->GetGUID()) != l_WildGrowthOriginalAmountMap.end())
                            {
                                int32 l_Amount = static_cast<int32>(l_WildGrowthOriginalAmountMap[l_Target->GetGUID()]);

                                l_Effect->SetAmount(l_Effect->GetAmount() + CalculatePct(l_Amount, 1 + l_SpellInfo->Effects[EFFECT_0].BasePoints));
                            }
                        }

                        l_Aura->SetDuration(l_Aura->GetDuration() + l_SpellInfo->Effects[EFFECT_0].BasePoints * TimeConstants::IN_MILLISECONDS);
                    }
                }
            }

            void Register() override
            {
                OnEffectHit += SpellEffectFn(spell_druid_flourish_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_druid_flourish_SpellScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Trash (cat form) - 106830
/// Called for Scent of Blood - 210664
class spell_dru_scent_of_blood : public SpellScriptLoader
{
    public:
        spell_dru_scent_of_blood() : SpellScriptLoader("spell_dru_scent_of_blood") { }

        class spell_dru_scent_of_blood_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_scent_of_blood_SpellScript);

            enum eSpells
            {
                ScentOfBloodAura = 210663,
                ScentOfBloodBuff = 210664
            };

            int32 m_HitTarget = 0;

            void HandleDamage(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::ScentOfBloodAura);
                if (!l_Caster || !l_SpellInfo || !l_Caster->HasAura(eSpells::ScentOfBloodAura))
                    return;

                m_HitTarget++;

                int32 l_Bp = -(m_HitTarget * l_SpellInfo->Effects[EFFECT_0].BasePoints);

                l_Caster->CastCustomSpell(l_Caster, eSpells::ScentOfBloodBuff, &l_Bp, nullptr, nullptr, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_scent_of_blood_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_scent_of_blood_SpellScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Mangle - 33917
class spell_dru_mangle_damage : public SpellScriptLoader
{
    public:
        spell_dru_mangle_damage() : SpellScriptLoader("spell_dru_mangle_damage") { }

        class spell_dru_mangle_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_mangle_damage_SpellScript);

            void HandleDamage()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                SpellInfo const* l_SpellInfo = GetSpellInfo();
                if (!l_Caster || !l_Target || !l_SpellInfo)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (!l_Player)
                    return;

                if (l_Player->GetActiveSpecializationID() == SPEC_DRUID_GUARDIAN && l_Target->HasAuraWithMechanic((1LL << Mechanics::MECHANIC_BLEED)))
                    SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_SpellInfo->Effects[EFFECT_2].BasePoints));
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_dru_mangle_damage_SpellScript::HandleDamage);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_dru_mangle_damage_SpellScript();
        }
};

/// Update 7.1.5 Build 23420
/// Called by Bear Form - 5487, Cat Form - 768, Travel Form - 783,  Cat Form (Claws of Shirvallah) - 171745, MoonkinForm - 24858
class spell_dru_shapeshifting_cooldown : public SpellScriptLoader
{
    public:
        spell_dru_shapeshifting_cooldown() : SpellScriptLoader("spell_dru_shapeshifting_cooldown") { }

        enum eSpells
        {
            TravelForm  = 783,
            MoonkinForm = 24858
        };

        enum eTimers
        {
            TimerFormCooldown = 1 * IN_MILLISECONDS
        };

        class spell_dru_shapeshifting_cooldown_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_shapeshifting_cooldown_AuraScript);

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Owner = GetUnitOwner();
                if (!l_Owner)
                    return;

                if (Player* l_Player = l_Owner->ToPlayer())
                {
                    uint32 l_SpellId = GetSpellInfo()->Id;
                    if (!l_Player->HasSpellCooldown(l_SpellId))
                    {
                        l_Player->AddSpellCooldown(l_SpellId, 0, eTimers::TimerFormCooldown, true);
                    }
                }
            }

            void Register() override
            {
                switch (m_scriptSpellId)
                {
                    case eSpells::TravelForm:
                        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_shapeshifting_cooldown_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                        break;
                    case eSpells::MoonkinForm:
                        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_shapeshifting_cooldown_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                        break;
                    default:
                        AfterEffectRemove += AuraEffectRemoveFn(spell_dru_shapeshifting_cooldown_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_shapeshifting_cooldown_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Shadow Trash - 210686
class spell_dru_shadow_trash_periodic : public SpellScriptLoader
{
    public:
        spell_dru_shadow_trash_periodic() : SpellScriptLoader("spell_dru_shadow_trash_periodic") { }

        class spell_dru_shadow_trash_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_shadow_trash_periodic_AuraScript);

            enum eSpells
            {
                ShadowTrashDamage = 210687
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::ShadowTrashDamage, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_shadow_trash_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_shadow_trash_periodic_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Bear Form - 5487, Cat Form - 768, Travel Form - 783,  Cat Form (Claws of Shirvallah) - 171745, MoonkinForm - 24858
/// Called for Mark of Shifting - 186372
class spell_dru_mark_of_shifting : public SpellScriptLoader
{
    public:
        spell_dru_mark_of_shifting() : SpellScriptLoader("spell_dru_mark_of_shifting") { }

        class spell_dru_mark_of_shifting_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_mark_of_shifting_AuraScript);

            enum eSpells
            {
                TravelForm              = 783,
                MoonkinForm             = 24858,
                MarkOfShifting          = 186372,
                MarkOfShiftingPeriodic  = 186370
            };

            void HandleAfterApply(AuraEffect const*, AuraEffectHandleModes)
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster || !l_Caster->HasAura(eSpells::MarkOfShifting))
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::MarkOfShiftingPeriodic, true);
            }

            void HandleAfterRemove(AuraEffect const*, AuraEffectHandleModes)
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                l_Caster->RemoveAurasDueToSpell(eSpells::MarkOfShiftingPeriodic);
            }

            void Register() override
            {
                switch (m_scriptSpellId)
                {
                case eSpells::TravelForm:
                    AfterEffectApply += AuraEffectApplyFn(spell_dru_mark_of_shifting_AuraScript::HandleAfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                    AfterEffectRemove += AuraEffectRemoveFn(spell_dru_mark_of_shifting_AuraScript::HandleAfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                    break;
                case eSpells::MoonkinForm:
                    AfterEffectApply += AuraEffectApplyFn(spell_dru_mark_of_shifting_AuraScript::HandleAfterApply, EFFECT_1, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                    AfterEffectRemove += AuraEffectRemoveFn(spell_dru_mark_of_shifting_AuraScript::HandleAfterRemove, EFFECT_1, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                    break;
                default:
                    AfterEffectApply += AuraEffectApplyFn(spell_dru_mark_of_shifting_AuraScript::HandleAfterApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                    AfterEffectRemove += AuraEffectRemoveFn(spell_dru_mark_of_shifting_AuraScript::HandleAfterRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                    break;
                }
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_mark_of_shifting_AuraScript();
        }
};

/// Last Update 7.1.5 Build 23420
/// Called by Mark of Shifting - 186370
class spell_dru_mark_of_shifting_periodic : public SpellScriptLoader
{
    public:
        spell_dru_mark_of_shifting_periodic() : SpellScriptLoader("spell_dru_mark_of_shifting_periodic") { }

        class spell_dru_mark_of_shifting_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_mark_of_shifting_periodic_AuraScript);

            enum eSpells
            {
                MarkOfShiftingHeal = 224392
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster || l_Caster->GetHealth() >= l_Caster->GetMaxHealth())
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::MarkOfShiftingHeal, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_mark_of_shifting_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dru_mark_of_shifting_periodic_AuraScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_druid_spell_scripts()
{
    new spell_dru_mark_of_shifting();
    new spell_dru_mark_of_shifting_periodic();
    new spell_dru_shadow_trash_periodic();
    new spell_dru_flourish_targets();
    new spell_dru_nourish_regrowth();
    new spell_dru_nourish();
    new spell_dru_enraged_mangle();
    new spell_dru_thorns();
    new spell_dru_essence_of_ghanir();
    new spell_dru_power_of_the_archdruid();
    new spell_dru_tigers_fury();
    new spell_dru_cyclone();
    new spell_dru_fury_of_elune();
    new spell_dru_dreamwalker_trigger();
    new spell_dru_moon_artifact();
    new spell_dru_frenzied_regeneration();
    new spell_dru_rage_of_the_sleeper();
    new spell_druid_lunar_strike();
    new spell_dru_power_of_goldrinn();
    new spell_dru_ashamanes_frenzy();
    new spell_dru_ashamanes_frenzy_dmg();
    new spell_dru_tranquility();
    new spell_druid_flourish();
    new spell_dru_nature_balance();
    new spell_dru_blessing_of_the_ancients();
    new spell_dru_mangle();
    new spell_dru_starlord();
    new spell_dru_innervate();
    new spell_dru_rend_and_tear();
    new spell_dru_guardian_of_elune_aura();
    new spell_dru_galactic_guardian();
    new spell_dru_earthwarden_absorb();
    new spell_dru_earthwarden();
    new spell_dru_brambles();
    new spell_dru_bristling_fur();
    new spell_dru_feral_affinity();
    new spell_dru_restoration_affinity();
    new spell_dru_guardian_affinity();
    new spell_dru_balance_affinity();;
    new spell_dru_germination();
    new spell_dru_one_with_nature();
    new spell_dru_living_seed();
    new spell_dru_guardian_of_elune();
    new spell_dru_astral_form();
    new spell_dru_incarnation_tree_of_life();
    new spell_dru_yseras_gift_ally_proc();
    new spell_dru_pulverize();
    new spell_dru_thrash_bear_periodic();
    new spell_dru_entangling_energy();
    new spell_dru_barkskin();
    new spell_dru_yseras_gift();
    new spell_dru_incarnation_chosen_of_elune();
    new spell_dru_incarnation_skins();
    new spell_dru_item_pvp_feral_4p();
    new spell_dru_thrash_bear();
    new spell_dru_swipe();
    new spell_dru_maul();
    new spell_dru_natures_vigil();
    new spell_dru_natures_vigil_proc();
    new spell_dru_dash();
    new spell_dru_lifebloom();
    new spell_dru_killer_instinct();
    new spell_dru_natures_cure();
    new spell_dru_activate_cat_form();
    new spell_dru_cat_form();
    new spell_dru_skull_bash();
    new spell_dru_efflorescence();
    new spell_dru_stampeding_roar();
    new spell_dru_moonfire();
    new spell_dru_moonfire_sunfire_damage();
    new spell_dru_t10_restoration_4p_bonus();
    new spell_dru_savage_roar();
    new spell_dru_survival_instincts();
    new spell_dru_swiftmend();
    new spell_dru_travel_form_removal();
    new spell_dru_travel_form();
    new spell_dru_travel_form_playerscript();
    new spell_dru_swift_flight_passive();
    new spell_dru_rake();
    new spell_dru_rake_triggered();
    new spell_dru_shred();
    new spell_dru_ferocious_bite();
    new spell_dru_rip();
    new spell_dru_dream_of_cenarius();
    new spell_dru_primal_fury();
    new spell_dru_healing_touch();
    new spell_dru_rejuvenation();
    new spell_dru_sunfire();
    new spell_dru_regrowth();
    new spell_dru_wild_growth();
    new spell_dru_druid_flames();
    new spell_dru_wrath();
    new spell_dru_efflorescence_heal();
    new spell_dru_efflorescence_heal_proc();
    new spell_dru_dream_of_cenarius_feral();
    new spell_dru_wod_pvp_2p_restoration();
    new spell_dru_WodPvpBalance4pBonus();
    new spell_dru_ursa_major_aura();
    new spell_dru_wod_pvp_balance_2p();
    new spell_dru_lunar_inspiration();
    new spell_dru_item_t17_feral_2p_bonus();
    new spell_dru_item_t17_feral_4p_bonus();
    new spell_dru_item_t17_feral_4p_bonus_proc_driver();
    new spell_dru_item_t17_restoration_4p_bonus();
    new spell_dru_charm_woodland_creature();
    new spell_dru_warrior_of_elune();
    new spell_dru_predatory_swiftness();
    new spell_dru_maim();
    new spell_dru_enraged_maim();
    new spell_dru_fury_of_elune_move();
    new spell_dru_touch_of_the_moon();
    new spell_dru_gore();
    new spell_dru_soul_of_the_forest_resto();
    new spell_dru_soul_of_the_forest_resto_aura();
    new spell_dru_solar_wrath();
    new spell_dru_moon_and_stars();
    new spell_dru_clearcasting();
    new spell_dru_impeccable_fel_essence();
    new spell_dru_lady_and_the_child();
    new spell_dru_starfall();
    new spell_dru_echoing_stars();
    new spell_dru_starsurge();
    new spell_dru_lifebloom_final();
    new spell_dru_ashamanes_bite();
    new spell_dru_ironbark();
    new spell_dru_predator_swiftness_proc();
    new spell_dru_infected_wounds();
    new spell_dru_infected_wounds_snare();
    new spell_dru_solar_beam();
    new spell_dru_ironfur();
    new spell_dru_bear_form();
    new spell_dru_mangle_damage();
    new spell_dru_shapeshifting_cooldown();

    /// PlayerScript
    new PlayerScript_soul_of_the_forest();
    new PlayerScript_predator();
    new PlayerScript_blessing_of_the_ancients();
    new PlayerScript_Frenzied_Regenation();
    new PlayerScript_Killer_Instinct();
    new PlayerScript_dru_blessings_of_the_ancients();

    ///Legendary items
    new spell_dru_skysecs_hold();
    new spell_dru_the_emerald_dreamcatcher();
    new spell_dru_oneths_intuition();
    new spell_dru_the_wildshapers_clutch();
    new spell_dru_promise_of_elune();
    new spell_dru_promise_of_elune_healing();
    new spell_dru_amanthuls_wisdom();
    new spell_dru_amanthuls_wisdom_target_aura();
    new spell_dru_oakhearts_puny_quods();
    new spell_dru_amanthuls_wisdom_cooldown();
    new spell_dru_edraith_bonds_of_aglaya();
    new spell_dru_fiery_red_maimers();
    new spell_dru_fiery_red_maimers_damage();
    new spell_dru_ekowraith_creator_of_worlds();
    new spell_dru_xonis_caress();
    new spell_dru_impeccable_fel_essence_reset();
    new spell_dru_ailuro_pouncers();
    new spell_dru_light_of_the_sun();
    new spell_dru_protection_of_ashmane();

    ///Honor Talents
    new spell_dru_primal_vitality();
    new spell_dru_rip_and_tear();
    new spell_dru_overgrowth();
    new spell_dru_pouncing_strikes();
    new spell_dru_shapemender();
    new spell_dru_focused_growth();
    new spell_dru_fortified_bark();
    new spell_dru_alpha_challenge();
    new spell_dru_revitalize();
    new spell_dru_revitalize_proc();
    new spell_dru_druid_of_the_claw();
    new spell_dru_moonkin_aura();
    new spell_dru_moonkin_aura_buff();
    new spell_dru_den_mother_check();
    new spell_dru_overrun();
    new spell_dru_celestial_guardian();
    new spell_dru_tooth_and_claw();
    new spell_dru_tooth_and_claw_remove();
    new spell_dru_ferocious_wound();

    ///Artifact Traits
    new spell_dru_adaptive_fur();
    new spell_dru_scent_of_blood();

    ///Feral
    new spell_dru_shadow_trash();
}
#endif
