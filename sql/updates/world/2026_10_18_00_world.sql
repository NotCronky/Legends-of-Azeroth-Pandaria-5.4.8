-- 2026_09_06_00_world.sql moved creature_template.dmg_multiplier into a DUNGEON_NORMAL row of
-- creature_template_difficulty for every creature that had one, but wrote health_mod = 1.0 into those rows.
-- In a dungeon the core takes a creature's health from that row instead of creature_template.Health_mod, so
-- normal-dungeon creatures spawned with a fraction of their health (Wise Mari 1/20, Ook-Ook 1/40). Give the
-- rows back the creature's own health modifier, which is what they had before the migration.
UPDATE creature_template_difficulty d
JOIN creature_template t ON t.entry = d.id
SET d.health_mod = t.Health_mod
WHERE d.difficulty = 'DUNGEON_NORMAL'
  AND d.health_mod = 1
  AND t.Health_mod <> 1;
