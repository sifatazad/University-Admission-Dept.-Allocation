#include<stdio.h>
#include<stdlib.h>
#include<string.h>

static int tot_syl;

struct applied_list {
    int admit_no;
    char name[100];
    float gpa;
    char college[150];
};

struct eligible_exam {
    int admit_no;
    char name[100];
    int *mark;
    int total;
};

struct choice_info {
    int admit_no;
    char name[100];
    char **sub;
};

struct dept_info {
    char short_form[10];
    int capacity;
};

struct dept_name_info {
    char short_form[10];
    char full_form[100];
};

struct allocate_info {
    int admit;
    int merit;
    char name[100];
    char college[150];
    char got_dept[20];
    char full_form[150];
};

static int total_applied_student(FILE *fp) {
    int n=0;
    char line[1000];
    while (fgets(line,sizeof(line),fp)) {
        n++;
    }
    rewind(fp);
    return n;
}

static int total_eligible_student(FILE *fp) {
    int n=0;
    char line[1000];
    while (fgets(line,sizeof(line),fp)) {
        n++;
    }
    rewind(fp);
    return n;
}

static int total_exam_subject(FILE *fp) {
    char line[200];
    fgets(line,sizeof(line),fp);
    int len=strlen(line);
    int n=0;
    for (int i=0; i<len; i++) {
        if (line[i]=='\t')
            n++;
    }
    rewind(fp);
    return n-1;
}

static int total_department(FILE *fp) {
    int n=0;
    char line[1000];
    while (fgets(line,sizeof(line),fp)) {
        n++;
    }
    rewind(fp);
    return n;
}

static int total_capacity(FILE *fp) {
    int n=0;
    char line[1000];
    while (fgets(line,sizeof(line),fp)) {
        n++;
    }
    rewind(fp);
    // return n;
    int s=0;
    for (int i=0; i<n; i++) {
        int a;
        // char shrt[10];
        fscanf(fp,"%*s\t%d",&a);
        s+=a;
    }
    rewind(fp);
    return s;
}

static int compare_total_mark(const void *a, const void *b) {
    struct eligible_exam *e1=(struct eligible_exam*) a;
    struct eligible_exam *e2=(struct eligible_exam*) b;

    if (e1->total > e2->total)
        return -1;
    if (e1->total < e2->total)
        return 1;
    for (int i=0; i<tot_syl; i++) {
        if (e1->mark[i] > e2->mark[i])
            return -1;
        if (e1->mark[i] < e2->mark[i])
            return 1;
    }
    return 0;
}

static int compare_dept(const void *a, const void *b) {
    struct dept_info *d1=(struct dept_info*) a;
    struct dept_info *d2=(struct dept_info*) b;
    return strcmp(d1->short_form,d2->short_form);
}

static int compare_admit_no(const void *a, const void *b) {
    struct applied_list *a1=(struct applied_list*) a;
    struct applied_list *b1=(struct applied_list*) b;
    return a1->admit_no-b1->admit_no;
}

static int compare_dept_full_name(const void *a, const void *b) {
    struct dept_name_info *d1=(struct dept_name_info*) a;
    struct dept_name_info *d2=(struct dept_name_info*) b;
    return strcmp(d1->short_form,d2->short_form);
}

int main() {
    FILE *fp1=fopen("input1.txt","r");
    if (!fp1) {
        return 1;
    }
    int total_applied=total_applied_student(fp1);

    struct applied_list *a;
    a=malloc(sizeof(struct applied_list)*total_applied);
    if (!a) {
        fclose(fp1);
        return 1;
    }

    for (int i=0; i<total_applied; i++) {
        fscanf(fp1,"%d\t%[^\t]\t%f\t%[^\n]",
            &a[i].admit_no,a[i].name,&a[i].gpa,a[i].college);
    }

    FILE *fp2=fopen("input2.txt","r");
    if (!fp2) {
        fclose(fp1);
        free(a);
        return 1;
    }
    int total_eligible=total_eligible_student(fp2);
    int total_syllabus=total_exam_subject(fp2);
    tot_syl=total_syllabus;

    struct eligible_exam *e=malloc(sizeof(struct eligible_exam)*total_eligible);
    if (!e) {
        fclose(fp1);
        fclose(fp2);
        free(a);
        return 1;
    }
    for (int i=0; i<total_eligible; i++) {
        e[i].mark=malloc(sizeof(int)*total_syllabus);
        if (!e[i].mark) {
            fclose(fp1);
            fclose(fp2);
            free(a);
            free(e);
            return 1;
        }
    }
    for (int i=0; i<total_eligible; i++) {
        fscanf(fp2,"%d\t%[^\t]",&e[i].admit_no,e[i].name);
        for (int j=0; j<total_syllabus; j++) {
            fscanf(fp2,"%d",&e[i].mark[j]);
        }
    }

    int s=0;
    for (int i=0; i<total_eligible; i++) {
        s=0;
        for (int j=0; j<total_syllabus; j++) {
            s+=e[i].mark[j];
        }
        e[i].total=s;
    }

    FILE *fp4=fopen("input4.txt","r");
    if (!fp4) {
        for (int i=0; i<total_eligible; i++) {
            free(e[i].mark);
        }
        fclose(fp1);
        fclose(fp2);
        free(a);
        free(e);
        return 1;
    }
    int total_dept=total_department(fp4);
    // printf("%d\n",total_capacity(fp4));

    struct dept_info *d=malloc(sizeof(struct dept_info)*total_dept);
    if (!d) {
        for (int i=0; i<total_eligible; i++) {
            free(e[i].mark);
        }
        fclose(fp1);
        fclose(fp2);
        fclose(fp4);
        free(a);
        free(e);
        return 1;
    }

    for (int i=0; i<total_dept; i++) {
        fscanf(fp4,"%s\t%d",d[i].short_form,&d[i].capacity);
    }

    FILE *fp3=fopen("input3.txt","r");
    if (!fp3) {
        for (int i=0; i<total_eligible; i++) {
            free(e[i].mark);
        }
        fclose(fp1);
        fclose(fp2);
        fclose(fp4);
        free(a);
        free(e);
        free(d);
        return 1;
    }

    struct choice_info *c=malloc(sizeof(struct choice_info)*total_eligible);
    if (!c) {
        for (int i=0; i<total_eligible; i++) {
            free(e[i].mark);
        }
        fclose(fp1);
        fclose(fp2);
        fclose(fp4);
        fclose(fp3);
        free(a);
        free(e);
        free(d);
        return 1;
    }

    for (int i=0; i<total_eligible; i++) {
        c[i].sub=malloc(sizeof(char*)*total_dept);
        if (!c[i].sub) {
            for (int ii=0; ii<total_eligible; ii++) {
                free(e[ii].mark);
            }
            fclose(fp1);
            fclose(fp2);
            fclose(fp4);
            fclose(fp3);
            free(a);
            free(e);
            free(d);
            free(c);
            return 1;
        }
        for (int j=0; j<total_dept; j++) {
            c[i].sub[j]=malloc(10);
            if (!c[i].sub[j]) {
                for (int ii=0; ii<total_eligible; ii++) {
                    free(e[ii].mark);
                }
                free(c[i].sub);
                fclose(fp1);
                fclose(fp2);
                fclose(fp4);
                fclose(fp3);
                free(a);
                free(e);
                free(d);
                free(c);
                return 1;
            }
        }
    }

    for (int i=0; i<total_eligible; i++) {
        fscanf(fp3,"%d\t%[^\t]",&c[i].admit_no,c[i].name);
        for (int j=0; j<total_dept; j++) {
            fscanf(fp3,"%s",c[i].sub[j]);
        }
    }

    FILE *fp5=fopen("input5.txt","r");
    if (!fp5) {
        for (int i=0; i<total_eligible; i++) {
            free(e[i].mark);
        }
        for (int i=0; i<total_eligible; i++) {
            for (int j=0; j<total_dept; j++) {
                free(c[i].sub[j]);
            }
            free(c[i].sub);
        }
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
        fclose(fp4);
        free(a);
        free(e);
        free(c);
        free(d);
        return 1;
    }

    struct dept_name_info *dn=malloc(sizeof(struct dept_name_info)*total_dept);
    if (!dn) {
        for (int i=0; i<total_eligible; i++) {
            free(e[i].mark);
        }
        for (int i=0; i<total_eligible; i++) {
            for (int j=0; j<total_dept; j++) {
                free(c[i].sub[j]);
            }
            free(c[i].sub);
        }
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
        fclose(fp4);
        fclose(fp5);
        free(a);
        free(e);
        free(c);
        free(d);
        return 1;
    }

    for (int i=0; i<total_dept; i++) {
        fscanf(fp5,"%s\t%[^\n]",dn[i].short_form,dn[i].full_form);
    }

    qsort(e,total_eligible,sizeof(struct eligible_exam),compare_total_mark);
    qsort(d,total_dept,sizeof(struct dept_info),compare_dept);

    FILE *fo=fopen("output.txt","w");
    if (!fo) {
        for (int i=0; i<total_eligible; i++) {
            free(e[i].mark);
        }
        for (int i=0; i<total_eligible; i++) {
            for (int j=0; j<total_dept; j++) {
                free(c[i].sub[j]);
            }
            free(c[i].sub);
        }
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
        fclose(fp4);
        fclose(fp5);
        free(a);
        free(e);
        free(c);
        free(d);
        free(dn);
        return 1;
    }

    for (int i=0; i<total_eligible; i++) {
        fprintf(fo,"%d\t%s\t",e[i].admit_no,e[i].name);
        for (int j=0; j<total_syllabus; j++) {
            fprintf(fo,"%d\t",e[i].mark[j]);
        }
        fprintf(fo,"%d\n",e[i].total);
    }

    struct allocate_info *alc= malloc(sizeof(struct allocate_info)*total_eligible);
    if (!alc) {
        for (int i=0; i<total_eligible; i++) {
            free(e[i].mark);
        }
        for (int i=0; i<total_eligible; i++) {
            for (int j=0; j<total_dept; j++) {
                free(c[i].sub[j]);
            }
            free(c[i].sub);
        }
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
        fclose(fp4);
        fclose(fp5);
        free(a);
        free(e);
        free(c);
        free(d);
        return 1;
    }

    for (int i=0; i<total_eligible; i++) {
        alc[i].full_form[0]='\0';
    }
    // qsort(c,total_eligible,sizeof(struct choice_info),compare_admit_no);
    for (int i=0; i<total_eligible; i++) {
        int idx;
        for (int k=0; k<total_eligible; k++) {
            if (e[i].admit_no==c[k].admit_no) {
                idx=k;
                break;
            }
        }
        for (int j=0; j<total_dept; j++) {
            struct dept_info key;
            strcpy(key.short_form,c[idx].sub[j]);

            struct dept_info *found=bsearch(&key,d,total_dept,
                sizeof(struct dept_info),compare_dept);
            if (found!=NULL) {
                if (found->capacity >0) {
                    found->capacity --;
                    alc[i].merit=i+1;
                    strcpy(alc[i].got_dept,c[idx].sub[j]);
                    alc[i].admit=c[idx].admit_no;
                    strcpy(alc[i].name,c[idx].name);
                    break;
                }
            }
            if (j==total_dept-1) {
                strcpy(alc[i].got_dept,"No Allocation");
                alc[i].admit=c[idx].admit_no;
                strcpy(alc[i].name,c[idx].name);
                alc[i].merit=i+1;
            }
        }
    }

    qsort(a,total_applied,sizeof(struct applied_list),compare_admit_no);
    for (int i=0; i<total_eligible; i++) {
        struct applied_list key;
        key.admit_no=alc[i].admit;
        struct applied_list *found= bsearch(&key,a,total_applied,
            sizeof(struct applied_list),compare_admit_no);
        if (found!=NULL) {
            strcpy(alc[i].college,found->college);
        }
    }

    qsort(dn,total_dept,sizeof(struct dept_name_info),compare_dept_full_name);
    for (int i=0; i<total_eligible; i++) {
        struct dept_name_info key;
        strcpy(key.short_form,alc[i].got_dept);
        struct dept_name_info *found= bsearch(&key,dn,total_dept,
            sizeof(struct dept_name_info),compare_dept_full_name);
        if (found!=NULL) {
            strcpy(alc[i].full_form,found->full_form);
        }
    }

    FILE *fo1=fopen("output2.txt","w");
    if (!fo1) {
        for (int i=0; i<total_eligible; i++) {
            free(e[i].mark);
        }
        for (int i=0; i<total_eligible; i++) {
            for (int j=0; j<total_dept; j++) {
                free(c[i].sub[j]);
            }
            free(c[i].sub);
        }
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
        fclose(fp4);
        fclose(fp5);
        free(a);
        free(e);
        free(c);
        free(d);
        free(alc);
        return 1;
    }

    for (int i=0; i<total_eligible; i++) {
        if (alc[i].full_form[0]!='\0') {
            fprintf(fo1,"%d.\t%d\t%s\t%s\t%s (%s)\n",alc[i].merit,alc[i].admit,alc[i].name,
                alc[i].college,alc[i].got_dept,alc[i].full_form);
        }
        else {
            fprintf(fo1,"%d.\t%d\t%s\t%s\t%s\n",alc[i].merit,alc[i].admit,alc[i].name,
                alc[i].college,alc[i].got_dept);
        }
    }

    FILE *fo2=fopen("output3.txt","w");
    if (!fo2) {
        for (int i=0; i<total_eligible; i++) {
            free(e[i].mark);
        }
        for (int i=0; i<total_eligible; i++) {
            for (int j=0; j<total_dept; j++) {
                free(c[i].sub[j]);
            }
            free(c[i].sub);
        }
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
        fclose(fp4);
        fclose(fp5);
        fclose(fo1);
        free(a);
        free(e);
        free(c);
        free(d);
        free(alc);
        return 1;
    }

    for (int i=0; i<total_dept; i++) {
        fprintf(fo2,"%s\n",dn[i].full_form);
        fprintf(fo2,"--------------------------------------------------\n");
        for (int j=0; j<total_eligible; j++) {
            if (!strcmp(dn[i].full_form,alc[j].full_form)) {
                fprintf(fo2,"%d.\t%s\t%s\n",alc[j].merit,alc[j].name,alc[j].college);
            }
        }
        fprintf(fo2,"\n\n");
    }

    // freeing everything
    for (int i=0; i<total_eligible; i++) {
        free(e[i].mark);
    }
    for (int i=0; i<total_eligible; i++) {
        for (int j=0; j<total_dept; j++) {
            free(c[i].sub[j]);
        }
        free(c[i].sub);
    }
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
    fclose(fo1);
    fclose(fo2);
    free(a);
    free(e);
    free(c);
    free(d);
    free(dn);
    free(alc);
    return 0;
}