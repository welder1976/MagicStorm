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

class instance_antorus_the_burning_throne : public InstanceMapScript
{
public:
    instance_antorus_the_burning_throne() : InstanceMapScript("instance_antorus", 1712) { }

    struct instance_antorus_the_burning_throne_InstanceMapScript : public InstanceScript
    {
        instance_antorus_the_burning_throne_InstanceMapScript(InstanceMap* map) : InstanceScript(map) { }

        // Garothi
        ObjectGuid GarothiWorldbreakerGUID;
        ObjectGuid GarothiWorldbreakerAnnihilatorGUID;
        ObjectGuid GarothiWorldbreakerDecimatorGUID;

        void Initialize() override
        {
            SetBossNumber(DATA_MAX_ENCOUNTERS);
            LoadDoorData(doorData);
        }

        void OnCreatureCreate(Creature* creature) override
        {
            switch (creature->GetEntry())
            {
            case NPC_GAROTHI_WORLDBREAKER:
                GarothiWorldbreakerGUID = creature->GetGUID();
                break;
            case NPC_CANNON_ANNIHILATOR:
                GarothiWorldbreakerAnnihilatorGUID = creature->GetGUID();
                break;
            case NPC_CANNON_DECIMATOR:
                GarothiWorldbreakerDecimatorGUID = creature->GetGUID();
                break;
            default:
                break;
            }
        }

        /*void OnGameObjectCreate(GameObject* object) override
        {
            switch (object->GetEntry())
            {
                default:
                    break;
            }
        }*/

        ObjectGuid GetGuidData(uint32 data) const override
        {
            switch (data)
            {
            case DATA_GAROTHI_WORLDBREAKER:
                return GarothiWorldbreakerGUID;
            case DATA_CANNON_ANNIHILATOR:
                return GarothiWorldbreakerAnnihilatorGUID;
            case DATA_CANNON_DECIMATOR:
                return GarothiWorldbreakerDecimatorGUID;
            }

            return ObjectGuid::Empty;
        }
    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const override
    {
        return new instance_antorus_the_burning_throne_InstanceMapScript(map);
    }
};

void AddSC_instance_antorus_the_burning_throne()
{
    new instance_antorus_the_burning_throne();
}
