#!/bin/bash

doss=RESULTAT
fich=fichier_erreur.txt

./make_interprete_i.sh

for i in 0 1 2 3 4 5 6 7 8 9
do  
    ./curiosity-test$i Fichier_EXEC_bon/*
    mv $fich "fichier_erreur$i.txt"
    echo "------------------------------------------------"
    echo "Appuyer sur entrée pour continuer..."
    read  num
done

mv fichier_erreur* $doss