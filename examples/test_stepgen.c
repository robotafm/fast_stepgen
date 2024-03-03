#include <stdio.h> //Для printf
#include <stdlib.h> //Для malloc
#include <string.h> //Для memcpy
#include <time.h>  //Для clock_gettime

#define NUMBER 10000

char* concat(char *s1, char *s2) {

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);                      

    char *result = malloc(len1 + len2 + 1);

    if (!result) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);    

    return result;
}

int main (void)
{  
    //Структуры для сохранения определенного времени
    struct timespec mt1, mt2, mt3; 
    
    //Переменная для расчета дельты времени
    long int tt;
    int counter = 0;
    int max_delay = 0;
    int min_delay = 1000000000;
   
    //Определяем текущее время
    clock_gettime (CLOCK_REALTIME, &mt1);
    clock_gettime (CLOCK_REALTIME, &mt3);
   
    // строка для записи
    char * message;
    
    // файл для записи
    char * filename = "/stp/step.txt";
    
    // запись в файл
    FILE *fp = fopen(filename, "a");
    
	char TimeString[20]; // для записи абсолютного времени (с начала программы)
	char DeltaString[20];  // для записи относительного времени

    for (int i=0; i<NUMBER; i++)
	{
		counter++;
		if(counter > 1) counter = 0;
		if(counter == 0) message = "1 0 1 0 ";
		else message = "0 1 0 1 ";
		
	    //Определяем текущее время
        clock_gettime (CLOCK_REALTIME, &mt2);
   
        //Рассчитываем абсолютное время в нс
        tt=1000000000*(mt2.tv_sec - mt1.tv_sec)+(mt2.tv_nsec - mt1.tv_nsec);
        
		//Преобразуем в число
		snprintf(TimeString, 20,"%d",tt);
		
		//Рассчитываем относительное время в нс
		tt=1000000000*(mt2.tv_sec - mt3.tv_sec)+(mt2.tv_nsec - mt3.tv_nsec);
		
		//Ищем максимальную задержку
		if (tt>max_delay) max_delay = tt;
		if (tt<min_delay) min_delay = tt;
		
		//Определяем текущее время
        clock_gettime (CLOCK_REALTIME, &mt3);
		
		//Преобразуем в число
		snprintf(DeltaString, 20,"%d",tt);
        
		message = concat(message, TimeString);
		message = concat(message, " ");
		message = concat(message, DeltaString);
		message = concat(message, "\n");

		if(fp)
		{
			// записываем строку
			fputs(message, fp);
		}
		else
		{
			printf("Can't open file...\n");
		}
	}
	printf("Максимальная задержка %d нс, %d кГц\n", max_delay, (int)(500000/max_delay));
	printf("Мимимальная задержка %d нс, %d кГц\n", min_delay, (int)(500000/min_delay));
	tt=1000000000*(mt3.tv_sec - mt1.tv_sec)+(mt3.tv_nsec - mt1.tv_nsec);
	printf("Сделано %d шагов за %f мс.\n", NUMBER, (double)tt/1000000);
    fclose(fp);	
    return 0;
}
