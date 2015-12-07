#include "includes.h"

int GetTemperatureFromProbe(char* filename,float* temperature)
{
	int i;
	char chaine[LIGNE_MAX][TAILLE_MAX] = {{0}};
	int ret = RET_ERR;
	int check_frame = 0;
	int found = 0;
	int temp_int = 0;
	FILE* file_sonde = NULL;
	
	file_sonde = fopen(filename, "r");	
	if (file_sonde != NULL)
	{	
		i = 0;
		while ((fgets(chaine[i], TAILLE_MAX, file_sonde) != NULL) && (i < LIGNE_MAX))
		{
			//printf("%s\n",chaine);
			i++;
		}

		// file exemple
		//fc 00 4b 46 7f ff 04 10 a6 : crc=a6 YES
		//fc 00 4b 46 7f ff 04 10 a6 t=15750
		i=2;
		check_frame = 0;
		
		
		// Search the YES indicate that transfer was ok
		while((chaine[LIGNE_CRC][i]!='\0') && (found == 0))
		{	
			i++;
			if(chaine[LIGNE_CRC][i-2]=='Y' && chaine[LIGNE_CRC][i-1]=='E' && chaine[LIGNE_CRC][i]=='S')
			{
				check_frame = 1;			
			}
		}
		if (check_frame == 1)
		{
			i=1;
			found = 0;
			while((chaine[LIGNE_TEMPER][i]!='\0') && (found == 0))
			{	
				i++;
				if(chaine[LIGNE_TEMPER][i-1]=='t' && chaine[LIGNE_TEMPER][i]=='=')
				{
					found = 1;
					temp_int = 		(chaine[LIGNE_TEMPER][i+1] - 0x30) * 10000
								+	(chaine[LIGNE_TEMPER][i+2] - 0x30) * 1000	
								+	(chaine[LIGNE_TEMPER][i+3] - 0x30) * 100	
								+	(chaine[LIGNE_TEMPER][i+4] - 0x30) * 10	
								+	(chaine[LIGNE_TEMPER][i+5] - 0x30) * 1	;				
				}
			}
			if(found == 1)
			{
				fclose(file_sonde); 
				*temperature = (float)temp_int / 1000.0 ;
				//printf("Temperature for %s : %f\r\n",filename,*temperature);
				ret = RET_OK;
			}else
			{
				printf("Temperature not found\n");
				ret = RET_ERR;
			}
		}else
		{
			printf("CRC not ok\n");
			ret = RET_ERR;
		}
	}else
	{
		printf("No temp file found ok\n");
		ret = RET_ERR;
	}
	
	return ret;
}

int GetConsigneFromFile(char* filename,char** table_bin,float * consigne_jour,float * consigne_absolu)
{
	int ret = RET_ERR;
	char Table[j_jour_size][TAILLE_MAX] ; // Chaîne vide de taille TAILLE_MAX
	char Ligne1[TAILLE_MAX] ; // Chaîne vide de taille TAILLE_MAX
	char Ligne2[TAILLE_MAX] ; // Chaîne vide de taille TAILLE_MAX
	e_jour index;
	char * pch;
	FILE* fichier = NULL;
	char temp[TAILLE_MAX];
	int temp1,temp2,i;
	
	fichier = fopen(filename, "r");
    if (fichier != NULL)
    {
		
		fgets(Ligne1, TAILLE_MAX, fichier);
		fgets(Ligne2, TAILLE_MAX, fichier);
		for(index = j_Lundi; index<j_jour_size;index++)
		{
			fgets(Table[index], TAILLE_MAX, fichier);
		}
        fclose(fichier);
		
		pch=strchr(Ligne1,';');
		temp1 = pch - Ligne1;
		pch=strchr(&Ligne1[temp1+1],';');
		temp2 = pch - Ligne1;
		
		pch=strchr(&Ligne1[temp2+1],';');
		temp2 = pch - Ligne1;
		for(i=0;i<(temp2-temp1);i++)
		{
			temp[i] = Ligne1[i+temp1+1];
		}
		temp[i] = 0;
		sscanf (temp,"%f;%f",consigne_jour,consigne_absolu);
		
		for(index = j_Lundi; index<j_jour_size;index++)
		{
			pch=strchr(Table[index],';');
			temp1 = pch - Table[index] +1;
			for(i=0;i<48;i++)
			{
				
				if(Table[index][temp1 + i*2] == 'O')
				{
					table_bin[index][i] = 1;
				}else
				{
					table_bin[index][i] = 0;
				}
			}
		}
		ret = RET_OK;
    }
	
	return ret;
	
}

int GetDataFromNOAA(char* filename,s_noaa* datastruct)
{
	int ret = RET_ERR;
	te_info index;
	int tab_ligne[te_info_size] ;
	char *tab_TAGS[te_info_size] = {TAG_VENT,TAG_VISIBILITE,TAG_SKYCOND,TAG_WEATHER,TAG_TEMPER,TAG_DEWPOINT,TAG_HUMIDIE,TAG_PRESSION};
	int i,j,offset,nb_lignes_lues;
	char found = TRUE;
	char fichier_txt[LIGNE_MAX][TAILLE_MAX] = {{0}};
	
	
	
	FILE* file_info = NULL;
    
	
	for(index = info_wind;index<te_info_size;index++)
	{
		tab_ligne[index] = -1;
	}
	
	file_info = fopen(FILE_INFO, "r");

	if (file_info != NULL)
	{
		i = 0;
		while ((fgets(fichier_txt[i], TAILLE_MAX, file_info) != NULL) && (i < LIGNE_MAX))
		{
			i++;
		}
		nb_lignes_lues = i;


        //récuperation de la date et heure
        offset = -1;
        for(i=0;i<TAILLE_MAX;i++)
        {
            if (fichier_txt[LIGNE_DATE][i] == '/')
            {
                offset = i+ 2;
                i = TAILLE_MAX+1;
            }
        }
        if(offset!=-1)
        {
            sscanf (&(fichier_txt[LIGNE_DATE][offset]),"%s %s",datastruct->date,datastruct->heure);
        }
		
		for(j=2;j<nb_lignes_lues;j++)
		{
			//identification de chaque entete de ligne
			for(index = info_wind;index<te_info_size;index++)
			{
				found = TRUE;
				//test de chaque TAG
				//sprintf(string_tmp,"%s",tab_TAGS[index]);
				for(i=0;i<sizeof(tab_TAGS[index])-1;i++)
				{
					if(fichier_txt[j][i] == tab_TAGS[index][i])
					{

					}else
					{
						found = FALSE;
						break;
					}
				}
				
				if(found == TRUE)
				{
					tab_ligne[index] = j;
					break;
				}
			}
			
		}

        //recuperation vent
		if(tab_ligne[info_wind] != -1)
		{
			offset = -1;
			for(i=0;i<TAILLE_MAX;i++)
			{
				if ((fichier_txt[tab_ligne[info_wind]][i] == 'a')&&(fichier_txt[tab_ligne[info_wind]][i+1] == 't'))
				{
					offset = i+ 2;
					i = TAILLE_MAX+1;
				}
			}
			if(offset!=-1)
			{
				sscanf (&(fichier_txt[tab_ligne[info_wind]][offset]),"%d",&(datastruct->vent));
			}
			datastruct->vent = (int)((float)datastruct->vent*_1_MPH);
		}

        //recuperation temperature
		if(tab_ligne[info_temperature] != -1)
		{
			offset = -1;
			for(i=0;i<TAILLE_MAX;i++)
			{
				if ((fichier_txt[tab_ligne[info_temperature]][i] == 'F')&&(fichier_txt[tab_ligne[info_temperature]][i+1] == ' ')&&(fichier_txt[tab_ligne[info_temperature]][i+2] == '('))
				{
					offset = i+ 3;
					i = TAILLE_MAX+1;
				}
			}
			if(offset!=-1)
			{
				sscanf (&(fichier_txt[tab_ligne[info_temperature]][offset]),"%d",&(datastruct->temperature));
			}
		}

        //recuperation humidity
		if(tab_ligne[info_humidity] != -1)
		{
			offset = -1;
			for(i=0;i<TAILLE_MAX;i++)
			{
				if ((fichier_txt[tab_ligne[info_humidity]][i] == ':'))
				{
					offset = i+ 1;
					i = TAILLE_MAX+1;
				}
			}
			if(offset!=-1)
			{
				sscanf (&(fichier_txt[tab_ligne[info_humidity]][offset]),"%d",&(datastruct->humidite));
			}
		}

		ret = RET_OK;
		fclose(file_info);
	}
	
	
	return ret;
	
}