
#include "Creature.h"
#include "GameObject.h"
#include "Map.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "antorus_the_burning_throne.h"

DoorData const doorData[] =
{
    { GO_DOOR_1,        DATA_GAROTHI_WORLDBREAKER,   DOOR_TYPE_PASSAGE },
    { GO_DOOR_2,        DATA_GAROTHI_WORLDBREAKER,   DOOR_TYPE_PASSAGE },
    { GO_DOOR_3,        DATA_GAROTHI_WORLDBREAKER,   DOOR_TYPE_PASSAGE },
    { GO_DOOR_4,        DATA_GAROTHI_WORLDBREAKER,   DOOR_TYPE_PASSAGE },
    { GO_BOSS6_1,       DATA_IMONAR_THE_SOULHUNTER,   DOOR_TYPE_PASSAGE },
    { GO_BOSS6_2,       DATA_IMONAR_THE_SOULHUNTER,   DOOR_TYPE_PASSAGE },
    { GO_BOSS7_1,       DATA_KINGAROTH,               DOOR_TYPE_PASSAGE },
    { GO_BOSS9_1,       DATA_NOURA_MOTHER_OF_FLAMES,  DOOR_TYPE_ROOM },
    { GO_BOSS9_2,       DATA_NOURA_MOTHER_OF_FLAMES,  DOOR_TYPE_PASSAGE },
    { GO_BOSS_VARIMATHRAS,       DATA_VARIMATHRAS,  DOOR_TYPE_ROOM },
    { GO_DOOR_PORTAL_KEEPER_HASABEL,       DATA_PORTAL_KEEPER_HASABEL,  DOOR_TYPE_ROOM },
    { GO_BOSS10,        DATA_AGGRAMAR,                DOOR_TYPE_ROOM },
};

struct instance_antorus_the_burning_throne : public InstanceScript
{
    instance_antorus_the_burning_throne(InstanceMap* map) : InstanceScript(map)
    {
        SetHeaders(DataHeader);
        SetBossNumber(DATA_MAX_ENCOUNTERS);
        LoadDoorData(doorData);
    }

    void OnCreatureCreate(Creature* creature) override
    {
        InstanceScript::OnCreatureCreate(creature);
    }

    bool SetBossState(uint32 type, EncounterState state) override
    {
        if (!InstanceScript::SetBossState(type, state))
        {
            return false;
        }

        switch (type)
        {
        case DATA_GAROTHI_WORLDBREAKER:
        {
            if (state == DONE)
            {

            }
            break;
        }
        case DATA_FHARG:
        {
            if (state == DONE)
            {

            }
            break;
        }
        default:
            break;
        }
    }
};

void AddSC_instance_antorus_the_burning_throne()
{
    RegisterInstanceScript(instance_antorus_the_burning_throne, 1712);
}
