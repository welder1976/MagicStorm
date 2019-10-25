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
#include "ScriptedCreature.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "GameObject.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "QuestPackets.h"
#include "ScenePackets.h"
#include "GameObjectAI.h"
#include "ObjectAccessor.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "WaypointManager.h"
#include "MotionMaster.h"
#include "PhasingHandler.h"
#include "SpellInfo.h"
#include "Log.h"
#include "Map.h"
#include "Transport.h"
#include "DBCEnums.h"
#include "LFGMgr.h"
#include "LFGQueue.h"
#include "LFGPackets.h"
#include "DynamicObject.h"
#include "CreatureTextMgr.h"
#include "MiscPackets.h"
#include "Creature.h"
#include "Vehicle.h"
#include "TemporarySummon.h"
#include "CombatAI.h"




#define GOSSIP_ACCEPT_DUEL      "Let''s duel"
#define EVENT_SPECIAL 20

class scene_azsuna_runes : public SceneScript
{
public:
    scene_azsuna_runes() : SceneScript("scene_azsuna_runes") { }

    // Called when a player receive trigger from scene
    void OnSceneTriggerEvent(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* sceneTemplate, std::string const& triggerName) override
    {
        if (triggerName == "Credit")
        {
            uint32 killCreditEntry = 0;

            switch (sceneTemplate->ScenePackageId)
            {
                case 1378: //Azsuna - Academy - Runes A(Arcane, Quest) - PRK
                case 1695: //Azsuna - Academy - Runes D(Arcane: Sophomore) - PRK
                case 1696: //Azsuna - Academy - Runes E(Arcane: Junior) - PRK
                case 1697: //Azsuna - Academy - Runes F(Arcane: Senior) - PRK
                    killCreditEntry = 89655;
                    break;
                case 1379: //Azsuna - Academy - Runes B(Fire, Quest) - PRK
                case 1698: //Azsuna - Academy - Runes G(Fire: Freshman) - PRK
                case 1699: //Azsuna - Academy - Runes H(Fire: Junior) - PRK
                case 1700: //Azsuna - Academy - Runes I(Fire: Senior) - PRK
                    killCreditEntry = 89656;
                    break;
                case 1380: //Azsuna - Academy - Runes C(Frost, Quest) - PRK
                case 1701: //Azsuna - Academy - Runes J(Frost: Freshman) - PRK
                case 1702: //Azsuna - Academy - Runes K(Frost: Junior) - PRK
                case 1703: //Azsuna - Academy - Runes L(Frost: Senior) - PRK
                    killCreditEntry = 89657;
                    break;
            }

            if (killCreditEntry)
                player->KilledMonsterCredit(killCreditEntry);
        }
    }

    void OnSceneComplete(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* sceneTemplate) override
    {
        uint32 nextSceneSpellId = 0;

        switch (sceneTemplate->ScenePackageId)
        {
            case 1378: nextSceneSpellId = 223283; break; //Azsuna - Academy - Runes A(Arcane, Quest) - PRK, next : fire quest
            case 1379: nextSceneSpellId = 223287; break; //Azsuna - Academy - Runes B(Fire, Quest) - PRK, next : frost quest
            default:
            case 1695: //Azsuna - Academy - Runes D(Arcane: Sophomore) - PRK
            case 1696: //Azsuna - Academy - Runes E(Arcane: Junior) - PRK
            case 1697: //Azsuna - Academy - Runes F(Arcane: Senior) - PRK
            case 1698: //Azsuna - Academy - Runes G(Fire: Freshman) - PRK
            case 1699: //Azsuna - Academy - Runes H(Fire: Junior) - PRK
            case 1700: //Azsuna - Academy - Runes I(Fire: Senior) - PRK
            case 1380: //Azsuna - Academy - Runes C(Frost, Quest) - PRK
            case 1701: //Azsuna - Academy - Runes J(Frost: Freshman) - PRK
            case 1702: //Azsuna - Academy - Runes K(Frost: Junior) - PRK
            case 1703: //Azsuna - Academy - Runes L(Frost: Senior) - PRK
                break;
        }

        if (nextSceneSpellId)
            player->CastSpell(player, nextSceneSpellId, true);
    }
};

// 93326
class npc_archmage_khadgar_93337 : public CreatureScript
{
public:
    npc_archmage_khadgar_93337() : CreatureScript("npc_archmage_khadgar_93337") { }

    enum eNpc
    {
        QUEST_INTO_THE_FRAY_DH = 44137,
        QUEST_INTO_THE_FRAY = 38834,
        SPELL_TRANSFORM_KHADGAR_RAVEN = 165291,
        EVENT_SAY_HURRY = 11,
        EVENT_TRANSFORM = 12,
        EVENT_MOVE_CAMP = 13,
        EVENT_DESPAWN = 14,
        DATA_DO_SOMETHING = 21,
    };

    bool OnQuestAccept(Player* /*player*/, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_INTO_THE_FRAY)
        {
            creature->GetAI()->SetData(DATA_DO_SOMETHING, DATA_DO_SOMETHING);
        }
        return true;
    }

    struct npc_archmage_khadgar_93337_AI : public ScriptedAI
    {
        npc_archmage_khadgar_93337_AI(Creature* creature) : ScriptedAI(creature) {
            Initialize();
        }

        void Initialize() {}

        void Reset() override
        {
            me->SetWalk(false);
            me->SetSpeed(MOVE_RUN, 1.0f);
            me->SetHomePosition(me->GetPosition());
            _events.Reset();            
            Initialize();
        }

        void SetData(uint32 id, uint32 /*value*/) override
        {
            switch (id)
            {
            case DATA_DO_SOMETHING:
                _events.ScheduleEvent(EVENT_SAY_HURRY, 300);
                break;
            default:
                break;
            }
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == WAYPOINT_MOTION_TYPE)
            {
                switch (id)
                {
                case 6:
                    _events.ScheduleEvent(EVENT_DESPAWN, 1000);
                    break;
                default:
                    break;
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SAY_HURRY:
                    me->AI()->Talk(1);
                    _events.ScheduleEvent(EVENT_TRANSFORM, 1000);
                    break;
                case EVENT_TRANSFORM:
                    DoCast(me, SPELL_TRANSFORM_KHADGAR_RAVEN, true);
                    _events.ScheduleEvent(EVENT_MOVE_CAMP, 300);
                    break;
                case EVENT_MOVE_CAMP:
                    me->GetMotionMaster()->MovePath(9332600, false);
                    break;
                case EVENT_DESPAWN:
                    me->RemoveAurasDueToSpell(SPELL_TRANSFORM_KHADGAR_RAVEN);
                    me->GetMotionMaster()->MoveTargetedHome();
                    me->DespawnOrUnsummon();
                    break;
                default:
                    break;
                }
            }
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_archmage_khadgar_93337_AI(creature);
    }
};

struct questnpc_soul_gem : public ScriptedAI
{
    questnpc_soul_gem(Creature* creature) : ScriptedAI(creature) { }

    ObjectGuid _playerGUID;

    void Reset() override
    {   
        _playerGUID = ObjectGuid::Empty;
    }

    void GetTargets()
    {
        /*if (!me->GetOwner() || !me->GetOwner()->IsPlayer())
            return;*/

        std::list<Creature*> targets = me->FindAllCreaturesInRange(100.0f);

        if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
        {
            for (auto itr : targets)
            {
                if (itr->isDead())
                {
                    switch (itr->GetEntry())
                    {
                    case 90230:
                    case 90241:
                    case 93619:
                    case 101943:
                        me->CastSpell(itr, 178753, true);
                        me->CastSpell(itr, 178752, true);
                        itr->DespawnOrUnsummon();
                        player->KilledMonsterCredit(90298, ObjectGuid::Empty);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner->ToPlayer()) {
            _playerGUID = player->GetGUID();
            GetTargets();
        }
    }
};

enum eManaDrainedWhelpling
{
    NPC_AZUREWING_WHELPLING = 90336
};

// 90167
struct questnpc_mana_drained_whelpling : public ScriptedAI
{
    questnpc_mana_drained_whelpling(Creature* creature) : ScriptedAI(creature) { }

    void OnSpellClick(Unit* /*clicker*/, bool& /*result*/) override
    {
        me->GetScheduler().Schedule(1s, [](TaskContext context)
        {
            Creature* crea = GetContextCreature();
            crea->UpdateEntry(NPC_AZUREWING_WHELPLING);
            crea->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            crea->SetByteValue(UNIT_FIELD_BYTES_1, UNIT_BYTES_1_OFFSET_STAND_STATE, 0);
        });

        me->GetScheduler().Schedule(3s, [](TaskContext context)
        {
            GetContextCreature()->SetCanFly(true);
            GetContextCreature()->GetMotionMaster()->MoveTakeoff(0, Position(1162.338135f, 6816.301270f, 236.106567f));
        });

        me->GetScheduler().Schedule(10s, [](TaskContext context)
        {
            GetContextCreature()->DisappearAndDie();
        });
    }
};

const Position PrinceFarondisWaypoints[45] =
{
    { -139.941f, 6418.82f, 27.5658f, 4.11514f },
    { -120.835f, 6406.78f, 27.8503f, 0.0f },
    { -120.335f, 6402.53f, 27.8503f, 0.0f },
    { -122.835f, 6399.28f, 27.8503f, 0.0f },
    { -125.335f, 6391.53f, 27.8503f, 0.0f },
    { -123.085f, 6385.03f, 23.1003f, 0.0f },
    { -118.085f, 6382.03f, 18.8503f, 0.0f },
    { -109.085f, 6379.53f, 13.3503f, 0.0f },
    { -101.335f, 6382.53f, 7.6003f, 0.0f },
    { -100.444f, 6386.17f, 6.83712f, 0.0f },
    { -100.444f, 6386.17f, 6.83712f, 2.07324f },
    { -95.8294f, 6388.88f, 7.26227f, 0.0f },
    { -94.8294f, 6389.63f, 7.26227f, 0.0f },
    { -93.3294f, 6390.38f, 7.01227f, 0.0f },
    { -93.0794f, 6390.88f, 7.01227f, 0.0f },
    { -89.0794f, 6390.38f, 5.01227f, 0.0f },
    { -87.8294f, 6390.13f, 4.51227f, 0.0f },
    { -86.5794f, 6390.13f, 3.76227f, 0.0f },
    { -86.3717f, 6390.11f, 3.89118f, 0.0f },
    { -85.8717f, 6390.11f, 3.64118f, 0.0f },
    { -84.8717f, 6389.86f, 2.89118f, 0.0f },
    { -80.3717f, 6388.86f, 2.64118f, 0.0f },
    { -77.1217f, 6389.61f, 2.39118f, 0.0f },
    { -75.1217f, 6389.61f, 2.14118f, 0.0f },
    { -72.8843f, 6386.12f, 1.87715f, 0.0f },
    { -69.7899f, 6380.26f, 1.26035f, 5.40096f },
    { -69.7899f, 6380.26f, 1.26035f, 5.69111f },
    { -69.7899f, 6380.26f, 1.26035f, 5.5598f },
    { -70.8134f, 6374.45f, 1.88123f, 0.0f },
    { -76.7876f, 6324.35f, 2.20502f, 0.0f },
    { -77.9117f, 6303.3f, 1.67607f, 0.0f },
    { -78.1617f, 6297.3f, 0.926067f, 0.0f },
    { -78.4117f, 6293.3f, 0.676067f, 0.0f },
    { -79.8264f, 6279.31f, 1.2063f, 0.0f },
    { -72.6649f, 6266.31f, 3.10757f, 0.0f },
    { -70.3085f, 6262.07f, 3.71657f, 0.0f },
    { -57.191f, 6253.5f, 3.63329f, 0.0f },
    { -55.302f, 6254.65f, 3.8715f, 0.0f },
    { -53.552f, 6255.4f, 4.3715f, 0.0f },
    { -47.552f, 6258.15f, 6.8715f, 0.0f },
    { -46.302f, 6258.65f, 7.1215f, 0.0f },
    { -46.4121f, 6258.42f, 7.22652f, 0.0f },
    { -45.4121f, 6258.67f, 7.47652f, 0.0f },
    { -43.1621f, 6259.42f, 8.22652f, 0.0f },
    { -40.4149f, 6260.48f, 8.81002f, 0.0f },
};

enum PrinceEvents
{
    EVENT_FIREBALL,
    EVENT_METEOR_STORM,
    EVENT_WAYPOINT_REACHED,
};

enum PrinceCreatures
{
    NPC_PRINCE_FARONDIS = 88889,
    NPC_DROWNED_MAGISTER = 87368,
    NPC_UNBREATHING_SOUL = 87369,
};

enum PrinceSpells
{
    SPELL_FIREBALL = 178784,
    SPELL_METEOR_STORM = 179215,
    SPELL_DUEL = 52996,
    SPELL_DUEL_FLAG = 52991,
    SPELL_DUEL_VICTORY = 52994,
};

class npc_prince_farondis : public CreatureScript
{
public:
    npc_prince_farondis() : CreatureScript("npc_prince_farondis") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(37467) == QUEST_STATUS_INCOMPLETE) // The Walk of Shame
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Please, show me where the Tidestone lies.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        SendGossipMenuFor(player, 26247, creature->GetGUID());
        

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        ClearGossipMenuFor(player);

        switch (action)
        {
        case GOSSIP_ACTION_INFO_DEF + 1:
            if (Creature* prince = creature->SummonCreature(NPC_PRINCE_FARONDIS, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 300000))
            {
                PhasingHandler::AddPhase(prince, 174, true);
                PhasingHandler::AddPhase(player, 174, true);
            }
            break;
        }

        return true;
    }
};

// 88889 - Prince Farondis
class npc_prince_farondis_escort : public CreatureScript
{
public:
    npc_prince_farondis_escort() : CreatureScript("npc_prince_farondis_escort") { }

    struct npc_prince_farondis_escortAI : public ScriptedAI
    {
        npc_prince_farondis_escortAI(Creature* creature) : ScriptedAI(creature) {}

        bool paused;
        uint8 count;
        bool wp_reached;
        ObjectGuid targetGuid;
        ObjectGuid targetGuid2;
        ObjectGuid playerGuid;

        void Reset() override
        {
            paused = false;
            count = 0;
            wp_reached = false;
            targetGuid = ObjectGuid::Empty;
            targetGuid2 = ObjectGuid::Empty;
            playerGuid = ObjectGuid::Empty;
        }

        void IsSummonedBy(Unit* /*me*/) override
        {
            wp_reached = false;
            me->SetWalk(false);
            me->GetMotionMaster()->MovePoint(count, PrinceFarondisWaypoints[count]);
            me->SetSpeedRate(MOVE_RUN, 1.0f);
            events.ScheduleEvent(EVENT_WAYPOINT_REACHED, 1000);
            if (Player* player = me->SelectNearestPlayer(10.0f))
                playerGuid = player->GetGUID();
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE || id != count || paused)
                return;

            switch (id)
            {
            case 0:
                Talk(0);
                break;

            case 10:
                if (Creature* summon = me->SummonCreature(NPC_DROWNED_MAGISTER, -120.073f, 6400.717f, 6.92f, 5.64f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                {
                    EnterCombat(summon);
                    me->SetInCombatWith(summon);
                    summon->SetMaxHealth(1009570);
                    summon->SetHealth(1009570);
                    summon->setFaction(14);
                    me->AI()->AttackStart(summon);
                    summon->GetMotionMaster()->MoveChase(me, 20.0f, summon->GetOrientation());
                    targetGuid = summon->GetGUID();
                    summon->Say("Farondis! You traitor! You brought this upon us!", LANG_UNIVERSAL);
                    me->GetMotionMaster()->Clear();
                    me->SetFacingToObject(summon, true);
                }
                break;

            case 11:
                Talk(1);
                break;

            case 26:
                if (Creature* summon = me->SummonCreature(NPC_UNBREATHING_SOUL, -63.1638f, 6379.03f, 1.200768f, 0.0f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                {
                    EnterCombat(summon);
                    me->SetInCombatWith(summon);
                    summon->SetMaxHealth(1196528);
                    summon->SetHealth(1196528);
                    summon->setFaction(14);
                    me->AI()->AttackStart(summon);
                    summon->GetMotionMaster()->MoveChase(me, 20.0f, summon->GetOrientation());
                    targetGuid2 = summon->GetGUID();
                    summon->Yell("DIE... FARONDIS....", LANG_UNIVERSAL);
                    me->GetMotionMaster()->Clear();
                    me->SetFacingToObject(summon, true);
                }
                break;

            case 44:
                Talk(2);
                if (Player* player = ObjectAccessor::GetPlayer(*me, playerGuid))
                {
                    player->KilledMonsterCredit(88746, ObjectGuid::Empty);
                    PhasingHandler::AddPhase(player, 174, true);
                    PhasingHandler::AddPhase(player, 169, true);
                }

                me->DespawnOrUnsummon();
                break;

            default:
                break;
            }

            if (id < 45)
            {
                if (!paused)
                {
                    ++count;
                    wp_reached = true;
                }
            }
        }

        void EnterCombat(Unit* /*who*/) override
        {
            if (!paused)
            {
                events.ScheduleEvent(EVENT_FIREBALL, 1000);
                events.ScheduleEvent(EVENT_METEOR_STORM, 12000);
                paused = true;
            }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_FIREBALL:
            {
                if (Unit* target = me->GetVictim())
                    me->CastSpell(target, SPELL_FIREBALL, true);

                events.ScheduleEvent(EVENT_FIREBALL, 5000);
                break;
            }

            case EVENT_METEOR_STORM:
            {
                if (Unit* target = me->GetVictim())
                    me->CastSpell(target, SPELL_METEOR_STORM, true);

                me->GetSpellHistory()->ResetAllCooldowns();
                events.ScheduleEvent(EVENT_METEOR_STORM, 2000);
                break;
            }

            case EVENT_WAYPOINT_REACHED:
            {
                if (targetGuid != ObjectGuid::Empty)
                {
                    if (Unit* target = ObjectAccessor::GetUnit(*me, targetGuid))
                    {
                        if (!target->IsAlive() && paused)
                        {
                            events.CancelEvent(EVENT_FIREBALL);
                            events.CancelEvent(EVENT_METEOR_STORM);
                            ++count;
                            paused = false;
                            wp_reached = true;
                            targetGuid = ObjectGuid::Empty;
                        }
                    }
                }

                if (targetGuid2 != ObjectGuid::Empty)
                {
                    if (Unit* target = ObjectAccessor::GetUnit(*me, targetGuid2))
                    {
                        if (!target->IsAlive() && paused)
                        {
                            events.CancelEvent(EVENT_FIREBALL);
                            events.CancelEvent(EVENT_METEOR_STORM);
                            ++count;
                            paused = false;
                            wp_reached = true;
                            targetGuid2 = ObjectGuid::Empty;
                        }
                    }
                }

                if (wp_reached && !paused)
                {
                    me->GetMotionMaster()->MovePoint(count, PrinceFarondisWaypoints[count]);
                    wp_reached = false;
                    me->SetWalk(false);
                    me->SetSpeedRate(MOVE_RUN, 1.0f);
                }

                events.ScheduleEvent(EVENT_WAYPOINT_REACHED, 1000);
                break;
            }
            }

            DoMeleeAttackIfReady();
        }

    protected:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_prince_farondis_escortAI(creature);
    }
};


// 250361
class go_sabotaged_portal_stabilizer : public GameObjectScript
{
public:
    go_sabotaged_portal_stabilizer() : GameObjectScript("go_sabotaged_portal_stabilizer") { }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        player->CastSpell(go, 6478, true);
        player->KillCreditGO(250361, go->GetGUID());

        return true;
    }
};

enum
{
    NPC_WALPLAUZE_BESIEGT = 89050,
    QUEST_42159 = 42159,
    FACTION_HOSTILE = 2068,
};

// 214482 - Radiant Ley Crystal
class spell_gen_radiant_ley_crystal : public SpellScriptLoader
{
public:
    spell_gen_radiant_ley_crystal() : SpellScriptLoader("spell_gen_radiant_ley_crystal") { }

    class spell_gen_radiant_ley_crystal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_radiant_ley_crystal_SpellScript);

        SpellCastResult CheckRequirement()
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* target = ObjectAccessor::GetUnit(*caster, GetCaster()->GetTarget()))
                {
                    if (target->GetTypeId() == TYPEID_UNIT)
                    {
                        switch (target->GetEntry())
                        {
                        case 107961:
                            caster->ToPlayer()->KilledMonsterCredit(107961, ObjectGuid::Empty);
                            return SPELL_CAST_OK;
                        case 107962:
                            caster->ToPlayer()->KilledMonsterCredit(107962, ObjectGuid::Empty);
                            return SPELL_CAST_OK;
                        case 107963:
                            caster->ToPlayer()->KilledMonsterCredit(107963, ObjectGuid::Empty);
                            return SPELL_CAST_OK;
                        case 107964:
                            caster->ToPlayer()->KilledMonsterCredit(107964, ObjectGuid::Empty);
                            return SPELL_CAST_OK;
                        default:
                            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                        }
                    }
                }
            }

            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_gen_radiant_ley_crystal_SpellScript::CheckRequirement);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_gen_radiant_ley_crystal_SpellScript();
    }
};

// 211546 - Word of Versatility
class spell_word_of_versatility : public SpellScriptLoader
{
public:
    spell_word_of_versatility() : SpellScriptLoader("spell_word_of_versatility") {}

    class spell_word_of_versatility_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_word_of_versatility_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
                if (Unit* target = GetHitUnit())
                    if (target->GetTypeId() == TYPEID_UNIT)
                        switch (target->GetEntry())
                        {
                        case 89278:
                        case 89969:
                            caster->ToPlayer()->KilledMonsterCredit(106642, ObjectGuid::Empty);
                            break;
                        default:
                            break;
                        }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_word_of_versatility_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_word_of_versatility_SpellScript();
    }
};

class npc_quest_43521 : public CreatureScript
{
public:
    npc_quest_43521() : CreatureScript("npc_quest_43521") { }

    bool OnQuestReward(Player* player, Creature* creature, const Quest *_Quest, uint32 /*slot*/) override
    {
        if (_Quest->GetQuestId() == 43521)
        {
            if (AchievementEntry const *ForgedforBattle = sAchievementStore.LookupEntry(10746))
            {
                player->CompletedAchievement(ForgedforBattle);
            }
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_quest_43521AI(creature);
    }

    struct npc_quest_43521AI : public ScriptedAI
    {
        npc_quest_43521AI(Creature* creature) : ScriptedAI(creature) { }
    };
};

class npc_quest_43520 : public CreatureScript
{
public:
    npc_quest_43520() : CreatureScript("npc_quest_43520") { }

    bool OnQuestReward(Player* player, Creature* creature, const Quest *_Quest, uint32 /*slot*/) override
    {
        if (_Quest->GetQuestId() == 43520)
        {
            if (AchievementEntry const *ForgedforBattle = sAchievementStore.LookupEntry(11144))
            {
                player->CompletedAchievement(ForgedforBattle);
            }
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_quest_43520AI(creature);
    }

    struct npc_quest_43520AI : public ScriptedAI
    {
        npc_quest_43520AI(Creature* creature) : ScriptedAI(creature) { }
    };
};

// quest local
// 210543 - Rally the Nightwatchers
class spell_rally_the_nightwatchers : public SpellScript
{
    PrepareSpellScript(spell_rally_the_nightwatchers);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {

        if (Unit* caster = GetCaster())
            if (Unit* target = GetHitUnit())
                if (target->GetTypeId() == TYPEID_UNIT)
                    if (target->GetEntry() == 88782)
                        caster->ToPlayer()->KilledMonsterCredit(106273, ObjectGuid::Empty);
    }

    void Register()
    {
        OnEffectHitTarget += SpellEffectFn(spell_rally_the_nightwatchers::HandleDummy, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
    }
};

class ps_quest_rally_the_nightwatchers : public PlayerScript
{
public:
    ps_quest_rally_the_nightwatchers() : PlayerScript("ps_quest_rally_the_nightwatchers") { }

    void OnQuestAccept(Player* player, Quest const* quest)
    {
        if (quest->GetQuestId() == 42108)
            player->CastSpell(player, 210554, true);
    }

    void OnUpdateArea(Player* player, Area* newArea, Area* /*oldArea*/)
    {
        switch (newArea->GetId())
        {
        case 7357:
        case 7355:
            if (player->HasQuest(42108))
                player->CastSpell(player, 210554, true);
            break;
        default:
            break;
        }
    }
};

//QQQ
class merayl_q42159 : public CreatureScript
{
public:
    merayl_q42159(const std::string str) : CreatureScript(str.c_str()) { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        ClearGossipMenuFor(player);
        if (action == GOSSIP_ACTION_INFO_DEF)
        {
            CloseGossipMenuFor(player);
            if (player->IsInCombat() || creature->IsInCombat())
                return true;

            if (merayl_q42159AI* pInitiateAI = CAST_AI(merayl_q42159::merayl_q42159AI, creature->AI()))
            {
                if (pInitiateAI->m_bIsDuelInProgress)
                    return true;
            }
            creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_15);

            player->CastSpell(creature, SPELL_DUEL, false);
            player->CastSpell(player, SPELL_DUEL_FLAG, true);
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if ((player->GetQuestStatus(QUEST_42159) == QUEST_STATUS_INCOMPLETE ||
            player->GetQuestStatus(QUEST_42159) == QUEST_STATUS_INCOMPLETE)
            && creature->IsFullHealth())
        {
            if (player->HealthBelowPct(10))
                return true;

            if (player->IsInCombat() || creature->IsInCombat())
                return true;

            AddGossipItemFor(player, GOSSIP_ICON_CHAT, GOSSIP_ACCEPT_DUEL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            SendGossipMenuFor(player, player->GetGossipTextId(creature), creature->GetGUID());
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new merayl_q42159AI(creature);
    }

    struct merayl_q42159AI : public CombatAI
    {
        merayl_q42159AI(Creature* creature) : CombatAI(creature)
        {
            m_bIsDuelInProgress = false;
        }

        bool lose;
        ObjectGuid m_uiDuelerGUID;
        uint32 m_uiDuelTimer;
        bool m_bIsDuelInProgress;
        uint32 spelltimer;

        void Reset() override
        {
            lose = false;
            me->RestoreFaction();
            CombatAI::Reset();

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_15);

            m_uiDuelerGUID.Clear();
            m_uiDuelTimer = 5000;
            spelltimer = 2000;
            m_bIsDuelInProgress = false;
        }

        void SpellHit(Unit* pCaster, const SpellInfo* spell) override
        {
            if (!m_bIsDuelInProgress && spell->Id == SPELL_DUEL)
            {
                m_uiDuelerGUID = pCaster->GetGUID();
                m_bIsDuelInProgress = true;
            }
        }

        void DamageTaken(Unit* pDoneBy, uint32 &uiDamage/*, DamageEffectType dmgType*/) override
        {
            if (m_bIsDuelInProgress && pDoneBy->IsControlledByPlayer())
            {
                if (pDoneBy->GetGUID() != m_uiDuelerGUID && pDoneBy->GetOwnerGUID() != m_uiDuelerGUID) // other players cannot help
                    uiDamage = 0;
                else if (uiDamage >= me->GetHealth())
                {
                    uiDamage = 0;

                    if (!lose)
                    {
                        if (Player* plr = pDoneBy->ToPlayer())
                            plr->KilledMonsterCredit(106552);

                        pDoneBy->RemoveGameObject(SPELL_DUEL_FLAG, true);
                        pDoneBy->AttackStop();
                        me->CastSpell(pDoneBy, SPELL_DUEL_VICTORY, true);
                        lose = true;
                        me->CastSpell(me, 7267, true);
                        me->RestoreFaction();
                    }
                }
            }
        }

        void UpdateAI(uint32 uiDiff) override
        {
            if (!UpdateVictim())
            {
                if (m_bIsDuelInProgress)
                {
                    if (m_uiDuelTimer <= uiDiff)
                    {
                        me->setFaction(FACTION_HOSTILE);

                        if (Unit* unit = ObjectAccessor::GetUnit(*me, m_uiDuelerGUID))
                            AttackStart(unit);
                    }
                    else
                        m_uiDuelTimer -= uiDiff;
                }
                return;
            }

            if (m_bIsDuelInProgress)
            {
                if (lose)
                {
                    if (!me->HasAura(7267))
                        EnterEvadeMode();
                    return;
                }
                else if (me->GetVictim() && me->GetVictim()->GetTypeId() == TYPEID_PLAYER && me->GetVictim()->HealthBelowPct(10))
                {
                    me->GetVictim()->CastSpell(me->GetVictim(), 7267, true); // beg
                    me->GetVictim()->RemoveGameObject(SPELL_DUEL_FLAG, true);
                    EnterEvadeMode();
                    return;
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (spelltimer <= uiDiff)
                {
                    uint32 spell = 0;
                    switch (me->GetEntry())
                    {
                    case 108752:
                        spell = 172673;
                        spelltimer = 2000;
                        break;
                    case 108765:
                        spell = 198623;
                        spelltimer = 2000;
                        break;
                    case 108767:
                    case 108750:
                        spell = 172757;
                        spelltimer = 11000;
                        break;
                    case 108723:
                        spell = 171777;
                        spelltimer = 5500;
                        break;
                    default:
                        break;
                    }
                    if (spell > 0)
                        DoCast(spell);
                }
                else
                    spelltimer -= uiDiff;
            }

            // TODO: spells

            CombatAI::UpdateAI(uiDiff);
        }
    };
};

class npc_q44281 : public merayl_q42159
{
public:
    npc_q44281() : merayl_q42159("npc_q44281") { }
};

// quest 37538
class npc_kimmruder_88911 : public CreatureScript
{
public:
    npc_kimmruder_88911() : CreatureScript("npc_kimmruder_88911") { }
     struct npc_kimmruder_88911AI : public ScriptedAI
    {
        npc_kimmruder_88911AI(Creature* creature) : ScriptedAI(creature) { }
         void MoveInLineOfSight(Unit* who) override
        {
            if (Player* player = who->ToPlayer())
            {
                if (player->GetQuestStatus(37538) == QUEST_STATUS_INCOMPLETE)
                {    
                    if (player->IsInDist(me, 28.0f))
                    {
                       if (!me->FindNearestCreature(89056, 50.0f, true))
				        {
                           if (!me->FindNearestCreature(89050, 50.0f, true))
                       
					        {
                               me->SummonCreature(89056, Position(-356.077f, 6655.04f, 0.539664f, 0.215275f), TEMPSUMMON_MANUAL_DESPAWN);
                            }
					    }
                    }
                }
            }
        }
    };
     CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_kimmruder_88911AI(creature);
    }
};

// quest 38015
struct cedonu_107962 : public ScriptedAI
{
    cedonu_107962(Creature* creature) : ScriptedAI(creature) { me->SetAIAnimKitId(0); }

    void OnSpellClick(Unit* clicker, bool& /*result*/)
    {
        if (Player* player = clicker->ToPlayer())
        {
            if (player->GetQuestStatus(38015) == QUEST_STATUS_INCOMPLETE)
            {
                me->SetAIAnimKitId(4061);
                player->KilledMonsterCredit(107962);
            }
		}
	}
};

struct kharmeera_107963 : public ScriptedAI
{
    kharmeera_107963(Creature* creature) : ScriptedAI(creature) { me->SetAIAnimKitId(0); }

    void OnSpellClick(Unit* clicker, bool& /*result*/)
    {
        if (Player* player = clicker->ToPlayer())
        {
            if (player->GetQuestStatus(38015) == QUEST_STATUS_INCOMPLETE)
            {
                me->SetAIAnimKitId(4061);
                player->KilledMonsterCredit(107963);
            }
		}
	}
};

struct emmigosa_107961 : public ScriptedAI
{
    emmigosa_107961(Creature* creature) : ScriptedAI(creature) { me->SetAIAnimKitId(0); }

    void OnSpellClick(Unit* clicker, bool& /*result*/)
    {
        if (Player* player = clicker->ToPlayer())
        {
            if (player->GetQuestStatus(38015) == QUEST_STATUS_INCOMPLETE)
            {
                me->SetAIAnimKitId(4061);
                player->KilledMonsterCredit(107961);
            }
		}
	}
};

struct berazus_107964 : public ScriptedAI
{
    berazus_107964(Creature* creature) : ScriptedAI(creature) { me->SetAIAnimKitId(0); }

    void OnSpellClick(Unit* clicker, bool& /*result*/)
    {
        if (Player* player = clicker->ToPlayer())
        {
            if (player->GetQuestStatus(38015) == QUEST_STATUS_INCOMPLETE)
            {
                me->SetAIAnimKitId(4061);
                player->KilledMonsterCredit(107964);
            }
		}
	}
};

// quest 42756
class npc_summon_91155 : public CreatureScript
{
public:
    npc_summon_91155() : CreatureScript("npc_summon_91155") { }
     struct npc_summon_91155AI : public ScriptedAI
    {
        npc_summon_91155AI(Creature* creature) : ScriptedAI(creature) { }
         void MoveInLineOfSight(Unit* who) override
        {
            if (Player* player = who->ToPlayer())
            {
                if (player->GetQuestStatus(42756) == QUEST_STATUS_INCOMPLETE)
                {    
                    if (player->IsInDist(me, 28.0f))
                    {
                           if (!me->FindNearestCreature(91155, 40.0f, true))
							   
						{
							 if (!me->FindNearestCreature(108721, 40.0f, true)) 
                       
					        {
                               me->SummonCreature(91155, Position(620.386f, 6650.63f, 60.0061f, 1.01631f), TEMPSUMMON_MANUAL_DESPAWN);
                            }
						}
                    }
                }
            }
        }
    };
     CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_summon_91155AI(creature);
    }
};

class spell_Wand_Practice : public SpellScript
{
    PrepareSpellScript(spell_Wand_Practice);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {

        if (Unit* caster = GetCaster())
            if (Unit* target = GetHitUnit())
                if (target->GetTypeId() == TYPEID_UNIT)
                    if (target->GetEntry() == 107279)
                    {
                        caster->ToPlayer()->KilledMonsterCredit(107279, ObjectGuid::Empty);
                    }

    }

    void Register()
    {
        OnEffectHitTarget += SpellEffectFn(spell_Wand_Practice::HandleDummy, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
    }
};

class ps_quest_Wandering : public PlayerScript
{
public:
    ps_quest_Wandering() : PlayerScript("ps_quest_Wanderings") { }

    void OnQuestAccept(Player* player, Quest const* quest)
    {
        if (quest->GetQuestId() == 42370)
            player->CastSpell(player, 212782, true);
    }

    void OnUpdateArea(Player* player, Area* newArea, Area* /*oldArea*/)
    {
        switch (newArea->GetId())
        {
        case 7358:
            if (player->HasQuest(42370))
                player->CastSpell(player, 212782, true);
            break;
        default:
            break;
        }
    }
};

void AddSC_azsuna()
{
    new scene_azsuna_runes();
    RegisterCreatureAI(questnpc_soul_gem);
    RegisterCreatureAI(questnpc_mana_drained_whelpling);
    new npc_prince_farondis();
    new npc_prince_farondis_escort();
    new spell_word_of_versatility();
    new go_sabotaged_portal_stabilizer();
    new spell_gen_radiant_ley_crystal();
    new npc_quest_43521();
    new npc_quest_43520();
    new npc_archmage_khadgar_93337();
    RegisterSpellScript(spell_rally_the_nightwatchers);
    new ps_quest_rally_the_nightwatchers();
    new merayl_q42159("merayl_q42159");
    new npc_kimmruder_88911();
    new npc_summon_91155();
    RegisterCreatureAI(cedonu_107962);
    RegisterCreatureAI(kharmeera_107963);
    RegisterCreatureAI(emmigosa_107961);
    RegisterCreatureAI(berazus_107964);
    RegisterSpellScript(spell_Wand_Practice);
    new ps_quest_Wandering();
}
