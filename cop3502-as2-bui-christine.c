#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "leak_detector_c.h"

struct failfish_struct
{
    int failfish_number;
    struct failfish_struct *next;
    struct failfish_struct *prev;
};

typedef struct failfish_struct failfish;

struct failfish_queue_struct
{
    char *pondname;
    int pond_number;
    int n;                  // number of failfish
    int e;                  // eating counter
    int th;                 // minimum failgroup threshold

    failfish *fishlist_head;
    failfish *fishlist_tail;
};

typedef struct failfish_queue_struct failfish_queue;

// this function was in assignment 1 that Gerber said was okay to use
int get_next_nonblank_line(FILE *ifp, char *buf, int max_length) {

    buf[0] = '\0';

    while(!feof(ifp) && (buf[0] == '\0'))
    {
        fgets(buf, max_length, ifp);
        remove_crlf(buf);
    }

    if(buf[0] != '\0') {
        return 1;
    } else {
        return 0;
    }
}

// this function was in assignment 1 that Gerber said was okay to use
void remove_crlf(char *s) {

    char *t = s + strlen(s);
    t--;
    while((t >= s) && (*t == '\n' || *t == '\r'))
    {
        *t = '\0';
        t--;
    }
}

int get_number_of_ponds(FILE *ifp)
{
    char buf[256];
    int numPonds;

    get_next_nonblank_line(ifp, buf, 255);
    sscanf(buf, "%d ponds", &numPonds);

    return numPonds;
}

failfish *create_failfish(int failfish_number)
{
    failfish *f = malloc(sizeof(failfish));
    f->next = NULL;
    f->prev = NULL;
    f->failfish_number - failfish_number;
    return f;
}

void dispose_failfish(failfish *f)
{
    free(f);
}

failfish_queue *create_failfish_queue(failfish_queue *fq, int pond_number, char *pondname, int n, int e, int th)
{
   fq->fishlist_head = NULL;
   fq->fishlist_tail = NULL;

   fq->pond_number = pond_number;
   fq->pondname = strdup(pondname);
   fq->n = n;
   fq->e = e;
   fq->th = th;

   return fq;
}

void print_failfish_queue(failfish *f, failfish_queue *fq, int totalnumber_ponds)
{
    printf("%d %s %d %d %d", fq->pond_number, fq->pondname, fq->n, fq->e, fq->th);

    if(f->prev == NULL) {
        printf("No link backward\n");
    } else {
        printf("Link backward %d\n", f->prev->failfish_number);
    }

    if(f->next == NULL) {
        printf("No link backward\n");
    } else {
        printf("Link backward %d\n", f->next->failfish_number);
    }
}

void dispose_failfish_queue(failfish_queue *fq)
{
    free(fq->pondname);
    free(fq);
}

// after getting number of ponds, this reads the pond name, n, e, and th
void read_pondinfo(FILE *ifp, failfish_queue *fq)
{
    char buf[256];
    int totalnumber_ponds;
    char pondnamebuf[256];
    int n;
    int e;
    int th;

    get_next_nonblank_line(ifp, buf, 255);
    sscanf(buf, "%d %s %d %d %d", &totalnumber_ponds, pondnamebuf, &n, &e, &th);

    fq->pond_number = totalnumber_ponds;
    fq->pondname = strdup(pondnamebuf);
    fq->n = n;
    fq->e = e;
    fq->th = th;
}

void sort_queue(failfish_queue *fq, int totalnumber_ponds)
{
    failfish_queue *temp;
    temp = malloc(sizeof(failfish_queue) * totalnumber_ponds);

    for(int i = 0; i < totalnumber_ponds; i++) {
        for(int j = 0; j < totalnumber_ponds; j++) {

            if(fq[j].pond_number > fq[j+1].pond_number) {
                *temp = fq[j+1];
                fq[j+1] = fq[j];
                fq[j] = *temp;
            }
        }
    }

    free(temp);
}

void print_initialstat(failfish *f, failfish_queue *fq, int totalnumber_ponds, failfish_queue *pondinfo)
{
    printf("\nInitial Pond Status\n");
    int i;
    int j;

    for(i = 0; i < totalnumber_ponds; i++) {
        printf("%d %s", fq[i].pond_number, fq[i].pondname);
        for(j = 0; j < fq[i].n; j++) {
            printf("%d", j+1);
        }
        printf("\n");
    }
}

void enqueue(failfish_queue *fq)
{
    failfish *f;
    for(int i = 0; i < fq->n; i++) {
        f = create_failfish(i+1);

        if(fq->fishlist_tail == NULL) {
            fq->fishlist_head = f;
            fq->fishlist_tail = f;
            f->next = f;
            f->prev = f;
        } else {
            f->prev->next = f->next;
            f->next->prev = f->prev;
        }
    }
}

void dequeue(failfish *f, failfish_queue *fq)
{
    if(fq->fishlist_head == NULL) {
        return;
    }

    if((fq->fishlist_head == fq->fishlist_tail) && (fq->fishlist_head == f)) {
        fq->fishlist_head = NULL;
        fq->fishlist_tail = NULL;

        return;
    }

    if(f == fq->fishlist_head) {
        fq->fishlist_head = fq->fishlist_head->next;
        fq->fishlist_head->prev = fq->fishlist_tail;
        fq->fishlist_tail->next = fq->fishlist_head;

        return;
    }

    if(f == fq->fishlist_tail) {
        fq->fishlist_tail = fq->fishlist_tail->prev;
        fq->fishlist_tail->next = fq->fishlist_head;
        fq->fishlist_head->prev = fq->fishlist_tail;

        return;
    }
}

void peek(failfish_queue *fq)
{
    fq->fishlist_head;
    return fq;
}

void print_first(failfish *f, failfish_queue *fq, int totalnumber_ponds)
{
    int i;
    int j;

    printf("\nFirst Course\n");
        for(i = 0; i < totalnumber_ponds; i++) {
            printf("Pond %d: %s\n", fq[i].pond_number, fq[i].pondname);
            f = fq[i].fishlist_head;
            for(j = 0; fq[i].e-1; j++) {
                f = f->next;
            }

            printf("%d", f->failfish_number);
            dequeue(fq+i, f);
            for(i = 0; (fq[i].n-fq[i].th)-1; i++) {
                for(j = 0; fq[i].e; j++) {
                    f = f->next;
                }

                printf("%d", f->failfish_number);
                dequeue(fq+i, f);

                for(j = 0; j < fq[i].n-fq[i].th; j++) {
                    printf("Failfish %d eaten\n", f[i].failfish_number);
                }
            }

            printf("\nEnd of Course Pond Status\n");
            for(i = 0; i < totalnumber_ponds; i++) {

                printf("\n%d %s", fq[i].pond_number, fq[i].pondname);
                for(j = 0; j < fq[i].th; j++) {
                    printf(" %d", f[i].failfish_number);
                }
            }
        }
}

void print_second(failfish *f, failfish_queue *fq, int totalnumber_ponds)
{
    int i;
    int j;

    printf("\nSecond Course\n");
            for(i = 0; i < totalnumber_ponds; i++) {
            printf("Pond %d: %s\n", fq[i].pond_number, fq[i].pondname);
            f = fq[i].fishlist_head;
            for(j = 0; fq[i].e-1; j++) {
                f = f->next;
            }

            printf("%d", f->failfish_number);
            dequeue(fq+i, f);
            for(i = 0; (fq[i].n-fq[i].th)-1; i++) {
                for(j = 0; fq[i].e; j++) {
                    f = f->next;
                }

                printf("%d", f->failfish_number);
                peek(f+i);

                for(j = 0; j < fq[i].n-fq[i].th; j++) {
                    printf("Eaten: Failfish %d from pond %d", f[i].failfish_number, fq[i].pond_number);
                }

                printf("\nFailfish %d from pond %d remains\n", f[i].failfish_number, fq[i].pond_number);
                    for(i = 0; i < totalnumber_ponds; i++) {

                    printf("\n%d %s", fq[i].pond_number, fq[i].pondname);
                    for(j = 0; j < fq[i].th; j++) {
                        printf(" %d", f[i].failfish_number);
                }
            }
        }
    }
}

int main()
{
    atexit(report_mem_leak);
    FILE *ifp;
    FILE *ofp;
    ifp = fopen("cop3502-as2-input.txt", "r");
    ofp = fopen("cop3502-as2-output-bui-christine.txt", "w");
    failfish_queue *pondinfo;
    failfish *f;
    failfish_queue *fq;

    int totalnumber_ponds = get_number_of_ponds(ifp);
    pondinfo = malloc(sizeof(failfish_queue) * totalnumber_ponds);

    // prints number of ponds from input file
    // printf("%d\n", totalnumber_ponds);

    for(int i = 0; i < totalnumber_ponds; i++) {
        read_pondinfo(ifp, pondinfo + i);

        // prints pond information from input file
        // printf("%d %s %d %d %d\n", pondinfo[i].pond_number, pondinfo[i].pondname, pondinfo[i].n, pondinfo[i].e, pondinfo[i].th);
    }

    // sorts the ponds from least to greatest
    // sort_queue(fq, totalnumber_ponds);
    // prints initial pond status
    // print_initialstat(f, fq, totalnumber_ponds, pondinfo);

    for (int i = 0; i < totalnumber_ponds; i++) {

    }

    // prints first course
    // print_first(f, fq, totalnumber_ponds);
    // prints second course
    // print_second(f, fq, totalnumber_ponds);

    fclose(ifp);
    fclose(ofp);

    return 0;
}
