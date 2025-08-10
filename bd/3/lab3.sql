-- پاکسازی همه چیز
DROP TRIGGER IF EXISTS check_door_status ON Impact;
DROP FUNCTION IF EXISTS update_door_status();
DROP TABLE IF EXISTS Impact, FridgeDoor CASCADE;

-- 1. Создание таблиц
CREATE TABLE IF NOT EXISTS FridgeDoor (
    id SERIAL PRIMARY KEY,
    resistance INTEGER NOT NULL,
    status TEXT NOT NULL DEFAULT 'intact'
);

CREATE TABLE IF NOT EXISTS Impact (
    id SERIAL PRIMARY KEY,
    velociraptor_id INTEGER NOT NULL,
    child_id INTEGER NOT NULL,
    door_id INTEGER REFERENCES FridgeDoor(id),
    impact_type TEXT NOT NULL,
    description TEXT
);

-- 2. Функция триггера
CREATE OR REPLACE FUNCTION update_door_status()
RETURNS TRIGGER AS $$
DECLARE
    total_hits INTEGER;
    door_resistance INTEGER;
BEGIN
    IF NEW.impact_type != 'hit' THEN
        RETURN NEW;
    END IF;

    -- Получаем сопротивление двери
    SELECT resistance INTO door_resistance
    FROM FridgeDoor
    WHERE id = NEW.door_id;

    -- Считаем только действительные удары (hit)
    SELECT COUNT(*) INTO total_hits
    FROM Impact
    WHERE door_id = NEW.door_id
    AND impact_type = 'hit'
    AND id != NEW.id;

    -- Проверяем условие поломки
    IF (total_hits + 1) >= door_resistance THEN
        UPDATE FridgeDoor
        SET status = 'broken'
        WHERE id = NEW.door_id;
        
        RAISE NOTICE 'Дверь % сломана! Удары: %, Сопротивление: %', 
            NEW.door_id, (total_hits + 1), door_resistance;
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- 3. Создание триггера
DROP TRIGGER IF EXISTS check_door_status ON Impact;
CREATE TRIGGER check_door_status
AFTER INSERT ON Impact
FOR EACH ROW
EXECUTE FUNCTION update_door_status();

-- 4. Тестовые данные
TRUNCATE TABLE FridgeDoor, Impact RESTART IDENTITY CASCADE;

-- Дверь с сопротивлением 3
INSERT INTO FridgeDoor (resistance, status) 
VALUES (3, 'intact');

-- Тест
INSERT INTO Impact (velociraptor_id, child_id, door_id, impact_type, description)
VALUES 
    (1, 1, 1, 'hit', 'Первый удар'),
    (1, 2, 1, 'hit', 'Второй удар'),
    (1, 3, 1, 'nothing', 'Третий удар'),
    (1, 4, 1, 'hit', 'Четвертый удар'); -- Должен сломать дверь

-- Проверка результатов
SELECT * FROM FridgeDoor; -- Статус должен быть 'broken'
SELECT * FROM Impact;


--برای چک کردن تریگر های موجود
SELECT trigger_name FROM information_schema.triggers 
WHERE event_object_table = 'impact';
