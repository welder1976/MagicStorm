#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "SpellScript.h"
#include "SpellMgr.h"
#include "Player.h"
#include "QuestPackets.h"
#include "ScenePackets.h"
#include "GameObjectAI.h"
#include "ObjectAccessor.h"
#include "SpellAuras.h"
#include "LFGMgr.h"
#include "MotionMaster.h"
#include "ObjectMgr.h"
#include "InstanceScript.h"
#include "CriteriaHandler.h"
#include "SceneMgr.h"
#include "Creature.h"
#include "GameObject.h"
#include "PhasingHandler.h"
#include "Log.h"
#include "Object.h"

enum
{
    NPC_KORVAS_BLOODTHORN_99343 = 99343,

    ///DH Quest
    QUEST_CALL_OF_THE_ILLIDARI_39261 = 39261,
    QUEST_CALL_OF_THE_ILLIDARI_39047 = 39047,

    QUEST_WEAPONS_OF_LEGEND_1 = 40816,
    QUEST_WEAPONS_OF_LEGEND_2 = 40814,
    QUEST_ONE_MORE_LEGEND = 44043,
    SPELL_WEAPONS_OF_LEGEND_PLAYER_CHOICE = 201092,
    PLAYER_CHOICE_DH_SELECTION = 255,

    ACTION_RESPONSEID_1 = 600, //Havoc
    ACTION_RESPONSEID_2 = 601, //Vengeance
    //Havoc
    QUEST_MAKING_ARRANGEMENTS_1 = 40819,
    QUEST_BY_ANY_MEANS_1 = 39051,
    QUEST_THE_HUNT_1 = 39247,

    QUEST_MAKING_ARRANGEMENTS_2 = 41120,
    QUEST_BY_ANY_MEANS_2 = 41121,
    QUEST_THE_HUNT_2 = 41119,
    //Vengeance
    QUEST_VENGEANCE_WILL_BE_OURS_1 = 40249,
    QUEST_VENGEANCE_WILL_BE_OURS_2 = 41863,
};

enum DataTypes
{
    DATA_STAGE_7 = 7,
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

struct npc_illidari_fel_bat_94324 : public ScriptedAI
{
    npc_illidari_fel_bat_94324(Creature* creature) : ScriptedAI(creature) { me->SetAIAnimKitId(0); }

    void OnSpellClick(Unit* clicker, bool& /*result*/)
    {
        if (Player* player = clicker->ToPlayer())
        {
            if (player->HasQuest(QUEST_THE_HUNT_1) || player->HasQuest(QUEST_THE_HUNT_2))
            {
                me->SetAIAnimKitId(4061);
                player->KilledMonsterCredit(94321);//52391
                player->AddConversationDelayedTeleport(3000, 1325, 1498, Position(1263.69f, 5236.659f, 93.531f, 2.73693f));
            }
            if (player->HasQuest(QUEST_VENGEANCE_WILL_BE_OURS_1) || player->HasQuest(QUEST_VENGEANCE_WILL_BE_OURS_2))
            {
                me->SetAIAnimKitId(4061);
                player->KilledMonsterCredit(99250);
                player->AddDelayedTeleport(2000, 1500, Position(-2379.679f, 174.2f, 3.5625f, 3.733872f));
            }
        }
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (Player* player = who->ToPlayer())
        {
            if (player->IsInDist(me, 2.9f))
            {
                me->SetAIAnimKitId(4061);
                player->KilledMonsterCredit(99250);
                player->AddDelayedTeleport(2000, 1500, Position(-2379.679f, 174.2f, 3.5625f, 3.733872f));
            }
        }
    }
};

class dh_playerchoice : public PlayerScript
{
public:
    dh_playerchoice() : PlayerScript("dh_playerchoice") { }

    void OnPlayerChoiceResponse(Player* player, uint32 choiceId, uint32 responseId)
    {
        if (choiceId != PLAYER_CHOICE_DH_SELECTION)
            return;
        uint32 quest = 0;
        
        if (player->HasQuest(QUEST_WEAPONS_OF_LEGEND_1))
            player->KilledMonsterCredit(105177);
        if (player->HasQuest(QUEST_WEAPONS_OF_LEGEND_2))
            player->KilledMonsterCredit(105177);
        if (player->HasQuest(QUEST_ONE_MORE_LEGEND))
            player->KilledMonsterCredit(112362);


        switch (responseId)
        {
        case ACTION_RESPONSEID_1: //Havoc
            quest = player->IsInHorde() ? QUEST_MAKING_ARRANGEMENTS_1 : QUEST_MAKING_ARRANGEMENTS_2;
            if (const Quest* quest_1 = sObjectMgr->GetQuestTemplate(quest))
                player->AddQuest(quest_1, nullptr);
            break;
        case ACTION_RESPONSEID_2://Vengeance
            quest = player->IsInHorde() ? QUEST_VENGEANCE_WILL_BE_OURS_1 : QUEST_VENGEANCE_WILL_BE_OURS_2;
            if (const Quest* quest_2 = sObjectMgr->GetQuestTemplate(quest))
                player->AddQuest(quest_2, nullptr);
            break;
        default:
            break;
        }
    }
};

enum Spells
{
    SPELL_CALL_OF_THE_WARBLADES  = 208464,
    SPELL_FELLSOUL_SLAM  = 216164,

    SPELL_SIGIL_OF_POWER  = 216228,

    SPELL_SOUL_CARVER   = 216188

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

        void EnterEvadeMode(EvadeReason /*why*/) override
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
		
		void JustDied(Unit* /*killer*/) override
        {
             _JustDied();
			 me->SummonCreature(105155, Position(-1019.01f, 2162.13f, 38.4897f, 4.55463f), TEMPSUMMON_MANUAL_DESPAWN);
             if (GameObject* go = me->SummonGameObject(248785,-1019.01f, 2162.13f, 38.4897f, 4.55463f, QuaternionData(), 1))
			 return;
        }

    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new caria_felsoulAI(creature);
    }
};

//248785
class go_aldrachi_warblades_248785 : public GameObjectScript
{
public:
    go_aldrachi_warblades_248785() : GameObjectScript("go_aldrachi_warblades_248785")
    {
        isloot = false;
    }

    void OnLootStateChanged(GameObject* go, uint32 state, Unit* unit)
    {
        if (state == GO_ACTIVATED && unit && !isloot)
        {
            isloot = true;
            go->GetScheduler().Schedule(5s, [this, go](TaskContext /*context*/)
            {
                if (InstanceScript* instance = go->GetInstanceScript())
                {
                    instance->DoPlayScenePackageIdOnPlayers(1601);
                    if (instance->GetData(DATA_STAGE_7) == NOT_STARTED)
                        instance->SetData(DATA_STAGE_7, DONE);
                    instance->DoKilledMonsterKredit(40249, 114514);
                    instance->DoKilledMonsterKredit(41863, 114514);
                }
            });
        }
    }
    bool isloot;
};

struct npc_105155 : public ScriptedAI
{
    npc_105155(Creature* creature) : ScriptedAI(creature) { me->SetAIAnimKitId(0); }

    void OnSpellClick(Unit* clicker, bool& /*result*/)
    {
        if (Player* player = clicker->ToPlayer())
        
            if (player->HasQuest(QUEST_VENGEANCE_WILL_BE_OURS_1) || player->HasQuest(QUEST_VENGEANCE_WILL_BE_OURS_2))
            
                player->KilledMonsterCredit(114514);
    } 

	void MoveInLineOfSight(Unit* who) override
    {
        if (Player* player = who->ToPlayer())
        {
            if (player->IsInDist(me, 2.9f))
            {
                player->KilledMonsterCredit(114514);
            }
        }
    }
};

void AddSC_class_hall_dh()
{
    RegisterCreatureAI(npc_korvas_bloodthorn_99343);
    RegisterCreatureAI(npc_illidari_fel_bat_94324);
    new dh_playerchoice();
    new caria_felsoul();
    new go_aldrachi_warblades_248785();
    RegisterCreatureAI(npc_105155);
}