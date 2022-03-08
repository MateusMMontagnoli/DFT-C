#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<dirent.h>
//---------------------------------------
int main(int argc, char *argv[])
{
    double soma_real;
    double soma_imag;
    double angulo;
    int i,k, t;
    int num = 10;
    double H[4095];
    double pi = 3.14159;
    char linha[150];
    FILE *arquivos;
    char arquivo_nome[100];
    for (t = 1; t < argc; t++)
    {
        if((arquivos=fopen(argv[t],"r"))!=NULL)
        {
            double temp;
            int l = 0;
            i = 0;
            while(fgets(linha, sizeof(linha), arquivos)) {
                sscanf(linha, "%lf", &temp);
                l++;
            }
            int tam = l;
            arquivos=fopen(argv[t],"r");
            double y[l];
            l = 0;
            while(fgets(linha, sizeof(linha), arquivos)) {
                sscanf(linha, "%lf", &temp);
                y[l] = temp;
                l++;
            }
            
            
            // Pre-Processing Signal
            double soma_amplitude_dom_tempo = 0;
            for (i = 0; i < tam; i++)
            {
                soma_amplitude_dom_tempo += y[i];
            }
            double media_amplitude_dom_tempo = soma_amplitude_dom_tempo/tam;
            for (i = 0; i < tam; i++)
            {
                y[i] = y[i] - media_amplitude_dom_tempo;
            }
            for (i = 0; i < tam; i++)
            {
                y[i] = y[i] - 0.95*y[i+1];
            }
            
            // Creating a txt file for DFT data
            char arquivo_nome[100];
            char arquivo_nome_temp[100];
            char arquivo_nome2[100];
            strcpy(arquivo_nome_temp, argv[t]);
            char temp_4;
            i = 0;
            do
            {
                temp_4 = arquivo_nome_temp[i];
                if( temp_4 != '.')
                    i++;
            }while(temp_4 != '.');
            strcpy(arquivo_nome2, argv[t]);
            arquivo_nome2[i] = 'D';
            arquivo_nome2[i + 1] = 'F';
            arquivo_nome2[i + 2] = 'T';
            arquivo_nome2[i + 3] = '.';
            arquivo_nome2[i + 4] = 't';
            arquivo_nome2[i + 5] = 'x';
            arquivo_nome2[i + 6] = 't';
            arquivo_nome2[i + 7] = '\0';
            sprintf(arquivo_nome, arquivo_nome2);

            FILE *DFT_MOVEL = fopen(arquivo_nome, "w");
            double H_Movel[2047]; // Half lenght of DFT
            if(DFT_MOVEL != NULL)
            {

                for (i = 0 ; i < 4096; i++)
                {
                     soma_real = 0;
                     soma_imag = 0;
                    for (k = 0; k < tam; k ++)
                    {

                        angulo = 2*M_PI*i*k/(4096);
                        soma_real += y[k]*cos(angulo);
                        soma_imag += y[k]*sin(angulo);
                    }
                    H[i] = (sqrt(pow(soma_real/4096,2) + pow(soma_imag/4096,2))) + 1; 
                    H[i] = 20*log10(H[i]); // db
                }

                // Storing half of the DFT data
                for (i = 0; i < 2048; i++)
                {
                    fprintf(DFT_MOVEL, "%lf\n", H[i]);
                }
            }
            num++;
            // Closing txt file
            fclose(DFT_MOVEL);
            fclose(arquivos);
        }
    }
    system("pause");
}
