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


#include "ScriptedEscortAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "QuestPackets.h"
#include "ScenePackets.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "ObjectAccessor.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "MotionMaster.h"
#include "WorldSession.h"
#include "PhasingHandler.h"

enum eVaultOfWardens
{
    // Bastillax
    EVENT_FEL_ANNIHILATION        = 0,
    EVENT_CRUSHING_SHADOWS        = 1,
    EVENT_BLUR_OF_SHADOWS         = 2,

    SPELL_FEL_ANNIHILATION        = 200007,
    SPELL_CRUSHING_SHADOWS        = 200027,
    SPELL_BLUR_OF_SHADOWS         = 200002,

    // Immolanth
    EVENT_BURNING_FEL             = 0,
    EVENT_CHAOS_NOVA              = 1,

    SPELL_BURNING_FEL             = 199758,
    SPELL_CHAOS_NOVA              = 199828,
};

 // 197180
struct npc_vault_of_the_wardens_sledge_or_crusher : public ScriptedAI
{
    npc_vault_of_the_wardens_sledge_or_crusher(Creature* creature) : ScriptedAI(creature) { }

    enum Quest
    {
        QUEST_STOP_GULDAN_DMG_SPEC = 38723,
        QUEST_STOP_GULDAN_TANK_SPEC = 40253,
    };

    void EnterCombat(Unit*) override
    {

    }

    void JustDied(Unit* /*killer*/) override
    {
        std::list<Player*> players;
        me->GetPlayerListInGrid(players, 50.0f);

        for (Player* player : players)
        {
            player->ForceCompleteQuest(QUEST_STOP_GULDAN_DMG_SPEC);
            player->ForceCompleteQuest(QUEST_STOP_GULDAN_TANK_SPEC);
        }
    }
};

class npc_vault_of_the_wardens_vampiric_felbat : public npc_escortAI
{
public:
    npc_vault_of_the_wardens_vampiric_felbat(Creature* creature) : npc_escortAI(creature)
    {
        me->SetCanFly(true);
        me->SetSpeed(MOVE_FLIGHT, 26);
        me->SetReactState(REACT_PASSIVE);
        me->SetMovementAnimKitId(3);
        me->SetSpeed(MOVE_FLIGHT, 75);
    }

    void OnCharmed(bool /*apply*/) override
    {
        // Make sure the basic cleanup of OnCharmed is done to avoid looping problems
        if (me->NeedChangeAI)
            me->NeedChangeAI = false;
    }

    void LastWaypointReached()
    {
        if (Player* rider = me->GetOwner()->ToPlayer())
        {
            rider->KilledMonsterCredit(96659);
        }
    }

    void EnterEvadeMode(EvadeReason /*why*/) override { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (summoner)
        {
            //me->GetScheduler().Schedule(1s, [this, summoner](TaskContext /*context*/));
            summoner->CastSpell(me, 46598);
        }
    }



    void PassengerBoarded(Unit* who, int8 seatId, bool apply) override
    {
        Start(false, true, who->GetGUID());
    }
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

class npc_immolanth : public CreatureScript
{
public:
    npc_immolanth() : CreatureScript("npc_immolanth") { }

    struct npc_immolanthAI : public ScriptedAI
    {
        npc_immolanthAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            events.Reset();
        }

        void JustDied(Unit* killer) override
        {
            if (killer->GetTypeId() == TYPEID_PLAYER)
                killer->ToPlayer()->KilledMonsterCredit(106254, ObjectGuid::Empty);
        }

        void EnterCombat(Unit* who) override
        {
            events.ScheduleEvent(EVENT_BURNING_FEL, 8s, 10s);
            events.ScheduleEvent(EVENT_CHAOS_NOVA, 18s, 20s);
        }

        void UpdateAI(const uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_BURNING_FEL:
                        if (Unit* target = me->GetVictim())
                            me->CastSpell(me, SPELL_BURNING_FEL, true);

                        events.ScheduleEvent(EVENT_BURNING_FEL, 8s, 10s);
                        break;
                    case EVENT_CHAOS_NOVA:
                        if (Unit* target = me->GetVictim())
                            me->CastSpell(target, SPELL_CHAOS_NOVA, true);

                        events.ScheduleEvent(EVENT_CHAOS_NOVA, 18s);
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
        return new npc_immolanthAI(creature);
    }
};

// 96665 kayn
class npc_khadgar : public CreatureScript
{
public:
    npc_khadgar() : CreatureScript("npc_khadgar") { }

    bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*opt*/) override
    {
        if (quest->GetQuestId() == 39690)
            player->CastSpell(player, 192758, true);

        if (quest->GetQuestId() == 39689)
            player->CastSpell(player, 192757, true);

        return true;
    }
};

// 92986 Altruis
class npc_altruis : public CreatureScript
{
public:
    npc_altruis() : CreatureScript("npc_altruis") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == 38689) // Fel Infusion
            player->CastSpell(player, 133508, true);

        return true;
    }

    bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*opt*/) override
    {
        if (quest->GetQuestId() == 38689) // Fel Infusion
            player->RemoveAurasDueToSpell(133508);

        return true;
    }
};

class npc_altruis_cell : public CreatureScript
{
public:
    npc_altruis_cell() : CreatureScript("npc_altruis_cell") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        player->CastSpell(creature, 184012, true);
        player->KilledMonsterCredit(112287, ObjectGuid::Empty);
        player->KilledMonsterCredit(112277, ObjectGuid::Empty);

        return true;
    }
};

class npc_kayn_cell : public CreatureScript
{
public:
    npc_kayn_cell() : CreatureScript("npc_kayn_cell") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        player->CastSpell(creature, 177803, true);
        player->KilledMonsterCredit(99326, ObjectGuid::Empty);
        player->KilledMonsterCredit(112276, ObjectGuid::Empty);
        return true;
    }
};

// 97644
class npc_korvas_2 : public CreatureScript
{
public:
    npc_korvas_2() : CreatureScript("npc_korvas_2") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Kayn", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Altruis", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        SendGossipMenuFor(player, player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();

        switch (action)
        {
        case GOSSIP_ACTION_INFO_DEF:
            player->CastSpell(player, 196661, true);
            CloseGossipMenuFor(player);
            break;
        case GOSSIP_ACTION_INFO_DEF + 1:
            player->CastSpell(player, 196662, true);
            CloseGossipMenuFor(player);
            break;
        }

        return true;
    }
};

// 243967 Illidari Banner
class go_pool_of_judgements : public GameObjectScript
{
public:
    go_pool_of_judgements() : GameObjectScript("go_pool_of_judgements") { }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        player->KilledMonsterCredit(100166, ObjectGuid::Empty);
        player->SendMovieStart(478);
        return true;
    }
};

// 244644 Warden's Ascent
class go_warden_ascent : public GameObjectScript
{
public:
    go_warden_ascent() : GameObjectScript("go_warden_ascent") { }

    struct go_warden_ascentAI : public GameObjectAI
    {
        go_warden_ascentAI(GameObject* pGO) : GameObjectAI(pGO)
        {
        }

        uint32 giveKillCredit;

        void Reset()
        {
            giveKillCredit = 1000;
        }

        void UpdateAI(uint32 diff)
        {
            if (Player* player = go->FindNearestPlayer(5.0f))
            {
                if (player->GetQuestStatus(39686) == QUEST_STATUS_INCOMPLETE) // Arriba del todo
                {
                    if (giveKillCredit <= diff)
                    {
                        if (player->GetPositionZ() >= 253.0f)
                            player->KilledMonsterCredit(96814, ObjectGuid::Empty);

                        giveKillCredit = 1000;
                    }
                    else
                        giveKillCredit -= diff;
                }
            }
        }
    };

    GameObjectAI* GetAI(GameObject* pGO) const
    {
        return new go_warden_ascentAI(pGO);
    }
};

// 204588
class spell_activate_countermeasure : public SpellScriptLoader
{
public:
    spell_activate_countermeasure() : SpellScriptLoader("spell_activate_countermeasure") { }

    class spell_activate_countermeasure_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_activate_countermeasure_SpellScript);

        void HandleKillCredit()
        {
            if (Creature* creature = GetCaster()->ToPlayer()->FindNearestCreature(99732, 5.0f))
                GetCaster()->ToPlayer()->KilledMonsterCredit(99732, ObjectGuid::Empty);

            if (Creature* creature = GetCaster()->ToPlayer()->FindNearestCreature(99731, 5.0f))
                GetCaster()->ToPlayer()->KilledMonsterCredit(99731, ObjectGuid::Empty);

            if (Creature* creature = GetCaster()->ToPlayer()->FindNearestCreature(99709, 5.0f))
                GetCaster()->ToPlayer()->KilledMonsterCredit(99709, ObjectGuid::Empty);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_activate_countermeasure_SpellScript::HandleKillCredit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_activate_countermeasure_SpellScript();
    }
};

// 96665 kayn
class npc_kayn_3 : public CreatureScript
{
public:
    npc_kayn_3() : CreatureScript("npc_kayn_3") { }

    bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*opt*/) override
    {
        if (quest->GetQuestId() == 38690)
        {
            if (GameObject* go = player->FindNearestGameObject(245467, 200.0f))
                go->UseDoorOrButton();

            if (GameObject* go = player->FindNearestGameObject(244404, 200.0f))
                go->UseDoorOrButton();
        }
        return true;
    }
};

enum BarbarusExtermineitor
{
    // Events
    EVENT_ANNIHILATE = 0,
    EVENT_BRUTAL_ATTACKS = 1,
    EVENT_SHOULDER_CHARGE = 2,
    EVENT_LEAPING_RETREAT = 3,

    // Spells
    SPELL_ANNIHILATE = 199604,
    SPELL_BRUTAL_ATTACKS = 199556,
    SPELL_SHOULDER_CHARGE = 199476,
    SPELL_LEAPING_RETREAT = 199474,
};

class npc_extermineitor : public CreatureScript
{
public:
    npc_extermineitor() : CreatureScript("npc_extermineitor") { }

    struct npc_extermineitorAI : public ScriptedAI
    {
        npc_extermineitorAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void Reset() override
        {
            events.Reset();
        }

        void JustDied(Unit* killer) override
        {
            if (killer->GetTypeId() == TYPEID_PLAYER)
                killer->ToPlayer()->KilledMonsterCredit(99303, ObjectGuid::Empty);
        }

        void EnterCombat(Unit* who) override
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
                who->ToPlayer()->KilledMonsterCredit(99303, ObjectGuid::Empty);

            me->KillSelf();
            events.ScheduleEvent(EVENT_ANNIHILATE, urand(35000, 40000));
            events.ScheduleEvent(EVENT_BRUTAL_ATTACKS, urand(3000, 6000));
            events.ScheduleEvent(EVENT_SHOULDER_CHARGE, urand(12000, 14000));
            events.ScheduleEvent(EVENT_LEAPING_RETREAT, urand(15000, 22000));
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_ANNIHILATE:
                    if (Unit* target = me->GetVictim())
                        me->CastSpell(me, SPELL_ANNIHILATE, true);

                    events.ScheduleEvent(EVENT_ANNIHILATE, 36000);
                    break;
                case EVENT_BRUTAL_ATTACKS:
                    if (Unit* target = me->GetVictim())
                        me->CastSpell(target, SPELL_BRUTAL_ATTACKS, true);

                    events.ScheduleEvent(EVENT_BRUTAL_ATTACKS, urand(8000, 10000));
                    break;
                case EVENT_LEAPING_RETREAT:
                    if (Unit* target = me->GetVictim())
                        me->CastSpell(target, SPELL_LEAPING_RETREAT, true);

                    events.ScheduleEvent(EVENT_LEAPING_RETREAT, urand(1500, 22000));
                    break;
                case EVENT_SHOULDER_CHARGE:
                    if (Unit* target = me->GetVictim())
                        me->CastSpell(target, SPELL_SHOULDER_CHARGE, true);

                    events.ScheduleEvent(EVENT_SHOULDER_CHARGE, urand(10000, 12000));
                    break;
                }
            }

            //if (UpdateVictim())
            //DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_extermineitorAI(creature);
    }
};

class npc_barbarus : public CreatureScript
{
public:
    npc_barbarus() : CreatureScript("npc_barbarus") { }

    struct npc_barbarusAI : public ScriptedAI
    {
        npc_barbarusAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void Reset() override
        {
            events.Reset();
        }

        void JustDied(Unit* killer) override
        {
            if (killer->GetTypeId() == TYPEID_PLAYER)
                killer->ToPlayer()->KilledMonsterCredit(106241, ObjectGuid::Empty);
        }

        void EnterCombat(Unit* who) override
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
                who->ToPlayer()->KilledMonsterCredit(106241, ObjectGuid::Empty);

            me->KillSelf();
            /*events.ScheduleEvent(EVENT_ANNIHILATE, urand(20000, 30000));
            events.ScheduleEvent(EVENT_BRUTAL_ATTACKS, urand(3000, 6000));
            events.ScheduleEvent(EVENT_SHOULDER_CHARGE, urand(12000, 14000));
            events.ScheduleEvent(EVENT_LEAPING_RETREAT, urand(15000, 22000));*/
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_ANNIHILATE:
                    if (Unit* target = me->GetVictim())
                        me->CastSpell(me, SPELL_ANNIHILATE, true);

                    events.ScheduleEvent(EVENT_ANNIHILATE, 30000);
                    break;
                case EVENT_BRUTAL_ATTACKS:
                    if (Unit* target = me->GetVictim())
                        me->CastSpell(target, SPELL_BRUTAL_ATTACKS, true);

                    events.ScheduleEvent(EVENT_BRUTAL_ATTACKS, urand(8000, 10000));
                    break;
                case EVENT_LEAPING_RETREAT:
                    if (Unit* target = me->GetVictim())
                        me->CastSpell(target, SPELL_LEAPING_RETREAT, true);

                    events.ScheduleEvent(EVENT_LEAPING_RETREAT, urand(1500, 22000));
                    break;
                case EVENT_SHOULDER_CHARGE:
                    if (Unit* target = me->GetVictim())
                        me->CastSpell(target, SPELL_SHOULDER_CHARGE, true);

                    events.ScheduleEvent(EVENT_SHOULDER_CHARGE, urand(10000, 12000));
                    break;
                }
            }

            //if (UpdateVictim())
            //DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_barbarusAI(creature);
    }
};

class npc_fel_infusion : public CreatureScript
{
public:
    npc_fel_infusion() : CreatureScript("npc_fel_infusion") { }

    struct npc_fel_infusionAI : public ScriptedAI
    {
        npc_fel_infusionAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {

        }

        void Reset() override
        {
            Initialize();
            events.Reset();
        }

        void JustDied(Unit* killer) override
        {
            if (killer->GetTypeId() == TYPEID_PLAYER)
            {
                killer->ToPlayer()->SetPower(POWER_ALTERNATE_POWER, killer->GetPower(POWER_ALTERNATE_POWER) + 10);

                for (uint8 i = 0; i < 10; ++i)
                    killer->ToPlayer()->KilledMonsterCredit(89297, ObjectGuid::Empty);
            }
        }

        void EnterCombat(Unit* who) override
        {

        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            /*while (uint32 eventId = events.ExecuteEvent())
            {
            switch (eventId)
            {
            }
            }*/

            if (UpdateVictim())
                DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_fel_infusionAI(creature);
    }
};

void AddSC_zone_vault_of_wardens()
{
    new npc_bastillax();
    new npc_immolanth();
    RegisterCreatureAI(npc_vault_of_the_wardens_sledge_or_crusher);
    RegisterCreatureAI(npc_vault_of_the_wardens_vampiric_felbat);
    new npc_khadgar();
    new npc_altruis();
    new npc_kayn_cell();
    new npc_altruis_cell();
    new go_pool_of_judgements();
    new npc_fel_infusion();
    new npc_barbarus();
    new spell_activate_countermeasure();
    new npc_extermineitor();
    new npc_kayn_3();
    new go_warden_ascent();
    new npc_korvas_2();
}
