DELETE FROM broadcast_text WHERE ID IN (147278, 147279);
INSERT INTO broadcast_text (ID, Text) VALUES
(147278, "Beneath the shadow of the darkened spire, there is no light, no mercy, only void, and the chaos within."),
(147279, "Right away, General!");
DELETE FROM broadcast_text_locale WHERE ID IN (147278, 147279);
INSERT INTO broadcast_text_locale (ID, locale, Text_lang) VALUES
(147278, "ruRU", "В тени темного шпиля нет ни света, ни милосердия, внутри только пустота и хаос."),
(147279, "ruRU", "Сейчас же, Генерал!");
