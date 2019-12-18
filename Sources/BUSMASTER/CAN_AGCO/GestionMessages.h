#ifndef GESTIONMESSAGES_H
#define GESTIONMESSAGES_H

#ifdef	GESTION_MESSAGES_PRIVATE
	#include "IoParallelCanBoxTypes.h"
	#include "IoParallelCanBoxTypesUser.h"
#endif
#include <afxmt.h>

//Structure utilisée pour créer la liste des messages à gérer.
typedef struct
{
	unsigned long	ident;
	unsigned long	masque;
	unsigned char	remote;

	unsigned char	data[8];
	unsigned char	taille;
	
	CString			chaine_Ident;			//Pour gerer un canalyser par exemple.
	CString			chaine_Time;			
	CString			chaine_Data;			//Pour gerer un canalyser par exemple.
	CString			chaine_Taille;

	SYSTEMTIME		time;
	SYSTEMTIME		time_ans;

	unsigned int	uiCptOverflowMillisec;
	unsigned long	ulTimeInMilliseconds;
	unsigned long	ulOldTimeInMilliseconds;
	unsigned int	uiMilliseconds;

	unsigned int	uiTimeSpan;

	void *fonction;
	void *suivant;
	void *precedent;

}messageStruct;


class CGestionMessages
{
	public:
		CGestionMessages();
		~CGestionMessages();

		int				AjouterMessage(unsigned long ident, unsigned long masque,unsigned char remote, void *fonction, unsigned char *data, unsigned char size);
		int				AjouterMessage(messageStruct *message);
		int				EffacerMessage(unsigned long ident,void *fonction);
		unsigned int	NombreMessage();
		int				EffacerMessages();

		int				RetourneElement(unsigned int index, messageStruct **ptrMessage);
		int				RetourneElement(unsigned long ident, messageStruct **ptrMessage);
	
		//Variables pour gérer un canalyzer (ajout reception ident 0x0)
		bool bModeCanalyzer;
		void * CanalyzerFonction;

	private:
		messageStruct	*dernierElementListe;
		messageStruct	*PtDebut;
		CMutex			partage;
};

#endif



