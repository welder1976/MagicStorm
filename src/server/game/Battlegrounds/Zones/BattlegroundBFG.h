#ifndef __Battleground_BG_H
#define __Battleground_BG_H

#include "Battleground.h"

enum BG_BGilneas_WorldStates
{
    BG_BGilneas_OP_OCCUPIED_BASES_HORDE       = 1778,
    BG_BGilneas_OP_OCCUPIED_BASES_ALLY        = 1779,
    BG_BGilneas_OP_RESOURCES_ALLY             = 1776,
    BG_BGilneas_OP_RESOURCES_HORDE            = 1777,
    BG_BGilneas_OP_RESOURCES_MAX              = 1780,
    BG_BGilneas_OP_RESOURCES_WARNING          = 1955
/*
    BG_BGilneas_OP_LIGHTHOUSE_ICON                = 1842,             //StBGle map icon (NONE)
    BG_BGilneas_OP_LIGHTHOUSE_STATE_ALIENCE       = 1767,             //StBGle map state (ALIENCE)
    BG_BGilneas_OP_LIGHTHOUSE_STATE_HORDE         = 1768,             //StBGle map state (HORDE)
    BG_BGilneas_OP_LIGHTHOUSE_STATE_CON_ALI       = 1769,             //StBGle map state (CON ALIENCE)
    BG_BGilneas_OP_LIGHTHOUSE_STATE_CON_HOR       = 1770,             //StBGle map state (CON HORDE)
    BG_BGilneas_OP_MINES_ICON                     = 1845,             //Farm map icon (NONE)
    BG_BGilneas_OP_MINES_STATE_ALIENCE            = 1772,             //Farm state (ALIENCE)
    BG_BGilneas_OP_MINES_STATE_HORDE              = 1773,             //Farm state (HORDE)
    BG_BGilneas_OP_MINES_STATE_CON_ALI            = 1774,             //Farm state (CON ALIENCE)
    BG_BGilneas_OP_MINES_STATE_CON_HOR            = 1775,             //Farm state (CON HORDE)
    BG_BGilneas_OP_WATERWORKS_ICON                = 1846,             //Blacksmith map icon (NONE)
    BG_BGilneas_OP_WATERWORKS_STATE_ALIENCE       = 1782,             //Blacksmith map state (ALIENCE)
    BG_BGilneas_OP_WATERWORKS_STATE_HORDE         = 1783,             //Blacksmith map state (HORDE)
    BG_BGilneas_OP_WATERWORKS_STATE_CON_ALI       = 1784,             //Blacksmith map state (CON ALIENCE)
    BG_BGilneas_OP_WATERWORKS_STATE_CON_HOR       = 1785,             //Blacksmith map state (CON HORDE)
    BG_BGilneas_OP_PRISON_ICON                    = 1843,             //Gold Mine map icon (NONE)
    BG_BGilneas_OP_PRISON_STATE_ALIENCE           = 1787,             //Gold Mine map state (ALIENCE)
    BG_BGilneas_OP_PRISON_STATE_HORDE             = 1788,             //Gold Mine map state (HORDE)
    BG_BGilneas_OP_PRISON_STATE_CON_ALI           = 1789,             //Gold Mine map state (CON ALIENCE
    BG_BGilneas_OP_PRISON_STATE_CON_HOR           = 1790,             //Gold Mine map state (CON HORDE)
*/
};

const uint32 BG_BGilneas_OP_NODESTATES[3] =    { 8852, 8851, 8853 };

/* Note: code uses that these IDs follow each other */
extern uint32 BG_BGilneas_OBJECTID_NODE_BANNER[3];

enum BG_BGilneas_ObjectType
{
    // for all 5 node points 8*5=40 objects
    BG_BGilneas_OBJECT_BANNER_NEUTRAL          = 0,
    BG_BGilneas_OBJECT_BANNER_CONT_A           = 1,
    BG_BGilneas_OBJECT_BANNER_CONT_H           = 2,
    BG_BGilneas_OBJECT_BANNER_ALLY             = 3,
    BG_BGilneas_OBJECT_BANNER_HORDE            = 4,
    BG_BGilneas_OBJECT_AURA_ALLY               = 5,
    BG_BGilneas_OBJECT_AURA_HORDE              = 6,
    BG_BGilneas_OBJECT_AURA_CONTESTED          = 7,
    //gates
    BG_BGilneas_OBJECT_GATE_A                  = 40,
    BG_BGilneas_OBJECT_GATE_H                  = 41,
    BG_BGilneas_OBJECT_GATE_GHOST_A            = 42,
    BG_BGilneas_OBJECT_GATE_GHOST_H            = 43,
    //buffs
    BG_BGilneas_OBJECT_SPEEDBUFF_WATERWORKS_STATION   = 44,
    BG_BGilneas_OBJECT_REGENBUFF_WATERWORKS_STATION   = 45,
    BG_BGilneas_OBJECT_BERSERKBUFF_WATERWORKS_STATION = 46,
    BG_BGilneas_OBJECT_SPEEDBUFF_LIGHTHOUSE        = 47,
    BG_BGilneas_OBJECT_REGENBUFF_LIGHTHOUSE        = 48,
    BG_BGilneas_OBJECT_BERSERKBUFF_LIGHTHOUSE      = 49,
    BG_BGilneas_OBJECT_SPEEDBUFF_GOLD_MINE         = 50,
    BG_BGilneas_OBJECT_REGENBUFF_GOLD_MINE         = 51,
    BG_BGilneas_OBJECT_BERSERKBUFF_GOLD_MINE       = 52,
    BG_BGilneas_OBJECT_MAX                         = 53,
};

/* Object id templates from DB */
enum BG_BGilneas_ObjectTypes
{
    BG_BGilneas_OBJECTID_BANNER_A             = 180058,
    BG_BGilneas_OBJECTID_BANNER_CONT_A        = 180059,
    BG_BGilneas_OBJECTID_BANNER_H             = 180060,
    BG_BGilneas_OBJECTID_BANNER_CONT_H        = 180061,

    BG_BGilneas_OBJECTID_AURA_A               = 180100,
    BG_BGilneas_OBJECTID_AURA_H               = 180101,
    BG_BGilneas_OBJECTID_AURA_C               = 180102,

    BG_BGilneas_OBJECTID_GATE_A               = 180255,
    BG_BGilneas_OBJECTID_GATE_GHOST_A         = 180322,
    BG_BGilneas_OBJECTID_GATE_H               = 180256,
    BG_BGilneas_OBJECTID_GATE_GHOST_H         = 180322
};

enum BG_BGilneas_Timers
{
    BG_BGilneas_FLAG_CAPTURING_TIME           = 60000,
};

enum BG_BGilneas_Score
{
    BG_BGilneas_WARNING_NEAR_VICTORY_SCORE    = 1800,
    BG_BGilneas_MAX_TEAM_SCORE                = 2000
};

/* do NOT change the order, else wrong behaviour */
enum BG_BGilneas_BattlegroundNodes
{
    BG_BGilneas_NODE_WATERWORKS       = 0,
    BG_BGilneas_NODE_LIGHTHOUSE       = 1,
    BG_BGilneas_NODE_GOLD_MINE        = 2,

    BG_BGilneas_DYNAMIC_NODES_COUNT   = 3,                        // dynamic nodes that can be captured

    BG_BGilneas_SPIRIT_ALIANCE        = 3,
    BG_BGilneas_SPIRIT_HORDE          = 4,

    BG_BGilneas_ALL_NODES_COUNT       = 5,                        // all nodes (dynamic and static)
};

enum BG_BGilneas_NodeStatus
{
    BG_BGilneas_NODE_TYPE_NEUTRAL             = 0,
    BG_BGilneas_NODE_TYPE_CONTESTED           = 1,
    BG_BGilneas_NODE_STATUS_ALLY_CONTESTED    = 1,
    BG_BGilneas_NODE_STATUS_HORDE_CONTESTED   = 2,
    BG_BGilneas_NODE_TYPE_OCCUPIED            = 3,
    BG_BGilneas_NODE_STATUS_ALLY_OCCUPIED     = 3,
    BG_BGilneas_NODE_STATUS_HORDE_OCCUPIED    = 4
};

enum BG_BGilneas_Sounds
{
    BG_BGilneas_SOUND_NODE_CLAIMED            = 8192,
    BG_BGilneas_SOUND_NODE_CAPTURED_ALLIANCE  = 8173,
    BG_BGilneas_SOUND_NODE_CAPTURED_HORDE     = 8213,
    BG_BGilneas_SOUND_NODE_ASSAULTED_ALLIANCE = 8212,
    BG_BGilneas_SOUND_NODE_ASSAULTED_HORDE    = 8174,
    BG_BGilneas_SOUND_NEAR_VICTORY            = 8456
};

enum BG_BGilneas_Objectives
{
    BGilneas_OBJECTIVE_ASSAULT_BASE           = 370,
    BGilneas_OBJECTIVE_DEFEND_BASE            = 371
};

enum BG_BG_CriteriaId
{
    BG_BG_CRITERIA_FULL_COVERAGE        = 14943,
    BG_BG_CRITERIA_JUGGER_NOT           = 14945,
};

enum BG_BG_Achievements
{
    BG_DONT_GET_COCKY_KID               = 5252,
};

#define BG_BGilneas_NotBGBGWeekendHonorTicks      330
#define BG_BGilneas_BGBGWeekendHonorTicks         200

// x, y, z, o
const Position BG_BGilneas_NodePositions[BG_BGilneas_DYNAMIC_NODES_COUNT] =
{
    {980.168701f, 948.640808f, 12.8f, 5.97374f},    // Waterworks
    {1057.85693f, 1279.03916f, 3.62f, 1.90303f},    // Lighthouse
    {1251.79269f, 958.486580f, 6.23f, 2.85965f}     // Mines
};

// x, y, z, o, rot0, rot1, rot2, rot3
const float BG_BGilneas_DoorPositions[2][8] =
{
    { 1396.044f, 977.344f, 7.43f, 3.16079f, -0.f, -0.f, -0.999954f, 0.00959934f },
    { 918.975f, 1336.67f, 27.6165f, 6.00124f, -0.f, -0.f, -0.140508f, 0.99008f }
};

// Tick intervals and given points: case 0,1,2,3 captured nodes
const uint32 BG_BGilneas_TickIntervals[4] = {0, 9000, 3000, 1000};
const uint32 BG_BGilneas_TickPoints[4] = {0, 10, 10, 30};

/// WorldSafeLocs ids for 3 nodes, and for ally, and horde starting location
const uint32 BG_BGilneas_GraveyardIds[BG_BGilneas_ALL_NODES_COUNT] = {1738, 1736, 1735, 1740, 1739};

// x, y, z, o
const Position BG_BGilneas_BuffPositions[BG_BGilneas_DYNAMIC_NODES_COUNT] =
{
    {962.218994f, 974.298279f, 14.168498f, 4.347988f},   // Waterworks
    {1064.738037f, 1311.437988f, 4.906008f, 4.199537f},  // Lighthouse
    {1252.445801f, 895.645935f, 18.763988f, 2.504837f}   // Mines
};

/// x, y, z, o
const Position BG_BGilneas_SpiritGuidePos[BG_BGilneas_ALL_NODES_COUNT] =
{
    {884.943115f, 935.980286f, 24.537628f, 0.363656f},      // Waterworks
    {1036.316f, 1341.434f, 11.542549f, 4.472407f},          // Lighthouse
    {1252.470825f, 830.552124f, 27.789499f, 1.559820f},     // Mines
    {901.88f, 1341.11f, 27.47f, 6.02f},                     // alliance starting base
    {1405.626f, 977.772f, 7.4409f, 3.085f}                  // horde starting base
};

struct BG_BGilneas_BannerTimer
{
    uint32      timer;
    uint8       type;
    uint8       teamIndex;
};

class Battleground_BGScore : public BattlegroundScore
{
    public:
        Battleground_BGScore(ObjectGuid guid, uint32 team): BattlegroundScore(guid, team), BasesAssaulted(0), BasesDefended(0) {};
        
        void UpdateScore(uint32 type, uint32 value) override
        {
            switch (type)
            {
            case SCORE_BASES_ASSAULTED:
                BasesAssaulted += value;
                break;
            case SCORE_BASES_DEFENDED:
                BasesDefended += value;
                break;
            default:
                BattlegroundScore::UpdateScore(type, value);
                break;
            }
        }

        void BuildObjectivesBlock(std::vector<int32>& stats) override
        {
            stats.push_back(BasesAssaulted);
            stats.push_back(BasesDefended);
        }

        uint32 BasesAssaulted;
        uint32 BasesDefended;
};

class Battleground_BG : public Battleground
{
    friend class BattlegroundMgr;

    public:
        Battleground_BG();
        ~Battleground_BG();

        void PostUpdateImpl(uint32 diff);
        void AddPlayer(Player *plr);
        virtual void StartingEventCloseDoors();
        virtual void StartingEventOpenDoors();
        void RemovePlayer(Player* /*player*/, ObjectGuid /*guid*/, uint32 /*team*/);
        void HandleAreaTrigger(Player *Source, uint32 Trigger, bool entered);
        virtual bool SetupBattleground();
        virtual void Reset();
        void EndBattleground(uint32 winner);
        virtual WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);

        /* Scorekeeping */
        virtual bool UpdatePlayerScore(Player *Source, uint32 type, uint32 value, bool doAddHonor = true);

        virtual void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& builder);

        /* Nodes occupying */
        virtual void EventPlayerClickedOnFlag(Player *source, GameObject* target_obj);

        /* achievement req. */
        bool IsAllNodesConrolledByTeam(uint32 team) const;  // overwrited
        bool CheckAchievementCriteriaMeet(uint32 criteriaId, Player const* source, Unit const* target = nullptr, uint32 miscvalue1 = 0);
    private:
        /* Gameobject spawning/despawning */
        void _CreateBanner(uint8 node, uint8 type, uint8 teamIndex, bool delay);
        void _DelBanner(uint8 node, uint8 type, uint8 teamIndex);
        void _SendNodeUpdate(uint8 node);

        /* Creature spawning/despawning */
        // TODO: working, scripted peons spawning
        void _NodeOccupied(uint8 node,Team team);
        void _NodeDeOccupied(uint8 node);

        int32 _GetNodeNameId(uint8 node);

        /* Nodes info:
            0: neutral
            1: ally contested
            2: horde contested
            3: ally occupied
            4: horde occupied     */
        uint8               m_Nodes[BG_BGilneas_DYNAMIC_NODES_COUNT];
        uint8               m_prevNodes[BG_BGilneas_DYNAMIC_NODES_COUNT];
        BG_BGilneas_BannerTimer   m_BannerTimers[BG_BGilneas_DYNAMIC_NODES_COUNT];
        uint32              m_NodeTimers[BG_BGilneas_DYNAMIC_NODES_COUNT];
        uint32              m_lastTick[BG_TEAMS_COUNT];
        uint32              m_HonorScoreTics[BG_TEAMS_COUNT];
        uint32              m_ReputationScoreTics[BG_TEAMS_COUNT];
        bool                m_IsInformedNearVictory;
        uint32              m_HonorTics;
        uint32              m_ReputationTics;
        // need for achievements
        bool                m_TeamScores500Disadvantage[BG_TEAMS_COUNT];
};
#endif