/*
    Funzione per la stampa del risultato
*/

#include "Standard.h"
#include "Strutture.h"
#include "Tipi.h"

//prototipi
int stampa_punto(Point*, FILE*);
int stampa_seg(Seg*, FILE*);
int stampa_faccia(Face*, FILE*);
int stampa_sol(Solid*, FILE*);

int Stampa()
{
    FILE* file_stream;
    Seg_List SgCurs;


    file_stream = fopen(RESULT,"w");

    if (file_stream == NULL)
    {
        fprintf(OUTPUT,"Stampa: Non posso scrivere sul file\n");
        return -1;
    }

    fprintf(file_stream, "hold on \n");
    fprintf(file_stream, "title('Risultato'); \n");
    fprintf(file_stream, "xlabel('x'); \n");
    fprintf(file_stream, "ylabel('y'); \n");
    fprintf(file_stream, "zlabel('z'); \n");
    fprintf(file_stream, "view(-37.5, 30); \n");

    SgCurs = Sg;

    while (SgCurs != NULL)
    {
        fprintf(file_stream,"plot3([ %lf , %lf ], [ %lf , %lf ], [ %lf , %lf ]); \n",SgCurs->S.A->x,SgCurs->S.B->x, SgCurs->S.A->y, SgCurs->S.B->y, SgCurs->S.A->z, SgCurs->S.B->z);

        SgCurs = SgCurs->next;
    }

    fclose(file_stream);

    system(PLOT_RESULT_COMM);


    return 0;
}


int stampa_punto(Point* P, FILE* output_stream)
{
    fprintf(output_stream, "%f %f %f  ", P->x, P->y, P->z );

    return 0;
}

int stampa_seg(Seg* S, FILE* output_stream)
{
    stampa_punto(S->A,output_stream);
    fprintf(output_stream, "<->  ");
    stampa_punto(S->B,output_stream);

    return 0;
}

int stampa_faccia(Face* F, FILE* output_stream)
{
    Seg_PointerList SgPtr;

    SgPtr = F->s;
    while (SgPtr != NULL)
        {
            stampa_seg(SgPtr->sptr, output_stream);
            fprintf(output_stream, " %c \n", SgPtr->orient);
            SgPtr = SgPtr -> next;
        }
    return 0;
}

int stampa_sol(Solid* S, FILE* output_stream)
{
    Face_PointerList FcPtr;
    int j = 1;

    FcPtr = S->f;
    while (FcPtr != NULL)
    {
        fprintf(output_stream,"  %d^ Faccia:\n", j);
        stampa_faccia(FcPtr->fptr, output_stream);
        fprintf(output_stream,"\n");
        j++;
        FcPtr = FcPtr->next;
    }

    return 0;
}

void print_all(char dest) // parametro: 0 per stampa su console, 1 su file
{
    Point_List cursPoint;
    Seg_List cursSeg;
    Face_PointerList cursf;
    Face_List cursface;
    Solid_List cursSolid;
    Seg_PointerList cursSegpointer;
    Face_PointerList cursFacepointer;

    FILE* DEST;
    if (dest == 0)
    {
        DEST = stdout;
    }
    else
    {
        DEST = fopen("Risultato.txt","w");
    }

    int k;
    cursPoint = Pt;
    k=1;
    fprintf(DEST,"\n\nPunti\n");
    while(cursPoint!=NULL){
            fprintf(DEST,"%d) %f %f %f %c \n",k++, cursPoint->P.x, cursPoint->P.y, cursPoint->P.z, cursPoint->P.side);
            cursPoint=cursPoint->next;
    }
    cursSeg=Sg;
    k=1;
    fprintf(DEST,"\n\n Segmenti\n");
    while(cursSeg!=NULL){
            fprintf(DEST,"%d) %f %f %f -> %f %f %f ",k++, cursSeg->S.A->x, cursSeg->S.A->y, cursSeg->S.A->z, cursSeg->S.B->x, cursSeg->S.B->y, cursSeg->S.B->z);
            cursf=cursSeg->S.f;
            fprintf(DEST," facce del segmento: ");
            while(cursf!=NULL){
                fprintf(DEST,"%p ",cursf->fptr);
                cursf=cursf->next;
            }
            fprintf(DEST,"\n");
            cursSeg=cursSeg->next;
    }
    cursface=Fc;
    k=1;
    fprintf(DEST,"\n\n Facce\n");
    while(cursface!=NULL){
            fprintf(DEST,"%d) %p %d sol1:%p sol2:%p " ,k++, &(cursface->F), cursface->F.side, cursface->F.Sol1, cursface->F.Sol2);
            cursSegpointer=cursface->F.s;
            fprintf(DEST,"segmenti della faccia: \n");
            while(cursSegpointer!=NULL){
                if (cursSegpointer->orient=='+'){
                    fprintf(DEST,"%f %f %f\n",cursSegpointer->sptr->A->x,cursSegpointer->sptr->A->y,cursSegpointer->sptr->A->z);
                    fprintf(DEST,"%f %f %f\n",cursSegpointer->sptr->B->x,cursSegpointer->sptr->B->y,cursSegpointer->sptr->B->z); }
                else
                {
                    fprintf(DEST,"%f %f %f\n",cursSegpointer->sptr->B->x,cursSegpointer->sptr->B->y,cursSegpointer->sptr->B->z);
                    fprintf(DEST,"%f %f %f\n",cursSegpointer->sptr->A->x,cursSegpointer->sptr->A->y,cursSegpointer->sptr->A->z);
                }
                cursSegpointer=cursSegpointer->next;
            }
            fprintf(DEST,"\n");
            cursface=cursface->next;
    }
    fprintf(DEST,"\n\nSolidi\n");
    k=1;
    cursSolid=Sol;
    while(cursSolid!=NULL){
            fprintf(DEST,"%d) %p ",k++, &cursSolid->So);
            cursFacepointer=cursSolid->So.f;
            fprintf(DEST,"facce del solido:\n");
            while(cursFacepointer!=NULL){
                    fprintf(DEST,"%p%c ",cursFacepointer->fptr, cursFacepointer->orient);
                    cursFacepointer=cursFacepointer->next;
            }
            fprintf(DEST,"\n");
            cursSolid=cursSolid->next;
            }
    }

void new_print_all(char dest) // parametro: 0 per stampa su console, 1 su file
{
    Point_List cursPoint;
    Seg_List cursSeg;
    Face_PointerList cursf;
    Face_List cursface;
    Solid_List cursSolid;
    Seg_PointerList cursSegpointer;
    Face_PointerList cursFacepointer;

    FILE* DEST;
    if (dest == 0)
    {
        DEST = stdout;
    }
    else
    {
        DEST = fopen("Risultato.txt","w");
    }

    cursPoint = Pt;
    fprintf(DEST,"Punti\n");
    while(cursPoint!=NULL)
    {
            fprintf(DEST,"%p - %f %f %f \n", &(cursPoint->P), cursPoint->P.x, cursPoint->P.y, cursPoint->P.z);
            cursPoint=cursPoint->next;
    }
    cursSeg=Sg;
    fprintf(DEST,"\nSegmenti\n");
    while(cursSeg!=NULL){
            fprintf(DEST,"%p - %p -> %p ", &(cursSeg->S), cursSeg->S.A, cursSeg->S.B);
            cursf=cursSeg->S.f;
            fprintf(DEST," facce: ");
            while(cursf!=NULL){
                fprintf(DEST,"%p ",cursf->fptr);
                cursf=cursf->next;
            }
            fprintf(DEST,"\n");
            cursSeg=cursSeg->next;
    }
    cursface=Fc;
    fprintf(DEST,"\nFacce\n");
    while(cursface!=NULL){
            fprintf(DEST,"%p sol1:%p sol2:%p ", &(cursface->F), cursface->F.Sol1, cursface->F.Sol2);
            cursSegpointer=cursface->F.s;
            fprintf(DEST,"segmenti: ");
            while(cursSegpointer!=NULL){

                fprintf(DEST,"%p %c ", cursSegpointer->sptr , cursSegpointer->orient );
                cursSegpointer=cursSegpointer->next;
            }
            fprintf(DEST,"\n");
            cursface=cursface->next;
    }
    fprintf(DEST,"\nSolidi\n");
    cursSolid=Sol;
    while(cursSolid!=NULL){
            fprintf(DEST,"%p ", &(cursSolid->So) );
            cursFacepointer=cursSolid->So.f;
            fprintf(DEST,"facce: ");
            while(cursFacepointer!=NULL){
                    fprintf(DEST,"%p %c ",cursFacepointer->fptr, cursFacepointer->orient);
                    cursFacepointer=cursFacepointer->next;
            }
            fprintf(DEST,"\n");
            cursSolid=cursSolid->next;
            }
    }

    void controllo_errori(){

        Seg_List segCurs;
        Face_PointerList facePointerCurs;
        Seg_PointerList segPointerCurs;

        Face_List faceCurs;

        Point *precedente, *primo;



        segCurs= Sg;
        while(segCurs!=NULL){
            facePointerCurs= segCurs->S.f;
            while(facePointerCurs!= NULL){
                segPointerCurs = facePointerCurs->fptr->s;
                while(segPointerCurs!=NULL && (segPointerCurs->sptr!=&segCurs->S) ){
                    segPointerCurs=segPointerCurs->next;
                }
                if (segPointerCurs==NULL){
                    printf("\nERRORE: la lista facce e la lista segmenti non sono d'accordo\n");
                    printf("\nsegmento:%f %f %f -> %f %f %f  faccia:%p \n",segCurs->S.A->x,segCurs->S.A->y,segCurs->S.A->z,segCurs->S.B->x,segCurs->S.B->y,segCurs->S.B->z, facePointerCurs->fptr);
                }
                facePointerCurs=facePointerCurs->next;
            }
            segCurs=segCurs->next;
        }

        faceCurs=Fc;
        while(faceCurs!=NULL){
            segPointerCurs=faceCurs->F.s;
            while(segPointerCurs!=NULL){
                facePointerCurs=segPointerCurs->sptr->f;
                while(facePointerCurs!=NULL && facePointerCurs->fptr!=&faceCurs->F){
                    facePointerCurs=facePointerCurs->next;
                }
                if(facePointerCurs==NULL){
                    printf("\nERRORE: la lista facce e la lista segmenti non sono d'accordo\n");
                    printf("\nsegmento:%f %f %f -> %f %f %f  faccia:%p \n",segPointerCurs->sptr->A->x,segPointerCurs->sptr->A->y,segPointerCurs->sptr->A->z,segPointerCurs->sptr->B->x,segPointerCurs->sptr->B->y,segPointerCurs->sptr->B->z,&faceCurs->F);
                }
                segPointerCurs=segPointerCurs->next;
            }
            faceCurs=faceCurs->next;

        }

        faceCurs=Fc;
        while(faceCurs!=NULL){
            facePointerCurs=faceCurs->F.Sol1->f;
            while(facePointerCurs!=NULL && facePointerCurs->fptr!=&faceCurs->F){
                facePointerCurs=facePointerCurs->next;
            }
            if(facePointerCurs==NULL){
                printf("ERRORE:la lista facce e la lista solidi non sono d'accordo\n");
                printf("\nfaccia:%p solido:%p",&faceCurs->F,faceCurs->F.Sol1);
            }

            if(faceCurs->F.Sol2!=NULL){
                facePointerCurs=faceCurs->F.Sol2->f;
                while(facePointerCurs!=NULL && facePointerCurs->fptr!=&faceCurs->F){
                    facePointerCurs=facePointerCurs->next;
                }
                if(facePointerCurs==NULL){
                    printf("ERRORE:la lista facce e la lista solidi non sono d'accordo\n");
                    printf("\nfaccia:%p solido:%p",&faceCurs->F,faceCurs->F.Sol2);
                }
            }
            faceCurs=faceCurs->next;
        }

        faceCurs=Fc;
        while(faceCurs!=NULL){

            segPointerCurs=faceCurs->F.s;
            if (segPointerCurs->orient=='+')
            {
               primo=segPointerCurs->sptr->A;
               precedente=segPointerCurs->sptr->B;
            }
            else
            {
                primo=segPointerCurs->sptr->B;
                precedente=segPointerCurs->sptr->A;
            }
            segPointerCurs=segPointerCurs->next;
            while(segPointerCurs!=NULL){

                if (segPointerCurs->orient=='+')
                {
                    if(precedente!=segPointerCurs->sptr->A ||( (segPointerCurs->next==NULL) && (primo!=segPointerCurs->sptr->B) ) )
                        printf("\nERRORE:segmenti non consecutivi. faccia:%p ", &faceCurs->F);

                    precedente=segPointerCurs->sptr->B;
                }
                else
                {
                    if(precedente!=segPointerCurs->sptr->B ||( (segPointerCurs->next==NULL) && (primo!=segPointerCurs->sptr->A) ) )
                        printf("\nERRORE:segmenti non consecutivi. faccia:%p ", &faceCurs->F);

                    precedente=segPointerCurs->sptr->A;
                }

                segPointerCurs=segPointerCurs->next;
            }
            faceCurs=faceCurs->next;
        }

    }
