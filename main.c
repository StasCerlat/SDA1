#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100
#define MAX_PRODUCTS 1000

typedef struct {
    char data[MAX_LEN];
    int id_produs;
    char nume_produs[MAX_LEN];
    char categorie_produs[MAX_LEN];
    char subcategorie_produs[MAX_LEN];
    float pret_unitar;
    int cantitate_vanduta;
    char tara[MAX_LEN];
    char oras[MAX_LEN];
} Tranzactie;

typedef struct Node {
    Tranzactie tranzactie;
    struct Node *next;
} Node;
//Optiunea 5
void citeste_date_din_fisier() {
    const char *nume_fisier = "C:\\Users\\Professional\\Documents\\Seimnar_1_SDA\\sales.txt";
    FILE *file = fopen(nume_fisier, "r");
    if (file == NULL) {
        printf("Nu s-a putut deschide fisierul.\n");
        exit(1);
    }

    Tranzactie *tranzactii = NULL;
    int count = 0;

    while (1) {
        Tranzactie tranzactie;
        if (fscanf(file, "%s %d %s %s %s %f %d %s %s",
                   tranzactie.data, &tranzactie.id_produs, tranzactie.nume_produs,
                   tranzactie.categorie_produs, tranzactie.subcategorie_produs,
                   &tranzactie.pret_unitar, &tranzactie.cantitate_vanduta,
                   tranzactie.tara, tranzactie.oras) != 9) {
            break;
        }

        printf("Tranzactie %d: %s %d %s %s %s %.2f %d %s %s\n", count + 1,
               tranzactie.data, tranzactie.id_produs, tranzactie.nume_produs,
               tranzactie.categorie_produs, tranzactie.subcategorie_produs,
               tranzactie.pret_unitar, tranzactie.cantitate_vanduta,
               tranzactie.tara, tranzactie.oras);

        count++;
        tranzactii = (Tranzactie*)realloc(tranzactii, count * sizeof(Tranzactie));
        if (tranzactii == NULL) {
            printf("Eroare la realocarea memoriei.\n");
            exit(1);
        }
        tranzactii[count - 1] = tranzactie;
    }

    printf("Numarul total de tranzactii citite din fisier: %d\n", count);

    free(tranzactii);
    fclose(file);
}

void adauga_tranzactie(Node **head, Tranzactie tranzactie) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Eroare la alocarea memoriei.\n");
        exit(1);
    }
    new_node->tranzactie = tranzactie;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        Node *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void citeste_tranzactii(FILE *file, Node **head) {
    Tranzactie tranzactie;
    while (fscanf(file, "%s %d %s %s %s %f %d %s %s",
                  tranzactie.data, &tranzactie.id_produs, tranzactie.nume_produs,
                  tranzactie.categorie_produs, tranzactie.subcategorie_produs,
                  &tranzactie.pret_unitar, &tranzactie.cantitate_vanduta,
                  tranzactie.tara, tranzactie.oras) == 9) {

        adauga_tranzactie(head, tranzactie);
    }
}
//Optiunea 1
float venit_total_lunar(Node *head, const char *nume_luna, const char *nume_an) {
    float venit_total = 0;
    Node *temp = head;

    while (temp != NULL) {
        char luna[3];
        char anul[5];

        sscanf(temp->tranzactie.data, "%4s-%2s", anul, luna);

        if (strcmp(luna, nume_luna) == 0 && strcmp(anul, nume_an) == 0) {
            venit_total += temp->tranzactie.pret_unitar * (float)temp->tranzactie.cantitate_vanduta;
        }
        temp = temp->next;
    }
    return venit_total;
}

void afiseaza_venit_lunar(Node *head, const char *an) {
    char luni[12][MAX_LEN] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
    for (int i = 0; i < 12; ++i) {
        float venit = venit_total_lunar(head, luni[i], an);
        printf("Venitul total pentru luna %s %s: %.2f\n", luni[i], an, venit);
    }
}
//Optiunea 2
void cele_mai_bine_vandute(Node *head, const char *year) {
    Node *temp = head;
    int num_products_displayed = 0;
    int max_quantity[MAX_PRODUCTS] = {0};

    while (temp != NULL) {
        char year_val[5];

        sscanf(temp->tranzactie.data, "%4s", year_val);
        if (strcmp(year, year_val) == 0) {
            if (temp->tranzactie.cantitate_vanduta > max_quantity[temp->tranzactie.id_produs]) {
                max_quantity[temp->tranzactie.id_produs] = temp->tranzactie.cantitate_vanduta;
            }
        }
        temp = temp->next;
    }

    printf("Top 5 best-selling products in %s are:\n", year);
    for (int i = 0; i < 5; ++i) {
        int max_qty = 0;
        int max_index = -1;
        for (int j = 0; j < MAX_PRODUCTS; ++j) {
            if (max_quantity[j] > max_qty) {
                max_qty = max_quantity[j];
                max_index = j;
            }
        }
        if (max_index != -1) {
            printf("Product ID: %d, Quantity sold: %d\n", max_index, max_qty);
            max_quantity[max_index] = 0;
            num_products_displayed++;
        }
    }
}

//Optiunea 3
void distribuie_vanzari_pe_categorii(Node *head) {
    int vanzari_produs=0;
    char category[30];
    printf("Scrie categoria cautata:");
    scanf("%s",category);
    Node *temp=head;
    while (temp != NULL) {
        if (strcmp(temp->tranzactie.categorie_produs,category ) == 0) {
            vanzari_produs++;
        }
        temp = temp->next;
    }
    if(vanzari_produs==0){
        printf("Categoria nu a fost gasita!");
    }
    printf("\nLa categoria %s sunt: %d\n", category,vanzari_produs);
}
//Optiunea 4
void cele_mai_mari_vanzari_pe_tara(Node *head, const char *tara) {
    float venit_maxim = 0.0;
    char oras_maxim[MAX_LEN] = "";
    Node *temp = head;

    while (temp != NULL) {
        if (strcmp(temp->tranzactie.tara, tara) == 0) {
            float venit_tranzactie = temp->tranzactie.pret_unitar * temp->tranzactie.cantitate_vanduta;
            if (venit_tranzactie > venit_maxim) {
                venit_maxim = venit_tranzactie;
                strcpy(oras_maxim, temp->tranzactie.oras);
            }
        }
        temp = temp->next;
    }

    if (venit_maxim > 0.0) {
        printf("Orasul cu cele mai mari vanzari pentru %s: %s [Venit: %.2f]\n", tara, oras_maxim, venit_maxim);
    } else {
        printf("Nu s-au găsit tranzacții pentru tara %s.\n", tara);
    }
}


void afiseaza_meniu(Node *head) {
    int optiune;
    do {
        printf("\nMeniu:\n");
        printf("1. Venitul total generat in fiecare luna a anului\n");
        printf("2. Primele 5 produse cel mai bine vandute\n");
        printf("3. Cum se distribuie vanzarile pe categoriile de produse\n");
        printf("4. Orasele cu cele mai mari vanzari pentru fiecare tara\n");
        printf("0. Iesire\n");
        printf("Introduceti optiunea: ");
        scanf_s("%d", &optiune);

        switch(optiune) {
            case 1: {
                char an[MAX_LEN];
                printf("Introduceti anul: ");
                scanf("%s", an);
                afiseaza_venit_lunar(head, an);
                break;
            }
            case 2: {
                char an[MAX_LEN];
                printf("Introduceti anul: ");
                scanf("%s", an);
                cele_mai_bine_vandute(head, an);
                break;
            }
            case 3: {
                distribuie_vanzari_pe_categorii(head);
                break;
            }
            case 4: {
                char tara[MAX_LEN];
                printf("Introduceti tara: ");
                scanf("%s", tara);
                cele_mai_mari_vanzari_pe_tara(head, tara);
                break;
            }
            case 5:{
                printf("Citeste datele din fisier:\n\n");
                citeste_date_din_fisier();

                break;
            }
            case 0:
                printf("Iesire din program.\n");
                break;
            default:
                printf("Optiune invalida! Incercati din nou.\n");
        }
    } while (optiune != 0);
}

int main() {
    FILE *file = fopen("C:\\Users\\Professional\\Documents\\Seimnar_1_SDA\\sales.txt", "r");
    if (file == NULL) {
        printf("Nu s-a putut deschide fisierul.\n");
        return 1;
    }

    Node *head = NULL;

    citeste_tranzactii(file, &head);

    fclose(file);

    afiseaza_meniu(head);

    return 0;
}
