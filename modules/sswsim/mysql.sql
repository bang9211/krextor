create database gw;
create user 'gw'@'%' identified by 'skbgw.123';
create user 'gw'@'localhost' identified by 'skbgw.123';
grant all privileges on gw.* to 'gw'@'%' with grant option;
grant all privileges on gw.* to 'gw'@'localhost' with grant option;

use gw;

CREATE TABLE IF NOT EXISTS ims_user (
    publicuid       VARCHAR(64)     NOT NULL,
    privateuid      VARCHAR(64)     NOT NULL,
    homenetdomain   VARCHAR(64),
    regdate         TIMESTAMP       NOT NULL DEFAULT NOW(),
    regstatus       INT,
    expire          INT,
    contact1        VARCHAR(64) NOT NULL,
    contact2        VARCHAR(64),
    contact3        VARCHAR(64),
    contact4        VARCHAR(64),
    contact5        VARCHAR(64),
    PRIMARY KEY (publicuid)
    );
