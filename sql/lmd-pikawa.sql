--- LMD (langage de manipulation de données)

--- Contenu des tables (tests)

-- Table Utilisateur

INSERT INTO Utilisateur(nom,prenom,identifiant,email) VALUES('VAIRA','Thierry','tvaira','tvaira@free.fr');
INSERT INTO Utilisateur(nom,prenom,identifiant,email) VALUES('BEAUMONT','Jérôme','jbeaumont','beaumont@lasalle84.org');

-- Table Capsule

INSERT INTO Capsule (marque,designation,description,intensite) VALUES('Nespresso','Colombia','Arabica cueilli en récolte tardive, unique comme la cordillère des Andes',6);
INSERT INTO Capsule (marque,designation,description,intensite) VALUES('Nespresso','Indonesia','Arabica, né des pluies de Sumatra, labellisé Fairtrade et certifié biologique',8);
INSERT INTO Capsule (marque,designation,description,intensite) VALUES('Nespresso','Ethiopia','Arabica dépulpé par voie sèche sous le soleil bienfaiteur',4);
INSERT INTO Capsule (marque,designation,description,intensite) VALUES('Nespresso','Volluto','Doux et léger',4);
INSERT INTO Capsule (marque,designation,description,intensite) VALUES('Nespresso','Capriccio','Riche et exclusif',5);
INSERT INTO Capsule (marque,designation,description,intensite) VALUES('Nespresso','Cosi','Équilibré et délicatement torréfié',4);
INSERT INTO Capsule (marque,designation,description,intensite) VALUES('Nespresso','Scuro','Idéal pour les amateurs de recettes intenses et équilibrées à base de lait',0);
INSERT INTO Capsule (marque,designation,description,intensite) VALUES('Nespresso','Vanilla Eclair','Saveur éclair à la vanille',0);

-- Table TypeBoisson

INSERT INTO TypeBoisson(idTypeBoisson,type) VALUES
(1,'Ristretto'),
(2,'Espresso'),
(3,'Lungo');

-- Table Magasin

INSERT INTO Magasin(idMagasin,nbRangees) VALUES(1,8);

-- Table StockMagasin

INSERT INTO StockMagasin (idMagasin,idCapsule,quantiteMax,quantite,rangee) VALUES(1,1,4,0,1);
INSERT INTO StockMagasin (idMagasin,idCapsule,quantiteMax,quantite,rangee) VALUES(1,2,4,0,2);
INSERT INTO StockMagasin (idMagasin,idCapsule,quantiteMax,quantite,rangee) VALUES(1,3,4,0,3);
INSERT INTO StockMagasin (idMagasin,idCapsule,quantiteMax,quantite,rangee) VALUES(1,4,4,0,4);
INSERT INTO StockMagasin (idMagasin,idCapsule,quantiteMax,quantite,rangee) VALUES(1,5,4,0,5);
INSERT INTO StockMagasin (idMagasin,idCapsule,quantiteMax,quantite,rangee) VALUES(1,6,4,0,6);
INSERT INTO StockMagasin (idMagasin,idCapsule,quantiteMax,quantite,rangee) VALUES(1,7,4,0,7);
INSERT INTO StockMagasin (idMagasin,idCapsule,quantiteMax,quantite,rangee) VALUES(1,8,4,0,8);

-- Table Statistiques

INSERT INTO Statistiques(idUtilisateur,nombreCafeDuJour,nombreCafeTotal,nombreBacVide,nombreEauRemplie,dureteeEau,qualiteEau) VALUES(1,0,0,0,0,0,0);
INSERT INTO Statistiques(idUtilisateur,nombreCafeDuJour,nombreCafeTotal,nombreBacVide,nombreEauRemplie,dureteeEau,qualiteEau) VALUES(2,0,0,0,0,0,0);

-- Table Preferences

INSERT INTO Preferences(idPreferences,idUtilisateur,capsuleActuelle,typeBoissonActuelle) VALUES(1,2,1,1);
INSERT INTO Preferences(idPreferences,idUtilisateur,capsuleActuelle,typeBoissonActuelle) VALUES(2,2,4,3);
