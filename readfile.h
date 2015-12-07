int GetConsigneFromFile(char* filename,char** table_bin,float * consigne_jour,float * consigne_absolu);
int GetTemperatureFromProbe(char* filename,float* temperature);
int GetDataFromNOAA(char* filename,s_noaa* datastruct);