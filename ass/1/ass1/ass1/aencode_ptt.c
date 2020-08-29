#include <stdio.h>
#include <gmp.h>
#include <mpfr.h>

int N = 256;
mpfr_rnd_t rnd = MPFR_RNDN;
char charArray[1024];

struct item {
    char c;
    int feq;
    mpfr_t range_low;
    mpfr_t range_high;
};

void initAllchar(struct item *array){
    int i;

    for(i=0;i<N;i++){
        array[i].c = "";
        array[i].feq = 0;
        mpfr_init2(array[i].range_low,256);
        mpfr_init2(array[i].range_high,256);

//        mpfr_printf("%.*R*f", 20, rnd, array[i].range_low);
    }
}

void setRangeAllchar(struct item *array,int sum){
    int i;
    mpfr_t dis;
    mpfr_t low;
    mpfr_t freq;
    mpfr_t count;

    mpfr_init2(dis,64);
    mpfr_init2(low,64);
    mpfr_init2(freq,64);
    mpfr_init2(count,64);

    mpfr_set_d(low,0,rnd);
    mpfr_set_d(count,sum,rnd);

    for(i=0;i<N;i++){
        if(array[i].feq!=0){

            mpfr_init2(array[i].range_low,256);
            mpfr_init2(array[i].range_high,256);

            int feq = array[i].feq;
            mpfr_set_d(freq,feq,rnd);
            mpfr_set(array[i].range_low,low,rnd);

            mpfr_div(dis, freq, count, rnd);
            mpfr_add(array[i].range_high,low,dis,rnd);

            mpfr_set(low,array[i].range_high,rnd);

           // printf("%c %d\n",array[i].c,feq);
           // mpfr_out_str(stdout, 10, 0, array[i].range_low,rnd);
           // putchar(' ');
           // mpfr_out_str(stdout, 10, 0, array[i].range_high,rnd);
           // putchar('\n');

        }
    }

    mpfr_clear(dis);
    mpfr_clear(low);
    mpfr_clear(count);
    mpfr_clear(freq);
}


void encode(struct item *array,int count){
    mpfr_t low;
    mpfr_t high;
    mpfr_t range;
    char ch;

    mpfr_init2(low,256);
    mpfr_init2(high,256);
    mpfr_init2(range,256);

    mpfr_set_d(low,0,rnd);
    mpfr_set_d(high,1,rnd);

    int k = 0;
    while( k < count){
        int i = charArray[k];
        printf("***%c \n",charArray[k]);
        mpfr_t tmp_low;
        mpfr_init2(tmp_low,256);

        mpfr_sub(range,high,low,rnd);

     //   printf("$$%d \n",i);
       // mpfr_printf("%.*R*f",20, rnd, array[i].range_high);
     //   printf("%%%%%%%\n");

        mpfr_mul(high, range,array[i].range_high,rnd);
     //   printf("%%%%%%%\n");
        mpfr_add(high, low, high,rnd);

        mpfr_mul(tmp_low, range, array[i].range_low,rnd);
     //   printf("%%%%%%%\n");
        mpfr_add(low, low, tmp_low, rnd);

        k++;
    }

    mpfr_printf("%.*R*f",50, MPFR_RNDA, low);
    printf(" ");
    mpfr_printf("%.*R*f",50, MPFR_RNDZ, high);
    printf("\n");
}


int main() {
    char ch;
    struct item allchar[256];
    int count = 0;
    int i;

    for(i=0;i<N;i++){
        allchar[i].c = "";
        allchar[i].feq = 0;
        mpfr_init2(allchar[i].range_low,256);
        mpfr_init2(allchar[i].range_high,256);
      //  mpfr_set_d(allchar[i].range_low,1,rnd);
      //  mpfr_set_d(allchar[i].range_high,1,rnd);
      //  mpfr_printf("%.*R*f\n", 20, rnd, allchar[i].range_low);
    }


//    initAllchar(allchar);
    while(scanf("%c",&ch) != EOF){
        printf("%c\n",ch);
        int index = ch;
        allchar[index].c = ch;
        allchar[index].feq = allchar[index].feq + 1;
        charArray[count] = ch;
        count++;
    }

    setRangeAllchar(allchar,count);

    encode(allchar,count);


    return 0;
}
