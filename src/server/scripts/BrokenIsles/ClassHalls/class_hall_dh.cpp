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
#include "WorldSession.h"
#include "PhasingHandler.h"
#include "Log.h"
#include "Object.h"

enum ClassHallDH
{
    /// DH Quests
    QUEST_CALL_OF_THE_ILLIDARI_39261        = 39261,
    QUEST_CALL_OF_THE_ILLIDARI_39047        = 39047,

    QUEST_WEAPONS_OF_LEGEND_1               = 40816,
    QUEST_WEAPONS_OF_LEGEND_2               = 40814,
    QUEST_ONE_MORE_LEGEND                   = 44043,

    // Havoc
    QUEST_MAKING_ARRANGEMENTS_1             = 40819,
    QUEST_BY_ANY_MEANS_1                    = 39051,
    QUEST_THE_HUNT_1                        = 39247,

    QUEST_MAKING_ARRANGEMENTS_2             = 41120,
    QUEST_BY_ANY_MEANS_2                    = 41121,
    QUEST_THE_HUNT_2                        = 41119,

    // Vengeance
    QUEST_VENGEANCE_WILL_BE_OURS_1          = 40249,
    QUEST_VENGEANCE_WILL_BE_OURS_2          = 41863,

    /// Texts
    KORVAS_BLOODTHORN_TEXT_00               = 0,
    KORVAS_BLOODTHORN_TEXT_01               = 1,

    /// Spells
    SPELL_WEAPONS_OF_LEGEND_PLAYER_CHOICE   = 201092,
    SPELL_CALL_OF_THE_WARBLADES             = 208464,
    SPELL_FELLSOUL_SLAM                     = 216164,
    SPELL_SIGIL_OF_POWER                    = 216228,
    SPELL_SOUL_CARVER                       = 216188,

    /// Events
    EVENT_CALL_OF_THE_WARBLADES             = 1,
    EVENT_FELLSOUL_SLAM                     = 2,
    EVENT_SIGIL_OF_POWER                    = 3,
    EVENT_SOUL_CARVER                       = 4,

    /// Misc
    PLAYER_CHOICE_DH_SELECTION              = 255,

    ACTION_RESPONSEID_1                     = 600, // Havoc
    ACTION_RESPONSEID_2                     = 601, // Vengeance
    QUEST_THE_POWER_TO_SURVIVE              = 40816,
    SPELL_PLAYERCHOICE                      = 201092,
    PLAYER_CHOICE_DH_ARTIFACT_SELECTION     = 255,
    PLAYER_CHOICE_DH_HAVOC                  = 641,
    PLAYER_CHOICE_DH_VENGEANCE              = 640,
    SPELL_DH_SPEC_HAVOK                     = 201093,
    SPELL_DH_SPEC_VENGEANCE                 = 201094,
    QUEST_ALDRACHI_WARBLADES_CHOSEN         = 40818,
    QUEST_TWINBLADES_OFTHE_DECEIVER_CHOSEN  = 40817,
    KILL_CREDIT_DH_ARTIFACT_CHOSEN          = 105177,
};

enum DataTypes
{
    DATA_STAGE_7                            = 7
};

struct npc_korvas_bloodthorn_99343 : public ScriptedAI
{
    npc_korvas_bloodthorn_99343(Creature* creature) : ScriptedAI(creature)
    {
        SayHi = false;
    }

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
            Talk(KORVAS_BLOODTHORN_TEXT_00, player);
        }
    }

    void sQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_CALL_OF_THE_ILLIDARI_39261 || quest->GetQuestId() == QUEST_CALL_OF_THE_ILLIDARI_39047)
        {
            Talk(KORVAS_BLOODTHORN_TEXT_01, player);
            me->DespawnOrUnsummon(5s);
        }
    }

private:
    bool SayHi;
};

class PlayerScript_DH_artifact_choice : public PlayerScript
{
public:
    PlayerScript_DH_artifact_choice() : PlayerScript("PlayerScript_DH_artifact_choice") {}

    void OnCompleteQuestChoice(Player* player, uint32 choiceID, uint32 responseID)
    {
        if (choiceID != PLAYER_CHOICE_DH_ARTIFACT_SELECTION)
            return;

        switch (responseID)
        {
            case PLAYER_CHOICE_DH_HAVOC:
            {
                player->RemoveRewardedQuest(QUEST_ALDRACHI_WARBLADES_CHOSEN);
                player->KilledMonsterCredit(KILL_CREDIT_DH_ARTIFACT_CHOSEN);

                if (ChrSpecializationEntry const* spec = sChrSpecializationStore.AssertEntry(577))
                    player->ActivateTalentGroup(spec);

                break;
            }   
            case PLAYER_CHOICE_DH_VENGEANCE:
            {
                player->RemoveRewardedQuest(QUEST_TWINBLADES_OFTHE_DECEIVER_CHOSEN);
                player->KilledMonsterCredit(KILL_CREDIT_DH_ARTIFACT_CHOSEN);

                if (ChrSpecializationEntry const* spec = sChrSpecializationStore.AssertEntry(581))
                    player->ActivateTalentGroup(spec);

                break;
            }   
            default:
                break;
        }
    }
};


void AddSC_class_hall_dh()
{
    RegisterCreatureAI(npc_korvas_bloodthorn_99343);
    new PlayerScript_DH_artifact_choice();
}
