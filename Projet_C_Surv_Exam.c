////////////////////////////////////////////////// 	Projet C : Surveillance des examens			Encadré par : M.Redouane	/////////////////////////////////////////////////////////
//////////////////////////////////////////////////																			/////////////////////////////////////////////////////////
//////////////////////////////////////////////////	Realisé par : 	MOSSATI Oussama		&	Mejdaoui Soufiane				/////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "structures.h"
#include "constantes.h"


////////////////////////////////////////////////// 	VARIABLES GLOBALES CONTIENT LES DONNEES		VARIABLES GLOBALES CONTIENT LES DONNEES////////////////////////////////////////////////
////////////////////////////////////////////////// 	VARIABLES GLOBALES CONTIENT LES DONNEES		VARIABLES GLOBALES CONTIENT LES DONNEES////////////////////////////////////////////////

static Enseignant *tab_ens;
static Module *tab_mod;
static Filliere *tab_fil;
static int *salles;

static liste* list_surv;
static liste* list_user;
static User* user;

////////////////////////////////////////////////// 	FONCTIONS PERMETTENT DE CHARGER LES DONNEES 	FONCTIONS PERMETTENT DE CHARGER LES DONNEES	///////////////////////////////////////
////////////////////////////////////////////////// 	FONCTIONS PERMETTENT DE CHARGER LES DONNEES		FONCTIONS PERMETTENT DE CHARGER LES DONNEES ///////////////////////////////////////


void charger_enseignants()
{
	FILE* fich = fopen(F_ENSEIGNANTS,"r");
	Enseignant ens;
	tab_ens = (Enseignant*)malloc(sizeof(Enseignant)*NBR_ENSEIGNANTS);
	int i=0;
	
	while(fread(&ens, sizeof(Enseignant), 1, fich) == 1)
	{
		
		tab_ens[i] = ens;
		i++;
	}
	fclose(fich);
}

void charger_modules()
{
	FILE* fich = fopen(F_MODULES,"r");
	Module mod;
	tab_mod = (Module*)malloc(sizeof(Module)*NBR_MODULES);
	int i=0;
	while(fread(&mod, sizeof(Module), 1, fich) == 1)
	{
		tab_mod[i] = mod;
		i++;
	}
	fclose(fich);
}

void charger_fillieres()
{
	FILE* fich = fopen(F_FILLIERE,"r");
	Filliere fil;
	tab_fil = (Filliere*)malloc(sizeof(Filliere)*NBR_FILLIERES);
	int i=0;
	while(fread(&fil, sizeof(Filliere), 1, fich) == 1)
	{
		tab_fil[i] = fil;
		i++;
	}
	fclose(fich);
}

void charger_salles()
{
	int i;
	salles = (int*)malloc(sizeof(int)*NBR_SALLES);
	for(i=0;i<NBR_SALLES;i++)
		*(salles+i) = i+1;
}

liste* add_tail(liste* first, void* _data)
{
	liste* nouveau = malloc(sizeof(liste));
	liste* p;
	
	nouveau->data = _data;		
	nouveau->next = NULL;
	if(first == NULL)
		first = nouveau;
	else
	{
		p = first;
		while(p->next != NULL)
			p = p->next;
		p->next = nouveau;
	}
	return first;
}

liste* charger(char *nomfichier)
{

	liste *L = NULL;
	FILE *fich = fopen(nomfichier, "r");

	if( !strcmp(nomfichier,F_SURV))
	{
		surveillance* surv = (surveillance*)malloc(sizeof(surveillance));
		while(fread(surv, sizeof(surveillance), 1, fich) == 1)
	    {
			L = add_tail(L, surv);
			surv = (surveillance*)malloc(sizeof(surveillance));
	    }
	}

	else if( !strcmp(nomfichier,F_USER))
	{
		User* u = (User*)malloc(sizeof(User));
		while(fread(u, sizeof(User), 1, fich) == 1)
	    {
			L = add_tail(L, u);
			u = (User*)malloc(sizeof(User));
	    }
	}

	fclose(fich);
    return L;
}



////////////////////////////////////////////// 	FONCTIONS PERMETTENT DE SAUVEGARDER LES DONNEES 	FONCTIONS PERMETTENT DE SAUVEGARDER LES DONNEES	//////////////////////////////////
////////////////////////////////////////////// 	FONCTIONS PERMETTENT DE SAUVEGARDER LES DONNEES		FONCTIONS PERMETTENT DE SAUVEGARDER LES DONNEES //////////////////////////////////////


void sauvgarder_surveillances()
{
	FILE* fich = fopen(F_SURV,"w");
	surveillance* surv = (surveillance*)malloc(sizeof(surveillance));
	liste *p = list_surv;
	
	while(p)
	{
		surv = (surveillance*)p->data;
		fwrite(surv, sizeof(surveillance), 1, fich);
		surv = (surveillance*)malloc(sizeof(surveillance));
		p = p->next;
	}
	fclose(fich);
}

void sauvgarder_users()
{
	FILE* fich = fopen(F_USER,"w");
	User* u = (User*)malloc(sizeof(User));
	liste *p = list_user;
	
	while(p)
	{
		u = (User*)p->data;
		fwrite(u, sizeof(User), 1, fich);
		u = (User*)malloc(sizeof(User));
		p = p->next;
	}
	fclose(fich);
}


////////////////////////////////////////////////// 	FONCTIONS DE BASE	FONCTIONS DE BASE	FONCTIONS DE BASE 	FONCTIONS DE BASE 	//////////////////////////////////////////////////
////////////////////////////////////////////////// 	FONCTIONS DE BASE	FONCTIONS DE BASE	FONCTIONS DE BASE 	FONCTIONS DE BASE 	//////////////////////////////////////////////////


void init()
{
	charger_enseignants();
	charger_modules();
	charger_fillieres();
	charger_salles();
	
	list_surv = charger(F_SURV);
	list_user = charger(F_USER);

	
	user = (User*)malloc(sizeof(User));
}

void destruction()
{
	sauvgarder_surveillances();
	sauvgarder_users();
}


int strcmp_insensitive(char *a, char *b)
{
	for(;; a++,b++)
	{
		int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
		if(d != 0 || !*a)
			return d;
	}
}


char* date_to_char(Date date)
{

	char *res = (char*)malloc(sizeof(char)*11);
	char* temp = (char*)malloc(sizeof(char)*4);
	strcpy(res, "0");
	if(date.jours<10)
		strcat(res, itoa(date.jours, temp, 10));
	else
		strcpy(res, itoa(date.jours, temp, 10));
	if(date.mois<10)
		strcat(res, "/0");
	else
		strcat(res, "/");
	strcat(res, itoa(date.mois, temp, 10));
	strcat(res, "/");
	strcat(res, itoa(date.annee, temp, 10));
	return res;
}

char* duree_to_char(Duree duree)
{

	char *res = (char*)malloc(sizeof(char)*12);
	char* temp = (char*)malloc(sizeof(char)*4);
	strcpy(res, itoa(duree.depart.heure, temp, 10));
	strcat(res, "H");
	if(duree.depart.minute <10)
		strcat(res, "0");
	strcat(res, itoa(duree.depart.minute, temp, 10));
	strcat(res, " - ");
	strcat(res, itoa(duree.fin.heure, temp, 10));
	strcat(res, "H");
	if(duree.fin.minute <10)
		strcat(res, "0");
	strcat(res, itoa(duree.fin.minute, temp, 10));
	return res;
}

int nbr_element(liste *_liste)
{
	liste *p = list_surv;
	int n=0;
	while(p)
	{
		n++;
		p = p->next;
	}
	return n;
}


int passwd_plus_8_char(char *pass)
{
	return (strlen(pass) > 8);
}


int confirmer_mdp(char *pass1, char *pass2 )
{
	return (!strcmp(pass1,pass2));
}

////////////////////////////////////////////	GETTERS		GETTERS		GETTERS		GETTERS		GETTERS		GETTERS		//////////////////////////////////////////////////////////////
////////////////////////////////////////////	GETTERS		GETTERS		GETTERS		GETTERS		GETTERS		GETTERS		//////////////////////////////////////////////////////////////


int get_enseignant_id(char* nom,char* pnom)
{
	int i;

	for(i=0;i<NBR_ENSEIGNANTS;i++)
	{
		if( !strcmp_insensitive(nom, tab_ens[i].nom) && !strcmp_insensitive(pnom, tab_ens[i].prenom))
			return i+1; // Id
	}
	return NONTROUVE;
}

Enseignant get_enseignant(int id_ens)
{
    return tab_ens[id_ens-1];
}

Module get_module(int id_mod)
{
    return tab_mod[id_mod-1];
}

char* get_filliere(int id_module)
{
	return tab_fil[tab_mod[id_module - 1].filliere_id - 1].nom ;
}

int get_filliere_id(char *filliere)
{
	int i;
	for(i=0;i<NBR_FILLIERES;i++)
		if(!strcmp_insensitive(tab_fil[i].nom, filliere))
			return i+1;
	return 0;
}


////////////////////////////////////////	TEST SUR LES DATES LES DUREES, LES SALLES		TEST SUR LES DATES LES DUREES, LES SALLES	//////////////////////////////////////////////
////////////////////////////////////////	TEST SUR LES DATES LES DUREES, LES SALLES		TEST SUR LES DATES LES DUREES, LES SALLES	//////////////////////////////////////////////

int date_valide(Date date)
{
	if(date.annee != 2019 && date.annee != 2020)
		return NONVALIDE;
	if(date.mois<1 || date.mois>12)
		return NONVALIDE;
	if((date.mois == 1 || date.mois == 3 || date.mois == 5 || date.mois == 7 || date.mois == 8 || date.mois == 10 || date.mois == 12) && (date.jours <1 || date.jours>31))
		return NONVALIDE;
	if((date.mois == 4 || date.mois == 6 || date.mois == 9 || date.mois == 11 ) && (date.jours <1 || date.jours>30))
		return NONVALIDE;
	if(date.mois == 2 && (date.jours <1 || date.jours>29))
		return NONVALIDE;

	return VALIDE;
}

int temps_valide(Temps t)
{
	return (( (t.heure >=8 && t.heure <=12) || (t.heure >=14 && t.heure <=18) ) && (t.minute >=0 && t.minute <=59 ));
}

int duree_valide(Duree duree)
{
	if(duree.depart.heure > duree.fin.heure)
		return NONVALIDE;
	else if(duree.depart.heure == duree.fin.heure && duree.depart.minute > duree.fin.minute)
		return NONVALIDE;

	if((duree.depart.heure<8 || duree.fin.heure>12) && (duree.depart.heure<14 || duree.fin.heure>18))
		return NONVALIDE;

	return VALIDE;
}

int equal_dates(Date date1, Date date2)
{
	return (date1.jours == date2.jours && date1.mois == date2.mois && date1.annee == date2.annee);
}

int time_bigger_than_time(Temps t1, Temps t2)
{
	if(t1.heure > t2.heure || (t1.heure == t2.heure && t1.minute > t2.minute))
		return TRUE;

	return FALSE;
}

int duree_separer(Duree duree1, Duree duree2)
{
	if(time_bigger_than_time(duree1.depart, duree2.fin) || time_bigger_than_time(duree2.depart, duree1.fin))
		return TRUE;
	return FALSE;
}


int _salle_disponible(int salle, Date date, Duree duree)
{
	liste *p = list_surv;
	surveillance *s = NULL;
	while(p)
	{
		s = (surveillance*)p->data;
		if(salle == s->id_salle)
			if(equal_dates(s->date, date) && !duree_separer(s->duree, duree))
				return FALSE;
		p = p->next;
	}
	return TRUE;
}

int* salles_disponibles(Date date, Duree duree)
{
	int *salles_disp = (int*)malloc(sizeof(int)*NBR_SALLES);
	int i,j=0;
	for(i=1 ; i<=NBR_SALLES ; i++)
	{
		if(_salle_disponible(i, date, duree))
		{
			salles[j] = i;
			j++;
		}
	}
	salles[j] = 0;				// pour marquer la fin du tableau;
	return salles;
}

int _enseignant_disponible(int ens_id, Date date, Duree duree)
{
	liste *p = list_surv;
	surveillance *s = NULL;
	while(p)
	{
		s = (surveillance*)p->data;
		if(ens_id == s->id_enseignant)
			if(equal_dates(s->date, date) && !duree_separer(s->duree, duree))
				return FALSE;
		p = p->next;
	}
	return TRUE;
}

Enseignant* enseignants_disponibles(Date date, Duree duree)
{
	Enseignant *ens_disp = (Enseignant*)malloc(sizeof(Enseignant)*NBR_ENSEIGNANTS);
	int i,j=0;
	for(i=1 ; i<=NBR_ENSEIGNANTS ; i++)
	{
		if(_enseignant_disponible(i, date, duree))
		{
			ens_disp[j] = get_enseignant(i);
			j++;
		}
	}
	ens_disp[j].id = 0;			// pour marquer la fin du tableau;
	return ens_disp;
}


/////////////////////////////////////////////	SURVEILLANCE	SURVEILLANCE	SURVEILLANCE	SURVEILLANCE	SURVEILLANCE	//////////////////////////////////////////////////////
/////////////////////////////////////////////	SURVEILLANCE	SURVEILLANCE	SURVEILLANCE	SURVEILLANCE	SURVEILLANCE	//////////////////////////////////////////////////////

void afficher_modules()
{
	int i;
	for(i=0;i<NBR_MODULES;i++)
	{
		printf("(%d) %s\n",tab_mod[i].id, tab_mod[i].nom);
		if(i!=1 && i%2 == 0)
			printf("\t\t\t\t");
	}
}

void afficher_surveillances(liste* _liste)
{
	surveillance* s = NULL;
	liste* p = _liste;
	Enseignant ens;
	Module module;
	int salle, i=1;
	Date *date = NULL;
	Duree *duree = NULL;

	if(!p)
		printf("\n\t\t** Aucune Surveillance **\n");
	else
	{	
		printf("\n Numero - Enseignant - Filliere - Module -  Salle   -   Date   -     Duree\n");
		printf(" ======   ==========   ========   ======    =====       ====         =====\n\n");
		while(p)
		{
			s = (surveillance*)p->data;

			ens = get_enseignant(s->id_enseignant);
			module = get_module(s->id_module);
			salle = s->id_salle;

			printf(" (%d) | %c.%s | %s | %s | \n\t\t\t\t\t   Salle %d   %s  %s\n\n",i, ens.prenom[0], ens.nom, get_filliere(module.id) , module.nom, salle, date_to_char(s->date), duree_to_char(s->duree));

			p = p->next;
			i++;
		}
	}
	printf("\n");
}

surveillance* saisie_surveillance()
{
	int ens_num, i;
	int *salles_dispo = NULL;
	Enseignant *ens_dispo = NULL;
	surveillance *s = (surveillance*)malloc(sizeof(surveillance));

	printf("\n============= CHOIX DU MODULE =============\n\n");
    afficher_modules();
    do{
        printf("\n Entrez le numero du module : ");
        scanf("%d",&s->id_module);
        if(s->id_module <1 || s->id_module >23)
            printf("\n ** Mauvais choix .. Ressayer ! **\n");
	}while(s->id_module <1 || s->id_module >23);

	printf("\n============= CHOIX DE DATE ET DUREE =============\n");

	do{
		do{
	        printf("\n Entrez la date d'examen (jj/mm/aaaa): ");
	        scanf("%d/%d/%d", &s->date.jours, &s->date.mois, &s->date.annee);
	        if(!date_valide(s->date))
	            printf("\n\t** Date invalide .. Ressayer ! **\n");
		}while(!date_valide(s->date));

	    do{
	        do{
	            printf("\n Entrez la duree depart d examen (ex: 8h30): ");
	            scanf("%dh%d", &s->duree.depart.heure, &s->duree.depart.minute);
	            if(!temps_valide(s->duree.depart))
	            	printf("\n\t** Temps invalide .. Ressayer ! **\n");
			}while(!temps_valide(s->duree.depart));

			do{
				printf("\n Entrez la duree fin d examen (ex: 8h30): ");
	            scanf("%dh%d", &s->duree.fin.heure, &s->duree.fin.minute);
	            if(!temps_valide(s->duree.fin))
	            	printf("\n\t** Temps invalide .. Ressayer ! **\n");
			}while(!temps_valide(s->duree.fin));

	        if(!duree_valide(s->duree))
	            printf("\n\t** Duree invalide .. Ressayer ! **\n");
		}while(!duree_valide(s->duree));

		printf("\n============= CHOIX DE SALLE D'EXAM =============\n");

		salles_dispo = salles_disponibles(s->date,s->duree);

		if(salles_dispo[0] == 0)
		{
			printf("\n ** Il y a aucune salle disponible le %s entre %s ! **\n");
			printf("\n\t** Frapper une touche pour entrer une autre date/duree ! **\n");
		}

	}while(salles_dispo[0] == 0);

	printf("\n\tFrapper une touche pour afficher les salles disponibles!\n");
	getch();

	printf("\n  =>Les salles disponibles dans cette duree :\n\n  ");
	for(i=0 ; salles_dispo[i]!=0; i++)
		printf(" %d,", salles_dispo[i]);

	do{
		printf("\n\n Choisissez l'un des salles disponibles : ");
		scanf("%d",&s->id_salle);
		if(!_salle_disponible(s->id_salle, s->date, s->duree))
			printf("\n\t** Salle non disponible ou introuvable .. Ressayer ! **\n");
	}while(!_salle_disponible(s->id_salle, s->date, s->duree));

	printf("\n============= CHOIX D'ENSEIGNANT =============\n");

    ens_dispo = enseignants_disponibles(s->date, s->duree);

	printf("\n\tFrapper une touche pour afficher les enseignants disponibles!\n");
    getch();

	printf("\n  => Les enseignants disponibles pour cette date et duree :\n\n");

	i=0;
	while(ens_dispo[i].id!=0)
	{
		printf("   (%d) %c.%s\n", i+1, ens_dispo[i].prenom[0] ,ens_dispo[i].nom);
		if(i % 2 == 0 && i!=1)
			printf("\t\t\t");
		i++;
	}

    do{
		printf("\n Donner le numero d un enseignant disponible dans la liste: ");
		scanf("%d",&ens_num);
		if(ens_num <1 || ens_num > i)
			printf("\n\t** Mauvais choix .. Ressayer ! **\n");
	}while(ens_num <1 || ens_num > i);

	s->id_enseignant = ens_dispo[ens_num - 1].id;

	return s;
}

void ajouter_surveillance(surveillance *surv)
{
    list_surv = add_tail(list_surv, surv);
}

void modifier_surv(int index, surveillance *surv)
{
	liste* p = list_surv;
	while(p && index != 1)
	{
		p = p->next;
		index--;
	}
	p->data = surv;
}


/////////////////////////////////////////////// SUPPRIMER	SUPPRIMER	SUPPRIMER	SUPPRIMER	SUPPRIMER  /////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// SUPPRIMER	SUPPRIMER	SUPPRIMER	SUPPRIMER	SUPPRIMER  /////////////////////////////////////////////////////////////////////////


void supprimer_surv(int index)
{
	liste *q, *p = list_surv;

	if(index == 1)
	{
		list_surv = list_surv->next;
		free(p);
	}
	else
	{
		while(p && index > 2)
		{
			p = p->next;
			index--;
		}
		q = p->next;
		p->next = q->next;
		free(q);
	}
}


// Fontion permet la suppression de tous les surveillance d'un enseignant spécifié par l'utilisateur
int supprimer_surv_ens(int ens_id)
{
		int n = 0;												//le nombre des élèments supprimés
        liste *temp, *current = list_surv;						// 'current' pointeur temporaire sur la tête de la liste, 'temp' pour marquer le noeud à supprimer
        surveillance *s = NULL;									// 's' variable de type surveillance pour Caster les surveillance à chaque itération (de void* à surveillance*)

        s = (surveillance*)list_surv->data;						// Charger le contenu du premier noued (Cast)

        // si la tête de la liste contient l'id d'enseignant à supprimer, on repète la procedure jusqu'au la tête de la liste ne verifie pas la condition
        while(s->id_enseignant == ens_id)
		{
			list_surv = list_surv->next;						// Changer la tête de la liste
			free(current);										// liberer l'espace memoire d'ancienne tête
			n++;												//incrémenter le nombre des surveillances supprimés
			current = list_surv;								// deplacer 'current' vers la tête de la liste pour la prochaine itération
			if(list_surv == NULL)
				return n;
			s = (surveillance*)list_surv->data;					// Charger le contenu du noued siuvant (Cast)
        }

        // Chercher dans les autres noeuds (autre que la tête de la liste)
        while(current->next != NULL)
		{
            s = (surveillance*)current->next->data;
            // tant que on trouve un noeud verifie la condition on le supprime
			while (s->id_enseignant == ens_id)
			{
				temp = current->next;							//marquer le noeud à supprimer
				current->next = current->next->next;			// relier le noeud précédent avec le noeud suivant du noeud à supprimer
				free(temp);										// liberer l'espace mémoire de l'élément à supprimer
				n++;											//incrémenter le nombre des surveillances supprimés
				if(current->next == NULL)
					break;
				else
					s = (surveillance*)current->next->data;			//charger le contenu du noeud suivant
			}
            current = current->next;							//Mise à jour de 'current' pour la prochaine iteration de la boucle exterieur
            if(current == NULL)
            	break;
        }
        return n;
}

int supprimer_surv_mod(int mod_id)
{
		int n = 0;												//le nombre des élèments supprimés
        liste *temp, *current = list_surv;						// 'current' pointeur temporaire sur la tête de la liste, 'temp' pour marquer le noeud à supprimer
        surveillance *s = NULL;									// 's' variable de type surveillance pour Caster les surveillance à chaque itération (de void* à surveillance*)

        s = (surveillance*)list_surv->data;						// Charger le contenu du premier noued (Cast)

        // si la tête de la liste contient l'id d'enseignant à supprimer, on repète la procedure jusqu'au la tête de la liste ne verifie pas la condition
        while(s->id_module == mod_id)
		{
			list_surv = list_surv->next;						// Changer la tête de la liste
			free(current);										// liberer l'espace memoire d'ancienne tête
			n++;												//incrémenter le nombre des surveillances supprimés
			current = list_surv;								// deplacer 'current' vers la tête de la liste pour la prochaine itération
			if(list_surv == NULL)
				return n;
			s = (surveillance*)list_surv->data;					// Charger le contenu du noued siuvant (Cast)
        }

        // Chercher dans les autres noeuds (autre que la tête de la liste)
        while(current->next != NULL)
		{
            s = (surveillance*)current->next->data;
            // tant que on trouve un noeud verifie la condition on le supprime
			while (s->id_module == mod_id)
			{
				temp = current->next;							//marquer le noeud à supprimer
				current->next = current->next->next;			// relier le noeud précédent avec le noeud suivant du noeud à supprimer
				free(temp);										// liberer l'espace mémoire de l'élément à supprimer
				n++;											//incrémenter le nombre des surveillances supprimés
				if(current->next == NULL)
					break;
				else
					s = (surveillance*)current->next->data;			//charger le contenu du noeud suivant
			}
            current = current->next;							//Mise à jour de 'current' pour la prochaine iteration de la boucle exterieur
            if(current == NULL)
            	break;
        }
        return n;
}

int supprimer_surv_date(Date date)
{
		int n = 0;												//le nombre des élèments supprimés
        liste *temp, *current = list_surv;						// 'current' pointeur temporaire sur la tête de la liste, 'temp' pour marquer le noeud à supprimer
        surveillance *s = NULL;									// 's' variable de type surveillance pour Caster les surveillance à chaque itération (de void* à surveillance*)

        s = (surveillance*)list_surv->data;						// Charger le contenu du premier noued (Cast)

        // si la tête de la liste contient l'id d'enseignant à supprimer, on repète la procedure jusqu'au la tête de la liste ne verifie pas la condition
        while(equal_dates(s->date, date))
		{
			list_surv = list_surv->next;						// Changer la tête de la liste
			free(current);										// liberer l'espace memoire d'ancienne tête
			n++;												//incrémenter le nombre des surveillances supprimés
			current = list_surv;								// deplacer 'current' vers la tête de la liste pour la prochaine itération
			if(list_surv == NULL)
				return n;
			s = (surveillance*)list_surv->data;					// Charger le contenu du noued siuvant (Cast)
        }

        // Chercher dans les autres noeuds (autre que la tête de la liste)
        while(current->next != NULL)
		{
            s = (surveillance*)current->next->data;
            // tant que on trouve un noeud verifie la condition on le supprime
			while (equal_dates(s->date, date))
			{
				temp = current->next;							//marquer le noeud à supprimer
				current->next = current->next->next;			// relier le noeud précédent avec le noeud suivant du noeud à supprimer
				free(temp);										// liberer l'espace mémoire de l'élément à supprimer
				n++;											//incrémenter le nombre des surveillances supprimés
				if(current->next == NULL)
					break;
				else
					s = (surveillance*)current->next->data;			//charger le contenu du noeud suivant
			}
            current = current->next;							//Mise à jour de 'current' pour la prochaine iteration de la boucle exterieur
            if(current == NULL)
            	break;
        }
        return n;
}

int supprimer_surv_salle(int salle)
{
		int n = 0;												//le nombre des élèments supprimés
        liste *temp, *current = list_surv;						// 'current' pointeur temporaire sur la tête de la liste, 'temp' pour marquer le noeud à supprimer
        surveillance *s = NULL;									// 's' variable de type surveillance pour Caster les surveillance à chaque itération (de void* à surveillance*)

        s = (surveillance*)list_surv->data;						// Charger le contenu du premier noued (Cast)

        // si la tête de la liste contient l'id d'enseignant à supprimer, on repète la procedure jusqu'au la tête de la liste ne verifie pas la condition
        while(s->id_salle == salle)
		{
			list_surv = list_surv->next;						// Changer la tête de la liste
			free(current);										// liberer l'espace memoire d'ancienne tête
			n++;												//incrémenter le nombre des surveillances supprimés
			current = list_surv;								// deplacer 'current' vers la tête de la liste pour la prochaine itération
			if(list_surv == NULL)
				return n;
			s = (surveillance*)list_surv->data;					// Charger le contenu du noued siuvant (Cast)
        }

        // Chercher dans les autres noeuds (autre que la tête de la liste)
        while(current->next != NULL)
		{
            s = (surveillance*)current->next->data;
            // tant que on trouve un noeud verifie la condition on le supprime
			while (s->id_salle == salle)
			{
				temp = current->next;							//marquer le noeud à supprimer
				current->next = current->next->next;			// relier le noeud précédent avec le noeud suivant du noeud à supprimer
				free(temp);										// liberer l'espace mémoire de l'élément à supprimer
				n++;											//incrémenter le nombre des surveillances supprimés
				if(current->next == NULL)
					break;
				else
					s = (surveillance*)current->next->data;			//charger le contenu du noeud suivant
			}
            current = current->next;							//Mise à jour de 'current' pour la prochaine iteration de la boucle exterieur
            if(current == NULL)
            	break;
        }
        return n;
}


/////////////////////////////////////////////////	RECHERCHER	RECHERCHER	RECHERCHER	RECHERCHER	RECHERCHER	/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////	RECHERCHER	RECHERCHER	RECHERCHER	RECHERCHER	RECHERCHER	/////////////////////////////////////////////////////////////////////


liste* rechercher_par_ens(int ens_id)
{
	liste* p = list_surv;
	liste* temp = NULL;

	surveillance* s;
	while(p)
	{
		s = (surveillance*)p->data;

		if(ens_id == s->id_enseignant)
			temp = add_tail(temp, p->data);

		p = p->next;
	}
	return temp;
}

liste* rechercher_par_module(int id_module)
{
	liste* p = list_surv;
	liste* temp = NULL;

	surveillance* s;
	while(p)
	{
		s = (surveillance*)p->data;
		if(id_module == s->id_module)
			temp = add_tail(temp,p->data);
		p = p->next;
	}
	return temp;
}

liste* rechercher_par_filliere(int id_filliere)
{
	liste* p = list_surv;
	liste* temp = NULL;

	surveillance* s;
	while(p)
	{
		s = (surveillance*)p->data;
		if(id_filliere == get_module(s->id_module).filliere_id)
			temp = add_tail(temp,p->data);
		p = p->next;
	}
	return temp;
}

liste* rechercher_par_salle(int id_salle)
{
	liste* p = list_surv;
	liste* temp = NULL;

	surveillance* S;
	while(p)
	{
		S = (surveillance*)p->data;
		if(id_salle == S->id_salle)
			temp = add_tail(temp,p->data);
		p = p->next;
	}
	return temp;
}

liste* rechercher_par_date(Date date)
{
	liste* p = list_surv;
	liste* temp = NULL;

	surveillance* S;
	while(p)
	{
		S = (surveillance*)p->data;
		if(equal_dates(date, S->date))
			temp = add_tail(temp,p->data);
		p = p->next;
	}
	return temp;
}


//////////////////////////////////////// MANIPULATION DE SESSION							MANIPULATION DE SESSION	/////////////////////////////////////////////////////////////////
////////////////////////////////////////						   MANIPULATION DE SESSION							/////////////////////////////////////////////////////////////////
//////////////////////////////////////// MANIPULATION DE SESSION							MANIPULATION DE SESSION	/////////////////////////////////////////////////////////////////


void user_session(User *u)
{
	strcpy(user->login, u->login) ;
	strcpy(user->password ,u->password);
	user->droit_accee = UTILISATEUR;
}



int est_admin(char* login, char* passwd)
{
	if (!strcmp(login, ADMIN_LOG) && !strcmp(passwd, ADMIN_PASS)) 
		return TRUE;
	return FALSE;
}

User* user_connect(char *login, char *passwd)
{
	liste* p = list_user;

	User* u = NULL;
	while(p)
	{
		u = (User*)p->data;
		if(!strcmp(login, u->login) && !strcmp(passwd, u->password))
			return u;
		p = p->next;
	}
	return NULL;
}


void fin_programme()
{
    int i;
    system("color 6e");
    system("cls");
    
    printf("\n\n\n\n\t\t\t  ____ Fin du Programme ____\n\n");
    printf("\n\n\t\t\t** Recuperation des donnees **\n\n");
    
    for(i=1; i<=80; i++)
    {
        Sleep(40);
        printf("*");
    }
    printf("\n\n\n");
    destruction();
    exit(0);
}

void connection()
{
	int i;
    printf("\n\n\n\t\t\t  Connexion en cours");
    for(i=0; i<4; i++)
    {
        Sleep(350);
        printf(".");
    }
    Sleep(400);
}

void deconnexion()
{
    int i;
    
    system("color 0a");
    system("cls");
    printf("\n\n\n\n\n\t\t\t  Deconnexion en cours");
    for(i=0; i<4; i++)
    {
        Sleep(350);
        printf(".");
    }
    Sleep(400);
}


////////////////////////////////////////	MANIPULATION DES USERS		MANIPULATION DES USERS		MANIPULATION DES USERS	/////////////////////////////////////////////////////////
////////////////////////////////////////	MANIPULATION DES USERS		MANIPULATION DES USERS		MANIPULATION DES USERS	/////////////////////////////////////////////////////////
////////////////////////////////////////	MANIPULATION DES USERS		MANIPULATION DES USERS		MANIPULATION DES USERS	/////////////////////////////////////////////////////////



void afficher_users(liste* _liste)
{
	int i=1;
	User* u = (User*)malloc(sizeof(User));
	liste* p = _liste;
	if(!p)
		printf("\n\t\t** Aucun Utilisateur existe **\n");
	else
	{
		printf("\tNumero\t\tLogin\t\tMot de passe\tDroit d'accee\n");
		printf("\t======\t\t=====\t\t============\t=============\n\n");
		while(p)
		{
			u = (User*)p->data;
			if(u->droit_accee == ADMIN)
				printf("\t (%d) \t\t%s\t\t%s\tAdmin\n", i, u->login, u->password);
			else
				printf("\t (%d) \t\t%s\t\t%s\tUtilisateur\n", i, u->login, u->password);			
			u = (User*)malloc(sizeof(User));
			p = p->next;
			i++;
		}
	}
}


void ajouter_user(User *u)
{
	list_user = add_tail(list_user, u);
}


User* chercher_user(char *login)
{
    liste* p = list_user; 
    User *_user = NULL;
    while(p)
    {
        _user = (User*)p->data;
        if(!strcmp(_user->login , login)) 
			return _user;
        p = p->next;
    }
    return NULL;
}


void supprimer_user(char *login)
{
	liste *prev, *temp = list_user;
	
	User* _user = (User*)temp->data;

	if(!strcmp(_user->login, login))
	{
		list_user = list_user->next;
		free(temp);
		return ;
	}
	
	while(temp && strcmp(_user->login, login) != 0)
	{
		prev = temp;
		temp = temp->next;
		if(temp != NULL)
			_user = (User*)temp->data;
	}
	//si l'utilisateur existe dans la liste
	if(temp != NULL)
	{
		prev->next = temp->next;
		free(temp);
	}
}


void modifier_droit_acc(char *login)
{
	liste* p = list_user;

	User* u = NULL;
	
	while(p)
	{
		u = (User*)p->data;
		if(!strcmp(login, u->login))
		{
			if(u->droit_accee == ADMIN)
				u->droit_accee = UTILISATEUR;
			else
				u->droit_accee = ADMIN;
			p->data = u ;
		}
		p = p->next;
	}
}


void modifier_passwd(char *login, char *new_pass)
{
	liste* p = list_user;

	User* u = NULL;
	
	strcpy(user->password ,new_pass);
	
	while(p)
	{
		u = (User*)p->data;
		if(!strcmp(login, u->login))
		{
			strcpy(u->password ,new_pass);
			p->data = u ;
		}
		p = p->next;
	}
}




////////////////////////////////////////	MENU	MENU	MENU	MENU	MENU	MENU	MENU	MENU	MENU	/////////////////////////////////////////////////////////////////
////////////////////////////////////////MENU	MENU	MENU	MENU	MENU	MENU	MENU	MENU	MENU	MENU/////////////////////////////////////////////////////////////////
////////////////////////////////////////	MENU	MENU	MENU	MENU	MENU	MENU	MENU	MENU	MENU	/////////////////////////////////////////////////////////////////


void menu_supprimer(){

        int num, nbr_surv, choix, ens_id, mod_id, salle;
        char nom[15], prenom[15];
        Date date;
		system("color 0e");
		system("cls");
		printf("\n\t\t\t\t\t\t\tSession: Admin\n");
        printf("\t\t\t\t\t\t\t______________\n");
        printf("\n\n\t\t\t  __MENU DE SUPPRESION__\n");
        printf("\n\t°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n\t°\t\t\t\t\t\t\t       °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     1) Supprimer une surveillance                            °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     2) Supprimer les surveillances d'un enseignant           °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     3) Supprimer les surveillances d'un module               °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     4) Supprimer les surveillances d'une date                °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     5) Supprimer les surveillances d'une salle               °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     6) Retourner au menu principale                          °\n\t°\t\t\t\t\t\t\t       °");
		printf("\n\t°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n");
		fflush(stdin);
        printf("\n\n\t\t==> Votre Choix: ");
        scanf("%d",&choix);

        switch (choix)
        {
            case 1 : 	system("cls");
            			system("color 0f");
						printf("\n  ==> Suppression d'une seule surveillance:\n");
            			printf("      =======================================\n\n");

            			afficher_surveillances(list_surv);
            			nbr_surv = nbr_element(list_surv);

            			do{
            				fflush(stdin);
							printf("\n Entrez le numero de la surveillance a supprimer: ");
            				scanf("%d", &num);
            				if(num<1 || num>nbr_surv)
            					printf("\n\t** Mauvais choix .. Ressayer ! **\n");
						}while(num<1 || num>nbr_surv);

						supprimer_surv(num);
						printf("\n => La surveillance numero %d est bien supprime \n", num);
                        break;

            case 2 : 	system("color 0f");
						printf("\n  ==> Suppression les surveillances d'un enseignant:\n");
            			printf("      ==============================================\n\n");

            			do{
            				fflush(stdin);
							printf("Entrez le nom de l'enseignant: ");
            				gets(nom);
							printf("Entrez le prenom de l'enseignant: ");
            				gets(prenom);
            				ens_id = get_enseignant_id(nom,prenom);
            				if(ens_id == 0)
            					printf("\n\t ** Cet enseignant n'existe pas .. Ressayer ! **\n\n");
						}while(ens_id == 0);

            			num = supprimer_surv_ens(ens_id);
            			if(num == 0)
            				printf("\n => Aucune surveillance concernant Mr %c.%s\n", prenom[0], nom);
            			else
            				printf("\n => %d serveillance(s) de Mr %c.%s est/sont supprime(s)\n", num, prenom[0], nom);
                        break;

            case 3 : 	system("color 0f");
						printf("\n  ==> Suppression les surveillances d'un module:\n");
            			printf("      ===========================================\n\n");

            			fflush(stdin);
						afficher_modules();
					    do{
					        fflush(stdin);
							printf("\n Entrez le numero du module : ");
					        scanf("%d",&mod_id);
					        if(mod_id <1 || mod_id >23)
					            printf("\n ** Mauvais choix .. Ressayer ! **\n");
						}while(mod_id <1 || mod_id >23);

            			num = supprimer_surv_mod(mod_id);
            			if(num == 0)
            				printf("\n => Aucune surveillance concernant le module %s\n", get_module(mod_id).nom);
            			else
            				printf("\n => %d serveillance(s) du module %s est/sont supprime(s)\n", num, get_module(mod_id).nom);
                        break;

            case 4 : 	system("color 0f");
						printf("\n  ==> Suppression les surveillances d'une date:\n");
            			printf("      ===========================================\n");

            			
					    do{
					        fflush(stdin);
							printf("\n Entrez la date d'examen (jj/mm/aaaa): ");
					        scanf("%d/%d/%d", &date.jours, &date.mois, &date.annee);
					        if(!date_valide(date))
					            printf("\n\t** Date invalide .. Ressayer ! **\n");
						}while(!date_valide(date));

            			num = supprimer_surv_date(date);
            			
            			if(num == 0)
            				printf("\n => Aucune surveillance le %s\n", date_to_char(date));
            			else
            				printf("\n => le %s est libre maintenant (%d surveillance(s) supprime(s) )\n", date_to_char(date), num);
            				
                        break;

            case 5 : 	system("color 0f");
						printf("\n  ==> Suppression les surveillances d'une salle:\n");
            			printf("      ===========================================\n\n");


					    do{
					        fflush(stdin);
							printf("\n Entrez le numero de la salle : ");
					        scanf("%d",&salle);
					        if(salle <1 || salle >20)
					            printf("\n ** Salle inexiste .. Ressayer ! **\n");
						}while(salle <1 || salle >20);

            			num = supprimer_surv_salle(salle);
            			if(num == 0)
            				printf("\n => Aucune surveillance concernant la salle %d\n", salle);
            			else
            				printf("\n => %d serveillance(s) de la salle %d est/sont supprime(s)\n", num, salle);
            				
                        break;

			case 6 : 	menu_admin();
                        break;

        	default :	printf("\n\t** Mauvais choix .. Ressayer ! **\n");
						printf("\n\tFrapper une touche pour ressayer !");
        				getch();
        }
}


void menu_recherche()
{
        int num, nbr_surv, choix, ens_id, mod_id, salle, fil_id;
        char nom[15], prenom[15], filliere[5];
        Date date;
        liste* rech_surv;
		
		fflush(stdin);
		
		system("cls");
		system("color 0e");
		printf("\n\t\t\t\t\t\t\tSession: %s\n", user->login);
        printf("\t\t\t\t\t\t\t__________________\n");
        printf("\n\t\t\t__MENU DE RECHERCHE__\n");
        
        printf("\n\t°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n\t°\t\t\t\t\t\t\t       °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     1) Rechercher par enseignant                             °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     2) Rechercher par module                                 °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     3) Rechercher par filliere                               °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     4) Rechercher par salle                                  °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     5) Rechercher par date                                   °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     6) Retourner au menu principale                          °\n\t°\t\t\t\t\t\t\t       °");
		printf("\n\t°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n");
        printf("\n\n\t\t==> Votre Choix: ");
        scanf("%d",&choix);

        switch (choix)
        {
            case 1 : 	printf("\n  ==> Chercher les surveillances d'un enseignant:\n");
            			printf("      ==============================================\n\n");

            			do{
            				fflush(stdin);
							printf("\n\tEntrez le nom de l'enseignant: ");
            				gets(nom);
							printf("\n\tEntrez le prenom de l'enseignant: ");
            				gets(prenom);
            				ens_id = get_enseignant_id(nom,prenom);
            				if(ens_id == 0)
            					printf("\n\t ** Cet enseignant n'existe pas .. Ressayer ! **\n\n");
						}while(ens_id == 0);

            			rech_surv = rechercher_par_ens(ens_id);

            			printf("\n  => Le(s) surveillance(s) de Mr %c.%s : ", prenom[0], nom);
            			printf("\n  => ------------------------------------------ \n\n");
            			afficher_surveillances(rech_surv);

                        break;

            case 2 : 	printf("\n  ==> Chercher les surveillances d'un module:\n");
            			printf("      =========================================\n\n");

            			fflush(stdin);
						afficher_modules();
					    do{
					        fflush(stdin);
							printf("\n Entrez le numero du module : ");
					        scanf("%d",&mod_id);
					        if(mod_id <1 || mod_id >23)
					            printf("\n ** Mauvais choix .. Ressayer ! **\n");
						}while(mod_id <1 || mod_id >23);

            			rech_surv = rechercher_par_module(mod_id);

            			printf("\n  => Le(s) surveillance(s) du module %s : \n\n", get_module(mod_id).nom);
            			afficher_surveillances(rech_surv);

                        break;

            case 3 : 	printf("\n  ==> Chercher les surveillances d'une filliere:\n");
            			printf("      ============================================\n\n");

					    do{
					        fflush(stdin);
							printf("\n Entrez la filliere a chercher : ");
					        gets(filliere);
					        fil_id = get_filliere_id(filliere);
					        if(!fil_id)
					            printf("\n ** Filliere n'existe pas .. Ressayer ! **\n");
						}while(!fil_id);

            			rech_surv = rechercher_par_filliere(fil_id);

            			printf("\n  => Le(s) surveillance(s) de la filliere %s : \n\n", filliere);
            			afficher_surveillances(rech_surv);

                        break;

            case 4 : 	printf("\n  ==> Chercher les surveillances d'une salle:\n");
            			printf("      ===========================================\n\n");

            			fflush(stdin);
					    do{
					        fflush(stdin);
							printf("\n Entrez le numero de la salle : ");
					        scanf("%d",&salle);
					        if(salle <1 || salle >20)
					            printf("\n ** Salle inexiste .. Ressayer ! **\n");
						}while(salle <1 || salle >20);

            			rech_surv = rechercher_par_salle(salle);

            			printf("\n  => Le(s) surveillance(s) de la salle %d : \n\n", salle);
            			afficher_surveillances(rech_surv);

                        break;

            case 5 : 	printf("\n  ==> Chercher les surveillances d'une date:\n");
            			printf("      ========================================\n");

					    do{
					        fflush(stdin);
							printf("\n Entrez la date d'examen (jj/mm/aaaa): ");
					        scanf("%d/%d/%d", &date.jours, &date.mois, &date.annee);
					        if(!date_valide(date))
					            printf("\n\t** Date invalide .. Ressayer ! **\n");
						}while(!date_valide(date));

            			rech_surv = rechercher_par_date(date);

            			printf("\n  => Le(s) surveillance(s) de %s : \n\n", date_to_char(date));
            			afficher_surveillances(rech_surv);

                        break;

			case 6 : 	if(user->droit_accee == ADMIN)
							menu_admin();
						else
							menu_user();
                        break;

        	default :	printf("\n\t** Mauvais choix .. Ressayer ! **\n");
						printf("\n\tFrapper une touche pour ressayer !");
        				getch();
        }
}


void menu_gestion_users()
{
	int choix;
	char log[15];
	char flag = '\0', droit_acc = '\0', answer = '\0';
	User *u = NULL;
	
	while(1)
	{
		
		fflush(stdin);
		
		system("cls");
		system("color 0e");
		printf("\n\t\t\t\t\t\t\tSession: Admin\n");
        printf("\t\t\t\t\t\t\t______________\n");
        printf("\n\n\t\t\t    __MENU GESTION DES USERS__\n");
        
        printf("\n\t°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n\t°\t\t\t\t\t\t\t       °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     1) Afficher les utilisateurs                             °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     2) Supprimer un utilisateur                              °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     3) Modifier le droit d'accee d'un utilisateur            °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     4) Rechercher un utilisateur                             °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     5) Retourner au menu principale                          °\n\t°\t\t\t\t\t\t\t       °");
		printf("\n\t°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n");
        printf("\n\n\t\t==> Votre Choix: ");
        scanf("%d",&choix);
		
        switch (choix)
        {
            case 1 : 	system("cls");
            			system("color 0f");
						printf("\n  ==> Les Utilisateurs:\n");
            			printf("      ===================\n\n");
						afficher_users(list_user);
						
						printf("\n\tFrapper une touche pour retourner au menu principale !");
                        getch();
                        break;

            case 2 : 	system("cls");
            			system("color 0f");
						if(list_user == NULL)
            			{
            				printf("\n\n\t\t\t** Aucun Utilisateur **\n");
            				printf("\n\tFrapper une touche pour retourner au menu principale !");
                        	getch();
            				break;
						}
						
						flag = '\0';
						
            			do{
            				system("cls");
							system("color 0f");
							printf("\n  ==> Supprimer un utilisateur:\n");
            				printf("      ===========================\n\n");
							fflush(stdin);
							printf("\n\tEntrez le login d'utilisateur a supprimer: ");
            				gets(log);
            				if(chercher_user(log) == NULL)
            				{
								system("color 0c");
								printf("\n   ** Cet utilisateur n'existe pas .. Pour ressayer entrez 'r' ou 'R' **\n\n");
            					flag = getch();
							}
							else
							{
								printf("\n\n  ** Voulez vous vraiment supprimer l'utilisateur '%s' ? (O/N) ", log);
								answer = getch();
								if(answer == 'o' || answer == 'O')
								{
									supprimer_user(log);
									printf("\n\n  => L'utilisateur %s est bien supprimer\n\n", log);
									flag = '\0';
								}
							}
            					
						}while(flag == 'R' || flag == 'r');
						
						
                        printf("\n\tFrapper une touche pour retourner au menu principale !");
                        getch();
                        break;

            case 3 : 	system("cls");
            			system("color 0f");
						if(list_user == NULL)
            			{
            				printf("\n\n\t\t\t** Aucun Utilisateur **\n");
            				printf("\n\tFrapper une touche pour retourner au menu principale !");
                        	getch();
            				break;
						}
            			
						flag = '\0';
						droit_acc = '\0';
						
            			do{
            				system("cls");
            				system("color 0f");
							printf("\n  ==> Modifier le droit d'accee d'un utilisateur:\n");
            				printf("      =============================================\n\n");
            				fflush(stdin);
							printf("\n\tEntrez le login d'utilisateur : ");
            				gets(log);
            				
            				u = chercher_user(log);
							
            				if(u == NULL)
            				{
            					system("color 0c");
								printf("\n\n   ** Cet utilisateur n'existe pas .. Pour ressayer entrez 'r' ou 'R' **\n\n");
            					flag = getch();
							}
							else
							{
								flag = '\0';
								printf("\n\n => Login : %s\n", u->login);
								printf("\n => Password : %s\n", u->password);
								printf("\n => Droit d'accee : ");
								if(u->droit_accee == ADMIN) 
								{
									printf("Admin \n");
									printf("\n  => Voulez vous changer le droit d'accee de '%s' (Admin => Utilisateur)? (O/N) ", u->login);
								}	
									
								else
								{
									printf("Utilisateur \n");
									printf("\n  ** Voulez vous changer le droit d'accee de '%s' (Utilisateur => Admin)? (O/N) ", u->login);
								}					
								droit_acc = getch();
								if(droit_acc == 'o' || droit_acc == 'O')
								{
									modifier_droit_acc(u->login);
									printf("\n\n\t Le droit d'accee de %s est bien modifie\n");
								}
							}
            					
						}while(flag == 'r' || flag == 'R');
						
				
                        printf("\n\n\tFrapper une touche pour retourner au menu principale !");
                        getch();
                        break;

            case 4 : 	system("cls");
            			system("color 0f");
						if(list_user == NULL)
            			{
            				printf("\n\n\t\t\t** Aucun Utilisateur **\n");
            				printf("\n\tFrapper une touche pour retourner au menu principale !");
                        	getch();
            				break;
						}
            			
						flag = '\0';
						
            			do{
            				system("color 0f");
            				system("cls");
							printf("\n  ==> Chercher un utilisateur:\n");
            				printf("      ==========================\n\n");
            				fflush(stdin);
							printf("\n\tEntrez le login d'utilisateur : ");
            				gets(log);
							
							u = chercher_user(log);
							
            				if(u == NULL)
            				{
            					system("color 0c");
								printf("\n   ** Cet utilisateur n'existe pas .. Pour ressayer entrez 'r' ou 'R' **\n\n");
            					flag = getch();
							}
							else
							{
								flag = '\0';
								printf("\n\n => Login : %s\n", u->login);
								printf("\n => Password : %s\n", u->password);
								printf("\n => Droit d'accee : ");
								if(u->droit_accee == ADMIN) 
									printf("Admin \n\n");
								else
									printf("Utilisateur \n\n");
							}
								
						}while(flag == 'r' || flag == 'R');
						
	
                        printf("\n\tFrapper une touche pour retourner au menu principale !");
                        getch();
                        
                      	system("color 0f");

                        break;

            case 5 : 	system("color 0f");
						menu_admin();
                        break;

        	default :	printf("\n\t** Mauvais choix .. Ressayer ! **\n");
						printf("\n\tFrapper une touche pour ressayer !");
        				getch();
        }
	}
}


void menu_admin()
{
	int choix, i, nbr;
	int id_mod , salle, ens_num, surv_num;
	Date _date;
	Duree _duree;
	int *salles_dispo = NULL;
	Enseignant *ens_dispo = NULL;
	surveillance *surv = NULL;
	
    while(1)
    {
    	
    	fflush(stdin);
    	
        system("cls");
        system("color 0B");
        printf("\n\t\t\t\t\t\t\tSession: Admin\n");
        printf("\t\t\t\t\t\t\t______________\n");
        printf("\n\t\t\t    __MENU PRINCIPALE__\n");

        printf("\n\t°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n\t°\t\t\t\t\t\t\t       °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     1) Consulter les surveillances                           °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     2) Ajouter une surveillance                              °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     3) Modifier une surveillance                             °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     4) Supprimer une surveillance                            °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     5) Rechercher                                            °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     6) Gerer les utilisateurs                                °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     7) Deconnecter                                           °\n\t°\t\t\t\t\t\t\t       °");
		printf("\n\t°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n");
        printf("\n\n\t\t==> Votre Choix: ");
        scanf("%d",&choix);

        system("cls");
        switch (choix)
        {
            case 1 : 	system("color 0f");
						printf("\n  ==> Les Surveillances:\n");
            			printf("      ==================\n\n");
						afficher_surveillances(list_surv);

                        printf("\n\tFrapper une touche pour retourner au menu principale !");
                        getch();
                        break;

            case 2 : 	system("color 0f");
						printf("\n=> Ajouter une surveillance :");
            			printf("\n  ** Frapper une touche pour continuer !\n");
                        getch();

                        surv = saisie_surveillance();
						ajouter_surveillance(surv);

						printf("\n => Une nouvelle surveillance est bien ajouter !\n");
                        printf("\n\tFrapper une touche pour retourner au menu princupale !");
                        getch();
                        break;

            case 3 : 	system("color 0f");
						if(!list_surv)
            				printf("\n\t\t** Aucune Surveillance **\n");
            			else
						{
							printf("\n  ==> Modifier une surveillance:\n");
	            			printf("     ===========================\n\n");

	            			afficher_surveillances(list_surv);
	            			nbr = nbr_element(list_surv);

	            			do{
	            				fflush(stdin);
								printf("\n Entrez le numero de surveillance a modifier: ");
	            				scanf("%d",&surv_num);
								if(surv_num < 1 || surv_num > nbr)
									printf("\n\t** Mauvais choix .. Ressayer ! **\n");
							}while(surv_num < 1 || surv_num > nbr);

							surv = saisie_surveillance();
	            			modifier_surv(surv_num, surv);

	            			printf("\n => La surveillance numero %d est bien modifier !\n",surv_num);
	            		}
						printf("\n\tFrapper une touche pour retourner au menu principale !");
                        getch();
                        break;

            case 4 : 	system("color 0f");
						if(!list_surv)
            				printf("\n\t\t** Aucune Surveillance **\n");
            			else
							menu_supprimer();
                        printf("\n\tFrapper une touche pour retourner au menu principale !");
                        getch();
                        break;

            case 5 : 	system("color 0f");
						if(!list_surv)
            				printf("\n\t\t** Aucune Surveillance **\n");
            			else
							menu_recherche();
                        printf("\n\tFrapper une touche pour retourner au menu principale !");
                        getch();
                        break;

			case 6 :	menu_gestion_users();
                        printf("\n\tFrapper une touche pour retourner au menu principale !");
                        getch();
                        break;

            case 7 : 	system("color 0f");
						deconnexion();
						menu_principale();

        	default :	printf("\n\t** Mauvais choix .. Ressayer ! **\n");
						printf("\n\tFrapper une touche pour ressayer !");
        				getch();
        }
    }
}

void menu_user()
{
	int choix, quitter=0, i, nbr;
	int id_mod , salle, ens_num, surv_num;
	Date _date;
	Duree _duree;
	int *salles_dispo = NULL;
	Enseignant *ens_dispo = NULL;
	surveillance *surv = NULL;
	char old_pass[15], new_pass[15], confirm_pass[15];
	
    while(1)
    {
    	fflush(stdin);
    	
        system("cls");
        system("color 0B");
        printf("\n\t\t\t\t\t\t\tSession: %s\n", user->login);
        printf("\t\t\t\t\t\t\t__________________\n");
        printf("\n\t\t\t    __MENU PRINCIPALE__\n");
        
        printf("\n\t°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n\t°\t\t\t\t\t\t\t       °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     1) Consulter les surveillances                           °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     2) Rechercher                                            °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     3) Modifier mon mot de passe                             °\n\t°\t\t\t\t\t\t\t       °");
        printf("\n\t°     4) Deconnecter                                           °\n\t°\t\t\t\t\t\t\t       °");
		printf("\n\t°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\n");
        printf("\n\n\t\t==> Votre Choix: ");
        scanf("%d",&choix);

        system("cls");
        switch (choix)
        {
            case 1 : 	system("color 0f");
						printf("\n  ==> Les Surveillances:\n");
            			printf("      ==================\n\n");
						afficher_surveillances(list_surv);

                        printf("\n\n\tFrapper une touche pour retourner au menu principale !");
                        getch();
                        break;

            case 2 : 	system("color 0f");
						if(!list_surv)
            				printf("\n\t\t** Aucune Surveillance **\n");
            			else
							menu_recherche();
                        printf("\n\n\tFrapper une touche pour retourner au menu principale !");
                        getch();
                        break;

            case 3 : 	system("color 0f");
						printf("\n  ==> Modification du mot de passe:\n");
            			printf("      ===============================\n\n");
            			
            			fflush(stdin);
            			
            			do{
            				fflush(stdin);
            				printf("\n\tEntrez votre mot de passe actuel : ");
            				gets(old_pass);
            				if(strcmp(old_pass, user->password) != 0)
            					printf("\n\t   ** Mot de passe incorrect .. Ressayer ! **\n");
						}while(strcmp(old_pass, user->password) != 0);
            			
            			do{
            				fflush(stdin);
							do{
            					fflush(stdin);
								printf("\n\tEntrez le nouveau mot de passe (Max 8 caract.): ");
	            				gets(new_pass);
	            				if(passwd_plus_8_char(new_pass) == TRUE)
	            					printf("\n\t** Le mot de passe contient plus de 8 caracteres .. Ressayer ! **\n");
							}while(passwd_plus_8_char(new_pass) == TRUE);
	            			
	            			printf("\n\tConfirmer le nouveau mot de passe : ");
	            			gets(confirm_pass);
	            			
	            			if(confirmer_mdp(new_pass, confirm_pass) == FALSE)
	            				printf("\n\t** Les mots de passe ne sont pas les memes .. Ressayer ! **\n");
						}while(confirmer_mdp(new_pass, confirm_pass) == FALSE);
						
						modifier_passwd(user->login, new_pass);
						printf("\n => Votre mot de passe est bien modifie !\n");
            			
						printf("\n\n\tFrapper une touche pour retourner au menu principale !");
                        getch();
                        break;

            case 4 : 	system("color 0f");
						deconnexion();
						menu_principale();
						break;

        	default :	printf("\n\t** Mauvais choix .. Ressayer ! **\n");
						printf("\n\n\tFrapper une touche pour ressayer !");
        				getch();
        }
    }
}

void menu_principale()
{
    int choix, typ_cmpt, connexion = FALSE;
    char login[15], passwd[15], conf_passwd[15],login_ins[15], passwd_ins[15], conf_passwd_ins[15], back = '\0';
    Date date;
    User* _u = (User*)malloc(sizeof(User));

	while(1)
	{
		fflush(stdin);
		
		system("cls");
		system("color 4f");
        printf("\n\n\n\t\t\t__MENU DE PRINCIPALE__\n");
        printf("\t     ============================================");
        printf("\n\t     |\t\t\t\t\t\t|\n\t     |\t\t\t\t\t\t|");
        printf("\n\t     |\t 1) Login\t\t\t\t|");
        printf("\n\t     |\t\t\t\t\t\t|");
        printf("\n\t     |\t 2) Creer un nouveau compte\t\t|");
        printf("\n\t     |\t\t\t\t\t\t|");
        printf("\n\t     |\t 3) Quitter\t\t\t\t|");
		printf("\n\t     |\t\t\t\t\t\t|\n\t     |\n");
		
		fflush(stdin);
        printf("\t     |====> Votre Choix: ");
        scanf("%d",&choix);
		connexion = FALSE;
        switch (choix)
        {
            case 1 : 	do{
            				fflush(stdin);
							system("cls");
	            			system("color 0a");
							printf("\n\n\n\n\t\t#################################################\n");
							printf("\t\t#\t\t\t\t\t\t#\n");
							printf("\t\t#\t\t\t\t\t\t#\n");
							printf("\t\t#\t  1) Admin  \t\t\t\t#\n");
							printf("\t\t#\t\t\t\t\t\t#\n");
							printf("\t\t#\t  2) Utilisateur  \t\t\t#\n");
							printf("\t\t#\t\t\t\t\t\t#\n");
							printf("\t\t#\t\t\t\t\t\t#\n");
							printf("\t\t#################################################\n");
							printf("\n\t\t==> Choisissez le type de votre compte: ");
	   					 	scanf("%d",&typ_cmpt);
							if(typ_cmpt != 1 && typ_cmpt != 2)
							{
								printf("\n\t** Mauvais choix .. Ressayer ! **\n");
								printf("\n\tFrapper une touche pour ressayer !");
        						getch();
        					}
						}while(typ_cmpt != 1 && typ_cmpt != 2);
   					 	
            			system("cls");
            			do{
							system("cls");
            				system("color 0a");
							printf("\n\n\t\t\t  ** Authentification **");
						    printf("\n\t\t\t  ----------------------\n\n\n");
						    fflush(stdin);
						    printf("\t\t  LOGIN : \t");
						    scanf("%s",login);
						    printf("\n\t\t  PASSWORD : \t");
						    scanf("%s",passwd);
							
							connection();
							
							if(typ_cmpt == 1 && est_admin(login,passwd) == TRUE)
	            			{
	            				user = (User*)malloc(sizeof(User));
								user->droit_accee = ADMIN;
								strcpy(user->login ,ADMIN_LOG);
								strcpy(user->password ,ADMIN_PASS);
								connexion = TRUE;
								menu_admin();
							}
							else if(typ_cmpt == 2 && user_connect(login,passwd) != NULL)
							{
								user = (User*)malloc(sizeof(User));
								user = user_connect(login,passwd);
								connexion = TRUE;
								menu_user();
							}
	            			else
	            			{
	            				system("color 0c");
								printf("\n\n    =====================================================================");
								printf("\n\n    ** Login ou password incorrect .. Frapper une touche pour ressayer **");
								printf("\n\n    =====================================================================");
								printf("\n\n\t\t  ( Pour retourner au menu principale (C/c) ) ");
	            				back = getch();
	            				
	            				if(back == 'c' || back == 'C')
	            					menu_principale();
							}
	            				
							
						}while(connexion == FALSE);

                        break;

            case 2 : 	
						
						do{
							system("cls");
							system("color 3e");
            				//system("color 0c");
							printf("\n\n\t\t\t  ** Inscription **");
							printf("\n\t\t\t  -----------------\n\n");
							
							fflush(stdin);
							printf("\n\t\t  LOGIN :\t\t\t");
							scanf("%s",_u->login);
							if(strlen(_u->login) > 10)
							{
								printf("\n\t ** 10 caracteres au plus .. Frapper une touche pour Ressayer ! **\n");
								getch();
							}
						}while(strlen(_u->login) > 10);
						
						
						do{
							do{
            					fflush(stdin);
								printf("\n\t\t  MOT DE PASSE :\t\t");
	            				gets(_u->password);
	            				
	            				if(passwd_plus_8_char(_u->password) == TRUE)
	            					printf("\n\t\t** 10 caracteres au plus .. Ressayer ! **\n");
	            					
							}while(passwd_plus_8_char(_u->password) == TRUE);
	            			
	            			printf("\n\t\t  CONFIRMER LE MOT DE PASSE :\t");
	            			gets(conf_passwd_ins);
	            			
	            			if(confirmer_mdp(_u->password, conf_passwd_ins) == FALSE)
	            				printf("\n\t** Les mots de passe ne sont pas les memes .. Ressayer ! **\n");
	            				
						}while(confirmer_mdp(_u->password, conf_passwd_ins) == FALSE);
						
						_u->droit_accee = UTILISATEUR;
						
						user_session(_u);
						ajouter_user(_u);
						
						system("color 3a");
						printf("\n\n\t=================================================================");
						printf("\n\n\t** Inscription avec succes ! Frapper une touche pour continuer **");
						printf("\n\n\t=================================================================");
						getch();
							
						menu_user();
													
                        break;

			case 3 : 	fin_programme();
						break;

        	default :	printf("\n\t** Mauvais choix .. Ressayer ! **\n");
						printf("\n\tFrapper une touche pour ressayer !");
        				getch();
        }
	}
}





////////////////////////////////////////////////////////////////	MAIN	MAIN	MAIN	MAIN	////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////	MAIN	MAIN	MAIN	MAIN	////////////////////////////////////////////////////////////////////////////////


void sauv_user(char *nom_fichier){
	FILE *fich = fopen(nom_fichier,"w");
	User u;
	int i;
	fflush(stdin);
		u.droit_accee = 1;
		strcpy(u.login, "admin");
		strcpy(u.password, "admin");
		
		fwrite(&u, sizeof(User), 1, fich);

}


main()
{
	
	//sauv_user(F_USER);
	init();
	
	menu_principale();

	
}



