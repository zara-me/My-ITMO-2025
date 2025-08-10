-- Drop existing tables if they exist
DROP TABLE IF EXISTS Children_Impact CASCADE;
DROP TABLE IF EXISTS Children_Interaction CASCADE;
DROP TABLE IF EXISTS Impact CASCADE;
DROP TABLE IF EXISTS Interaction CASCADE;
DROP TABLE IF EXISTS FridgeDoor CASCADE;
DROP TABLE IF EXISTS Children CASCADE;
DROP TABLE IF EXISTS Velociraptor CASCADE;

-- Create tables
CREATE TABLE Velociraptor (
    Id SERIAL PRIMARY KEY,
    Power INT NOT NULL,
    Behavior TEXT NOT NULL
);

CREATE TABLE Children (
    Id SERIAL PRIMARY KEY,
    Name TEXT NOT NULL,
    Reaction TEXT NOT NULL
);

CREATE TABLE FridgeDoor (
    Id SERIAL PRIMARY KEY,
    Resistance INT NOT NULL,
    Status TEXT NOT NULL
);

CREATE TABLE Interaction (
    Id SERIAL PRIMARY KEY,
    VelociraptorId INT REFERENCES Velociraptor(Id),
    DoorId INT REFERENCES FridgeDoor(Id),
    Result TEXT NOT NULL
);

CREATE TABLE Children_Interaction (
    ChildId INT REFERENCES Children(Id),
    InteractionId INT REFERENCES Interaction(Id),
    PRIMARY KEY (ChildId, InteractionId)
);

CREATE TABLE Impact (
    Id SERIAL PRIMARY KEY,
    VelociraptorId INT REFERENCES Velociraptor(Id),
    DoorId INT REFERENCES FridgeDoor(Id),
    ImpactType TEXT NOT NULL,
    Description TEXT NOT NULL
);

CREATE TABLE Children_Impact (
    ChildId INT REFERENCES Children(Id),
    ImpactId INT REFERENCES Impact(Id),
    PRIMARY KEY (ChildId, ImpactId)
);

-- Insert data into Velociraptor table
INSERT INTO Velociraptor (Power, Behavior) VALUES (10, 'Aggressive');
INSERT INTO Velociraptor (Power, Behavior) VALUES (8, 'Calm');

-- Insert data into Children table
INSERT INTO Children (Name, Reaction) VALUES ('Tim', 'Scared');
INSERT INTO Children (Name, Reaction) VALUES ('Alice', 'Curious');

-- Insert data into FridgeDoor table
INSERT INTO FridgeDoor (Resistance, Status) VALUES (15, 'Closed');
INSERT INTO FridgeDoor (Resistance, Status) VALUES (10, 'Open');

-- Insert data into Interaction table
INSERT INTO Interaction (VelociraptorId, DoorId, Result) VALUES (1, 1, 'Door held');
INSERT INTO Interaction (VelociraptorId, DoorId, Result) VALUES (2, 2, 'Door opened');

-- Insert data into Children_Interaction table
INSERT INTO Children_Interaction (ChildId, InteractionId) VALUES (1, 1); -- Tim participated in Interaction 1
INSERT INTO Children_Interaction (ChildId, InteractionId) VALUES (2, 1); -- Alice participated in Interaction 1
INSERT INTO Children_Interaction (ChildId, InteractionId) VALUES (1, 2); -- Tim participated in Interaction 2

-- Insert data into Impact table
INSERT INTO Impact (VelociraptorId, DoorId, ImpactType, Description) VALUES (1, 1, 'Physical', 'Door resisted');
INSERT INTO Impact (VelociraptorId, DoorId, ImpactType, Description) VALUES (2, 2, 'Psychological', 'Children scared');

-- Insert data into Children_Impact table
INSERT INTO Children_Impact (ChildId, ImpactId) VALUES (1, 1); -- Tim was affected by Impact 1
INSERT INTO Children_Impact (ChildId, ImpactId) VALUES (2, 2); -- Alice was affected by Impact 2