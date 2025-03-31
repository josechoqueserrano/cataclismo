#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameEventMgr.h"
#include "Player.h"
#include "WorldSession.h"
#include "SpellInfo.h"    // Para obtener información de hechizos (duración base, etc.)

// Definición del hechizo que se aplicará (ID del buff)
enum
{
    SPELL_POWER_WORD_FORTITUDE  = 74973,
};

/// Clase que implementa el comportamiento (IA) del NPC
class npc_sister_gold_skimmer : public CreatureScript
{
public:
    npc_sister_gold_skimmer() : CreatureScript("npc_sister_gold_skimmer") { }

    // Definición de la clase de IA que controla al NPC.
    struct npc_sister_gold_skimmerAI : public ScriptedAI
    {
        npc_sister_gold_skimmerAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 checkBuffTimer;

        // Se inicializa la IA.
        void Reset() override
        {
            // Se configura un temporizador para revisar cada 2 segundos
            checkBuffTimer = 2000;
        }

        // Se llama periódicamente (cada frame) para actualizar la IA
        void UpdateAI(uint32 diff) override
        {
            // Reducir el temporizador
            if (checkBuffTimer <= diff)
            {
                // Se define el radio en el que se buscarán jugadores (por ejemplo, 30 yardas)
                float searchRadius = 30.0f;
                // Se obtiene la lista de jugadores cercanos
                std::list<Player*> playerList;
                // Esta función de TrinityCore visita todos los objetos jugadores dentro de un radio
                Trinity::AnyPlayerInObjectRangeCheck check(me, searchRadius);
                Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, playerList, check);
                me->VisitNearbyObject(searchRadius, searcher);

                // Iteramos por cada jugador encontrado
                for (Player* player : playerList)
                {
                    if (!player)
                        continue;

                    // Si el jugador no tiene el buff, se lo aplica
                    if (!player->HasAura(SPELL_POWER_WORD_FORTITUDE))
                    {
                        me->CastSpell(player, SPELL_POWER_WORD_FORTITUDE, true);
                    }
                    else
                    {
                        // Si el jugador ya tiene el buff, se revisa la duración del aura
                        Aura* aura = player->GetAura(SPELL_POWER_WORD_FORTITUDE, player->GetGUID());
                        if (aura)
                        {
                            // Se obtiene la duración base del hechizo (la duración máxima esperada)
                            uint32 baseDuration = aura->GetSpellInfo()->GetDuration();
                            // Si la duración actual del aura es menor que la duración base,
                            // se asume que no está en su "estado máximo", por lo que se renueva
                            if (aura->GetDuration() < baseDuration)
                            {
                                // Se remueve el aura actual y se vuelve a aplicar el buff
                                player->RemoveAurasDueToSpell(SPELL_POWER_WORD_FORTITUDE);
                                me->CastSpell(player, SPELL_POWER_WORD_FORTITUDE, true);
                            }
                        }
                    }
                }
                // Se reinicia el temporizador para la siguiente comprobación
                checkBuffTimer = 2000;
            }
            else
            {
                checkBuffTimer -= diff;
            }

            // Si el NPC tiene otras acciones, se pueden implementar aquí.
            // En este ejemplo no hay combate, por lo que no se llama a DoMeleeAttackIfReady().
        }
    };

    // Se sobreescribe el método GetAI para retornar nuestra IA personalizada
    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_sister_gold_skimmerAI(creature);
    }
};

void AddSC_npc_sister_gold_skimmer()
{
    new npc_sister_gold_skimmer();
}
