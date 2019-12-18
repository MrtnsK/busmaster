// GestionMessage.cpp: implementation of the CGestionMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GestionMessages.h"
#include "TypeDefinition.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Pointeur de début de liste chainée
//messageStruct *PtDebut;


// Déclaration d'une variable d'Exclusion mutuelle
CMutex Exclusion;


CGestionMessages::CGestionMessages()
{	
	Exclusion.Lock();
	PtDebut = NULL;
	Exclusion.Unlock();	
}


// Désallocation de tous les messages lors de l'appel du destructeur
CGestionMessages::~CGestionMessages()
{	
	messageStruct *Pt;
	Exclusion.Lock();

	if (PtDebut != NULL)
	{	
		Pt=PtDebut;
	
		while (Pt!=NULL)
		{
			PtDebut=(messageStruct*)Pt->suivant;
			delete(Pt);
			Pt=PtDebut;
	
		}

		delete(PtDebut);
		PtDebut = NULL;
	}
	
	Exclusion.Unlock();
}


// retourne le nb d'éléments de la liste
unsigned int CGestionMessages::NombreMessage()
{
	messageStruct *Pt;
	int NbMessage=0;
	
	Exclusion.Lock();

	Pt=PtDebut;

	while(Pt!=NULL)
	{
		NbMessage++;
		Pt=(messageStruct*)Pt->suivant;
	}	

	Exclusion.Unlock();


	return NbMessage;
}


// Efface tous les messages de la liste
int CGestionMessages::EffacerMessages()
{
	messageStruct *Pt;
	Exclusion.Lock();
	
	if (PtDebut != NULL)
	{
		while(PtDebut != NULL)
		{
			Pt = (messageStruct*) PtDebut->suivant;
			PtDebut = Pt;
		
		};
	}
	
	Exclusion.Unlock();

	return OK;
}




// Désallocation d'un message en particulier en le localisant par son identifiant
int CGestionMessages::EffacerMessage(unsigned long Ident,void *fonction)
{
	messageStruct *Pt1,*Pt2;

	Exclusion.Lock();

	if (PtDebut != NULL)
	{
		Pt1=PtDebut;
		Pt2= (messageStruct*) PtDebut->suivant;

		// Si c'est le 1er élément
		if (Pt1->ident == Ident)
		{	
			PtDebut = Pt2;
			delete Pt1;
		}
		else
		{
			while( (Pt2->ident!=Ident) && (Pt2->suivant!=NULL))
			{
				Pt2= (messageStruct*) Pt2->suivant;
				Pt1= (messageStruct*) Pt1->suivant;
			};

			if ( (Pt2 == NULL) && ((Pt1->ident) != Ident) )
			{
				#if NIV_AFFICH_ERROR == 2
					AfxMessageBox("Le message a supprimer n'a pas été trouvé",MB_ICONERROR);
				#endif
		
				Exclusion.Unlock();
				return ERROR_MEMORY;
			}
	
			Pt1->suivant = Pt2->suivant;
			delete Pt2;
		}	

		Exclusion.Unlock();

		return OK;
	}
	else
	{
		Exclusion.Unlock();
		return NOT_OK;
	}

}

// Ajout d'un message dans la liste
int CGestionMessages::AjouterMessage(messageStruct *message)
{
	messageStruct *NouvelElement,*Pt1,*Pt2;

	Exclusion.Lock();

	if (PtDebut == NULL)	// Insertion du 1er message
	{
		// Allocation d'un nouveau message
		NouvelElement = (messageStruct*) new messageStruct;
		if (NouvelElement == NULL)
		{	
			// Impossible d'allouer un nouveau message		
			#if NIV_AFFICH_ERROR == 2
				AfxMessageBox("Impossible d'allouer un nouveau message",MB_ICONERROR);
			#endif		
			Exclusion.Unlock();
			return ERROR_MEMORY;
		}
 
		NouvelElement->ident = message->ident;
		NouvelElement->masque = message->masque;
		NouvelElement->remote = message->remote;
		if (NouvelElement->remote == 1)
			NouvelElement->taille = 0;
		else
			NouvelElement->taille = message->taille;

		for (int i=0;i<message->taille;i++)
			NouvelElement->data[i] = message->data[i];

		NouvelElement->fonction = message->fonction;
		if (bModeCanalyzer)
		{
			CanalyzerFonction = message->fonction;
		}

		NouvelElement->suivant=NULL;
		PtDebut=NouvelElement;
		
		Exclusion.Unlock();
		
		return OK;		
	}
	else	// Insertion des autres messages de façon croissante
	{
		Pt1=PtDebut;
		Pt2= (messageStruct*) PtDebut->suivant;

		while( (Pt2!=NULL) && (Pt2->ident<message->ident)) 
		{	
			Pt2= (messageStruct*) Pt2->suivant;
			Pt1= (messageStruct*) Pt1->suivant;
		};
				
		if (Pt1->ident==message->ident) // Le message existe déja --> Mise à jour des données
		{
			
			Pt1->fonction = message->fonction;
			Pt1->masque = message->masque;
			Pt1->remote = message->remote;
			if(Pt1->remote == 1)
				Pt1->taille = 0;
			else
				Pt1->taille = message->taille;
			
			for (int i=0;i<Pt1->taille;i++)
				Pt1->data[i] = message->data[i];
		
		}
		else if ( (Pt1==PtDebut) && (Pt1->ident>message->ident))	// Ajout en début de liste
		{
			NouvelElement = (messageStruct*) new messageStruct;
			if (NouvelElement == NULL)
			{
				// Impossible d'allouer un nouveau message			
				#if NIV_AFFICH_ERROR == 2
					AfxMessageBox("Impossible d'allouer un nouveau message",MB_ICONERROR);
				#endif
				Exclusion.Unlock();
				return ERROR_MEMORY;
			}

			NouvelElement->ident = message->ident;
			NouvelElement->remote = message->remote;
			NouvelElement->fonction = message->fonction;
			NouvelElement->masque = message->masque;
			if (NouvelElement->remote == 1)
				NouvelElement->taille = 0;
			else
				NouvelElement->taille = message->taille;
			
			for (int i=0;i<NouvelElement->taille;i++)
				NouvelElement->data[i] = message->data[i];

			NouvelElement->suivant=PtDebut;
			PtDebut=NouvelElement;

			Exclusion.Unlock();

			return OK; 
		}
		else // Insertion du message dans la liste
		{
			NouvelElement = (messageStruct*) new messageStruct;
			if (NouvelElement == NULL)
			{
				// Impossible d'allouer un nouveau message
				#if NIV_AFFICH_ERROR == 2
					AfxMessageBox("Impossible d'allouer un nouveau message",MB_ICONERROR);
				#endif
				Exclusion.Unlock();
				return ERROR_MEMORY;
			}

			NouvelElement->ident = message->ident;
			NouvelElement->remote = message->remote;
			if (NouvelElement->remote == 1)
				NouvelElement->taille = 0;
			else
				NouvelElement->taille = message->taille;
			NouvelElement->fonction = message->fonction;
			NouvelElement->masque = message->masque;
			
			for (int i=0;i<NouvelElement->taille;i++)
				NouvelElement->data[i] = message->data[i];

			NouvelElement->suivant=Pt2;
			Pt1->suivant=NouvelElement;

			Exclusion.Unlock();
			
			return OK;
		}
	}

	Exclusion.Unlock();

	return OK;
}


// Ajout d'un message dans la liste chainée
int CGestionMessages::AjouterMessage(unsigned long Ident, unsigned long masque, unsigned char remote, void *fonction, unsigned char *data, unsigned char Dlc)
{
	messageStruct *msg;

	msg = (messageStruct*) new messageStruct;
	msg->ident = Ident;
	msg->remote = remote;
	msg->taille = Dlc;
	msg->fonction = fonction;
	msg->masque = masque;
	
	for (int i=0;i<msg->taille;i++)
		msg->data[i] = data[i];

	msg->suivant=NULL;

	return AjouterMessage(msg);
}

// Permet de retourner un message par adresse en le localisant par son identifiant
int CGestionMessages::RetourneElement(unsigned long Ident,messageStruct **PtMessage)
{
	messageStruct *Pt;

	Exclusion.Lock();
	
	messageStruct	*PtDeb = PtDebut;
	if (PtDeb != NULL )
	{		
		Pt=PtDeb;

		while( (Pt->ident!=Ident) && (Pt->suivant!=NULL) )
			Pt= (messageStruct*) Pt->suivant;

		if (Pt->ident==Ident)
		{
			*PtMessage=Pt;
			Exclusion.Unlock();
			return OK;
		}
		else
		{
			Exclusion.Unlock();
			return NOT_OK;
		}
	}
	else
	{	
		Exclusion.Unlock();
		return NOT_OK;
	}
}

// Permet de retourner un message par adresse en le localisant par sa position dans la liste
int CGestionMessages::RetourneElement(unsigned int Desc,messageStruct **PtMessage)
{
	messageStruct *Pt;
	
	Exclusion.Lock();

	if (PtDebut != NULL)
	{
		Pt = PtDebut;
		for(int j=0;j<(int)Desc;j++)
		{	
			if (Pt!=NULL)
				Pt = (messageStruct*) Pt->suivant;
			else
				return NOT_OK;
		}
			
		*PtMessage = Pt;
		
		Exclusion.Unlock();
		
		return OK;
	}
	else
	{
		Exclusion.Unlock();
		return NOT_OK;
	}
}