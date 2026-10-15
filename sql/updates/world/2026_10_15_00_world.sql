-- Content filled in from the mangosfour MoP 5.4.8 world database (github.com/mangosfour/Database, GPLv3), where ours
-- had nothing: quest givers and turn-ins, creature loot and two profession trainers. Only links between things that
-- already exist here; no new spawns or templates.

-- Quest givers for quests nobody offered. Every giver is spawned here and flagged as a quest giver, and every quest has
-- a turn-in (existing, or added below). Quests our scripts or game events already hand out are left alone.
DELETE FROM `creature_queststarter` WHERE (`id`, `quest`) IN (
(12696, 2),
(1105, 273),
(1375, 310),
(1378, 317),
(1872, 400),
(1374, 413),
(1378, 415),
(1872, 415),
(2988, 764),
(2988, 765),
(5518, 3630),
(5174, 3632),
(3494, 3635),
(4586, 3637),
(7406, 3642),
(1694, 5541),
(13841, 7202),
(13841, 7382),
(3362, 7660),
(3362, 7661),
(3685, 7662),
(3685, 7663),
(7952, 7664),
(7952, 7665),
(4730, 7671),
(4730, 7672),
(1261, 7673),
(1261, 7674),
(7955, 7676),
(384, 7677),
(14833, 7881),
(14833, 7882),
(14833, 7883),
(14833, 7884),
(14832, 7889),
(14832, 7890),
(14832, 7891),
(14832, 7892),
(14832, 7893),
(14841, 7894),
(14841, 7895),
(14841, 7896),
(14841, 7897),
(14841, 7898),
(14829, 7899),
(14829, 7900),
(14829, 7901),
(14829, 7902),
(14829, 7903),
(14828, 7940),
(14829, 8222),
(5145, 8250),
(5498, 8250),
(17513, 8250),
(17479, 9607),
(17558, 9608),
(17712, 9672),
(17856, 9714),
(17856, 9715),
(17866, 9719),
(17841, 9731),
(17884, 9738),
(17240, 9751),
(17841, 9764),
(17831, 9911),
(17876, 10283),
(18723, 10283),
(16819, 10558),
(22007, 10825),
(18481, 10888),
(22421, 10901),
(18528, 10985),
(25256, 11595),
(16128, 13158),
(31080, 13159),
(32963, 13506),
(32971, 13591),
(40589, 13825),
(12919, 13826),
(37779, 24802),
(37120, 24912),
(40487, 25543),
(39605, 26865),
(44217, 26865),
(36648, 26866),
(44217, 26866),
(10181, 26867),
(3188, 26951),
(48431, 28376),
(11801, 29298),
(14847, 29464),
(14829, 29508),
(54548, 30105),
(54972, 30105),
(55779, 30105),
(58114, 30127),
(61819, 31031),
(63014, 31308),
(63014, 31309),
(63596, 31316),
(64508, 31367),
(64531, 31368),
(64036, 31372),
(64011, 31374),
(64032, 31376),
(64001, 31377),
(64032, 31378),
(64001, 31379),
(64032, 31380),
(64001, 31381),
(64032, 31382),
(64001, 31383),
(60981, 31492),
(63014, 31550),
(63061, 31570),
(63061, 31571),
(63080, 31579),
(63083, 31584),
(63086, 31585),
(63626, 31588),
(63626, 31589),
(63626, 31590),
(63596, 31591),
(63596, 31592),
(63596, 31593),
(63014, 31785),
(66126, 31813),
(63596, 31821),
(63077, 31825),
(63626, 31827),
(63086, 31828),
(63061, 31830),
(63083, 31832),
(63596, 31889),
(66733, 31954),
(63596, 32008),
(63626, 32009),
(58718, 32189),
(70520, 32275),
(67985, 32294),
(70316, 32296),
(67990, 32304),
(64582, 32428),
(67660, 32521),
(67989, 32521),
(67990, 32521),
(70520, 32521),
(67660, 32522),
(67989, 32522),
(67990, 32522),
(70520, 32522),
(67660, 32523),
(67989, 32523),
(67990, 32523),
(70520, 32523),
(67660, 32524),
(67989, 32524),
(67990, 32524),
(70520, 32524),
(67660, 32561),
(67989, 32561),
(70520, 32561),
(67985, 32563),
(67985, 32564),
(67985, 32565),
(67990, 32589),
(64582, 32604),
(64231, 32684),
(67663, 32730),
(63596, 32863),
(63626, 32863),
(64582, 32863),
(64582, 32868),
(64582, 32869),
(73082, 33136),
(64231, 33146),
(64231, 33358));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(12696, 2), -- Sharptalon's Claw (Senani Thunderheart)
(1105, 273), -- Resupplying the Excavation (Jern Hornhelm)
(1375, 310), -- Bitter Rivals (Marleth Barleybrew)
(1378, 317), -- Stocking Jetsteam (Pilot Bellowfiz)
(1872, 400), -- Tools for Steelgrill (Tharek Blackstone)
(1374, 413), -- Shimmer Stout (Rejold Barleybrew)
(1378, 415), -- Rejold's New Brew (Pilot Bellowfiz)
(1872, 415), -- Rejold's New Brew (Tharek Blackstone)
(2988, 764), -- The Venture Co. (Morin Cloudstalker)
(2988, 765), -- Supervisor Fizsprocket (Morin Cloudstalker)
(5518, 3630), -- Gnome Engineering (Lilliam Sparkspindle)
(5174, 3632), -- Gnome Engineering (Springspindle Fizzlegear)
(3494, 3635), -- Gnome Engineering (Tinkerwiz)
(4586, 3637), -- Gnome Engineering (Graham Van Talen)
(7406, 3642), -- The Pledge of Secrecy (Oglethorpe Obnoticus)
(1694, 5541), -- Ammo for Rumbleshot (Loslor Rudge)
(13841, 7202), -- Korrak the Bloodrager (Lieutenant Haggerdin)
(13841, 7382), -- Korrak the Everliving (Lieutenant Haggerdin)
(3362, 7660), -- Wolf Swapping - Arctic Wolf (Ogunaro Wolfrunner)
(3362, 7661), -- Wolf Swapping - Red Wolf (Ogunaro Wolfrunner)
(3685, 7662), -- New Kodo - Teal (Harb Clawhoof)
(3685, 7663), -- New Kodo - Green (Harb Clawhoof)
(7952, 7664), -- Ivory Raptor Replacement (Zjolnir)
(7952, 7665), -- Red Raptor Replacement (Zjolnir)
(4730, 7671), -- Frostsaber Replacement (Lelanai)
(4730, 7672), -- Nightsaber Replacement (Lelanai)
(1261, 7673), -- Frost Ram Exchange (Veron Amberstill)
(1261, 7674), -- Black Ram Exchange (Veron Amberstill)
(7955, 7676), -- White Mechanostrider Replacement (Milli Featherwhistle)
(384, 7677), -- White Stallion Exchange (Katie Hunter)
(14833, 7881), -- Carnival Boots (Chronos)
(14833, 7882), -- Carnival Jerkins (Chronos)
(14833, 7883), -- The World's Largest Gnome! (Chronos)
(14833, 7884), -- Crocolisk Boy and the Bearded Murloc (Chronos)
(14832, 7889), -- Coarse Weightstone (Kerri Hicks)
(14832, 7890), -- Heavy Grinding Stone (Kerri Hicks)
(14832, 7891), -- Green Iron Bracers (Kerri Hicks)
(14832, 7892), -- Big Black Mace (Kerri Hicks)
(14832, 7893), -- Rituals of Strength (Kerri Hicks)
(14841, 7894), -- Copper Modulator (Rinling)
(14841, 7895), -- Whirring Bronze Gizmo (Rinling)
(14841, 7896), -- Green Fireworks (Rinling)
(14841, 7897), -- Mechanical Repair Kits (Rinling)
(14841, 7898), -- Thorium Widget (Rinling)
(14829, 7899), -- Small Furry Paws (Yebb Neblegear)
(14829, 7900), -- Torn Bear Pelts (Yebb Neblegear)
(14829, 7901), -- Soft Bushy Tails (Yebb Neblegear)
(14829, 7902), -- Vibrant Plumes (Yebb Neblegear)
(14829, 7903), -- Evil Bat Eyes (Yebb Neblegear)
(14828, 7940), -- 1200 Tickets - Orb of the Darkmoon (Gelvas Grimegate)
(14829, 8222), -- Glowing Scorpid Blood (Yebb Neblegear)
(5145, 8250), -- Magecraft (Juli Stormkettle)
(5498, 8250), -- Magecraft (Elsharin)
(17513, 8250), -- Magecraft (Harnan)
(17479, 9607), -- Heart of Rage (Gunny)
(17558, 9608), -- Heart of Rage (Caza'rez)
(17712, 9672), -- The Bloodcurse Legacy (Captain Edward Hanes)
(17856, 9714), -- Bring Me Another Shrubbery! (Gzhun'tt)
(17856, 9715), -- Bring Me A Shrubbery! (Gzhun'tt)
(17866, 9719), -- Stalk the Stalker (Khn'nix)
(17841, 9731), -- Drain Schematics (Ysiel Windsinger)
(17884, 9738), -- Lost in Action (Watcher Jhang)
(17240, 9751), -- The Bloodcurse Legacy (Admiral Odesyus)
(17841, 9764), -- Orders from Lady Vashj (Ysiel Windsinger)
(17831, 9911), -- The Count of the Marshes (Watcher Leesa'oh)
(17876, 10283), -- Taretha's Diversion (Thrall)
(18723, 10283), -- Taretha's Diversion (Erozion)
(16819, 10558), -- Revered Among Honor Hold (Force Commander Danath Trollbane)
(22007, 10825), -- The Truth Unorbed (Tree Warden Chawn)
(18481, 10888), -- Trial of the Naaru: Magtheridon (A'dal)
(22421, 10901), -- The Cudgel of Kar'desh (Skar'this the Heretic)
(18528, 10985), -- A Distraction for Akama (Xi'ri)
(25256, 11595), -- The Defense of Warsong Hold (High Overlord Saurfang)
(16128, 13158), -- Discretion is Key (Rhonin)
(31080, 13159), -- Containment (Warden Alturas)
(32963, 13506), -- Reason to Worry (Lieutenant Morra Starbreeze)
(32971, 13591), -- Disturbing Connections (Ranger Glynda Nal'Shea)
(40589, 13825), -- Clamlette Surprise (Dirge Quikcleave)
(12919, 13826), -- Nat Pagle, Angler Extreme (Nat Pagle)
(37779, 24802), -- Wrath of the Lich King (Dark Ranger Loralen)
(37120, 24912), -- Empowerment (Highlord Darion Mograine)
(40487, 25543), -- Circle the Wagons... er, Boats (Razzeric)
(39605, 26865), -- Enemies Below (Garrosh Hellscream)
(44217, 26865), -- Enemies Below (Stone Guard Kurjack)
(36648, 26866), -- Enemies Below (Baine Bloodhoof)
(44217, 26866), -- Enemies Below (Stone Guard Kurjack)
(10181, 26867), -- Enemies Below (Lady Sylvanas Windrunner)
(3188, 26951), -- Cleanup on Isle E. (Master Gadrin)
(48431, 28376), -- Myzerian's Head (Sullah)
(11801, 29298), -- A Smoke-Stained Locket (Rabine Saturna)
(14847, 29464), -- Tools of Divination (Professor Thaddeus Paleo)
(14829, 29508), -- Baby Needs Two Pair of Shoes (Yebb Neblegear)
(54548, 30105), -- The Twilight Prophet (Thrall)
(54972, 30105), -- The Twilight Prophet (Thrall)
(55779, 30105), -- The Twilight Prophet (Thrall)
(58114, 30127), -- Threat from Dojan (Kor Bloodtusk)
(61819, 31031), -- Into the Monastery (Ban Bearheart)
(63014, 31308), -- Learning the Ropes (Marcus Jensen)
(63014, 31309), -- On The Mend (Marcus Jensen)
(63596, 31316), -- Julia, The Pet Tamer (Audrey Burnhep)
(64508, 31367), -- The Lorewalkers (Scrollmaker Resshi)
(64531, 31368), -- The Lorewalkers (Lena Stonebrush)
(64036, 31372), -- The Tillers (Tang Ironhoe)
(64011, 31374), -- The Tillers (Farmhand Dooka)
(64032, 31376), -- Attack At The Temple of the Jade Serpent (Sage Whiteheart)
(64001, 31377), -- Attack At The Temple of the Jade Serpent (Sage Lotusbloom)
(64032, 31378), -- Challenge At The Temple of the Red Crane (Sage Whiteheart)
(64001, 31379), -- Challenge At The Temple of the Red Crane (Sage Lotusbloom)
(64032, 31380), -- Trial At The Temple of the White Tiger (Sage Whiteheart)
(64001, 31381), -- Trial At The Temple of the White Tiger (Sage Lotusbloom)
(64032, 31382), -- Defense At Niuzao Temple (Sage Whiteheart)
(64001, 31383), -- Defense At Niuzao Temple (Sage Lotusbloom)
(60981, 31492), -- The Torch of Strength (Lin Tenderpaw)
(63014, 31550), -- Got one! (Marcus Jensen)
(63061, 31570), -- Got one! (Narzak)
(63061, 31571), -- Learning the Ropes (Narzak)
(63080, 31579), -- Learning the Ropes (Jarson Everlong)
(63083, 31584), -- Got one! (Will Larsons)
(63086, 31585), -- Learning the Ropes (Matty)
(63626, 31588), -- Learning the Ropes (Varzok)
(63626, 31589), -- On The Mend (Varzok)
(63626, 31590), -- Got one! (Varzok)
(63596, 31591), -- Learning the Ropes (Audrey Burnhep)
(63596, 31592), -- On The Mend (Audrey Burnhep)
(63596, 31593), -- Got one! (Audrey Burnhep)
(63014, 31785), -- Level Up! (Marcus Jensen)
(66126, 31813), -- Dagra the Fierce (Zunta)
(63596, 31821), -- Level Up! (Audrey Burnhep)
(63077, 31825), -- Level Up! (Lehna)
(63626, 31827), -- Level Up! (Varzok)
(63086, 31828), -- Level Up! (Matty)
(63061, 31830), -- Level Up! (Narzak)
(63083, 31832), -- Level Up! (Will Larsons)
(63596, 31889), -- Battle Pet Tamers: Kalimdor (Audrey Burnhep)
(66733, 31954), -- Grand Master Mo'ruk (Mo'ruk)
(63596, 32008), -- Audrey Burnhep (Audrey Burnhep)
(63626, 32009), -- Varzok (Varzok)
(58718, 32189), -- A Shabby New Face (Merchant Greenfield)
(70520, 32275), -- Surgical Death (Halduron Brightwing)
(67985, 32294), -- Raining Bones (Scout Captain Elsia)
(70316, 32296), -- Treasures of the Thunder King (Taoshi)
(67990, 32304), -- High Recognition (Lor'themar Theron)
(64582, 32428), -- Pandaren Spirit Tamer (Gentle San)
(67660, 32521), -- Encroaching Force (Archmage Aethas Sunreaver)
(67989, 32521), -- Encroaching Force (Grand Magister Rommath)
(67990, 32521), -- Encroaching Force (Lor'themar Theron)
(70520, 32521), -- Encroaching Force (Halduron Brightwing)
(67660, 32522), -- Imposing Threat (Archmage Aethas Sunreaver)
(67989, 32522), -- Imposing Threat (Grand Magister Rommath)
(67990, 32522), -- Imposing Threat (Lor'themar Theron)
(70520, 32522), -- Imposing Threat (Halduron Brightwing)
(67660, 32523), -- The Beast Pens (Archmage Aethas Sunreaver)
(67989, 32523), -- The Beast Pens (Grand Magister Rommath)
(67990, 32523), -- The Beast Pens (Lor'themar Theron)
(70520, 32523), -- The Beast Pens (Halduron Brightwing)
(67660, 32524), -- The Beating of Troll Drums (Archmage Aethas Sunreaver)
(67989, 32524), -- The Beating of Troll Drums (Grand Magister Rommath)
(67990, 32524), -- The Beating of Troll Drums (Lor'themar Theron)
(70520, 32524), -- The Beating of Troll Drums (Halduron Brightwing)
(67660, 32561), -- Competing Magic (Archmage Aethas Sunreaver)
(67989, 32561), -- Competing Magic (Grand Magister Rommath)
(70520, 32561), -- Competing Magic (Halduron Brightwing)
(67985, 32563), -- Encroaching Force (Scout Captain Elsia)
(67985, 32564), -- Zandalari on the Rise (Scout Captain Elsia)
(67985, 32565), -- Encroaching Force (Scout Captain Elsia)
(67990, 32589), -- Life Blood (Lor'themar Theron)
(64582, 32604), -- Beasts of Fable Book I (Gentle San)
(64231, 32684), -- You Want Fish? (Sungshin Ironpaw)
(67663, 32730), -- Ihgaluk Crag (Magister Hathorel)
(63596, 32863), -- What We've Been Training For (Audrey Burnhep)
(63626, 32863), -- What We've Been Training For (Varzok)
(64582, 32863), -- What We've Been Training For (Gentle San)
(64582, 32868), -- Beasts of Fable Book II (Gentle San)
(64582, 32869), -- Beasts of Fable Book III (Gentle San)
(73082, 33136), -- The Rainy Day is Here (Master Li)
(64231, 33146), -- Noodle Time (Sungshin Ironpaw)
(64231, 33358); -- Noodle Time: Bonus (Sungshin Ironpaw)

-- Quest-giver objects
DELETE FROM `gameobject_queststarter` WHERE (`id`, `quest`) IN (
(187851, 11790),
(206293, 27760),
(187236, 29385),
(214903, 29755),
(212389, 31068));
INSERT INTO `gameobject_queststarter` (`id`, `quest`) VALUES
(187851, 11790), -- Cultists Among Us (Cultist Shrine)
(206293, 27760), -- Artificial Intelligence (A.I.D.A. Terminal)
(187236, 29385), -- A Winter Veil Gift (Winter Veil Gift)
(214903, 29755), -- Pei-Back (The Tan-Chao)
(212389, 31068); -- Sacred Recipe (Scroll of Auspice)

-- Quest turn-ins for quests nobody accepted
DELETE FROM `creature_questender` WHERE (`id`, `quest`) IN (
(2278, 495),
(2594, 615),
(2500, 618),
(12736, 908),
(12736, 909),
(8126, 3526),
(8126, 3629),
(7944, 3630),
(7944, 3632),
(8126, 3633),
(7406, 3635),
(7406, 3637),
(8126, 3639),
(7944, 3641),
(7406, 3642),
(7406, 3643),
(8126, 4181),
(13840, 7181),
(13841, 7202),
(13816, 7221),
(13817, 7222),
(13840, 7381),
(13841, 7382),
(14347, 7522),
(3362, 7660),
(3362, 7661),
(3685, 7662),
(3685, 7663),
(7952, 7664),
(7952, 7665),
(7955, 7675),
(384, 7678),
(16819, 9607),
(3230, 9608),
(18389, 9943),
(18386, 9947),
(15991, 9980),
(18386, 9981),
(16575, 10089),
(19935, 10445),
(17909, 10897),
(22427, 10899),
(22421, 10901),
(19052, 10902),
(17909, 10905),
(19052, 10906),
(22427, 10907),
(17076, 10960),
(23306, 11072),
(25279, 11595),
(37120, 24912),
(44217, 26865),
(44217, 26866),
(44217, 26867),
(3188, 26951),
(44823, 27041),
(58114, 30127),
(58113, 30131),
(63014, 31308),
(63014, 31309),
(58721, 31372),
(58721, 31374),
(60506, 31378),
(60506, 31379),
(60968, 31380),
(60968, 31381),
(60981, 31492),
(63014, 31550),
(63070, 31552),
(63070, 31553),
(63070, 31555),
(63077, 31556),
(63077, 31568),
(63077, 31569),
(63061, 31570),
(63061, 31571),
(63061, 31572),
(63080, 31579),
(63080, 31580),
(63080, 31581),
(63083, 31582),
(63083, 31583),
(63083, 31584),
(63086, 31585),
(63086, 31586),
(63086, 31587),
(63626, 31588),
(63626, 31589),
(63626, 31590),
(63596, 31591),
(63596, 31592),
(63596, 31593),
(63014, 31785),
(66126, 31812),
(63596, 31821),
(63080, 31824),
(63077, 31825),
(63070, 31826),
(63626, 31827),
(63086, 31828),
(63061, 31830),
(63083, 31832),
(63596, 31917),
(63626, 31918),
(66733, 31954),
(63596, 31975),
(63596, 31976),
(63626, 31977),
(63626, 31980),
(63596, 31981),
(63626, 31982),
(63626, 31983),
(63596, 31984),
(63596, 31985),
(63626, 31986),
(63596, 32008),
(63626, 32009),
(70520, 32275),
(67985, 32294),
(70316, 32296),
(67990, 32304),
(64582, 32428),
(67990, 32521),
(67990, 32522),
(67985, 32523),
(67985, 32524),
(67990, 32561),
(67985, 32563),
(67985, 32564),
(67985, 32565),
(67990, 32589),
(64582, 32604),
(67660, 32730),
(63596, 32863),
(63626, 32863),
(64582, 32863),
(64582, 32868),
(64582, 32869),
(73082, 33136),
(64231, 33146),
(61636, 33248),
(61636, 33249),
(64231, 33358));
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(2278, 495), -- The Crown of Will (Melisara)
(2594, 615), -- The Captain's Cutlass (Sprogger)
(2500, 618), -- Facing Negolash (Captain Hecklebury Smotts)
(12736, 908), -- Amongst the Ruins (Je'neu Sancrea)
(12736, 909), -- Baron Aquanis (Je'neu Sancrea)
(8126, 3526), -- Goblin Engineering (Nixx Sprocketspring)
(8126, 3629), -- Goblin Engineering (Nixx Sprocketspring)
(7944, 3630), -- Gnome Engineering (Tinkmaster Overspark)
(7944, 3632), -- Gnome Engineering (Tinkmaster Overspark)
(8126, 3633), -- Goblin Engineering (Nixx Sprocketspring)
(7406, 3635), -- Gnome Engineering (Oglethorpe Obnoticus)
(7406, 3637), -- Gnome Engineering (Oglethorpe Obnoticus)
(8126, 3639), -- Show Your Work (Nixx Sprocketspring)
(7944, 3641), -- Show Your Work (Tinkmaster Overspark)
(7406, 3642), -- The Pledge of Secrecy (Oglethorpe Obnoticus)
(7406, 3643), -- Show Your Work (Oglethorpe Obnoticus)
(8126, 4181), -- Goblin Engineering (Nixx Sprocketspring)
(13840, 7181), -- The Legend of Korrak (Warmaster Laggrond)
(13841, 7202), -- Korrak the Bloodrager (Lieutenant Haggerdin)
(13816, 7221), -- Speak with Prospector Stonehewer (Prospector Stonehewer)
(13817, 7222), -- Speak with Voggah Deathgrip (Voggah Deathgrip)
(13840, 7381), -- The Return of Korrak (Warmaster Laggrond)
(13841, 7382), -- Korrak the Everliving (Lieutenant Haggerdin)
(14347, 7522), -- Examine the Vessel (Highlord Demitrian)
(3362, 7660), -- Wolf Swapping - Arctic Wolf (Ogunaro Wolfrunner)
(3362, 7661), -- Wolf Swapping - Red Wolf (Ogunaro Wolfrunner)
(3685, 7662), -- New Kodo - Teal (Harb Clawhoof)
(3685, 7663), -- New Kodo - Green (Harb Clawhoof)
(7952, 7664), -- Ivory Raptor Replacement (Zjolnir)
(7952, 7665), -- Red Raptor Replacement (Zjolnir)
(7955, 7675), -- Icy Blue Mechanostrider Replacement (Milli Featherwhistle)
(384, 7678), -- Palomino Exchange (Katie Hunter)
(16819, 9607), -- Heart of Rage (Force Commander Danath Trollbane)
(3230, 9608), -- Heart of Rage (Nazgrel)
(18389, 9943), -- Return to Thander (Thander)
(18386, 9947), -- Return to Rokag (Rokag)
(15991, 9980), -- Rescue Deirom! (Lady Dena Kennedy)
(18386, 9981), -- Rescue Dugar! (Rokag)
(16575, 10089), -- Forge Camps of the Legion (Shadow Hunter Ty'jin)
(19935, 10445), -- The Vials of Eternity (Soridormi)
(17909, 10897), -- Master of Potions (Lauranna Thar'well)
(22427, 10899), -- Master of Transmutation (Zarevhi)
(22421, 10901), -- The Cudgel of Kar'desh (Skar'this the Heretic)
(19052, 10902), -- Master of Elixirs (Lorokeem)
(17909, 10905), -- Master of Potions (Lauranna Thar'well)
(19052, 10906), -- Master of Elixirs (Lorokeem)
(22427, 10907), -- Master of Transmutation (Zarevhi)
(17076, 10960), -- When I Grow Up... (Lady Liadrin)
(23306, 11072), -- Adversarial Blood (Hazzik)
(25279, 11595), -- The Defense of Warsong Hold (Overlord Razgor)
(37120, 24912), -- Empowerment (Highlord Darion Mograine)
(44217, 26865), -- Enemies Below (Stone Guard Kurjack)
(44217, 26866), -- Enemies Below (Stone Guard Kurjack)
(44217, 26867), -- Enemies Below (Stone Guard Kurjack)
(3188, 26951), -- Cleanup on Isle E. (Master Gadrin)
(44823, 27041), -- Decryption Made Easy (Examiner Rowe)
(58114, 30127), -- Threat from Dojan (Kor Bloodtusk)
(58113, 30131), -- Life (Sunwalker Dezco)
(63014, 31308), -- Learning the Ropes (Marcus Jensen)
(63014, 31309), -- On The Mend (Marcus Jensen)
(58721, 31372), -- The Tillers (Farmer Yoon)
(58721, 31374), -- The Tillers (Farmer Yoon)
(60506, 31378), -- Challenge At The Temple of the Red Crane (Thelonius)
(60506, 31379), -- Challenge At The Temple of the Red Crane (Thelonius)
(60968, 31380), -- Trial At The Temple of the White Tiger (Xuen)
(60968, 31381), -- Trial At The Temple of the White Tiger (Xuen)
(60981, 31492), -- The Torch of Strength (Lin Tenderpaw)
(63014, 31550), -- Got one! (Marcus Jensen)
(63070, 31552), -- Learning the Ropes (Valeena)
(63070, 31553), -- On The Mend (Valeena)
(63070, 31555), -- Got one! (Valeena)
(63077, 31556), -- Learning the Ropes (Lehna)
(63077, 31568), -- On The Mend (Lehna)
(63077, 31569), -- Got one! (Lehna)
(63061, 31570), -- Got one! (Narzak)
(63061, 31571), -- Learning the Ropes (Narzak)
(63061, 31572), -- On The Mend (Narzak)
(63080, 31579), -- Learning the Ropes (Jarson Everlong)
(63080, 31580), -- On The Mend (Jarson Everlong)
(63080, 31581), -- Got one! (Jarson Everlong)
(63083, 31582), -- Learning the Ropes (Will Larsons)
(63083, 31583), -- On The Mend (Will Larsons)
(63083, 31584), -- Got one! (Will Larsons)
(63086, 31585), -- Learning the Ropes (Matty)
(63086, 31586), -- On The Mend (Matty)
(63086, 31587), -- Got one! (Matty)
(63626, 31588), -- Learning the Ropes (Varzok)
(63626, 31589), -- On The Mend (Varzok)
(63626, 31590), -- Got one! (Varzok)
(63596, 31591), -- Learning the Ropes (Audrey Burnhep)
(63596, 31592), -- On The Mend (Audrey Burnhep)
(63596, 31593), -- Got one! (Audrey Burnhep)
(63014, 31785), -- Level Up! (Marcus Jensen)
(66126, 31812), -- Zunta, The Pet Tamer (Zunta)
(63596, 31821), -- Level Up! (Audrey Burnhep)
(63080, 31824), -- Level Up! (Jarson Everlong)
(63077, 31825), -- Level Up! (Lehna)
(63070, 31826), -- Level Up! (Valeena)
(63626, 31827), -- Level Up! (Varzok)
(63086, 31828), -- Level Up! (Matty)
(63061, 31830), -- Level Up! (Narzak)
(63083, 31832), -- Level Up! (Will Larsons)
(63596, 31917), -- A Tamer's Homecoming (Audrey Burnhep)
(63626, 31918), -- A Tamer's Homecoming (Varzok)
(66733, 31954), -- Grand Master Mo'ruk (Mo'ruk)
(63596, 31975), -- The Returning Champion (Audrey Burnhep)
(63596, 31976), -- The Returning Champion (Audrey Burnhep)
(63626, 31977), -- The Returning Champion (Varzok)
(63626, 31980), -- The Returning Champion (Varzok)
(63596, 31981), -- Exceeding Expectations (Audrey Burnhep)
(63626, 31982), -- Exceeding Expectations (Varzok)
(63626, 31983), -- A Brief Reprieve (Varzok)
(63596, 31984), -- A Brief Reprieve (Audrey Burnhep)
(63596, 31985), -- The Triumphant Return (Audrey Burnhep)
(63626, 31986), -- The Triumphant Return (Varzok)
(63596, 32008), -- Audrey Burnhep (Audrey Burnhep)
(63626, 32009), -- Varzok (Varzok)
(70520, 32275), -- Surgical Death (Halduron Brightwing)
(67985, 32294), -- Raining Bones (Scout Captain Elsia)
(70316, 32296), -- Treasures of the Thunder King (Taoshi)
(67990, 32304), -- High Recognition (Lor'themar Theron)
(64582, 32428), -- Pandaren Spirit Tamer (Gentle San)
(67990, 32521), -- Encroaching Force (Lor'themar Theron)
(67990, 32522), -- Imposing Threat (Lor'themar Theron)
(67985, 32523), -- The Beast Pens (Scout Captain Elsia)
(67985, 32524), -- The Beating of Troll Drums (Scout Captain Elsia)
(67990, 32561), -- Competing Magic (Lor'themar Theron)
(67985, 32563), -- Encroaching Force (Scout Captain Elsia)
(67985, 32564), -- Zandalari on the Rise (Scout Captain Elsia)
(67985, 32565), -- Encroaching Force (Scout Captain Elsia)
(67990, 32589), -- Life Blood (Lor'themar Theron)
(64582, 32604), -- Beasts of Fable Book I (Gentle San)
(67660, 32730), -- Ihgaluk Crag (Archmage Aethas Sunreaver)
(63596, 32863), -- What We've Been Training For (Audrey Burnhep)
(63626, 32863), -- What We've Been Training For (Varzok)
(64582, 32863), -- What We've Been Training For (Gentle San)
(64582, 32868), -- Beasts of Fable Book II (Gentle San)
(64582, 32869), -- Beasts of Fable Book III (Gentle San)
(73082, 33136), -- The Rainy Day is Here (Master Li)
(64231, 33146), -- Noodle Time (Sungshin Ironpaw)
(61636, 33248), -- Proving Grounds (Trial Master Rotun)
(61636, 33249), -- Proving Grounds (Trial Master Rotun)
(64231, 33358); -- Noodle Time: Bonus (Sungshin Ironpaw)

-- Quest turn-in objects
DELETE FROM `gameobject_questender` WHERE (`id`, `quest`) IN (
(184477, 9929),
(184477, 9930),
(187236, 29385));
INSERT INTO `gameobject_questender` (`id`, `quest`) VALUES
(184477, 9929), -- The Missing Merchant (Abandoned Wagon)
(184477, 9930), -- The Missing Merchant (Abandoned Wagon)
(187236, 29385); -- A Winter Veil Gift (Winter Veil Gift)

-- Creature loot for spawned creatures that had none. Rows pointing into mangos reference tables or with mangos conditions
-- are left out; world bosses keep their personal loot
UPDATE `creature_template` SET `lootid` = `entry` WHERE `lootid` = 0 AND `entry` IN (888, 3577, 3578, 3891, 4485, 5086, 5848, 9499, 10881, 12239, 12240, 12241, 12242, 12243, 13776, 13777, 13816, 13840, 13841, 14322, 14325, 15387, 15442, 16845, 17715, 18262, 20889, 21736, 22099, 24291, 24458, 25211, 25301, 27371, 27416, 27545, 28082, 28095, 29592, 29885, 30247, 30341, 30469, 44372, 48182, 63947);
DELETE FROM `creature_loot_template` WHERE `entry` IN (888, 3577, 3578, 3891, 4485, 5086, 5848, 9499, 10881, 12239, 12240, 12241, 12242, 12243, 13776, 13777, 13816, 13840, 13841, 14322, 14325, 15387, 15442, 16845, 17715, 18262, 20889, 21736, 22099, 24291, 24458, 25211, 25301, 27371, 27416, 27545, 28082, 28095, 29592, 29885, 30247, 30341, 30469, 44372, 48182, 63947);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(888, 4538, 16.6667, 1, 0, 1, 1), -- Watcher Dodds: Snapvine Watermelon
(888, 4593, 50, 1, 0, 1, 1), -- Bristle Whisker Catfish
(3577, 414, 6.0124, 1, 0, 1, 1), -- Ambermill Brewmaster: Dalaran Sharp
(3577, 858, 2.3873, 1, 0, 1, 1), -- Lesser Healing Potion
(3577, 1179, 3.8904, 1, 0, 1, 1), -- Ice Cold Milk
(3577, 2455, 1.3263, 1, 0, 1, 1), -- Minor Mana Potion
(3577, 2589, 60.4775, 1, 0, 1, 3), -- Linen Cloth
(3577, 3394, 0.0626, 1, 0, 1, 1), -- Recipe: Potion of Curing
(3577, 5110, 2.9178, 1, 0, 1, 1), -- Dalaran Wizard's Robe
(3577, 7293, -100, 1, 0, 1, 1), -- Dalaran Mana Gem
(3578, 414, 8.3056, 1, 0, 1, 1), -- Ambermill Miner: Dalaran Sharp
(3578, 858, 2.2148, 1, 0, 1, 1), -- Lesser Healing Potion
(3578, 1179, 2.2148, 1, 0, 1, 1), -- Ice Cold Milk
(3578, 2455, 0.7752, 1, 0, 1, 1), -- Minor Mana Potion
(3578, 2589, 59.8007, 1, 0, 1, 3), -- Linen Cloth
(3578, 4767, 2.7685, 1, 0, 1, 1), -- Coppercloth Gloves
(3578, 5110, 2.7685, 1, 0, 1, 1), -- Dalaran Wizard's Robe
(3578, 7293, -100, 1, 0, 1, 1), -- Dalaran Mana Gem
(3891, 5505, -100, 1, 0, 1, 1), -- Teronis' Corpse: Teronis' Journal
(4485, 7974, 33.3333, 1, 0, 1, 1), -- Belgrom Rockmaul: Zesty Clam Meat
(5086, 1707, 6.6667, 1, 0, 1, 1), -- Captain Wymor: Stormwind Brie
(5086, 1710, 3.3333, 1, 0, 1, 1), -- Greater Healing Potion
(5086, 3827, 3.3333, 1, 0, 1, 1), -- Mana Potion
(5086, 4306, 30, 1, 0, 1, 2), -- Silk Cloth
(5848, 929, 1.3536, 1, 0, 1, 1), -- Malgin Barleybrew: Healing Potion
(5848, 1205, 2.3689, 1, 0, 1, 1), -- Melon Juice
(5848, 2589, 14.044, 1, 0, 1, 3), -- Linen Cloth
(5848, 2592, 24.8731, 1, 0, 1, 2), -- Wool Cloth
(5848, 3385, 0.6768, 1, 0, 1, 1), -- Lesser Mana Potion
(5848, 4306, 2.1997, 1, 0, 1, 1), -- Silk Cloth
(5848, 4542, 3.7225, 1, 0, 1, 1), -- Moist Cornbread
(5848, 6566, 2.5381, 1, 0, 1, 1), -- Shimmering Amice
(5848, 6579, 2.8765, 1, 0, 1, 1), -- Defender Spaulders
(9499, 5758, 0.3475, 1, 0, 1, 1), -- Plugger Spazzring: Mithril Lockbox
(9499, 8766, 1.8763, 1, 0, 1, 1), -- Morning Glory Dew
(9499, 8950, 3.7758, 1, 0, 1, 1), -- Homemade Cherry Pie
(9499, 11754, 5, 1, 0, 1, 1), -- Black Diamond
(9499, 13443, 0.3706, 1, 0, 1, 1), -- Superior Mana Potion
(9499, 13446, 0.8802, 1, 0, 1, 1), -- Major Healing Potion
(9499, 14047, 25.8976, 1, 0, 1, 4), -- Runecloth
(9499, 18653, 16.0065, 1, 0, 1, 1), -- Schematic: Goblin Jumper Cables XL
(9499, 18945, 25, 1, 0, 1, 10), -- Dark Iron Residue
(9499, 22528, 10.5861, 1, 0, 1, 4), -- Dark Iron Scraps
(9499, 12791, 10, 1, 1, 1, 1), -- Barman Shanker
(9499, 12793, 30, 1, 1, 1, 1), -- Mixologist's Tunic
(10881, 3927, 16.6667, 1, 0, 1, 1), -- Bluff Runner Windstrider: Fine Aged Cheddar
(10881, 8766, 16.6667, 1, 0, 1, 1), -- Morning Glory Dew
(12239, 1645, 2.1242, 1, 0, 1, 1), -- Spirit of Gelk: Moonberry Juice
(12239, 3864, 0.3268, 1, 0, 1, 1), -- Citrine
(12239, 3928, 1.4706, 1, 0, 1, 1), -- Superior Healing Potion
(12239, 4306, 10.1307, 1, 0, 1, 4), -- Silk Cloth
(12239, 4338, 14.8693, 1, 0, 1, 4), -- Mageweave Cloth
(12239, 4599, 3.7582, 1, 0, 1, 1), -- Cured Ham Steak
(12239, 4637, 0.1634, 1, 0, 1, 1), -- Steel Lockbox
(12239, 14826, 0.3268, 1, 0, 1, 1), -- Symbolic Gauntlets
(12239, 14832, 0.1013, 1, 0, 1, 1), -- Symbolic Vambraces
(12239, 14899, 0.1634, 1, 0, 1, 1), -- Saltstone Helm
(12239, 17684, -25, 1, 0, 1, 1), -- Theradric Crystal Carving
(12239, 17762, 100, 1, 0, 1, 1), -- Gem of the Second Khan
(12240, 3914, 0.2, 1, 0, 1, 1), -- Spirit of Kolk: Journeyman's Backpack
(12240, 4306, 11.1111, 1, 0, 1, 4), -- Silk Cloth
(12240, 4338, 13.7346, 1, 0, 1, 4), -- Mageweave Cloth
(12240, 4599, 4.0123, 1, 0, 1, 1), -- Cured Ham Steak
(12240, 4637, 0.1543, 1, 0, 1, 1), -- Steel Lockbox
(12240, 17684, -25, 1, 0, 1, 1), -- Theradric Crystal Carving
(12240, 17761, 100, 1, 0, 1, 1), -- Gem of the First Khan
(12241, 3914, 0.2, 1, 0, 1, 1), -- Spirit of Magra: Journeyman's Backpack
(12241, 4306, 10.3339, 1, 0, 1, 4), -- Silk Cloth
(12241, 4338, 17.3291, 1, 0, 1, 4), -- Mageweave Cloth
(12241, 4599, 4.4515, 1, 0, 1, 1), -- Cured Ham Steak
(12241, 4637, 0.4769, 1, 0, 1, 1), -- Steel Lockbox
(12241, 17684, -25, 1, 0, 1, 1), -- Theradric Crystal Carving
(12241, 17763, 100, 1, 0, 1, 1), -- Gem of the Third Khan
(12242, 4338, 25.3456, 1, 0, 1, 4), -- Spirit of Maraudos: Mageweave Cloth
(12242, 4599, 5.9908, 1, 0, 1, 1), -- Cured Ham Steak
(12242, 4637, 0.4608, 1, 0, 1, 1), -- Steel Lockbox
(12242, 17684, -25, 1, 0, 1, 1), -- Theradric Crystal Carving
(12242, 17764, 100, 1, 0, 1, 1), -- Gem of the Fourth Khan
(12243, 4338, 19.7349, 1, 0, 1, 4), -- Spirit of Veng: Mageweave Cloth
(12243, 4599, 2.9455, 1, 0, 1, 1), -- Cured Ham Steak
(12243, 4637, 0.5891, 1, 0, 1, 1), -- Steel Lockbox
(12243, 17684, -25, 1, 0, 1, 1), -- Theradric Crystal Carving
(12243, 17765, 100, 1, 0, 1, 1), -- Gem of the Fifth Khan
(13776, 17422, 100, 1, 0, 1, 1), -- Corporal Teeka Bloodsnarl: Armor Scraps
(13776, 17423, -100, 1, 0, 1, 1), -- Storm Crystal
(13776, 17502, 100, 1, 0, 1, 1), -- Frostwolf Soldier's Medal
(13776, 17503, 100, 1, 0, 1, 1), -- Frostwolf Lieutenant's Medal
(13777, 17306, -100, 1, 0, 1, 1), -- Sergeant Durgen Stormpike: Stormpike Soldier's Blood
(13816, 17306, -100, 1, 0, 1, 1), -- Prospector Stonehewer: Stormpike Soldier's Blood
(13816, 17326, 70, 1, 0, 1, 9), -- Stormpike Soldier's Flesh
(13816, 17327, 100, 1, 0, 1, 1), -- Stormpike Lieutenant's Flesh
(13816, 17422, 100, 1, 0, 1, 1), -- Armor Scraps
(13840, 17422, 100, 1, 0, 1, 1), -- Warmaster Laggrond: Armor Scraps
(13840, 17423, -100, 1, 0, 1, 1), -- Storm Crystal
(13840, 17503, 100, 1, 0, 1, 1), -- Frostwolf Lieutenant's Medal
(13840, 17504, 100, 1, 0, 1, 1), -- Frostwolf Commander's Medal
(13841, 17306, -100, 1, 0, 1, 1), -- Lieutenant Haggerdin: Stormpike Soldier's Blood
(13841, 17327, 100, 1, 0, 1, 1), -- Stormpike Lieutenant's Flesh
(13841, 17328, 100, 1, 0, 1, 1), -- Stormpike Commander's Flesh
(13841, 17422, 100, 1, 0, 1, 1), -- Armor Scraps
(14322, 4500, 0.2, 1, 0, 1, 1), -- Stomper Kreeg: Traveler's Backpack
(14322, 5759, 0.5472, 1, 0, 1, 1), -- Thorium Lockbox
(14322, 8766, 2.4624, 1, 0, 1, 1), -- Morning Glory Dew
(14322, 8952, 4.2408, 1, 0, 1, 1), -- Roasted Quail
(14322, 13444, 0.342, 1, 0, 1, 1), -- Major Mana Potion
(14322, 13446, 1.2312, 1, 0, 1, 1), -- Major Healing Potion
(14322, 14047, 29.6854, 1, 0, 1, 4), -- Runecloth
(14322, 18334, 0.342, 1, 0, 1, 1), -- Libram of Protection
(14322, 18425, 37.0041, 1, 0, 1, 1), -- Kreeg's Mug
(14322, 18640, 1.5732, 1, 0, 1, 1), -- Happy Fun Rock
(14322, 22890, 0.2052, 1, 0, 1, 1), -- Tome of Frost Ward
(14322, 2593, 0, 1, 1, 1, 8), -- Flask of Stormwind Tawny
(14322, 2594, 0, 1, 1, 1, 7), -- Flagon of Dwarven Mead
(14322, 2595, 0, 1, 1, 1, 6), -- Jug of Badlands Bourbon
(14322, 2596, 0, 1, 1, 1, 8), -- Skin of Dwarven Stout
(14322, 2723, 0, 1, 1, 1, 7), -- Bottle of Dalaran Noir
(14322, 4595, 0, 1, 1, 1, 7), -- Junglevine Wine
(14322, 4600, 0, 1, 1, 1, 8), -- Cherry Grog
(14325, 5760, 0.3113, 1, 0, 1, 1), -- Captain Kromcrush: Eternium Lockbox
(14325, 8766, 2.6463, 1, 0, 1, 1), -- Morning Glory Dew
(14325, 8952, 5.0436, 1, 0, 1, 1), -- Roasted Quail
(14325, 13444, 0.5293, 1, 0, 1, 1), -- Major Mana Potion
(14325, 13446, 1.0897, 1, 0, 1, 1), -- Major Healing Potion
(14325, 14047, 30.4795, 1, 0, 1, 4), -- Runecloth
(14325, 18250, 15, 1, 0, 1, 1), -- Gordok Shackle Key
(14325, 18334, 0.3736, 1, 0, 1, 1), -- Libram of Protection
(14325, 18640, 1.5567, 1, 0, 1, 1), -- Happy Fun Rock
(14325, 18502, 0, 1, 1, 1, 1), -- Monstrous Glaive
(14325, 18503, 0, 1, 1, 1, 1), -- Kromcrush's Chestplate
(14325, 18505, 0, 1, 1, 1, 1), -- Mugger's Belt
(14325, 18507, 0, 1, 1, 1, 1), -- Boots of the Full Moon
(15387, 21222, 50, 1, 0, 1, 1), -- Qiraji Warrior: Armored Chitin
(15442, 20810, -100, 1, 0, 1, 1), -- Ironforge Brigade Footman: Signed Field Duty Papers
(16845, 21877, 40.3846, 1, 0, 1, 3), -- Gorkan Bloodfist: Netherweave Cloth
(16845, 22832, 1.6129, 1, 0, 1, 1), -- Super Mana Potion
(16845, 24886, 1.9231, 1, 0, 1, 1), -- Skettis Belt
(16845, 25054, 1.9231, 1, 0, 1, 1), -- Sodalite Band
(16845, 27854, 1.9231, 1, 0, 1, 1), -- Smoked Talbuk Venison
(16845, 27860, 1.9231, 1, 0, 1, 1), -- Purified Draenic Water
(17715, 858, 0.9793, 1, 0, 1, 1), -- Atoph the Bloodcursed: Lesser Healing Potion
(17715, 1179, 1.8498, 1, 0, 1, 1), -- Ice Cold Milk
(17715, 2455, 0.5441, 1, 0, 1, 1), -- Minor Mana Potion
(17715, 5523, 40, 1, 0, 1, 1), -- Small Barnacled Clam
(17715, 6289, 4.3526, 1, 0, 1, 3), -- Raw Longjaw Mud Snapper
(17715, 14368, 0.1088, 1, 0, 1, 1), -- Mystic's Shoulder Pads
(17715, 23984, 50, 1, 0, 1, 1), -- Irradiated Crystal Shard
(18262, 14047, 15.3846, 1, 0, 1, 3), -- Unkor the Ruthless: Runecloth
(18262, 21877, 61.5385, 1, 0, 1, 1), -- Netherweave Cloth
(18262, 27854, 23.0769, 1, 0, 1, 1), -- Smoked Talbuk Venison
(20889, 21877, 11.6208, 1, 0, 1, 3), -- Ethereum Prisoner (Group Energy Ball): Netherweave Cloth
(20889, 27854, 1.3761, 1, 0, 1, 1), -- Smoked Talbuk Venison
(20889, 30809, 40, 1, 0, 1, 1), -- Mark of Sargeras
(20889, 31553, 12.844, 1, 0, 1, 1), -- Windchanneller's Mantle
(20889, 31561, 15.7492, 1, 0, 1, 1), -- Skystalker's Shoulders
(20889, 31569, 13.6086, 1, 0, 1, 1), -- Mistshroud Shoulders
(20889, 31577, 14.0673, 1, 0, 1, 1), -- Slatesteel Shoulders
(20889, 31941, -100, 1, 0, 1, 1), -- Mark of the Nexus-King
(20889, 32522, 13.6086, 1, 0, 1, 1), -- Demonic Bulwark
(21736, 22575, 14.2857, 1, 0, 1, 2), -- Wildhammer Defender: Mote of Life
(22099, 21877, 41.0294, 1, 0, 1, 3), -- Wyrmcult Provisioner: Netherweave Cloth
(22099, 22829, 1.2594, 1, 0, 1, 1), -- Super Healing Potion
(22099, 24661, 0.0183, 1, 0, 1, 1), -- Shadow Council Chain
(22099, 24662, 0.0183, 1, 0, 1, 1), -- Shadow Council Boots
(22099, 24666, 0.0183, 1, 0, 1, 1), -- Shadow Council Pants
(22099, 24667, 0.0183, 1, 0, 1, 1), -- Shadow Council Mantle
(22099, 24775, 0.0183, 1, 0, 1, 1), -- Boneshredder Jerkin
(22099, 24780, 0.0183, 1, 0, 1, 1), -- Boneshredder Wristguards
(22099, 24886, 0.0183, 1, 0, 1, 1), -- Skettis Belt
(22099, 24888, 0.0183, 1, 0, 1, 1), -- Skettis Chestpiece
(22099, 24893, 0.0183, 1, 0, 1, 1), -- Skettis Bracer
(22099, 25003, 0.0365, 1, 0, 1, 1), -- Bloodfist Legplates
(22099, 25005, 0.0183, 1, 0, 1, 1), -- Bloodfist Vambraces
(22099, 25054, 0.0365, 1, 0, 1, 1), -- Sodalite Band
(22099, 25222, 0.0183, 1, 0, 1, 1), -- Ceremonial Slayer's Axe
(22099, 25264, 0.0183, 1, 0, 1, 1), -- Pocket Ballista
(22099, 25306, 0.0183, 1, 0, 1, 1), -- Permafrost Dagger
(22099, 25320, 0.0183, 1, 0, 1, 1), -- Queen's Insignia
(22099, 27857, 4.9644, 1, 0, 1, 1), -- Garadar Sharp
(22099, 27860, 2.6282, 1, 0, 1, 1), -- Purified Draenic Water
(22099, 31952, 0.0365, 1, 0, 1, 1), -- Khorium Lockbox
(24291, 22829, 2.6359, 1, 0, 1, 1), -- Anton: Super Healing Potion
(24291, 22832, 1.318, 1, 0, 1, 1), -- Super Mana Potion
(24291, 33444, 4.4747, 1, 0, 1, 1), -- Pungent Seal Whey
(24291, 33452, 5.4475, 1, 0, 1, 1), -- Honey-Spiced Lichen
(24291, 43851, 46.1089, 1, 0, 1, 1), -- Fur Clothing Scraps
(24458, 33452, 30, 1, 0, 1, 1), -- Lydell: Honey-Spiced Lichen
(25211, 34800, -30, 1, 0, 1, 1), -- Cleaver Bmurglbrm: Tempest Mote
(25301, 22829, 2.9595, 1, 0, 1, 1), -- Counselor Talbot: Super Healing Potion
(25301, 22832, 2.3364, 1, 0, 1, 1), -- Super Mana Potion
(25301, 33443, 10, 1, 0, 1, 1), -- Sour Goat Cheese
(25301, 33444, 4.8837, 1, 0, 1, 1), -- Pungent Seal Whey
(25301, 43622, 0.6977, 1, 0, 1, 1), -- Froststeel Lockbox
(25301, 43852, 14.7674, 1, 0, 1, 1), -- Thick Fur Clothing Scraps
(27371, 22829, 4.4444, 1, 0, 1, 1), -- Synipus: Super Healing Potion
(27371, 22832, 6.25, 1, 0, 1, 1), -- Super Mana Potion
(27371, 33443, 9.375, 1, 0, 1, 1), -- Sour Goat Cheese
(27371, 33444, 9.375, 1, 0, 1, 1), -- Pungent Seal Whey
(27371, 43851, 46.875, 1, 0, 1, 1), -- Fur Clothing Scraps
(27416, 22829, 9.6774, 1, 0, 1, 1), -- Pipthwack: Super Healing Potion
(27416, 33443, 16.129, 1, 0, 1, 1), -- Sour Goat Cheese
(27416, 33444, 3.2258, 1, 0, 1, 1), -- Pungent Seal Whey
(27416, 43851, 41.9355, 1, 0, 1, 1), -- Fur Clothing Scraps
(27545, 22829, 2.8169, 1, 0, 1, 1), -- Katja: Super Healing Potion
(27545, 33443, 8.6705, 1, 0, 1, 1), -- Sour Goat Cheese
(27545, 33444, 3.4682, 1, 0, 1, 1), -- Pungent Seal Whey
(27545, 43851, 54.9133, 1, 0, 1, 1), -- Fur Clothing Scraps
(28082, 43852, 20, 1, 0, 1, 1), -- High-Shaman Rakjak: Thick Fur Clothing Scraps
(28095, 33445, 18.1818, 1, 0, 1, 1), -- Tracker Gekgek: Honeymint Tea
(28095, 43852, 33.3333, 1, 0, 1, 1), -- Thick Fur Clothing Scraps
(29592, 33445, 2.2727, 1, 0, 1, 1), -- Brijana: Honeymint Tea
(29592, 33447, 4.5455, 1, 0, 1, 1), -- Runic Healing Potion
(29592, 33448, 2.2727, 1, 0, 1, 1), -- Runic Mana Potion
(29592, 33454, 11.3636, 1, 0, 1, 1), -- Salted Venison
(29592, 42780, 50, 1, 0, 1, 1), -- Relic of Ulduar
(29592, 43851, 25, 1, 0, 1, 1), -- Fur Clothing Scraps
(29592, 43852, 13.6364, 1, 0, 1, 1), -- Thick Fur Clothing Scraps
(29885, 33445, 4.2471, 1, 0, 1, 1), -- Mildred the Cruel: Honeymint Tea
(29885, 33448, 1.1583, 1, 0, 1, 1), -- Runic Mana Potion
(29885, 33454, 5.7915, 1, 0, 1, 1), -- Salted Venison
(29885, 42780, 50, 1, 0, 1, 1), -- Relic of Ulduar
(29885, 43852, 13.8996, 1, 0, 1, 1), -- Thick Fur Clothing Scraps
(30247, 42780, 50, 1, 0, 1, 1), -- Bloodguard Lorga: Relic of Ulduar
(30247, 43852, 25, 1, 0, 1, 1), -- Thick Fur Clothing Scraps
(30341, 37701, 12.5, 1, 0, 1, 3), -- Eisenfaust: Crystallized Earth
(30469, 33445, 2.6316, 1, 0, 1, 1), -- Tracker Val'zij: Honeymint Tea
(30469, 33447, 1.9737, 1, 0, 1, 1), -- Runic Healing Potion
(30469, 33454, 9.8684, 1, 0, 1, 1), -- Salted Venison
(30469, 42780, 50, 1, 0, 1, 1), -- Relic of Ulduar
(30469, 43851, 17.7632, 1, 0, 1, 1), -- Fur Clothing Scraps
(30469, 43852, 19.0789, 1, 0, 1, 1), -- Thick Fur Clothing Scraps
(44372, 67112, 33.3, 1, 0, 1, 1), -- Avalanchion: Brittany's Ceremonial Spaulders
(48182, 20708, 33.3333, 1, 0, 1, 1), -- Shimmershell Snail: Tightly Sealed Trunk
(48182, 54809, -33.3333, 1, 0, 1, 1), -- Rocket Car Parts
(48182, 63127, -33.3333, 1, 0, 1, 1), -- Highborne Scroll
(63947, 72988, 100, 1, 0, 1, 6), -- Shao-Tien Subjugator: Windwool Cloth
(63947, 81404, 48.9547, 1, 0, 1, 1), -- Dried Needle Mushrooms
(63947, 81406, 23.3449, 1, 0, 1, 1), -- Roasted Barley Tea
(63947, 81412, 81.3589, 1, 0, 1, 1), -- Blanched Needle Mushrooms
(63947, 81414, 44.0767, 1, 0, 1, 1), -- Pearl Milk Tea
(63947, 86547, 1.3937, 1, 0, 1, 1), -- Skyshard
(63947, 87779, 12.1951, 1, 0, 1, 1); -- Ancient Guo-Lai Cache Key

-- Borgus Steelhand (Stormwind blacksmithing trainer) teaches what the other city trainers do (Grumnus Steelshaper's list)
DELETE FROM `npc_trainer` WHERE `entry` = 7232;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
SELECT 7232, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel` FROM `npc_trainer` WHERE `entry` = 5164;

-- Vazario Linkgrease (Goblin Engineering trainer, Ratchet)
DELETE FROM `npc_trainer` WHERE `entry` = 8738;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(8738, 8895, 1870, 202, 225, 0),
(8738, 12715, 850, 202, 205, 0),
(8738, 12716, 1275, 202, 205, 0),
(8738, 12717, 1275, 202, 205, 0),
(8738, 12718, 1275, 202, 205, 0),
(8738, 12754, 2210, 202, 235, 0),
(8738, 12755, 2040, 202, 230, 0),
(8738, 12758, 2550, 202, 245, 0),
(8738, 12760, 1275, 202, 205, 0),
(8738, 12908, 2380, 202, 240, 0),
(8738, 30558, 21250, 202, 325, 0),
(8738, 30560, 29750, 202, 340, 0),
(8738, 30563, 42500, 202, 350, 0),
(8738, 30565, 85000, 202, 375, 0),
(8738, 30566, 85000, 202, 375, 0),
(8738, 56514, 127500, 202, 425, 0),
(8738, 84412, 136000, 202, 475, 0),
(8738, 95707, 170000, 202, 500, 0);
