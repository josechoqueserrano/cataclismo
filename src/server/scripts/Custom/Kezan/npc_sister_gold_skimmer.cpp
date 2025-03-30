#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameEventMgr.h"
#include "Player.h"
#include "WorldSession.h"


class npc_sister_gold_skimmer : public CreatureScript
{
public:
    npc_sister_gold_skimmer() : CreatureScript("npc_sister_gold_skimmer") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->IsInCombat())
        {
            creature->Say("You are in combat!");
            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        if (player->HasItemCount(50000, 1))
        {
            creature->Say("You already have the item!");
            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        player->AddItem(50000, 1);
        creature->Say("Here you are!");
        player->CLOSE_GOSSIP_MENU();
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_sister_gold_skimmerAI(creature);
    }

    struct npc_sister_gold_skimmerAI : public ScriptedAI
    {
        npc_sister_gold_skimmerAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() override
        {
            ScriptedAI::Reset();
        }

        void UpdateAI(uint32 diff) override
        {
            ScriptedAI::UpdateAI(diff);
        }
    };
};
void AddSC_npc_sister_gold_skimmer()
{
    new npc_sister_gold_skimmer();
}