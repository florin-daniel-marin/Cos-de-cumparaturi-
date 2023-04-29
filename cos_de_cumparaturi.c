#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define NR_PRODUSE 42

typedef struct list_nod{
	int id_produs;
	int cant_produs;
	float pret_produs;
	char nume[50];
	struct list_nod *urm;
} list_nod;

//FUNCTII STOCK MAGAZIN:-------------------------------------------------------------
list_nod* Create_Stock (int nr, char nume_brand[NR_PRODUSE][20], char catalog[NR_PRODUSE][30]){
	int i, j;
	char mod;
	list_nod *head = NULL, *p, *q;

	printf ("\nCreeare stock magazin:\n");
	printf ("Creeati stock-ul manual sau automat(random): press m (manual)/ a(automat): ");
	getchar();
	scanf("%c", &mod);

	if (mod == 'a' || mod == 'm')
		for (i = 1; i < nr + 1 ; i++){
			q = (list_nod*) malloc (sizeof(list_nod));

			if (mod == 'a'){ 
				//automat:
				strcpy(q->nume, catalog[rand() % NR_PRODUSE]);
				strcat(q->nume," ");
				strcat(q->nume, nume_brand[rand() % NR_PRODUSE]);
				q->id_produs = i;
				q->cant_produs = rand() % 100 + 1;
				q->pret_produs = ((float)rand()/RAND_MAX)*(float)(20.0);
			}else if (mod == 'm'){
				//manual:
				for(j = 0; j < NR_PRODUSE; j++)
					printf("id - %d | %s\n", j, catalog[j]);
				printf ("\nAlegeti produsul %d: ",i);

				//initializare id, cantitate, pret si nume produs
				scanf("%d", &j);
				//verificare produs:
				if (j >= NR_PRODUSE){
					printf("\nComanda necunoscuta!\n");
					break;
				}
				strcpy(q->nume, catalog[j]);
				q->id_produs = i;
				printf ("Introduceti cantitatea pentru produsul %d: ",i);
				scanf("%d", &q->cant_produs);
				printf ("Introduceti pretul produsului %d: ",i);
				scanf("%f", &q->pret_produs);
			}
			q->urm = NULL;
			if (head == NULL){
				head = q;
			}
			else{
				p = head;
				while(p->urm != NULL)
					p = p->urm;
				p->urm = q;
			}
		}
	else printf("\nComanda necunoscuta!\n");
	printf("\n");
	return head;	
}

void Display_List(list_nod *headstock){
	list_nod *p = headstock;
	printf ("Cantitate: | Produs: | Pret: | Tasta:\n");
	printf("────────────────────────────────────────────────────────────────────────────────────\n");
	while (p != NULL){
		printf ("%d | %s | %.2f | tasta - %d\n", p->cant_produs, p->nume, p->pret_produs, p->id_produs);
		p = p->urm;
	}
}

void Modify_List(list_nod *headstock){
	
}

//FUNCTII COS CLIENT:-----------------------------------------------
void Adauga_Cos (list_nod *headstock, list_nod **headcos){
	//Declarare si initializare:
	list_nod *p_stock, *r, *p_cos;
	int i, cant, tasta, ok = 0;
	char oper;
	p_stock = headstock;

	
	//........................
	printf ("\nCe produs cumparati:\n");
	Display_List (headstock);
	printf ("Introduceti tasta produsului: ");
	scanf ("%d", &tasta);

	while (p_stock != NULL){
		//caut produsul in stock
		if (tasta == p_stock->id_produs){
			// Initializare Cos
			p_cos = (list_nod*) malloc (sizeof(list_nod));
			p_cos->id_produs = p_stock->id_produs;
			p_cos->pret_produs = p_stock->pret_produs;
			strcpy(p_cos->nume, p_stock->nume);
			p_cos->urm = NULL;
			
			//cantitate:
			printf ("Ce cantitate doriti sa adaugati?\n");
			printf ("--Limita stoc: %d--\n", p_stock->cant_produs);
			scanf ("%d", &cant);
			if (cant <= p_stock->cant_produs){
				p_cos->cant_produs = cant;
			    p_stock->cant_produs = p_stock->cant_produs - cant;
			}else{
				printf("\nCantitatea introdusa depaseste cantitatea din stock!\n");
				free (p_cos);
				return;
			}

			// Verificare cos
			//............................
			//cos este gol:
			if (*headcos == NULL){
				*headcos = p_cos;
				return;
			}
			else{
				//cosul nu este gol:	
				r = *headcos;
				//itineram pana la ultimul prod din lista
				while (r != NULL){
					//produs apartine cos:
					if (p_stock->id_produs == r->id_produs){
						r->cant_produs = r->cant_produs + cant;
						return;
					}
				r = r->urm;
				}

				//produs nu apartine cos:
				r = *headcos;
				while (r->urm != NULL)
					r = r->urm;
				r->urm = p_cos;
				return;
			}
		}
		p_stock = p_stock->urm;
	}
	printf("\nTasta necunoscuta!\n");
}

void Eliminare_Cos (list_nod *headstock, list_nod **headcos){
	list_nod *r, *p_stock;
	list_nod *p_cos = *headcos;
	int tasta, i, cant;

	//........................
	printf ("\nCe produs eliminati din cos:\n");
	printf ("Introduceti tasta produsului: ");
	scanf ("%d", &tasta);

	if (tasta == (*headcos)->id_produs){
		r = *headcos;
		cant = p_cos->urm->cant_produs;
		*headcos = r->urm;
		free(r);
		return;
	}
	
	while (p_cos->urm != NULL){
		if (tasta == p_cos->urm->id_produs){
			r = p_cos->urm->urm;
			cant = p_cos->urm->cant_produs;
			free(p_cos->urm);
			p_cos->urm = r;
			return;
		}
	p_cos = p_cos->urm;
	}

	p_stock = headstock;
	while (p_stock != NULL){
		if (tasta == p_stock->id_produs){
			p_stock->cant_produs = p_stock->cant_produs + cant;
		}
		p_stock = p_stock->urm;
	}
	printf("\nTasta necunoscuta!\n");
}

void Modificare_Cos (list_nod *headstock, list_nod **headcos){
	list_nod *r, *p_stock;
	list_nod *p_cos = *headcos;
	int tasta, cant, i;
	p_stock = headstock;

	//........................
	printf ("\nCe produs modificati din cos:\n");
	printf ("Introduceti tasta produsului: ");
	scanf ("%d", &tasta);
	
	//cautam produsul ce trebuie modificat
	while (p_cos != NULL){
		if (tasta == p_cos->id_produs){
			printf ("%s | cant: %d | pret: %f | tasta - %d\n", p_cos->nume, p_cos->cant_produs, (p_cos->pret_produs)*(p_cos->cant_produs), p_cos->id_produs);
			goto cant;
		}
	p_cos = p_cos->urm;
	}
	printf("\nTasta necunoscuta!\n");
	return;

	cant:
	//........................
	printf ("\nModificati cantitatea: ");
	scanf ("%d", &cant);

	
	while (p_stock != NULL){
		if (tasta == p_stock->id_produs){
			if (cant <= p_stock->cant_produs + p_cos->cant_produs){
				p_stock->cant_produs = p_stock->cant_produs + p_cos->cant_produs - cant;
				p_cos->cant_produs = cant;
			}else{
				printf("\nCantitatea introdusa depaseste cantitatea din stock!\n");
			}
		}
		p_stock = p_stock->urm;
	}
	if (!cant){
		if (tasta == (*headcos)->id_produs){
			r = *headcos;
			*headcos = r->urm;
			free(r);
			return;
		}
		p_cos = *headcos;

		while (p_cos->urm != NULL){
			if (tasta == p_cos->urm->id_produs){
				r = p_cos->urm->urm;
				free(p_cos->urm);
				p_cos->urm = r;
				return;
			}
		p_cos = p_cos->urm;
		}
	}
}

void Vizualizare_Cos(list_nod *headcos){
	list_nod *p = headcos;
	printf("\nCosul dumneavoastra:\n");
	while (p != NULL){
		printf ("%s | cant: %d | pret: %f | tasta - %d\n", p->nume, p->cant_produs, (p->pret_produs)*(p->cant_produs), p->id_produs);
		p = p->urm;
	}
	printf("────────────────────────────────────────────────────────────────────────────────────\n");
}

int main ()
{
	//init valori
	char nume_brand[NR_PRODUSE][20]={"Openlane","Yearin","Goodsilron","Condax","Opentech","Golddex","year-job","Isdom",
	"Gogozoom","Y-corporation","Nam-zim","Donquadtech","Warephase","Donware","Faxquote","Sunnamplex","Lexiqvolax","Sumace","Treequote",
	"Iselectrics","Zencorporation","Plusstrip","dambase","Toughzap","Codehow","Zotware","Statholdings","Conecom","Zathunicon","Labdrill",
	"Ron-tech","Green-Plus","Groovestreet","Zoomit","Bioplex","Zumgoity","Scotfind","Dalttechnology","Kinnamplus","Konex","Stanredtax","Cancity"
	};
	char catalog[NR_PRODUSE][30]={"Portocale 1kg", "Mere 1kg", "Banane 1kg", "Pere 1kg", "Capsuni 1kg", "Struguri 1kg", 
			"Cirese 1kg", "Piersici 1kg", "Nectarine 1kg", "Mandarine 1kg", "Ulei 1l", "Faina 1kg", "Malai 1kg", "Orez 1kg", 
			"Spaghette 500g", "Paste fainoase 500g", "Paine alba 300g", "Paine neagra 300g", "Franzela 100g", "Chifle 45g",
			"Conserva ton 200g", "Suc de tomate 500g", "Pasta de tomate 400g", "Borcan mazare", "Borcan ciuperci", 
			"Borcan castraveti 500g", "Borcan fasole 500g", "Pate de porc 200g", "Pate de pui 200g", "Pate de post 200g", 
			"Masline 250g", "Salam feliat 100g", "Sunca presata 100g", "Burger 1buc", "Pizza 1buc", "Lasagna 1buc",
			"Bere doza 500ml", "Bere sticla 330ml", "Vin alb 75cl", "Vin roze 75cl", "Vin rosu 75cl", "Palinca 50cl"
	};
	int i, nr;
	char operatie;
	srand ( time(NULL) );

	printf("Creeare stock magazin: Cate produse sunt in stock? ");
	if(!scanf("%d",&nr)){
		printf("\nComanda necunoscuta!\n");
		return 1;
	}

	list_nod *headcos; //linked list cos client
	list_nod *headstock; //linked list stock magazin
	headstock = (list_nod*) malloc (sizeof(list_nod));
	headstock = Create_Stock(nr, nume_brand, catalog);
	headcos = (list_nod*) malloc (sizeof(list_nod));
	headcos = NULL;

	printf("\nCosul virtual:\n ");
	while (1){
		printf("Apasati tasta: a (Adauga produs), e (Elimina produs), m(Modifica produs), f(Finalizare plata), r(Renunta)\n ");
		getchar();
		scanf("%c",&operatie);

		switch(operatie){
			case 'a':
				Adauga_Cos (headstock, &headcos);
				Vizualizare_Cos(headcos);
				break;
			case 'e':
				Eliminare_Cos(headstock, &headcos);
				Vizualizare_Cos(headcos);
				break;
			case 'm':
				Modificare_Cos(headstock, &headcos);
				Vizualizare_Cos(headcos);
				break;
			case 'f':
				//functie cost total
				free (headcos);
				free (headstock);
				return 0;
			case 'r':
				printf("\nAti renuntat la cosul de cumparaturi!\n");
				free (headcos);
				free (headstock);
				return 0;
			default:
				printf("\nIntroduceti o comanda valabila!\n");
		}
	}
	return 0;
}
