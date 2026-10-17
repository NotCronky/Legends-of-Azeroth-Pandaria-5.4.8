# Playerbots: Audit and Parity Plan

Comparison of `modules/mod_playerbots` (MoP 5.4.8) against AzerothCore
[mod-playerbots](https://github.com/mod-playerbots/mod-playerbots) (WotLK 3.3.5,
checkout dated 2026-09-20), and a plan to bring the MoP bots to parity.

Audit date: 2026-10-15. Last updated: 2026-10-17.

## Summary

The MoP module is the core engine (combat and follow) plus partly updated class
rotations. Most of what makes the AzerothCore bots feel alive was either never
ported or is compiled but never switched on.

## Status

| Phase | Status |
|---|---|
| 0: make the existing code work | Done (2026-10-15) |
| 1: chat commands | Done (2026-10-15): chat plumbing, control commands, and item, trade, quest, spell and talent commands. Other commands come with the systems they control |
| 2: upkeep | In progress: part 1 (looting, rolls, item usage, stat weights, upgrades) done 2026-10-17 |
| 3: class rotations for 5.4.8 | Not started; to-do list in the Phase 0 results |
| 4: life in the world | Not started |
| 5: group content | Not started |

The audit below describes the module before any of this work; see the results sections at
the end for what changed.

## Audit

### Size

| | AzerothCore | MoP | MoP as a share |
|---|---|---|---|
| Lines of code | 253k | 47k | 19% |
| Generic actions | 200 | 31 | 16% |
| Generic triggers | 185 | 64 | 35% |
| Generic strategies | 84 | 20 | 24% |
| Chat commands | 115 | 0 | 0% |
| Game-event (packet) handlers | 60 actions, 56 triggers | 5 actions, 7 triggers | about 10% |
| Dungeon / raid / PvP strategies | 20 dungeons, 19 raids, battlegrounds | none | 0% |
| Config options the code actually reads | ~all | 80 of 511 | — |

### Why random bots stand around doing nothing

`AiFactory::AddDefaultNonCombatStrategies` gives every random bot the
out-of-combat strategies `nc`, `food`, `chat`, `quest`, `loot`, `gather`, `duel`,
`buff`, `mount` and `emote`. None of these are registered in the MoP
`StrategyContext`. `NamedObjectContext::create` returns `nullptr` for unknown
names without logging anything. So out of combat, a random bot only says hello,
follows, or idles.

### Built but switched off

- **Wandering and grinding:** the code for bots to grind, walk to innkeepers and
  wander between NPCs exists in `src/strategy/RPG/`
  (`NewRpgStrategy`, `NewRpgGoGrindAction`, `NewRpgGoInnKeeperAction`,
  `NewRpgMoveRandomAction`, `NewRpgMoveNpcAction`), but every function body is
  commented out and nothing is registered in any context. It is a stub, not a
  working feature.
- **Dead config options:** about 430 of the 511 `AiPlayerbot.*` settings in
  `playerbots.conf.dist` were copied from AzerothCore and nothing reads them.
  Only 80 are read in `PlayerbotAIConfig.cpp`.
- **Travel tables:** the `playerbots_travelnode*` tables (about 70 MB) are
  WotLK map data. No MoP code uses them; only `Travel/WorldPosition` exists.

### Bot creation

- `BotFactory::Randomize` gives new random bots a spec, a pet and gold, but no
  talents or gear. `InitTalentsTree` and `InitEquipment` only run from the GM
  `addclass` command (`PlayerbotMgr`) and from
  `AutoMaintenanceOnLevelupAction`.
- AzerothCore's `PlayerbotFactory` also sets up glyphs, skills, mounts,
  consumables, reagents, bags, professions, reputation and guilds. MoP does none
  of these.
- `InitTalentsTree` picks a spec with `rand() % 3`, so druids can never roll their
  4th spec (Guardian).
- **Monk is not supported.** Random Monk bots are created, but there is no Monk
  class context or strategies, and `RandomPlayerbotMgr::PrepareAddclassCache` skips
  class 10, so the GM `addclass` command can't add one. Spec detection
  (`AiFactory`, `GetSpecialization()`) is MoP-aware and already maps the Monk specs.

### Class rotations

Judged by which spell names each class's actions use.

| State | Classes |
|---|---|
| Mostly MoP | Warlock, Warrior |
| About half updated | Mage |
| Mostly WotLK spells that don't exist in 5.4.8 | Priest (Lesser Heal, Divine Spirit), Druid (Dire Bear Form, Tree of Life), Hunter (Aspect of the Viper; DPS strategy only), Shaman (Totem of Wrath, Flametongue Totem), Paladin (old auras), Rogue (no Combat/Subtlety strategies), Death Knight (Blood Strike, Hungering Cold, Unbreakable Armor) |
| Missing | Monk |

### Talking to bots

There is no whisper or party command handling. The only control is the GM
`.npcbot` command (`addclass`, `addspec`, `setspec`). Players can't tell a bot to
attack, loot, trade, equip or stay.

## Plan

Two principles:

- **Port, don't copy.** AzerothCore runs on a WotLK 3.3.5 core. This core differs
  in packets, talents (tiers instead of trees), glyphs, stats (mastery, no armor
  penetration) and spell IDs. Each AzerothCore system has to be adapted.
- **Order by what players notice in game,** cheapest big wins first.

### Phase 0: make the existing code work (days)

1. Register the wandering/grinding strategy (`NewRpgStrategy`) and its actions,
   and make it the default out-of-combat behaviour for random bots.
2. Make `AiFactory` stop requesting strategies that don't exist: implement them in
   later phases or remove them. Log a warning in `NamedObjectContext::create` when
   an unknown name is requested, so these gaps stop being silent.
3. Run talent and gear setup when random bots are created. Fix the druid spec roll.
4. Add a startup check that every spell name in each class strategy exists in this
   client's spell data, and log the ones that don't. That list becomes the to-do
   list for Phase 3.
5. Trim `playerbots.conf.dist` to the options the code reads.

### Phase 1: chat commands (1–2 weeks)

Port AzerothCore's chat command layer: `ChatActionContext`, `ChatTriggerContext`
and `ChatCommandHandlerStrategy`. Start with the everyday commands: follow, stay,
attack, flee, loot, summon, stats, quests, equip, sell, trade, talents and the
strategy toggles (`co` / `nc`). This is the biggest usability gap for players
grouping with bots.

- **Part 1 (done):** the chat plumbing, replies, and the control commands
  (see "Phase 1 results, part 1").
- **Part 2 (done):** a full `ChatHelper` (item, quest and spell link parsing and
  formatting) and the inventory commands: `equip`, `unequip`, `sell`, `use`,
  `trade`, item links, `quests`, `talents`, `spells` (see "Phase 1 results, part 2").
  Loot commands wait for the Phase 2 loot port.

### Phase 2: upkeep (2–3 weeks)

1. Looting and rolling, eating and drinking, potions, buffs, mounts, and repairing
   and selling at vendors. Looting and rolling are done (part 1); the rest is part 2.
2. Gear choice: port AzerothCore's stat-weight system (`Mgr/Item`), adjusted for
   MoP stats (mastery; hit, expertise and haste caps; reforging can wait), so bots
   equip upgrades.
3. Talents and glyphs: MoP talents (6 tiers with 3 choices each, picked per spec
   from templates) and MoP glyphs (major and minor).

### Phase 3: class rotations for 5.4.8 (3–5 weeks, can run alongside other phases)

1. Use the missing-spell list from Phase 0 to rewrite Priest, Druid, Shaman,
   Hunter, Paladin, Rogue and Death Knight around their 5.4.8 abilities. Add the
   missing specs (Hunter BM/MM/SV, Rogue Combat/Subtlety).
2. Write Monk from scratch (Brewmaster, Mistweaver, Windwalker) using the existing
   spec mapping, then remove the `claz == 10` skip in
   `RandomPlayerbotMgr::PrepareAddclassCache` so `addclass` can add Monks.
3. Adopt AzerothCore's per-class folder layout (`Ai/Class/<Class>/...`) while doing
   this, so later ports line up.

### Phase 4: life in the world (3–4 weeks)

1. Questing: accept, track, complete and hand in quests, and share them (the
   packet-driven quest flow from AzerothCore's `WorldPacketActionContext`).
2. Travel: flight paths, hearthstone, and travelling between zones. Either
   regenerate the travel-node data for MoP maps (including Pandaria, map 870), or
   skip the node graph and rely on mmaps pathfinding plus flight paths.
3. Guilds and LFG queueing. Auction house and mail are optional.

### Phase 5: group content (ongoing)

1. Generic dungeon behaviour first (pulling, staying out of bad ground, focusing
   marked targets), then specific scripts for the Pandaria 5-mans.
2. AzerothCore's dungeon and raid scripts are for WotLK and TBC instances. They are
   useful as templates but only matter for old content.
3. Battlegrounds: port Warsong Gulch, Arathi Basin, Alterac Valley and Eye of the
   Storm from AzerothCore, then add the MoP ones (Temple of Kotmogu, Silvershard
   Mines, Deepwind Gorge).

## Expectations

Phases 0–2 are what make the bots feel on par in everyday play: roughly 1–1.5
months of focused work. Full parity, including group-content scripts, is much
longer. AzerothCore's 253k lines represent years of work, and most of its instance
scripts don't apply to MoP.

## Phase 0 results (2026-10-15)

### Done

- **Missing names no longer fail silently or crash.** `AiObjectContext` logs each
  unregistered strategy, action, trigger or value once per class. `GetValue` returns a
  per-bot default value instead of `nullptr` for unregistered values; 22 value names were
  used without being registered, so code reaching them (for example the spirit-healer path
  in `ReviveFromCorpseAction`) used to dereference a null pointer.
- **Random bots live on their own.** The RPG strategy is ported onto this module's data
  (farming spots, plus a new innkeeper cache loaded from the world DB) and registered with
  its actions, triggers and the `possible rpg targets` and `nearest npcs` values. Random bots
  that aren't following anyone get `grind` (attack nearby mobs) plus `new rpg`, or
  `move random` when `AiPlayerbot.EnableNewRpgStrategy = 0`.
  `AiPlayerbot.RandomBotNonCombatStrategies` is now applied. Most zones have no farming-spot
  data, so bots there wander and grind locally. Travelling states give up after 10 minutes.
- **`AiFactory` no longer requests strategies that exist for no class**:
  `food`, `chat`, `quest`, `loot`, `gather`, `duel`, `mount`, `emote` (non-combat),
  `racials`, `cast time`, `duel`, `avoid aoe` (combat), `chat` (dead).
- **Random bot creation** now sets talents and gear (`InitTalentsTree`, `InitEquipment`).
  With no premade link configured, bots pick a random talent in each unlocked tier. Druids
  can roll all four specs. Talents are checked by spell, as `Player::LearnTalent` stores them.
- **Data race fixed:** `GetCityForPlayer` and `GetFarmZoneForPlayer` shuffled the shared
  caches in place while bots update on map threads; they now shuffle local index lists.
- **Config trimmed** from 517 to 116 settings: only options the code reads remain.
  `RandomBotMaxLevel` default raised from 80 to 90. The WotLK talent links were removed;
  they used keys the code never read.
- **Spell check:** the first bot of each class to log in reports cast actions whose spell
  this class can't learn in the client (`Utils/SpellNameAudit.cpp`).

### Findings for later phases

**Hunter bots have no class AI.** `HunterAiObjectContext`'s constructor has all four
registrations commented out, so hunters run on generic strategies only.

**Monk bots are created but have no class context**, so they also run on generic
strategies (Phase 3). The `addclass` cache also skips Monks.

**Spell check output** (cast actions whose spell the class can't learn in 5.4.8; bots look
spells up by exact lowercase name, so renamed abilities such as `anti magic shell` vs
"Anti-Magic Shell" count too). Some warlock demons granted by Grimoire of Supremacy may be
false positives.

| Class | Dead / total | Spells |
|---|---|---|
| Druid | 23 / 82 | abolish poison, bash, cancel tree form, caster form, claw, cower, cure poison, demoralizing roar, dire bear form, entangling roots on cc, faerie fire (feral), feral charge - bear, feral charge - cat, hibernate on cc, insect swarm, mangle (bear), mangle (cat), omen of clarity, remove curse, swipe (bear), swipe (cat), thorns, tree of life |
| Warlock | 19 / 77 | banish on cc, chaos wave, demonic empowerment, doom, fear on cc, grimoire of supremacy, hand of guldan, immolation aura, soul burn, soul link, summon abyssal, summon fel imp, summon observer, summon shivarra, summon terrorguard, summon voidlord, summon wrathguard, touch of chaos, void ray |
| Priest | 18 / 82 | abolish disease, chastise, consume magic, cure disease, divine spirit, elune's grace, feedback, hex of weakness, lesser heal, mana burn, mind soothe, prayer of fortitude, prayer of spirit, shadow protection, shadowguard, starshards, symbol of hope, touch of weakness |
| Paladin | 13 / 60 | concentration aura, crusader aura, divine illumination, divine sacrifice, hand of reckoning, holy shield, judgement, purify, retribution aura, righteous defense, sanctity aura, shield of righteousness, turn undead |
| Shaman | 11 / 62 | cure disease, cure poison, flametongue totem, grace of air totem, lesser healing wave, mana spring totem, strength of earth totem, totem of wrath, water breathing, windfury totem, wrath of air totem |
| Death Knight | 10 / 48 | anti magic shell, anti magic zone, death rune mastery, deathchill, ghoul frenzy, hungering cold, improved icy talons, killing machine, mark of blood, unbreakable armor |
| Rogue | 2 / 30 | hunger for blood, riposte |
| Warrior | 1 / 46 | dragonroar |
| Mage | 1 / 43 | fingers of frost |
| Hunter, Monk | — | no class actions (see above) |

**Unregistered names referenced by class strategies** (from the runtime warnings). Many
class rotations reference triggers that don't exist, so those branches never fire:

- Every class: actions `combat formation move`, `stay line`; triggers `combat stuck`,
  `mounted`.
- Priest: triggers `shadow word: pain`, `shadow word: pain on attacker`, `vampiric touch`,
  `vampiric touch on attacker`, `devouring plague`, `shadowfiend`, `power infusion`,
  `inner fire`, `holy fire`, `dispel magic`, party buff and cure triggers.
- Druid: form, eclipse and combo-point triggers (`cat form`, `eclipse (solar)`,
  `eclipse (lunar)`, `combo points available`, `rake`, `savage roar`, `moonfire`,
  `insect swarm`, `mark of the wild`, ...).
- Shaman: every totem and weapon-imbue trigger, shields, `flame shock`, `purge`,
  `wind shear`, `bloodlust`/`heroism`, cleanse and cure triggers.
- Paladin: aura, blessing, cleanse and `hammer of justice` triggers; actions
  `judgement of wisdom`, `seal of corruption`, `word of glory on party`.
- Rogue: `slice and dice`, `kick`, `expose armor`, combo-point and poison triggers.
- Mage: `combustion`, `mirror image`, `arcane blast stack`, ward triggers; actions
  `frost bomb`, `living bomb on attackers`, `invisibility`, `shoot`.
- Warlock: `amplify curse`, `no healthstone`, `life tap glyph buff`; action `howl of terror`.
- Death Knight: `bone shield`, `horn of winter`. Warrior: `demoralizing shout`; action
  `dragon_roar`.

## Phase 1 results, part 1 (2026-10-15)

Bots can now be commanded by whisper and by party or raid chat, and they answer.
Built and run without errors; the chat flow itself still needs testing in game.

### Done

- **Chat reaches bots.** The module's player script hooks the core's whisper and group
  `OnChat` events. `PlayerbotAI::HandleCommand` queues each command, keyed by the
  sender's GUID and guarded by a mutex because chat arrives on the sender's map thread;
  `HandleCommands` runs the queue on the bot's next update.
- **Who can command a bot:** its master (for example the player who invited it), GMs,
  and players in its group. Other players get "I only take orders from my group."
  Chat from other bots is ignored, so a bot master can't loop on its bots' replies.
- **Bots answer.** `TellMaster` was entirely commented out; it now replies on the channel
  the command came from (whisper or party/raid) and doesn't repeat a line within
  `AiPlayerbot.RepeatDelay`. A whispered unknown command gets a hint to whisper `help`.
- **Ordinary chat isn't parsed as missing names.** `ExternalEventHelper::ParseChatCommand`
  only looks up registered triggers and reports whether anything matched.
- **Commands:**

  | Command | Action |
  |---|---|
  | `help` | List commands and strategies |
  | `follow`, `stay` | Follow the master / hold position |
  | `attack` | Attack the master's target |
  | `flee`, `runaway` | Follow without fighting (new `passive` strategy) / run away |
  | `grind` | Fight nearby mobs |
  | `tank attack`, `max dps`, `move from group` | Tank engage, all-out damage, spread out |
  | `summon` | Teleport to the master, reviving first if dead |
  | `stats` | Gold, bag space, durability and repair cost (same formula as the core), XP |
  | `co`, `nc`, `de` | Change (`+name`, `-name`) or list (`?`) combat, non-combat or dead strategies |
  | `leave`, `rti`, `rpg status`, `reset` | Leave the group, raid icon targets, wandering status, reset the AI |

- **Config:** `AiPlayerbot.CommandPrefix` (require a prefix such as `!`) and
  `AiPlayerbot.CommandSeparator` (several commands in one message, default `\\`).

### Not yet ported

AzerothCore's other chat commands, roughly 100. The inventory, trade, quest and talent
commands are part 2; others (loot, mail, bank, guild, LFG, crafting, glyphs, pets,
battlegrounds) come with the phases that port those systems.

## Phase 1 results, part 2 (2026-10-15)

Bots can be asked about and told to handle their items, quests, spells and talents, and
they trade. Built and run (50 random bots) without errors or new warnings; the commands
themselves still need testing in game.

### Done

- **`ChatHelper`** parses item, quest and spell links, money (`1g 20s`), item qualities,
  consumable and trade-goods classes, equipment slots and professions, and formats item,
  quest and spell links. Item links use the 10-property layout the core's `ChatLink`
  validates. Lookup tables are function-local constants, so bots on different map threads
  share them safely (AzerothCore fills static maps from a per-bot constructor).
- **Commands:**

  | Command | Action |
  |---|---|
  | `c [items]` | Count matching items, or list the whole inventory by item class |
  | `e`, `equip <items>` | Equip from the bags, through the core's auto-equip handler (swaps, bags, two-handers) |
  | `ue`, `unequip <items or slots>` | Move equipped items to the bags; slots can be listed (`ue head, hands`) |
  | `s`, `sell <items>` | Sell to a vendor in range. `s gray` / `s *`: grey items; `s <quality>`: armor and weapons up to that quality; `s <quality> all`: every kind |
  | `u`, `use <item> [target item]` | Use an item on the bot, on a second linked item, or (harmful items) on the bot's or master's target |
  | `t`, `trade <items or money>` | Open a trade with the commander and put items or gold in it |
  | `quests [co\|in\|all]` | Quest log counts, or completed / incomplete / all quests as links |
  | `spells [profession] [name]` | Spells as links; profession recipes show what they make and the reagents the bot has |
  | `talents` | Specialization and the talent in each unlocked tier |
  | `talents spec <name>` | Switch specialization (keeps gear, picks talents, resets strategies) |
  | `talents <name or link>` | Take that talent, replacing the one in its tier |

  Items can be named by link, by part of the name, by quality, by class (`potion`,
  `food`, `cloth`, `herb`, ...), by slot or by keyword (`food`, `drink`, `mana potion`,
  `healing potion`, `mount`, `pet`, `recipe`, `quest`). A trailing number limits how many a
  quality or class matches.
- **Shift-clicked items:** a message containing an item link gets an item count from the
  bots that have it, and goes into an open trade with that player.
- **Trading:** bots open trades from their master, GMs and group (others get "I'm kind of
  busy now"), add items requested before the window opened once it is, and accept when the
  other side accepts. Items only go into the window on a command from the player they are
  trading with.
- **Who can manage a bot:** commands that give away, sell, use or change things (`e`,
  `ue`, `s`, `u`, `t`, changing talents) are limited to the master and GMs. Listing commands
  are open to the group.
- **Fixes:** the bot packet queue was a `std::stack` (handled newest first) with no lock
  while packets arrive from other sessions; it is now a locked FIFO. `TellMaster` no
  longer drops repeated lines (list headers, a second `c`) when answering a command.
  `strstri`, declared but never defined, is defined.

### Not ported from AzerothCore

- Random-bot trade economy (selling to players for gold, discounts, buy/sell multipliers).
- `s vendor` and auto-equip of upgrades: need the item usage value (Phase 2).
- Outfits, the bank, opening lootable items (`u` refuses them; looting is Phase 2),
  quest travel detail, `talents` premade links and glyphs.

## Bot management commands (2026-10-15)

The GM command is now `.playerbots` (it was `.npcbot`; same RBAC permission, 1344).

| Command | Action |
|---|---|
| `.playerbots list` | Your bots (`+`) and the other characters on your account (`-`) |
| `.playerbots add NAME[,NAME...]` | Log in characters from your account, or a guild member's character while they are offline |
| `.playerbots remove NAME[,NAME...]` | Log them out |
| `.playerbots addclass CLASS` | Add a random-account bot of that class at your level |
| `.playerbots setspec TAB` | Change your target bot's specialization |

`add` and `remove` (alt bots) were advertised but never implemented: `ProcessBotCommand` was a stub.

Addclass bots attacked everything because `OnBotLogin` set their strategies before adding them to the
master's group: as ungrouped bots from a random-bot account they got the solo `grind` strategy, and nothing
reset it once they joined. Strategies are now set after the bot joins, and bots with a real-player master never
get the solo strategies. Addclass setup (level, spec, talents, gear) also moved from a detached thread, which
changed the bot while it was in the world and sometimes teleported it away, into the login callback.

## Phase 2 results, part 1 (2026-10-17)

Bots loot, gather, roll on group loot and wear the upgrades they find; new random bots get gear
chosen by stat weights. Built and run with 50 random bots for 10 minutes: no errors or new
warnings, 23 bots picked up items (grey items, quest items, food, potions, herbs) and 15 looted money.
Group rolls, `ll` and `s vendor` still need testing in game.

### Done

- **Stat weights** (`Utils/StatsWeightCalculator`), written for MoP rather than ported: weights
  per role (strength or agility tank, strength or agility melee, hunter, caster, healer) by
  specialization, or by class before level 10. Counts mastery, bonus armor, spirit as hit for
  Balance, Shadow and Elemental, hit and expertise up to the 7.5% / 15% caps, sockets, random
  suffixes, and on-equip, on-use and proc effects by uptime. Items without the role's primary stat,
  lower armor types (weighted more from level 50, when armor specialization applies) and weapons the
  spec can't use well (bows for non-hunters in the shared main hand, non-daggers for Assassination and
  Subtlety, one-handers for two-handed specs) score low.
- **Item usage value** (`item usage`): equip, replace, broken equip, quest, profession tool,
  profession reagent or recipe, consumable to carry (two stacks of the best food, drink and potions),
  disenchant, auction or vendor. Rings, trinkets and dual-wielded weapons compare against the weaker
  of the two slots; two-handers against both hands.
- **Looting** (`loot` and `gather` strategies, now default out of combat): corpses, chests without
  quest conditions, and herbs, ore and skinnable corpses for bots with the profession. Items go through
  the core's loot handlers (`CMSG_LOOT`, `CMSG_AUTOSTORE_LOOT_ITEM`) with the same checks the loot
  window applies: round robin, blocked items, personal, quest and free-for-all items, bag space. The
  loot strategy decides which items: `normal` (anything with a use, including selling), `gray`,
  `disenchant`, `all`. Bots don't loot with free-for-all group loot unless
  `AiPlayerbot.FreeMethodLoot` is set.
- **Group rolls:** `SMSG_LOOT_START_ROLL` is queued per bot, and the bot votes need, greed,
  disenchant or pass by item usage, limited by what the roll allows. AzerothCore's settings and
  defaults: `LootNeedRollLevel` (1: greed instead of need), `LootGreedRollLevel`, `LootRollRecipe`,
  `LootRollDisenchant`.
- **Upgrades:** when an item arrives in the bags (`SMSG_ITEM_PUSH_RESULT`), bots out of combat equip
  the items that are upgrades by `AiPlayerbot.EquipUpgradeThreshold` (default 1.1), into the slot they
  were compared with. Random bots always do; other bots if `AiPlayerbot.AutoEquipUpgradeLoot` is set.
- **New random bots' gear:** `InitEquipment` keeps the best-scoring of its random candidates for each
  slot instead of the last one. Art test items and item level 1 placeholder copies of real items
  (126 of them, for example a level 1 High Warlord's Street Sweeper) are no longer candidates.
- **Restoration shamans get the healer role:** `AiFactory::GetPlayerRoles` checked
  `SPEC_DRUID_RESTORATION` for shamans, so they were treated as DPS.
- **Commands:** `ll [normal|gray|disenchant|all]` sets the loot strategy, `ll <item>` / `ll -<item>`
  edits the always-loot list, `ll ?` and `ll ?<item>` report them. `s vendor` sells items whose only
  use is selling.

### Found, not fixed

- **The worldserver crashes on shutdown** with random bots online: `Map::UnloadAll` deletes a creature
  whose chase movement still points at a removed bot (`ChaseMovementGenerator` destructor ->
  `AbstractFollower::SetTarget`). It happens on the build before this work too.

### Not ported from AzerothCore

- Looting items in the bags (clams, lockboxes), fishing, and the random-bot trade economy.
- Guild task items, the drop map values, and broadcasting loot to chat.

