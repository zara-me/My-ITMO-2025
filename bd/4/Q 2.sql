DROP TABLE IF EXISTS Н_УЧЕНИКИ CASCADE;
DROP TABLE IF EXISTS Н_ОБУЧЕНИЯ CASCADE;
DROP TABLE IF EXISTS Н_ЛЮДИ CASCADE;

CREATE TABLE Н_ЛЮДИ (
    ИД SERIAL PRIMARY KEY,
    ФАМИЛИЯ VARCHAR(100),
    ИМЯ VARCHAR(100),
    ОТЧЕСТВО VARCHAR(100),
    ИНН VARCHAR(20),
    ДАТА_РОЖДЕНИЯ DATE,
    ПОЛ CHAR(1),
    МЕСТО_РОЖДЕНИЯ VARCHAR(255),
    ИНОСТРАН INT,
    КТО_СОЗДАЛ VARCHAR(100),
    КОГДА_СОЗДАЛ TIMESTAMP,
    КТО_ИЗМЕНИЛ VARCHAR(100),
    КОГДА_ИЗМЕНИЛ TIMESTAMP,
    ДАТА_СМЕРТИ DATE
);

CREATE TABLE Н_ОБУЧЕНИЯ (
    ИД SERIAL PRIMARY KEY,
    ЧЛВК_ИД INT REFERENCES Н_ЛЮДИ(ИД),
    НЗК VARCHAR(50),
    КТО_СОЗДАЛ VARCHAR(100),
    КОГДА_СОЗДАЛ TIMESTAMP,
    КТО_ИЗМЕНИЛ VARCHAR(100),
    КОГДА_ИЗМЕНИЛ TIMESTAMP
);

CREATE TABLE Н_УЧЕНИКИ (
    ИД SERIAL PRIMARY KEY,
    ЧЛВК_ИД INT REFERENCES Н_ЛЮДИ(ИД),
    ПРИЗНАК VARCHAR(50),
    СОСТОЯНИЕ VARCHAR(50),
    НАЧАЛО DATE,
    КОНЕЦ DATE,
    ПЛАН_ИД INT,
    П_ПРОК_ИД INT,
    ВИД_ОБУЧ_ИД INT,
    ПРИМЕЧАНИЕ TEXT,
    КТО_СОЗДАЛ VARCHAR(100),
    КОГДА_СОЗДАЛ TIMESTAMP,
    КТО_ИЗМЕНИЛ VARCHAR(100),
    КОГДА_ИЗМЕНИЛ TIMESTAMP,
    ПО_ПРИКАЗУ BOOLEAN,
    В_СВЯЗИ_С VARCHAR(255)
);

INSERT INTO Н_ЛЮДИ (ФАМИЛИЯ, ИМЯ, ОТЧЕСТВО, ИНН, ДАТА_РОЖДЕНИЯ, ПОЛ, МЕСТО_РОЖДЕНИЯ, ИНОСТРАН)
VALUES ('Алексеев', 'Илья', 'Сергеевич', '1234567890', '1980-05-12', 'М', 'Москва', 0);

INSERT INTO Н_ОБУЧЕНИЯ (ЧЛВК_ИД, НЗК)
VALUES (1, '001000');

INSERT INTO Н_УЧЕНИКИ (ЧЛВК_ИД, НАЧАЛО)
VALUES (1, '1990-01-01');

SET enable_seqscan = OFF;

-- 1) Для фильтрации по фамилии:
CREATE INDEX idx_люди_фамилия ON Н_ЛЮДИ(ФАМИЛИЯ);

-- 2) Для фильтрации по дате начала учёбы:
CREATE INDEX idx_ученики_начало ON Н_УЧЕНИКИ(НАЧАЛО);

-- 3) Для фильтрации по НЗК:
CREATE INDEX idx_обучения_нзк ON Н_ОБУЧЕНИЯ(НЗК);

-- 4) Для ускорения JOIN по ЧЛВК_ИД в обучении:
CREATE INDEX idx_обучения_члвк_ид ON Н_ОБУЧЕНИЯ(ЧЛВК_ИД);

-- 5) Для ускорения JOIN по ЧЛВК_ИД в учениках:
CREATE INDEX idx_ученики_члвк_ид ON Н_УЧЕНИКИ(ЧЛВК_ИД);

EXPLAIN ANALYZE
SELECT Н_ЛЮДИ.ФАМИЛИЯ, Н_ОБУЧЕНИЯ.ЧЛВК_ИД, Н_УЧЕНИКИ.НАЧАЛО
FROM Н_ЛЮДИ
RIGHT JOIN Н_ОБУЧЕНИЯ ON Н_ЛЮДИ.ИД = Н_ОБУЧЕНИЯ.ЧЛВК_ИД
RIGHT JOIN Н_УЧЕНИКИ ON Н_ОБУЧЕНИЯ.ЧЛВК_ИД = Н_УЧЕНИКИ.ЧЛВК_ИД
WHERE Н_ЛЮДИ.ФАМИЛИЯ < 'Иванов'
AND Н_ОБУЧЕНИЯ.НЗК = '001000'
AND Н_УЧЕНИКИ.НАЧАЛО < '1996-09-01';
