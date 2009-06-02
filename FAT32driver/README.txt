
Driver pentru sistem de fisiere FAT32


== Functionalitate ==
Driverul este intentionat pentru ATMEGA16 si ocupa 512+ bytes  de memorie.
Are un sistem de cahce-ing pentru a nu citi un sector de mai multe ori la rand.
Sistemul de fisiere e read_only, nu suporta nume lungi , directoare multi-cluster.


== GENERIC ==
Se bazeaza pe o functie de citire definita in hw.h

== FILES ==
hw.c / hw.h   	-- interfata cu driverul de storage
partitions.c/.h -- parseaza MBR
volid.c/h		-- identifica parametrii sistemului de fisiere
file.c/dir.c	-- pun la dispozitie un API asemanator celui din linux.
utils.c/.h		-- functii de debugging / checking
fat.c/fat.h		-- suportul epentru fisiere multi-cluster folosind tabela FAT.








