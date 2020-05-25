#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#include "constantes.h"


	/*
		Structure Enseignant
	*/
			typedef struct Enseignant
			{
			    int id;
			    char nom[CMAX];
			    char prenom[CMAX];
			    char email[CMAX];
			
			}Enseignant;
		
	/*
		Structure Module
	*/
			typedef struct Module
			{
			    int id;
			    char nom[CMAX];
			    int resp_id;
			    int filliere_id;
			
			}Module;
		
	/*
		Structure Filliere
	*/
			typedef struct Filliere
			{
			    int id;
			    char nom[CMAX];
			    int resp_id;
			}Filliere;
		
	/*
		Structure Salle
	*/
			typedef struct Salle
			{
			    int numero;
			
			}Salle;
		
	/*
		Structure Date
	*/
			typedef struct Date
			{
			    int jours;
			    int mois;
			    int annee;
			
			}Date;
			
	/*
		Structure Temps ... besoin : utiliser dans la structure durée
	*/
			typedef struct Temps
			{
			    int heure;
			    int minute;
			
			}Temps;
		
	/*
		Structure Duree
	*/
			typedef struct Duree
			{
			    Temps depart;
			    Temps fin;
			
			}Duree;
		
	/*
		Structure User
	*/
			typedef struct User
			{
			    char login[10];
			    char password[8];
			    int droit_accee;
			
			}User;
			
	/*
		Structure User
	*/
		typedef struct surveillance
		{
			int id_enseignant;
			int id_module;
			int id_salle;
			Date date;
			Duree duree;
			
		}surveillance;
		
	/*
		Structure liste pour manipuler les listes chainées
	*/
		typedef struct liste
		{
			void* data;
			struct liste* next;
			
		}liste;

#endif // STRUCTURES_H_INCLUDED
