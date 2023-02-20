CREATE TABLE events (
    id int NOT NULL AUTO_INCREMENT,
    device_id varchar(20) NOT NULL,
    type varchar(20) NOT NULL,
    value double NOT NULL,
    created datetime NOT NULL,
    PRIMARY KEY (id)
);