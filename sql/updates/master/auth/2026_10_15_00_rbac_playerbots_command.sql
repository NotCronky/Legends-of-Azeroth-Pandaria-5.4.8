-- The bot command is now .playerbots (was .npcbot); same permission id
UPDATE `rbac_permissions` SET `name` = 'Command: playerbots' WHERE `id` = 1344;
