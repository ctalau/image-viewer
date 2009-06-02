
Driver pentru sistem de fisiere FAT32


== LOW RESOURCES ==
Driverul este intentionat pentru ATMEGA16 si ocupa 512+ bytes  de memorie (un singur buffer de 512).
Sistemul de fisiere e read_only, nu suporta nume lungi , etc.


== GENERIC ==
Se bazeaza pe o functie de citire definita in hw.h ( si implementata temporar in hw.c).


== FILES ==
hw.c / hw.h   -- exemplu de implementare a driverului hw
partitions.c/.h -- parseaza MBR
utils.c/.h	-- functii de debugging / checking
fat.c/fat.h	-- foloseste functiile


LBA - bucati de 512 octeti
-D_LOW_MEM_ pentru a ocupa mai putina memorie







