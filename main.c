#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<float.h>
typedef struct Note {
    float NoteMod;
    char NomMod[50];
    struct Note *suivantNote;
}Note;
typedef struct Etudiant{
    int idEt;
    char nomEt[50];
    char prenomEt[50];
    int ageEt;
    Note* notes;
    float moyenneEt;
    struct Etudiant* suivantEt;
}Etudiant;
typedef struct ListeEt{
    Etudiant* tete;
    Etudiant* queue;
    int nombreEtudiants;
}ListeEt;
Etudiant* creerEtudiant(){
    Etudiant* ElementEt;
    ElementEt=(Etudiant*)malloc(sizeof(Etudiant));
    ElementEt->ageEt=0;
    ElementEt->idEt=0;
    strcpy(ElementEt->nomEt, "");
    strcpy(ElementEt->prenomEt, "");
    ElementEt->suivantEt=NULL;
    ElementEt->notes=NULL;
    ElementEt->moyenneEt=0;
    return(ElementEt);
}
ListeEt* creerListeEtudiant(){
    ListeEt* listeEt;
    listeEt=(ListeEt*)malloc(sizeof(ListeEt));
    listeEt->tete=NULL;
    listeEt->queue=NULL;
    listeEt->nombreEtudiants=0;
    return(listeEt);
}
void ajouterEtudiant(ListeEt* listeEt) {
    Etudiant* etudiantAjoute=creerEtudiant();
    if (listeEt==NULL || etudiantAjoute==NULL) {
        exit(EXIT_FAILURE);
    }
    printf("Veuillez entrer ces informations de l\'etudiant a ajouter\n");
    printf("L\'identifiant :");
    scanf("%d",&etudiantAjoute->idEt);
    getchar();  // Consommer le '\n' laissé par scanf précédent
    printf("Le nom :");
    scanf("%49s",etudiantAjoute->nomEt);
    // Lire le prénom avec espaces
    printf("Le prenom :");
    scanf("%49s",etudiantAjoute->prenomEt);
    printf("L\'age :");
    scanf("%d",&etudiantAjoute->ageEt);
    if (listeEt->tete==NULL) {
        listeEt->tete=etudiantAjoute;
        listeEt->queue=etudiantAjoute;
    }else {
        listeEt->queue->suivantEt=etudiantAjoute;
        listeEt->queue=etudiantAjoute;
    }
    listeEt->nombreEtudiants++;
    printf("\nEtudiant ajoute avec succes !\n");
}
void ModifierEtudiant(ListeEt* Liste, const char* nomFichier) {
    if (Liste == NULL) {
        printf("Erreur : La liste n'existe pas.\n");
        return;
    }
    if (Liste->tete == NULL) {
        printf("La liste est vide.\n");
        return;
    }

    int id;
    printf("Veuillez saisir l\'identifiant de l\'etudiant : ");
    scanf("%d", &id);

    Etudiant* actuel = Liste->tete;
    int trouve = 0;

    // Recherche et modification de l'étudiant
    while (actuel != NULL) {
        if (actuel->idEt == id) {
            printf("Etudiant trouve : %s %s, Age : %d\n", actuel->nomEt, actuel->prenomEt, actuel->ageEt);
            printf("Veuillez entrer les nouvelles informations :\n");

            getchar();  // Consommer le '\n' laissé par scanf précédent
            printf("Le nouveau nom :");
            scanf("%49s",&actuel->nomEt);
            // Lire le prénom avec espaces
            printf("Le nouveau prenom :");
            scanf("%49s",&actuel->prenomEt);
            printf("Nouvel age :");
            scanf("%d", &actuel->ageEt);

            printf("Informations mises a jour avec succes !\n");
            trouve = 1;
            break;
        }
        actuel = actuel->suivantEt;
    }

    if (!trouve) {
        printf("L\'etudiant avec l\'identifiant %d n\'existe pas !\n", id);
        return;
    }

    // Mise à jour du fichier
    FILE* fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        printf("Erreur : Impossible d\'ouvrir le fichier %s pour mise a jour.\n", nomFichier);
        return;
    }

    // Réécriture de la liste dans le fichier
    actuel = Liste->tete;
    while (actuel != NULL) {
        fprintf(fichier, "L\'identifiant : %d\nLe nom : %s\nLe prenom : %s\nL\'age : %d ans\n", actuel->idEt, actuel->nomEt, actuel->prenomEt, actuel->ageEt);

        Note* noteActuelle = actuel->notes;
        while (noteActuelle != NULL) {
            fprintf(fichier, "Le nom de module : %s->%.2f\n", noteActuelle->NomMod, noteActuelle->NoteMod);
            noteActuelle = noteActuelle->suivantNote;
        }
        fprintf(fichier, "END\n");
        actuel = actuel->suivantEt;
    }

    fclose(fichier);
    printf("Le fichier %s a ete mis a jour avec succes.\n", nomFichier);
}
void SupprimerEtudiant(ListeEt* Liste, const char* nomFichier) {
    if (Liste == NULL) {
        printf("Erreur : La liste n'existe pas.\n");
        return;
    }
    if (Liste->tete == NULL) {
        printf("La liste est vide.\n");
        return;
    }

    int id;
    printf("Veuillez entrer l\'identifiant de l\'etudiant a supprimer :");
    scanf("%d", &id);

    Etudiant* actuel = Liste->tete;
    Etudiant* precedent = NULL;

    while (actuel != NULL) {
        if (actuel->idEt == id) {
            // Mise à jour des pointeurs pour supprimer l'étudiant
            if (precedent == NULL) {
                Liste->tete = actuel->suivantEt;
            } else {
                precedent->suivantEt = actuel->suivantEt;
            }
            if (actuel == Liste->queue) {
                Liste->queue = precedent;
            }

            // Libérer les notes associées
            Note* noteActuelle = actuel->notes;
            while (noteActuelle != NULL) {
                Note* noteSuivante = noteActuelle->suivantNote;
                free(noteActuelle);
                noteActuelle = noteSuivante;
            }
            // Libérer l'étudiant
            free(actuel);
            Liste->nombreEtudiants--;

            printf("L\'etudiant avec l\'identifiant %d a ete supprime avec succes.\n", id);

            // Mettre à jour le fichier
            FILE* fichier = fopen(nomFichier, "w");
            if (fichier == NULL) {
                printf("Erreur : Impossible d\'ouvrir le fichier %s pour mise a jour.\n", nomFichier);
                return;
            }

            // Réécrire la liste dans le fichier
            Etudiant* etudiantActuel = Liste->tete;
            while (etudiantActuel != NULL) {
                fprintf(fichier, "L\'identifiant : %d\nLe nom : %s\nLe prenom : %s\nL\'age : %d ans\n", etudiantActuel->idEt, etudiantActuel->nomEt, etudiantActuel->prenomEt, etudiantActuel->ageEt);

                Note* note = etudiantActuel->notes;
                while (note != NULL) {
                    fprintf(fichier, "Le nom de module : %s->%.2f\n", note->NomMod, note->NoteMod);
                    note = note->suivantNote;
                }
                fprintf(fichier, "END\n");
                etudiantActuel = etudiantActuel->suivantEt;
            }
            fclose(fichier);
            return;
        }
        precedent = actuel;
        actuel = actuel->suivantEt;
    }
    printf("L\'etudiant avec l\'identifiant %d n\'existe pas dans la liste.\n", id);
}
void AjouterNote(ListeEt* Liste, const char* nomFichier) {
    if (Liste == NULL) {
        printf("Erreur : Liste non initialisee.\n");
        return;
    }
    if (Liste->tete == NULL) {
        printf("La liste est vide.\n");
        return;
    }
    int id;
    printf("Veuillez entrer l\'identifiant de l\'etudiant : ");
    scanf("%d", &id);
    Etudiant* etudiant = Liste->tete;
    while (etudiant != NULL) {
        if (etudiant->idEt == id) {
            printf("Etudiant trouve : %s %s\n", etudiant->nomEt, etudiant->prenomEt);
            // Allocation de mémoire pour une nouvelle note
            Note* nouvelleNote = (Note*)malloc(sizeof(Note));
            if (nouvelleNote == NULL) {
                printf("Erreur d\'allocation memoire.\n");
                return;
            }
            // Saisie des informations de la note
            printf("Veuillez saisir le nom du module : ");
            scanf("%49s",nouvelleNote->NomMod);
            printf("Veuillez saisir la note du module (entre 0 et 20) : ");
            scanf("%f", &nouvelleNote->NoteMod);

            // Validation de la note
            if (nouvelleNote->NoteMod < 0 || nouvelleNote->NoteMod > 20) {
                printf("Erreur : La note doit etre comprise entre 0 et 20.\n");
                free(nouvelleNote); // Libération de mémoire
                return;
            }
            // Ajout de la note à la liste des notes de l'étudiant
            nouvelleNote->suivantNote = etudiant->notes;
            etudiant->notes = nouvelleNote;
            printf("Note ajoutee avec succes au module '%s' avec une note de %.2f \n",
                   nouvelleNote->NomMod, nouvelleNote->NoteMod);
            Note* note = etudiant->notes;
            while (note != NULL) {
                printf( "Le nom de module : %s->%.2f \n", note->NomMod, note->NoteMod);
                note = note->suivantNote;
            }
            // Mise à jour du fichier
            FILE* fichier = fopen(nomFichier, "w");
            if (fichier == NULL) {
                printf("Erreur : Impossible d\'ouvrir le fichier %s pour mise a jour.\n", nomFichier);
                return;
            }
            // Réécriture de la liste dans le fichier
            Etudiant* etudiantActuel = Liste->tete;
            while (etudiantActuel != NULL) {
                fprintf(fichier, "L\'identifiant : %d\nLe nom : %s\nLe prenom : %s\nL\'age : %d ans\n", etudiantActuel->idEt,
                        etudiantActuel->nomEt, etudiantActuel->prenomEt, etudiantActuel->ageEt);

                Note* note = etudiantActuel->notes;
                while (note != NULL) {
                    fprintf(fichier, "Le nom de module : %s->%.2f\n", note->NomMod, note->NoteMod);
                    note = note->suivantNote;
                }
                fprintf(fichier, "END\n");
                etudiantActuel = etudiantActuel->suivantEt;
            }
            fclose(fichier);
            return;
        }
        etudiant = etudiant->suivantEt;
    }
    printf("Aucun etudiant avec l\'identifiant %d n\'a ete trouve.\n", id);
}
void ModifierNote(ListeEt* Liste, const char* nomFichier) {
    if (Liste == NULL) {
        printf("Erreur : Liste non initialisee.\n");
        return;
    }
    if (Liste->tete == NULL) {
        printf("La liste est vide.\n");
        return;
    }

    int id;
    printf("Veuillez entrer l\'identifiant de l\'etudiant :");
    scanf("%d", &id);

    Etudiant* etudiant = Liste->tete;
    while (etudiant != NULL) {
        if (etudiant->idEt == id) {
            printf("Etudiant trouve : %s %s\n", etudiant->nomEt, etudiant->prenomEt);
            if (etudiant->notes == NULL) {
                printf("Aucune note disponible pour cet etudiant.\n");
                return;
            }
            char nomModule[50];
            printf("Veuillez entrer le nom du module :");
            scanf("%49s", nomModule);
            Note* noteActuelle = etudiant->notes;
            while (noteActuelle != NULL) {
                if (strcmp(noteActuelle->NomMod, nomModule) == 0) {
                    printf("Note actuelle pour le module %s : %.2f\n", noteActuelle->NomMod, noteActuelle->NoteMod);
                    printf("Entrez la nouvelle note (entre 0 et 20) : ");
                    float nouvelleNote;
                    scanf("%f", &nouvelleNote);

                    if (nouvelleNote < 0 || nouvelleNote > 20) {
                        printf("Erreur : La note doit etre comprise entre 0 et 20.\n");
                        return;
                    }

                    noteActuelle->NoteMod = nouvelleNote;
                    printf("La note a ete modifiee avec succes !\n");

                    // Mise à jour du fichier
                    FILE* fichier = fopen(nomFichier, "w");
                    if (fichier == NULL) {
                        printf("Erreur : Impossible d\'ouvrir le fichier %s pour mise a jour.\n", nomFichier);
                        return;
                    }

                    // Réécriture de la liste dans le fichier
                    Etudiant* etudiantActuel = Liste->tete;
                    while (etudiantActuel != NULL) {
                        fprintf(fichier, "L'identifiant : %d\nLe nom : %s\nLe prenom : %s\nL'age : %d ans\n",
                                etudiantActuel->idEt, etudiantActuel->nomEt, etudiantActuel->prenomEt, etudiantActuel->ageEt);

                        Note* note = etudiantActuel->notes;
                        while (note != NULL) {
                            fprintf(fichier, "Le nom de module : %s->%.2f\n", note->NomMod, note->NoteMod);
                            note = note->suivantNote;
                        }
                        fprintf(fichier, "END\n");
                        etudiantActuel = etudiantActuel->suivantEt;
                    }
                    fclose(fichier);
                    printf("Le fichier %s a ete mis a jour avec succes.\n", nomFichier);
                    return;
                }
                noteActuelle = noteActuelle->suivantNote;
            }
            printf("Module non trouve pour cet etudiant.\n");
            return;
        }
        etudiant = etudiant->suivantEt;
    }
    printf("Aucun etudiant avec l\'identifiant %d n\'a ete trouve.\n", id);
}
void SupprimerNote(ListeEt* Liste, const char* nomFichier) {
    if (Liste == NULL) {
        printf("Erreur : Liste non initialisee.\n");
        return;
    }
    if (Liste->tete == NULL) {
        printf("La liste est vide.\n");
        return;
    }

    int id;
    printf("Veuillez entrer l\'identifiant de l\'etudiant : ");
    scanf("%d", &id);

    Etudiant* etudiant = Liste->tete;
    while (etudiant != NULL) {
        if (etudiant->idEt == id) {
            printf("Etudiant trouve : %s %s\n", etudiant->nomEt, etudiant->prenomEt);
            if (etudiant->notes == NULL) {
                printf("Aucune note à supprimer pour cet etudiant.\n");
                return;
            }

            char nomModule[50];
            printf("Veuillez entrer le nom du module a supprimer : ");
            scanf("%49s", nomModule);

            Note* actuel = etudiant->notes;
            Note* precedent = NULL;

            while (actuel != NULL) {
                if (strcmp(actuel->NomMod, nomModule) == 0) {
                    if (precedent == NULL) {
                        // Supprimer la première note
                        etudiant->notes = actuel->suivantNote;
                    } else {
                        precedent->suivantNote = actuel->suivantNote;
                    }
                    free(actuel);
                    printf("La note pour le module '%s' a ete supprimee avec succes.\n", nomModule);

                    // Mise à jour du fichier
                    FILE* fichier = fopen(nomFichier, "w");
                    if (fichier == NULL) {
                        printf("Erreur : Impossible d\'ouvrir le fichier %s pour mise a jour.\n", nomFichier);
                        return;
                    }

                    Etudiant* etudiantActuel = Liste->tete;
                    while (etudiantActuel != NULL) {
                        fprintf(fichier, "L\'identifiant : %d\nLe nom : %s\nLe prenom : %s\nL\'age : %d ans\n",
                                etudiantActuel->idEt, etudiantActuel->nomEt, etudiantActuel->prenomEt, etudiantActuel->ageEt);

                        Note* note = etudiantActuel->notes;
                        while (note != NULL) {
                            fprintf(fichier, "Le nom de module : %s->%.2f\n", note->NomMod, note->NoteMod);
                            note = note->suivantNote;
                        }
                        fprintf(fichier, "END\n");
                        etudiantActuel = etudiantActuel->suivantEt;
                    }

                    fclose(fichier);
                    printf("Le fichier %s a ete mis e jour avec succes.\n", nomFichier);
                    return;
                }
                precedent = actuel;
                actuel = actuel->suivantNote;
            }

            printf("Module '%s' introuvable pour cet etudiant.\n", nomModule);
            return;
        }
        etudiant = etudiant->suivantEt;
    }

    printf("Aucun etudiant avec l\'identifiant %d n\'a ete trouve.\n", id);
}
void calculerMoyenneEtSauvegarder(ListeEt* listeEtudiants, const char* nomFichier) {
    if (listeEtudiants == NULL || listeEtudiants->tete == NULL) {
        printf("La liste des etudiants est vide ou non initialisee.\n");
        return;
    }

    // Ouvrir le fichier en mode écriture
    FILE* fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        perror("Erreur d\'ouverture du fichier");
        return;
    }

    Etudiant* etudiant = listeEtudiants->tete;

    // Parcourir tous les étudiants
    while (etudiant != NULL) {
        Note* noteActuelle = etudiant->notes;
        float sommeNotes = 0;
        int nombreNotes = 0;

        // Écriture des informations de l'étudiant
        fprintf(fichier,"L\'identifiant : %d\nLe nom : %s\nLe prenom : %s\nL\'age : %d ans\n", etudiant->idEt, etudiant->nomEt, etudiant->prenomEt, etudiant->ageEt);

        // Parcourir les notes de l'étudiant
        while (noteActuelle != NULL) {
            fprintf(fichier, "Le nom de module : %s->%.2f\n", noteActuelle->NomMod, noteActuelle->NoteMod);// Sauvegarde de chaque note
            sommeNotes += noteActuelle->NoteMod;
            nombreNotes++;
            noteActuelle = noteActuelle->suivantNote;
        }
        // Calcul de la moyenne
        if (nombreNotes > 0) {
            etudiant->moyenneEt = sommeNotes / nombreNotes;
        } else {
            etudiant->moyenneEt = 0;  // Pas de notes, moyenne = 0
        }

        // Écriture de la moyenne
        fprintf(fichier, "Moyenne : %.2f\n\n", etudiant->moyenneEt);
        fprintf(fichier,"------------------------------------------------------------------------------------------------------------------\n");
        // Passer à l'étudiant suivant
        etudiant = etudiant->suivantEt;
    }
    // Fermer le fichier
    fclose(fichier);
}
void chargerEtudiantsAvecMoyennes(ListeEt* liste, const char* nomFichier) {
    if (liste == NULL) {
        printf("Erreur d\'allocation memoire\n");
        exit(EXIT_FAILURE);
    }

    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d\'ouvrir le fichier %s en lecture.\n", nomFichier);
        return;
    }

    while (!feof(fichier)) {
        Etudiant* nouvelEtudiant = creerEtudiant();
        if (fscanf(fichier, "L\'identifiant : %d\nLe nom : %s\nLe prenom : %s\nL\'age : %d ans\n",
                   &nouvelEtudiant->idEt, nouvelEtudiant->nomEt, nouvelEtudiant->prenomEt, &nouvelEtudiant->ageEt) != 4) {
            free(nouvelEtudiant);
            break; // Fin du fichier ou erreur
        }

        // Lire les notes associées
        nouvelEtudiant->notes = NULL;
        Note* derniereNote = NULL;
        while (1) {
            char ligne[100];
            if (fgets(ligne, sizeof(ligne), fichier) == NULL) {
                break;
            }

            if (strcasecmp(ligne, "END\n") == 0) {
                break; // Fin des notes pour cet étudiant
            }

            Note* nouvelleNote = (Note*)malloc(sizeof(Note));
            if (sscanf(ligne, "Le nom de module : %[^-]->%f\n", nouvelleNote->NomMod, &nouvelleNote->NoteMod)) {
                nouvelleNote->suivantNote = NULL;
                if (derniereNote == NULL) {
                    nouvelEtudiant->notes = nouvelleNote;
                    derniereNote = nouvelleNote;
                } else {
                    derniereNote->suivantNote = nouvelleNote;
                    derniereNote = nouvelleNote;
                }
            } else {
                free(nouvelleNote);
            }
        }
        fscanf(fichier, "La moyenne : %f\n", &nouvelEtudiant->moyenneEt);
        // Ajouter l'étudiant à la liste
        if (liste->tete == NULL) {
            liste->tete = nouvelEtudiant;
            liste->queue = nouvelEtudiant;
        } else {
            liste->queue->suivantEt = nouvelEtudiant;
            liste->queue = nouvelEtudiant;
        }
        liste->nombreEtudiants++;
    }

    fclose(fichier);
}
void genererRapport(ListeEt* liste) {
    if (liste == NULL || liste->tete == NULL) {
        printf("La liste est vide ou non initialisee.\n");
        return;
    }
    int idRecherche;
    printf("Veuillez entrer l\identifiant de l\'etudiant a generer le rapport :");
    scanf("%d", &idRecherche);
    Etudiant* etudiant = liste->tete;
    while (etudiant != NULL) {
        if (etudiant->idEt == idRecherche) {
            // L'étudiant avec l'ID recherché est trouvé, affichage des informations
            printf("Informations de l\'etudiant :\n");
            printf("Identifiant : %d\n", etudiant->idEt);
            printf("Nom : %s\n", etudiant->nomEt);
            printf("Prenom : %s\n", etudiant->prenomEt);
            printf("Age : %d ans\n", etudiant->ageEt);
            // Afficher les notes de l'étudiant
            Note* note = etudiant->notes;
            if (note == NULL) {
                printf("Aucune note disponible pour cet etudiant.\n");
            } else {
                printf("Notes :\n");
                while (note != NULL) {
                    printf("Module : %s -> Note : %.2f\n", note->NomMod, note->NoteMod);
                    note = note->suivantNote;
                }
            }
            printf("Moyenne : %.2f\n", etudiant->moyenneEt);
            return; // L'étudiant a été trouvé, on sort de la fonction
        }
        etudiant = etudiant->suivantEt;
    }

    // Si l'étudiant avec l'ID recherché n'est pas trouvé
    printf("Aucun etudiant trouve avec l'identifiant %d.\n", idRecherche);
}
Etudiant* rechercheParId(ListeEt* listeEt) {
    int idRecherche;
    if (listeEt==NULL) {
        printf("Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }
    if (listeEt->tete==NULL) {
        printf("La liste est vide !");
        return NULL;
    }
    printf("Veuillez entrer l\'identifiant de l\'etudiant a rechercher:");
    scanf("%d",&idRecherche);
    Etudiant* actuel=listeEt->tete;
    while (actuel!=NULL) {
        if (actuel->idEt==idRecherche) {
            printf("Etudiant trouve :%s %s",actuel->nomEt,actuel->prenomEt);
            return actuel;
        }
        actuel=actuel->suivantEt;
    }
    return NULL;
}
Etudiant* rechercheParNom(ListeEt* listeEt) {
    char nomRecherche[50];
    if (listeEt==NULL) {
        printf("Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }
    if (listeEt->tete==NULL) {
        printf("La liste est vide !");
        return NULL;
    }
    printf("Veuillez entrer le nom de l\'etudiant a rechercher :");
    scanf("%49s",&nomRecherche);
    Etudiant* actuel=listeEt->tete;
    while (actuel!=NULL) {
        if (strcmp(actuel->nomEt,nomRecherche)==0) {
            printf("Etudiant trouve :%s %s",actuel->nomEt,actuel->prenomEt);
            return actuel;
        }
        actuel=actuel->suivantEt;
    }
    return NULL;
}
Etudiant* rechercheParAge(ListeEt* listeEt) {
    int ageRecherche;
    if (listeEt==NULL) {
        printf("Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }
    if (listeEt->tete==NULL) {
        printf("La liste est vide !");
        return NULL;
    }
    printf("Veuillez entrer l\'age de l\'etudiant a rechercher :");
    scanf("%d",&ageRecherche);
    Etudiant* actuel=listeEt->tete;
    while (actuel!=NULL) {
        if (actuel->ageEt==ageRecherche) {
            printf("Etudiant trouve :%s %s",actuel->nomEt,actuel->prenomEt);
            return actuel;
        }
        actuel=actuel->suivantEt;
    }
    return NULL;
}
void afficherInformationsEtudiant(ListeEt* liste) {
    int choix;
    int trouve = 0; // Pour savoir si des étudiants ont été trouvés
    Etudiant* etudiantTrouve = NULL;

    if (liste == NULL) {
        printf("Erreur d\'allocation memoire\n");
        exit(EXIT_FAILURE);
    }

    if (liste->tete == NULL) {
        printf("La liste est vide !\n");
        return;
    }

    // Demander à l'utilisateur de choisir un critère de recherche
    printf("Veuillez choisir un critere de recherche:\n");
    printf("1. Rechercher par ID\n");
    printf("2. Rechercher par Nom\n");
    printf("3. Rechercher par Age\n");
    printf("Votre choix : ");
    scanf("%d", &choix);

    // Appeler la fonction appropriée selon le choix de l'utilisateur
    switch (choix) {
        case 1:
            etudiantTrouve = rechercheParId(liste);
            if (etudiantTrouve != NULL) {
                trouve = 1; // Un étudiant a été trouvé
                printf("\nInformations de l\'etudiant trouve :\n");
                printf("ID : %d\n", etudiantTrouve->idEt);
                printf("Nom : %s\n", etudiantTrouve->nomEt);
                printf("Prenom : %s\n", etudiantTrouve->prenomEt);
                printf("Age : %d ans\n", etudiantTrouve->ageEt);

                // Affichage des notes de l'étudiant
                printf("Notes de l\'etudiant :\n");
                Note* noteActuelle = etudiantTrouve->notes;
                while (noteActuelle != NULL) {
                    printf("Module : %s -> Note : %.2f\n", noteActuelle->NomMod, noteActuelle->NoteMod);
                    noteActuelle = noteActuelle->suivantNote;
                }
            }
            break;
        case 2:
            // Recherche par nom
            {
                char nomRecherche[50];
                printf("Veuillez entrer le nom de l\'etudiant a rechercher :");
                scanf("%49s", nomRecherche);
                Etudiant* actuel = liste->tete;
                while (actuel != NULL) {
                    if (strcmp(actuel->nomEt, nomRecherche) == 0) {
                        if (!trouve) {
                            printf("\nEtudiants trouves avec le nom '%s' :\n", nomRecherche);
                        }
                        printf("ID : %d\n", actuel->idEt);
                        printf("Nom : %s\n", actuel->nomEt);
                        printf("Prenom : %s\n", actuel->prenomEt);
                        printf("Age : %d ans\n", actuel->ageEt);

                        // Affichage des notes de l'étudiant
                        Note* noteActuelle = actuel->notes;
                        while (noteActuelle != NULL) {
                            printf("Module : %s -> Note : %.2f\n", noteActuelle->NomMod, noteActuelle->NoteMod);
                            noteActuelle = noteActuelle->suivantNote;
                        }
                        trouve = 1;// Indiquer qu'on a trouvé au moins un étudiant
                        printf("                                              ******\n");
                    }
                    actuel = actuel->suivantEt;
                }
            }
            break;
        case 3:
            // Recherche par âge
            {
                int ageRecherche;
                printf("Veuillez entrer l\'age de l\'etudiant a rechercher :");
                scanf("%d", &ageRecherche);
                Etudiant* actuel = liste->tete;
                while (actuel != NULL) {
                    if (actuel->ageEt == ageRecherche) {
                        if (!trouve) {
                            printf("\nEtudiants trouves avec l\'age '%d' :\n", ageRecherche);
                        }
                        printf("ID : %d\n", actuel->idEt);
                        printf("Nom : %s\n", actuel->nomEt);
                        printf("Prenom : %s\n", actuel->prenomEt);
                        printf("Age : %d ans\n", actuel->ageEt);

                        // Affichage des notes de l'étudiant
                        Note* noteActuelle = actuel->notes;
                        while (noteActuelle != NULL) {
                            printf("Module : %s -> Note : %.2f\n", noteActuelle->NomMod, noteActuelle->NoteMod);
                            noteActuelle = noteActuelle->suivantNote;
                        }
                        trouve = 1; // Indiquer qu'on a trouvé au moins un étudiant
                        printf("                                              ******\n");
                    }
                    actuel = actuel->suivantEt;
                }
            }
            break;
        default:
            printf("Choix invalide !\n");
            return;
    }

    // Si aucun étudiant n'a été trouvé
    if (!trouve) {
        printf("Aucun etudiant trouve avec ce critere.\n");
    }
}
void sauvegarderEtudiants(ListeEt* liste, const char* nomFichier) {
    if (liste == NULL) {
        printf("Erreur d\'allocation memoire\n");
        exit(EXIT_FAILURE);
    }
    FILE* fichier = fopen(nomFichier, "a");
    if (fichier == NULL) {
        printf("Erreur : Impossible d\'ouvrir le fichier %s en ecriture.\n",nomFichier);
        return;
    }

    Etudiant* etudiant = liste->tete;
    while (etudiant != NULL) {
        // Sauvegarder les informations de l'étudiant
        fprintf(fichier,"L\'identifiant : %d\nLe nom : %s\nLe prenom : %s\nL\'age : %d ans\n", etudiant->idEt, etudiant->nomEt, etudiant->prenomEt, etudiant->ageEt);

        // Sauvegarder les notes de l'étudiant
        Note* note = etudiant->notes;
        while (note != NULL) {
            fprintf(fichier, "Le nom de module : %s->%.2f\n", note->NomMod, note->NoteMod);
            note = note->suivantNote;
        }

        fprintf(fichier, "END\n"); // Indiquer la fin des notes pour cet étudiant
        etudiant = etudiant->suivantEt;
    }

    fclose(fichier);
    printf("\nDonnees sauvegardees avec succes dans le fichier %s.\n",nomFichier);
}
void chargerEtudiants(ListeEt* liste, const char* nomFichier) {
    if (liste == NULL) {
        printf("Erreur d\'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d\'ouvrir le fichier %s en lecture.\n", nomFichier);
        return;
    }

    while (!feof(fichier)) {
        Etudiant* nouvelEtudiant = creerEtudiant();
        if (fscanf(fichier, "L\'identifiant : %d\nLe nom : %s\nLe prenom : %s\nL\'age : %d ans\n", &nouvelEtudiant->idEt,nouvelEtudiant->nomEt,nouvelEtudiant->prenomEt,&nouvelEtudiant->ageEt) != 4) {
            free(nouvelEtudiant);
            break; // Fin du fichier ou erreur
        }
        // Lire les notes associées
        nouvelEtudiant->notes = NULL ;
        Note* derniereNote = NULL;

        while (1) {

            char ligne[100];
            if (fgets(ligne, sizeof(ligne), fichier)==NULL) {
                break;
            }

            if (strcasecmp(ligne, "END\n") == 0) {
                break; // Fin des notes pour cet étudiant
            }
            Note* nouvelleNote = (Note*)malloc(sizeof(Note));

            if ((sscanf(ligne, "Le nom de module : %[^-]->%f\n", nouvelleNote->NomMod, &nouvelleNote->NoteMod))) {
                nouvelleNote->suivantNote = NULL;
                if (derniereNote == NULL) {
                    nouvelEtudiant->notes = nouvelleNote;
                    derniereNote = nouvelleNote;
                } else {
                    derniereNote->suivantNote= nouvelleNote;
                    derniereNote = nouvelleNote;
                }

            } else {
                free(nouvelleNote);
            }
        }
        // Ajouter l'étudiant à la liste
        if (liste->tete == NULL) {
            liste->tete = nouvelEtudiant;
            liste->queue = nouvelEtudiant;
        } else {
            liste->queue->suivantEt= nouvelEtudiant;
            liste->queue = nouvelEtudiant;
        }
        liste->nombreEtudiants++;
    }
    fclose(fichier);
}
void afficherResumePerformanceAcademique(ListeEt* liste) {
    if (liste == NULL || liste->tete == NULL) {
        printf("La liste des etudiants est vide ou non initialisee.\n");
        return;
    }

    Etudiant* etudiant = liste->tete;
    while (etudiant != NULL) {
        Note* noteActuelle = etudiant->notes;
        float sommeNotes = 0;
        int nombreNotes = 0;
        while (noteActuelle != NULL) {
            sommeNotes += noteActuelle->NoteMod;
            nombreNotes++;
            noteActuelle = noteActuelle->suivantNote;
        }
        if (nombreNotes > 0) {
            etudiant->moyenneEt = sommeNotes / nombreNotes;
        } else {
            etudiant->moyenneEt = 0;  // Pas de notes, moyenne = 0
        }
        etudiant = etudiant->suivantEt;
    }

    int nombreEtudiants = 0;
    float sommeMoyennes = 0;
    int repartition[5] = {0}; // Répartition : [0-9[, [9-12[, [12-14[, [14-16[, [16-20]]

    Etudiant* meilleurEtudiant = NULL;
    Etudiant* moinsBonEtudiant = NULL;
    float meilleurMoyenne =-FLT_MAX ;  // Utilisation de -FLT_MAX pour garantir la bonne sélection
    float pireMoyenne = FLT_MAX ;       // Utilisation de FLT_MAX pour la pire moyenne

    Etudiant* actuel = liste->tete;
    // Parcourir la liste
    while (actuel != NULL) {
        nombreEtudiants++;
        sommeMoyennes += actuel->moyenneEt;

        // Calcul de la répartition par performance
        if (actuel->moyenneEt < 9) {
            repartition[0]++;
        } else if (actuel->moyenneEt < 12) {
            repartition[1]++;
        } else if (actuel->moyenneEt < 14) {
            repartition[2]++;
        } else if (actuel->moyenneEt < 16) {
            repartition[3]++;
        } else {
            repartition[4]++;
        }

        // Trouver le meilleur étudiant
        if (actuel->moyenneEt > meilleurMoyenne) {
            meilleurMoyenne = actuel->moyenneEt;
            meilleurEtudiant = actuel;
        }

        // Trouver le moins bon étudiant
        if (actuel->moyenneEt < pireMoyenne) {
            pireMoyenne = actuel->moyenneEt;
            moinsBonEtudiant = actuel;
        }

        actuel = actuel->suivantEt;
    }

    // Calculer la moyenne générale
    float moyenneGenerale = (nombreEtudiants > 0) ? (sommeMoyennes / nombreEtudiants) : 0;

    // Afficher les résultats
    printf("\n*******************************************  Resume de la performance academique  **************************************\n");
    printf("                                              Nombre total d\'etudiants : %d\n", nombreEtudiants);
    printf("                                              Moyenne generale de la classe : %.2f\n", moyenneGenerale);

    printf("\n                                              Repartition des performances :\n");
    printf("                                              [0-9[   : %d etudiants\n", repartition[0]);
    printf("                                              [9-12[  : %d etudiants\n", repartition[1]);
    printf("                                              [12-14[ : %d etudiants\n", repartition[2]);
    printf("                                              [14-16[ : %d etudiants\n", repartition[3]);
    printf("                                              [16-20] : %d etudiants\n", repartition[4]);

    if (meilleurEtudiant != NULL) {
        printf("\n                                              Meilleur etudiant :\n");
        printf("                                              ID : %d\n", meilleurEtudiant->idEt);
        printf("                                              Nom : %s\n", meilleurEtudiant->nomEt);
        printf("                                              Prenom : %s\n", meilleurEtudiant->prenomEt);
        printf("                                              Moyenne : %.2f\n", meilleurEtudiant->moyenneEt);
    }

    if (moinsBonEtudiant != NULL) {
        printf("\n                                              Moins bon etudiant :\n");
        printf("                                              ID : %d\n", moinsBonEtudiant->idEt);
        printf("                                              Nom : %s\n", moinsBonEtudiant->nomEt);
        printf("                                              Prenom : %s\n", moinsBonEtudiant->prenomEt);
        printf("                                              Moyenne : %.2f\n", moinsBonEtudiant->moyenneEt);
    }
}
void afficherMenue() {
    printf("\n---------------------------------------------------- MENU DES CHOIX ----------------------------------------------------\n");
printf("\n");
    printf("                                              1-Ajouter un etudiant\n");
    printf("                                              2-Modifier un etudiant\n");
    printf("                                              3-Supprimer un etudiant\n");
    printf("                                              4-Ajouter une note\n");
    printf("                                              5-Modifier une note\n");
    printf("                                              6-Supprimer une note\n");
    printf("                                              7-Afficher etudiant\n");
    printf("                                              8-Generer un rapport academique\n");
    printf("                                              9-Afficher un resume de la performance academique\n");
    printf("\n");
    printf("------------------------------------------------------------------------------------------------------------------------");
}
int main()
{
    Etudiant* Etcreer=creerEtudiant();
    ListeEt* liste=creerListeEtudiant();
    Etudiant etudiant;
    int choix;
    afficherMenue();
    do{
        printf("\n                                              Veuillez saisir votre choix :");
        scanf("%d",&choix);
        switch(choix){
            case 1:ajouterEtudiant(liste);
                   sauvegarderEtudiants(liste,"fatima&aya.txt");/*FICHIER 1*/
            break;
            case 2:chargerEtudiants(liste,"fatima&aya.txt");/*FICHIER 1*/
                   ModifierEtudiant(liste,"fatima&aya.txt");/*FICHIER 1*/
            break;
            case 3:chargerEtudiants(liste,"fatima&aya.txt");/*FICHIER 1*/
                   SupprimerEtudiant(liste,"fatima&aya.txt");/*FICHIER 1*/
            break;
            case 4:chargerEtudiants(liste,"fatima&aya.txt");/*FICHIER 1*/
                   AjouterNote(liste,"fatima&aya.txt");/*FICHIER 1*/
            break;
            case 5:chargerEtudiants(liste,"fatima&aya.txt");/*FICHIER 1*/
                   ModifierNote(liste,"fatima&aya.txt");/*FICHIER 1*/
            break;
            case 6:chargerEtudiants(liste,"fatima&aya.txt");/*FICHIER 1*/
                  SupprimerNote(liste,"fatima&aya.txt");/*FICHIER 1*/
            break;
            case 7:chargerEtudiants(liste,"fatima&aya.txt");/*FICHIER 1*/
                   afficherInformationsEtudiant(liste);
            break;
            case 8:chargerEtudiants(liste,"fatima&aya.txt");/*FICHIER 1*/
                   calculerMoyenneEtSauvegarder(liste,"fatima&ayatun.txt");/*FICHIER 2*/
                   chargerEtudiantsAvecMoyennes(liste,"fatima&ayatun.txt");/*FICHIER 2*/
                   genererRapport(liste);
            break;
            case 9:chargerEtudiants(liste,"fatima&aya.txt");
                   afficherResumePerformanceAcademique(liste);
            break;
            default :printf("Votre choix n\'est pas valide\n");
        }
    }while(choix<1 || choix>9);
    return 0;
}