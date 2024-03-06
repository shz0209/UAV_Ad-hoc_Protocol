#include <stdio.h>
#include <stdlib.h>
#include <net/if.h>
    
int main(void)
{
    struct if_nameindex *head, *ifni;
    ifni = if_nameindex();
    head = ifni;
    
    if (head == NULL) {
        perror("if_nameindex()");
        exit(EXIT_FAILURE);
    }   
    
    while (ifni->if_index != 0) {
        printf("Interfece %d : %s\n", ifni->if_index, ifni->if_name);
        ifni++;
    }   
    
    if_freenameindex(head);
    head = NULL;
    ifni = NULL;
    
    exit(EXIT_SUCCESS);
}