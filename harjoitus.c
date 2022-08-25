#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[31];
    float price;
    int in_stock;
} Product;

void testaa(Product *lista){


    Product *list = malloc(120);

    list[1].in_stock = 2;

    printf("%d", list[0].in_stock);
    printf("%d", list[1].in_stock);
    
}




int main(){
    
    Product eka = {"Appelsiini", 1.2, 2};
    Product toka = {"Banaani", 3.4, 4};
    Product kolmas = {"Citruuna", 7.8, 6};

    Product lista[3];

    lista[0] = eka;
    lista[1] = toka;
    lista[2] = kolmas;

    testaa(lista);


    return 0;
}