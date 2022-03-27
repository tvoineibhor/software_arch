CREATE DATABASE IF NOT EXISTS stud;

USE stud

DROP TABLE IF EXISTS Person;

CREATE TABLE Person (
  login INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
  first_name VARCHAR(255) NOT NULL,
  last_name VARCHAR(255) NOT NULL,
  age INT NOT NULL,
  CHECK (age>=0)
);

INSERT INTO Person (first_name, last_name, age) VALUES
('boba', 'bibovich', FLOOR(RAND()*(100-1)+1)),
('biba', 'bobovsky', FLOOR(RAND()*(100-1)+1));
