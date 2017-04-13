#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>
#include <string.h>

#define ILOSC_ELEMENTOW 10

#define SC_W 800
#define SC_H 600
#define P_W 250
#define P_H 200

struct element{
        int x;
        int y;
        int rx;
        int ry;
        char nazwa[25];
};

typedef struct element pole;

pole interfejs[ILOSC_ELEMENTOW];
BITMAP *buffor;
BITMAP *button;
BITMAP *button2;
BITMAP *button3;
BITMAP *button4;
BITMAP *button5;
BITMAP *button6;
BITMAP *button7;
BITMAP *button8;
BITMAP *bckg;
BITMAP *orginalBITMAP;
BITMAP *changedBITMAP;
int screen_pixels_W;
int screen_pixels_H;


double macierz[3][3];

unsigned char **we;
unsigned char **wy;


int p_x,p_y,poczatek_prostokata_x,poczatek_prostokata_y;
int k_x,k_y,koniec_prostokata_x, koniec_prostokata_y;

void allocate_tabs(int kolumny, int wiersze){

    int i=0,j=0;
    we= (unsigned char **)malloc(sizeof(unsigned char *) * kolumny);
    for(i=0; i< kolumny;i++)
        {
            we[i] = (unsigned char *)malloc (sizeof(unsigned char) * wiersze);
        }
    wy= (unsigned char **)malloc(sizeof(unsigned char *) * kolumny);
    for(i=0; i< kolumny;i++)
        {
            wy[i] = (unsigned char *)malloc (sizeof(unsigned char) * wiersze);
        }

    for(i=0; i < kolumny; i++)
        for(j=0; j < wiersze; j++) {
            we[i][j] = 0;
            wy[i][j] = 0;
        }
}

struct Node  {
	BITMAP *pastBitmap;
	double m[3][3];
	struct Node* next;
	struct Node* prev;
};

struct Node* head;
struct Node* current;


struct Node* utworzNowyWezel(BITMAP *bitmap) {
	struct Node* newNode
		= (struct Node*)malloc(sizeof(struct Node));
	newNode->pastBitmap = bitmap;
	memcpy(newNode->m, macierz, sizeof(macierz));
	newNode->prev = NULL;
	newNode->next = NULL;
	printf("nowy");
	return newNode;
}


void dodajDoKolejki(BITMAP *bitmap) {
	struct Node* newNode = utworzNowyWezel(bitmap);
	if(head == NULL) {
		head = newNode;
		current = head;
		return;
	}
	head->next = newNode;
	newNode->prev = head;
	head = newNode;
	current = head;
}

void goBack() {
	printf("BACK: ");
	if((current !=NULL) && (current->prev != NULL)) {
		current = current->prev;
        memcpy(macierz, current-> m, sizeof(macierz));
		//blit(current->pastBitmap, screen, 0, 0, 0, 0, SC_W, SC_H);
		blit(current->pastBitmap,buffor, 0,0,400,350,screen_pixels_W,screen_pixels_H);
	}
}

void goFront() {
	printf("FRONT: ");
	if((current !=NULL) && (current->next != NULL)) {
		current = current->next;
        memcpy(macierz, current-> m, sizeof(macierz));
		blit(current->pastBitmap,buffor, 0,0,400,350,screen_pixels_W,screen_pixels_H);
	}
}

void bitmapToArray(BITMAP *bitmap){

    int i,j;
    for (i=0; i<screen_pixels_W; i++) {
        for (j=0 ; j<screen_pixels_H; j++) {
            we[i][j] = (unsigned char) getpixel(bitmap,i,j);
        }
    }
}

void arrayToBitmap(BITMAP *bitmap) {
    int i,j;

    for (i=0; i<screen_pixels_W; i++) {
        for (j=0 ; j<screen_pixels_H; j++) {
            putpixel(bitmap,i,j,wy[i][j]);
        }
    }
}


void filtr_splotowy_licz(){

int screen_pixels_H_tmp;
int screen_pixels_W_tmp;

double sum, suma_liczb_w_macierzy=0;
int wynik,x,y,i,j;

for(x=0;x<3;x++)
    for(y=0;y<3;y++)
        suma_liczb_w_macierzy=suma_liczb_w_macierzy+macierz[x][y];

if(p_x > k_x){
    int tmp;
    tmp=p_x;
    p_x=k_x;
    k_x=tmp;
}

if(p_y> k_y){
    int tmp;
    tmp=p_y;
    p_y=k_y;
    k_y=tmp;
}

int tmp_x_first,tmp_x_second,tmp_y_first,tmp_y_second;

if(p_x>400 && p_x<(390+screen_pixels_W+10)){
    tmp_x_first = p_x-400;
    tmp_x_second = k_x-400;
    tmp_y_first = p_y-50;
    tmp_y_second=k_y-50;
}else{
    tmp_x_first = 0;
    tmp_x_second = screen_pixels_W;
    tmp_y_first = 0;
    tmp_y_second=screen_pixels_H;
}
for(x=tmp_x_first;x <tmp_x_second;x++){
        for(y=tmp_y_first;y <tmp_y_second;y++) {
                 sum = 0;
                     for(i=0;i < 3;i++)
                        for(j=0;j < 3;j++){
                           if(x==0 && y==0){
                                sum +=we[x+i][y+j]* macierz[i][j];
                                continue;
                            }
                            if(x==0 && y==(screen_pixels_H-1)){
                                sum +=we[x+i][y-j]*  macierz[i][j];
                                continue;
                            }
                            if(y==(screen_pixels_H-1) && x==(screen_pixels_W-1) ){
                                sum +=we[x-i][y-j]*  macierz[i][j];
                                continue;
                            }
                            if(x==(screen_pixels_W-1) && y==0){
                                sum +=we[x-i][y+j]* macierz[i][j];
                                continue;
                            }

                            if((x==0)&& (i==0)){
                                sum +=we[x][y-1+j]* macierz[i][j];
                                continue;
                               }
                            if ((y==0) && (j==0)){
                                sum +=we[x-1+i][y]* macierz[i][j];
                                continue;
                                }
                            if ((x==(screen_pixels_W-1)) && (i==2) ){
                                sum +=we[x][y-1+j]* macierz[i][j];
                                continue;
                                }
                            if ((y==(screen_pixels_H-1)) && (j==2) ) {
                                sum +=we[x-1+i][y]* macierz[i][j];
                                continue;
                                }
                                 sum += we[x-1+i][y-1+j] * macierz[i][j];
                        }
                wynik = (int)(sum/suma_liczb_w_macierzy);
                if(wynik > 255) wynik = 255;
                if(wynik < 0) wynik = 0;
                wy[x][y] = (unsigned char)wynik;
        }
}
    BITMAP* bitmapToQueue = create_bitmap_ex(8,screen_pixels_W,screen_pixels_H);
    arrayToBitmap(bitmapToQueue);
    dodajDoKolejki(bitmapToQueue);
}


void rozmycie_jednorodne(){

    double tmp_macierz[3][3] ={
                    {1,1,1},
                    {1,1,1},
                    {1,1,1}
            };
     memcpy (macierz, tmp_macierz, sizeof(macierz));
     filtr_splotowy_licz(we,wy);
     arrayToBitmap(changedBITMAP);
     blit(changedBITMAP,buffor, 0,0,400,350,screen_pixels_W,screen_pixels_H);
}

void rozmycie_Gaussa(){

     double tmp_macierz[3][3] ={
                    {0,1,0},
                    {1,4,1},
                    {0,1,0}
            };
    memcpy (macierz, tmp_macierz, sizeof(macierz));
    filtr_splotowy_licz(we,wy);
    arrayToBitmap(changedBITMAP);
    blit(changedBITMAP,buffor, 0,0,400,350,screen_pixels_W,screen_pixels_H);
}

void wyostrzanie(){

     double tmp_macierz[3][3] ={
                    {0,-1,0},
                    {-1,5,-1},
                    {0,-1,0}
            };
    memcpy (macierz, tmp_macierz, sizeof(macierz));
    filtr_splotowy_licz(we,wy);
    arrayToBitmap(changedBITMAP);
    blit(changedBITMAP,buffor, 0,0,400,350,screen_pixels_W,screen_pixels_H);
}

void wlasnaMacierz(){

    filtr_splotowy_licz(we,wy);
    arrayToBitmap(changedBITMAP);
    blit(changedBITMAP,buffor, 0,0,400,350,screen_pixels_W,screen_pixels_H);
}

void rysujMacierz(int tmp,int kolor){

    rectfill(buffor, 40, 390, 170 , 520, makecol(58,52,49));
    int x=50,y=400,r=30,c=2,r2=15;
    int kolorCzerwony = makecol(255,0,0);
    int kolorBialy=makecol(255,255,255);
    int kolorInny;
    if(kolor!=-1)
        kolorInny=makecol(255, 165, 0);
    else
        kolorInny = kolorCzerwony;
    if(tmp==1) {
        rect(buffor, x, y, x+r, y+r, kolorCzerwony);
        textprintf_ex(buffor,font,x+r2-c,y+r2-c,kolorInny,-1,"%d",(int)macierz[0][0]);
    }else {
        rect(buffor, x, y, x+r, y+r, kolorBialy);
        textprintf_ex(buffor,font,x+r2-c,y+r2-c,kolorBialy,-1,"%d",(int)macierz[0][0]);
    }
    if(tmp==2){
        rect(buffor, x+40, y, x+r+40, y+r, kolorCzerwony);
        textprintf_ex(buffor,font,x+r2-c+40,y+r2-c,kolorInny,-1,"%d",(int)macierz[0][1]);
    }else{
        rect(buffor, x+40, y, x+r+40, y+r, kolorBialy);
        textprintf_ex(buffor,font,x+r2-c+40,y+r2-c,kolorBialy,-1,"%d",(int)macierz[0][1]);
    }
    if(tmp==3){
        rect(buffor, x+80, y, x+r+80, y+r, kolorCzerwony);
        textprintf_ex(buffor,font,x+r2-c+80,y+r2-c,kolorInny,-1,"%d",(int)macierz[0][2]);
    }else{
        rect(buffor, x+80, y, x+r+80, y+r, kolorBialy);
        textprintf_ex(buffor,font,x+r2-c+80,y+r2-c,kolorBialy,-1,"%d",(int)macierz[0][2]);
    }
    if(tmp==4){
        rect(buffor, x, y+40, x+r, y+r+40, kolorCzerwony);
        textprintf_ex(buffor,font,x+r2-c,y+r2-c+40,kolorInny,-1,"%d",(int)macierz[1][0]);
    }else{
        rect(buffor, x, y+40, x+r, y+r+40, kolorBialy);
        textprintf_ex(buffor,font,x+r2-c,y+r2-c+40,kolorBialy,-1,"%d",(int)macierz[1][0]);
    }
    if(tmp==5){
        rect(buffor, x+40, y+40, x+r+40, y+r+40, kolorCzerwony);
         textprintf_ex(buffor,font,x+r2-c+40,y+r2-c+40,kolorInny,-1,"%d",(int)macierz[1][1]);
    }else{
        rect(buffor, x+40, y+40, x+r+40, y+r+40, kolorBialy);
        textprintf_ex(buffor,font,x+r2-c+40,y+r2-c+40,kolorBialy,-1,"%d",(int)macierz[1][1]);
    }
    if(tmp==6){
        rect(buffor, x+80, y+40, x+r+80, y+r+40, kolorCzerwony);
        textprintf_ex(buffor,font,x+r2-c+80,y+r2-c+40,kolorInny,-1,"%d",(int)macierz[1][2]);
    }else{
       rect(buffor, x+80, y+40, x+r+80, y+r+40, kolorBialy);
       textprintf_ex(buffor,font,x+r2-c+80,y+r2-c+40,kolorBialy,-1,"%d",(int)macierz[1][2]);
    }
    if(tmp==7){
        rect(buffor, x, y+80, x+r, y+r+80, kolorCzerwony);
        textprintf_ex(buffor,font,x+r2-c,y+r2-c+80,kolorInny,-1,"%d",(int)macierz[2][0]);
    }else{
       rect(buffor,  x, y+80, x+r, y+r+80, kolorBialy);
       textprintf_ex(buffor,font,x+r2-c,y+r2-c+80,kolorBialy,-1,"%d",(int)macierz[2][0]);
    }
    if(tmp==8){
        rect(buffor, x+40, y+80, x+r+40, y+r+80, kolorCzerwony);
        textprintf_ex(buffor,font,x+r2-c+40,y+r2-c+80,kolorInny,-1,"%d",(int)macierz[2][1]);
    }else{
        rect(buffor, x+40, y+80, x+r+40, y+r+80, kolorBialy);
        textprintf_ex(buffor,font,x+r2-c+40,y+r2-c+80,kolorBialy,-1,"%d",(int)macierz[2][1]);
    }
    if(tmp==9){
        rect(buffor, x+80, y+80, x+r+80, y+r+80, kolorCzerwony);
        textprintf_ex(buffor,font,x+r2-c+80,y+r2-c+80,kolorInny,-1,"%d",(int)macierz[2][2]);
    }else{
       rect(buffor, x+80, y+80, x+r+80, y+r+80, kolorBialy);
       textprintf_ex(buffor,font,x+r2-c+80,y+r2-c+80,kolorBialy,-1,"%d",(int)macierz[2][2]);
    }
}

void zwrocIndeks(int aktualnaPozycja,int *i_x, int *i_y){
        switch(aktualnaPozycja){
        case 1:
            *i_x=0;
            *i_y=0;
            break;
        case 2:
            *i_x=0;
            *i_y=1;
            break;
        case 3:
            *i_x=0;
            *i_y=2;
            break;
        case 4:
            *i_x=1;
            *i_y=0;
            break;
        case 5:
            *i_x=1;
            *i_y=1;
            break;
        case 6:
            *i_x=1;
            *i_y=2;
            break;
        case 7:
            *i_x=2;
            *i_y=0;
            break;
        case 8:
            *i_x=2;
            *i_y=1;
            break;
        case 9:
            *i_x=2;
            *i_y=2;
            break;
        }
}

void edytujMacierz(){

     int i_x,i_y,aktualnaPozycja=7;
     int liczbaWyboru;
     int klawisz, klawiszWyboruLiczby;
     while(1){
            rysujMacierz(aktualnaPozycja,-1);
            blit(buffor,screen,0,0,0,0,SC_W,SC_H);
            klawisz = readkey();
       if(key[KEY_ESC])
            break;
       if(klawisz==20992){
            aktualnaPozycja=aktualnaPozycja-1;
                if(aktualnaPozycja==0)
                    aktualnaPozycja=3;
                else if(aktualnaPozycja==3)
                    aktualnaPozycja=6;
                else if(aktualnaPozycja==6)
                    aktualnaPozycja=9;
        }
        if(klawisz==21248){
             aktualnaPozycja=aktualnaPozycja+1;
                 if(aktualnaPozycja==4)
                    aktualnaPozycja=0;
                else if(aktualnaPozycja==7)
                    aktualnaPozycja=4;
                else if(aktualnaPozycja==10)
                    aktualnaPozycja=7;
        }
         if(klawisz==21504){
             aktualnaPozycja=aktualnaPozycja-3;
                 if(aktualnaPozycja==-2)
                    aktualnaPozycja=7;
                else if(aktualnaPozycja==-1)
                    aktualnaPozycja=8;
                else if(aktualnaPozycja==0)
                    aktualnaPozycja=9;
        }
        if(klawisz==21760){
             aktualnaPozycja=aktualnaPozycja+3;
                 if(aktualnaPozycja==10)
                    aktualnaPozycja=1;
                else if(aktualnaPozycja==11)
                    aktualnaPozycja=2;
                else if(aktualnaPozycja==12)
                    aktualnaPozycja=3;
        }
        if(klawisz==17165){
            while(klawiszWyboruLiczby!=17165){


                klawiszWyboruLiczby = readkey();
                if(klawiszWyboruLiczby==21504){
                    zwrocIndeks(aktualnaPozycja,&i_x,&i_y);
                    liczbaWyboru = (int) macierz[i_x][i_y];
                    liczbaWyboru++;
                    if(liczbaWyboru==-10)
                        liczbaWyboru=9;
                    if(liczbaWyboru==10)
                        liczbaWyboru=-9;
                    macierz[i_x][i_y]=liczbaWyboru;
                }
                if(klawiszWyboruLiczby==21760){
                    zwrocIndeks(aktualnaPozycja,&i_x,&i_y);
                    liczbaWyboru = (int) macierz[i_x][i_y];
                    liczbaWyboru--;
                    if(liczbaWyboru==-10)
                        liczbaWyboru=9;
                    if(liczbaWyboru==10)
                        liczbaWyboru=-9;
                    macierz[i_x][i_y]=liczbaWyboru;
                }
                rysujMacierz(aktualnaPozycja,1);
                clear_bitmap(screen);
                blit(buffor,screen,0,0,0,0,SC_W,SC_H);
            }
        }
        klawiszWyboruLiczby=0;
        printf("%d klaiwsz",klawisz);
     }
}

void save_current_bitmap(){
        PALETTE tmp;
        get_palette_range(tmp,0,255);
        save_bmp("wynik.bmp",current->pastBitmap,tmp);
        allegro_message("\n   Zapisano BITMAPE  -  \"wynik.bmp\" \n");
}


int wczytaj(){

    BITMAP *orginal=create_bitmap(SC_W, SC_H);
    BITMAP *tmpBIT;
    blit(buffor,orginal,0,0,0,0,SC_W,SC_H);
    char name[250]={'\0'};
    char tmp[13]={'\0'};
    int klawisz=0,i=0;
    rectfill(buffor, 150, 80, 285, 113, makecol(58,52,49));
    textprintf_ex( buffor, font, 164, 93,makecol( 255, 255, 255 ), -1,"%c",(char)15711 );
    blit(buffor,screen,0,0,0,0,SC_W,SC_H);

    while( (klawisz=readkey())!=17165 )
    {
        if(key[KEY_ESC]) {blit(orginal,buffor,0,0,0,0,SC_W,SC_H); return 0;}
        rectfill(buffor, 150, 80, 285, 113, makecol(58,52,49));
        blit(buffor,screen,0,0,0,0,SC_W,SC_H);
        if(klawisz==16136)
            {
                if(i>0)
                {
                    name[i-1]='\0';
                    i--;
                }
            }
        else{
                name[i]= (char ) klawisz;
                i++;
        }
        if(strlen(name)>12){
          rectfill(buffor, 150, 80, 285, 113, makecol(58,52,49));
          int j;
          for(j=0;j<12;j++){
                tmp[j]=name[strlen(name)-11+j];
          }
          tmp[12]='\0';
         textprintf_ex( buffor, font, 164, 93,makecol( 255, 255, 255 ), -1,"%s%c",tmp,(char)15711 );
          blit(buffor,screen,0,0,0,0,SC_W,SC_H);
        }
        else{
            textprintf_ex( buffor, font, 164, 93,makecol( 255, 255, 255 ), -1,"%s%c",name,(char)15711 );
            blit(buffor,screen,0,0,0,0,SC_W,SC_H);
        }

    }
    blit(orginal,buffor,0,0,0,0,SC_W,SC_H);


    if((tmpBIT =  load_bmp(name,default_palette))!=NULL )
    {
        orginalBITMAP=create_bitmap(P_W,P_H);
        stretch_blit(tmpBIT, orginalBITMAP,0, 0,tmpBIT->w,tmpBIT->h, 0, 0, P_W,P_H);
        destroy_bitmap(tmpBIT);
        if(we==NULL){
                screen_pixels_W = orginalBITMAP->w;
                screen_pixels_H = orginalBITMAP->h;
                allocate_tabs(screen_pixels_W,screen_pixels_H);
        }
        else {
             for(i=0; i<screen_pixels_W; ++i) {
                    free(we[i]);
                    free(wy[i]);
                    }
                free(we);
                free(wy);
                we = NULL; wy=NULL;
                screen_pixels_W = orginalBITMAP->w;
                screen_pixels_H = orginalBITMAP->h;
                allocate_tabs(screen_pixels_W,screen_pixels_H);
        }

    changedBITMAP = create_bitmap_ex(8,screen_pixels_W,screen_pixels_H);
    bitmapToArray(orginalBITMAP);

        rectfill(buffor, 390, 40, 390+P_W+20 , 40+P_H+20, makecol(58,52,49));
        rectfill(buffor, 390, 340, 390+P_W+20 , 340+P_H+20, makecol(58,52,49));
        draw_sprite(buffor,orginalBITMAP,400,50);

        rectfill(buffor, 10, 5, 205, 22, makecol(58,52,49));
        if(strlen(name)<14) textprintf_ex( buffor, font, 13, 10,makecol( 255, 255, 255 ), -1,"PLIK: %s",name );
        else textprintf_ex( buffor, font, 13, 10,makecol( 255, 255, 255 ), -1,"PLIK: ...%s",tmp );
        return 1;
    }
    else{
        if(we!=NULL){
             for(i=0; i<screen_pixels_W; ++i) {
                    free(we[i]);
                    free(wy[i]);
                    }
                free(we);
                free(wy);
                we=NULL;
                wy=NULL;
        }
        blit(bckg,buffor, 0,0,0,0,SC_W,SC_H);
        allegro_message("\n  Nie Znaleziono Pliku  \n");
        return 0;
    }


}


void inicjuj(){

    interfejs[0].x=10; interfejs[0].y=30; interfejs[0].rx=145; interfejs[0].ry=63; sprintf(interfejs[0].nazwa,"R. Jednorodne");
    interfejs[1].x=10; interfejs[1].y=80; interfejs[1].rx=145; interfejs[1].ry=113 ; sprintf(interfejs[1].nazwa,"R. Gaussa ");
    interfejs[2].x=10; interfejs[2].y=130; interfejs[2].rx=145; interfejs[2].ry=163; sprintf(interfejs[2].nazwa,"Wyostrzenie ");
    interfejs[3].x=10; interfejs[3].y=180; interfejs[3].rx=145; interfejs[3].ry=213; sprintf(interfejs[3].nazwa,"Wl. Macierz");
    interfejs[4].x=10; interfejs[4].y=230; interfejs[4].rx=145; interfejs[4].ry=263; sprintf(interfejs[4].nazwa,"Edytuj Mac.");
    interfejs[5].x=10; interfejs[5].y=280; interfejs[5].rx=145; interfejs[5].ry=313; sprintf(interfejs[5].nazwa,"Do przodu");
    interfejs[6].x=10; interfejs[6].y=330; interfejs[6].rx=145; interfejs[6].ry=363; sprintf(interfejs[6].nazwa,"Do tylu");
    interfejs[7].x=150; interfejs[7].y=550; interfejs[7].rx=285; interfejs[7].ry=583; sprintf(interfejs[7].nazwa,"Zapisz");
    interfejs[8].x=150; interfejs[8].y=30; interfejs[8].rx=285; interfejs[8].ry=63; sprintf(interfejs[8].nazwa,"Wczytaj");
    interfejs[9].x=10; interfejs[9].y=550; interfejs[9].rx=145; interfejs[9].ry=583; sprintf(interfejs[9].nazwa,"Wyjscie");

    buffor= create_bitmap(SC_W, SC_H);
    bckg= load_bitmap("bck.bmp", default_palette );
    blit(bckg,buffor, 0,0,0,0,SC_W,SC_H);
    button= load_bitmap("icon1.bmp", default_palette );
    button2 = load_bitmap("icon7.bmp", default_palette );
    button3 = load_bitmap("icon3.bmp", default_palette );
    button4 = load_bitmap("icon4.bmp", default_palette );
    button5 = load_bitmap("icon5.bmp", default_palette );
    button6 = load_bitmap("icon6.bmp", default_palette );
    button7 = load_bitmap("icon8.bmp", default_palette );
    button8 = load_bitmap("icon9.bmp", default_palette );

}

void ustaw(){
    int i=0;
    for (i=0;i<ILOSC_ELEMENTOW;i++){
            if (i==4)  draw_sprite(buffor,button3,interfejs[i].x,interfejs[i].y);
            else if(i==5) draw_sprite(buffor,button4,interfejs[i].x,interfejs[i].y);
            else if(i==6) draw_sprite(buffor,button5,interfejs[i].x,interfejs[i].y);
            else if(i==7) draw_sprite(buffor,button7,interfejs[i].x,interfejs[i].y);
            else if(i==8) draw_sprite(buffor,button8,interfejs[i].x,interfejs[i].y);
            else if(i==9) draw_sprite(buffor,button6,interfejs[i].x,interfejs[i].y);
            else draw_sprite(buffor,button,interfejs[i].x,interfejs[i].y);
            textprintf_ex(buffor,font,interfejs[i].x+5,interfejs[i].y+12,makecol(240,255,240),-1,"%s",interfejs[i].nazwa);
    }
    rysujMacierz(0,-1);
    if(we!=NULL){
    if(poczatek_prostokata_x>400 && poczatek_prostokata_x<(390+screen_pixels_W+10))
        if(poczatek_prostokata_y>50 && poczatek_prostokata_y<(40+screen_pixels_H+10)){
            rect(buffor, poczatek_prostokata_x, poczatek_prostokata_y, koniec_prostokata_x, koniec_prostokata_y, makecol(255,0,0));
             p_x = poczatek_prostokata_x;
             p_y = poczatek_prostokata_y;
             k_x = koniec_prostokata_x;
             k_y = koniec_prostokata_y;
        }
    }
    blit(buffor,screen,0,0,0,0,SC_W,SC_H);
}


void mouseEvent(){
    int i;

    if(mouse_b & 1){
    poczatek_prostokata_x=mouse_x;
    poczatek_prostokata_y=mouse_y;

        for(i=0;i<ILOSC_ELEMENTOW;i++){
            if(mouse_x>interfejs[i].x && mouse_x<interfejs[i].rx)
                if(mouse_y>interfejs[i].y && mouse_y<interfejs[i].ry)
                    {
                    draw_sprite(screen,button2,interfejs[i].x,interfejs[i].y);
                    textprintf_ex(screen,font,interfejs[i].x+5,interfejs[i].y+12,makecol(240,255,240),-1,"%s",interfejs[i].nazwa);
                    while(mouse_b &1);
                      switch(i){
                        case 0 : if(we!=NULL)
                                  rozmycie_jednorodne();
                                   break;
                        case 1 : if(we!=NULL)
                                  rozmycie_Gaussa();
                                   break;
                        case 2 : if(we!=NULL)
                                  wyostrzanie();
                                   break;
                        case 3 : if(we!=NULL)
                                   wlasnaMacierz();
                                   break;
                        case 4 : edytujMacierz();
                                  break;
                        case 5 : goFront();
                                  break;
                        case 6 : goBack();
                                  break;
                        case 7 : if(current!=NULL)
                                 save_current_bitmap();
                                 break;
                        case 8 : wczytaj();
                                 break;
                        case 9 : exit(0);
                                 break;

                        default : break;
                        }
                    }
        }
        long i=0;
        if(we!=NULL)
        while(mouse_b &1){
            if(mouse_x>400 && mouse_x<(390+screen_pixels_W+10) && poczatek_prostokata_x>400 && poczatek_prostokata_x<(390+screen_pixels_W+10)){
                if(mouse_y>50 && mouse_y<(40+screen_pixels_H+10) && poczatek_prostokata_y>50 && poczatek_prostokata_y<(40+screen_pixels_H+10)){
                    draw_sprite(buffor,orginalBITMAP,400,50);
                    koniec_prostokata_x = mouse_x;
                    koniec_prostokata_y = mouse_y;
                    rect(screen, poczatek_prostokata_x, poczatek_prostokata_y, mouse_x, mouse_y, makecol(255,0,0));
                    if(i%14==0)
                        blit(buffor,screen,0,0,0,0,SC_W,SC_H);
                    i++;
                }else{
                    p_x=400;
                    k_x = 390+screen_pixels_W+10;
                    p_y=50;
                    k_y = 40+screen_pixels_H+10;
                    draw_sprite(buffor,orginalBITMAP,400,50);
                }
            }else{

              p_x=400;
              k_x = 390+screen_pixels_W+10;
              p_y=50;
              k_y = 40+screen_pixels_H+10;
              draw_sprite(buffor,orginalBITMAP,400,50);
            }
        }
    }
}

void gui(){

    inicjuj();
    while(1){
        ustaw();
        mouseEvent();
    }
}

int main()
{
    allegro_init();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SC_W, SC_H, 0, 0);
    install_keyboard();
    install_mouse();
    show_os_cursor(1);

    //orginalBITMAP =  load_bmp("Icon1.bmp",default_palette);

    //screen_pixels_W = orginalBITMAP->w;
    //screen_pixels_H = orginalBITMAP->h;

 //   changedBITMAP = create_bitmap_ex(8,screen_pixels_W,screen_pixels_H);

 //   allocate_tabs(screen_pixels_W,screen_pixels_H);

  //  bitmapToArray(orginalBITMAP);

    gui( );
    return 0;
}
END_OF_MAIN()
