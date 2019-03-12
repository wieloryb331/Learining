#include <stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>

void zmien_kolor(char * kolor){
    if(!strcmp(kolor,"czerwony")){
        printf("\033[0;31m");
    }
    else if (!strcmp(kolor,"zielony")){
        printf("\033[0;32m");
    }
    else if (!strcmp(kolor,"zolty")){
        printf("\033[0;33m");
    }
    else if(!strcmp(kolor,"reset")){
        printf("\033[0m");
    }
    else{
        printf("Wpisany kolor nie jest obslugiwany, wybierz inny\n");
    }
}

void wyswietl_login(){
    char login[128];
    getlogin_r(login,128);
    printf("Login aktualnie zalogowanego uzytkownika to: ");printf("\033[1;32m");printf("%s\n",login);printf("\033[0m");
}

void polecenie_cp(char * skad, char * dokad){
    FILE *skad_plik;
    FILE *dokad_plik;
    int buf;
    if((skad_plik = fopen(skad,"r")) && (dokad_plik=fopen(dokad,"w"))){
        while((buf = getc(skad_plik))!=EOF){ 
            putc(buf, dokad_plik);
        } 
        fclose(skad_plik);
        fclose(dokad_plik);
    }
}

void wyswietl_path(){
    char path[1024];
    getcwd(path, 1024);
    printf("[%s]$ ", path);
}

void wyswietl_help(){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~\n");
            printf("Microshell 2k18/2k19\n");
            printf("Autor:");printf("\033[1;32m");printf(" Dominik Szymkowiak\n");printf("\033[0m");
            printf("\033[1;31m");printf("Funkcjonalnosci:\n");printf("\033[0m");
            printf("-wpisz ");printf("\033[0;36m");printf("'exit'");printf("\033[0m");printf(", aby wyjsc z programu\n"); 
            printf("-wpisz ");printf("\033[0;36m");printf("'cd'");printf("\033[0m");printf(" a nastepnie katalog do ktorego chcialbys sie udac, aby zmienic biezacy katalog\n"); 
            printf("-wpisz ");printf("\033[0;36m");printf("'cp plik1 plik2'");printf("\033[0m");printf(", aby skopiowac zawartosc plik1 do plik2 \n"); 
            printf("-wpisz ");printf("\033[0;36m");printf("'wyswietl login'");printf("\033[0m");printf(", aby wyswietlic login aktualnie zalogowanego uzytkownika\n"); 
            printf("-wpisz ");printf("\033[0;36m");printf("'zmien kolor: twoj_kolor'");printf("\033[0m");printf(", aby zmienic kolor(dostepne: czerwony, zielony, zolty)\n"); 
            printf("-wpisz ");printf("\033[0;36m");printf("'history'");printf("\033[0m");printf(" aby zobaczyc wpisane dotychczas komendy\n"); 
            printf("-nacisnij ");printf("\033[0;36m");printf("CTRL+Z");printf("\033[0m");printf(" lub ");printf("\033[0;36m");printf("CTRL+C");printf("\033[0m");printf(" lub zabij proces poleceniem ");printf("\033[0;36m");printf("'kill'");printf("\033[0m");printf(" aby obsluzyc sygnaly\n");
            printf("~~~~~~~~~~~~~~~~~~~~~~\n\n");
}

void czysc_output(char * output[]){
    output[0]=NULL;
    output[1]=NULL;
    output[2]=NULL;
    output[3]=NULL;
    output[4]=NULL;
    output[5]=NULL;
    output[6]=NULL;
    output[7]=NULL;
    output[8]=NULL;
    output[9]=NULL;
    output[10]=NULL;
    output[11]=NULL;
    output[12]=NULL;
    output[13]=NULL;
    output[14]=NULL;
    output[15]=NULL;
}

void foo_sig(int numer_sygnalu){
    printf("\nZLAPANO SYGNAL O NUMERZE %d czyli ",numer_sygnalu);
    if(numer_sygnalu==2){
        printf("SIGINT\n");
    }
    else if(numer_sygnalu==20){
        printf("SIGTSTP\n");
    }
    else{
        printf("SIGTERM\n");
    }
    exit(numer_sygnalu);
}

int wielkosc = 0;
int j;
char * historia[100][15];

int main() {

    printf("\n~~~MICROSHELL~~~\n\n");
    printf("Wpisz ");printf("\033[0;36m");printf("'help'");printf("\033[0m");printf(", aby zobaczyc informacje o autorze programu i oferowanych przez niego funkcjonalnosciach\n"); /*help*/
    
    while(1){

        
        signal(SIGINT,foo_sig);
        signal(SIGTSTP, foo_sig);
        signal(SIGTERM, foo_sig);

        char * input;
        char * do_parsowania;
        char * output[15];
        czysc_output(output);
        int i = 0;
        size_t dl_input = 0; 

        wyswietl_path();
        
        getline(&input, &dl_input, stdin);


        do_parsowania = strtok(input, " \n\"");
        while (do_parsowania!=NULL){
            output[i]=do_parsowania;
            i++;
            do_parsowania = strtok(NULL, " \n\"");
        }


        if(output[0]==NULL){
            continue;
        }


        else{
            for(j=0;;){
                if(output[j]!=NULL){
                    historia[wielkosc][j]=output[j];
                    j++;
                }
                else{
                    break;
                }
            }
            wielkosc++;
        }
        

        if(!strcmp(output[0], "cd")){
            if(output[1] == NULL){
                char login[128];
                getlogin_r(login,128);
                char home[]="/home/";
                strcat(home, login);
                output[1]=home;
            }
            if(chdir(output[1])==0){
                continue;
            }
            else {
                printf("Nie ma takiego katalogu\n");
            }
        }

        else if(!strcmp(output[0],"history")){
            int licz=1;
            int a;
            int b;
            for(a=0;a<wielkosc;a++){
                printf("%d ", licz);
                for(b=0;b<15;b++){
                    if(historia[a][b]!=NULL){
                        printf("%s " ,historia[a][b]);  
                    }
                }
                licz++;
                printf("\n");
            }
        }
        
        else if(!strcmp(output[0], "exit")){
            exit(EXIT_SUCCESS);
        }

        else if(!strcmp(output[0], "help")){
            wyswietl_help();
        }

        else if(!strcmp(output[0],"cp")){
            polecenie_cp(output[1],output[2]);
        }

        else if(!strcmp(output[0],"wyswietl") && !strcmp(output[1],"login")){
            wyswietl_login();
            
        }

        else if(!strcmp(output[0],"zmien") && !strcmp(output[1],"kolor:")){
            zmien_kolor(output[2]);
        }
        else {
            pid_t fpid;
            fpid = fork();
            if(fpid==0){
            execvp(output[0], output);
            perror("execvp");
            printf("Wpisano nieistniejaca komende\n");
            }
            else {
                wait(NULL);
            }
        }
    }
    return 0;
}
