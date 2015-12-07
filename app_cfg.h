#define THERMO_PIN 		4
#define RET_ERR 		-1
#define RET_OK 			1
#define FILE_SONDE1		"/sys/bus/w1/devices/28-0000053072af/w1_slave"
#define FILE_CONSIGNE 	"consigne.csv"
#define FILE_INFO		"LFOP.TXT"
#define TAILLE_MAX 		1000
#define LIGNE_MAX 		1000
#define LIGNE_CRC 		0
#define LIGNE_TEMPER 	1



#define LIGNE_LIEU 		0
#define LIGNE_DATE 		1
#define LIGNE_VENT 		2
#define LIGNE_VISIBILITE    3

#define LIGNE_TEMP_1 		4
#define LIGNE_DEWPOINT_1 	5
#define LIGNE_HUMIDITE_1 	6
#define LIGNE_PRESSION_1 	7

#define LIGNE_SKYCOND_1	    4
#define LIGNE_TEMP_2 		5
#define LIGNE_DEWPOINT_2 	6
#define LIGNE_HUMIDITE_2    7
#define LIGNE_PRESSION_2 	8

#define LIGNE_SKYCOND_2 	4
#define LIGNE_WEATHER_1 	5
#define LIGNE_TEMP_3 		6
#define LIGNE_DEWPOINT_3 	7
#define LIGNE_HUMIDITE_3    8
#define LIGNE_PRESSION_3 	9

#define TAG_VENT 		"Wind"
#define TAG_VISIBILITE 	"Visibility"
#define TAG_SKYCOND 	"Sky conditions"
#define TAG_WEATHER		"Weather"
#define TAG_TEMPER 		"Temperature"
#define TAG_DEWPOINT 	"Dew Point"
#define TAG_HUMIDIE 	"Relative Humidity"
#define TAG_PRESSION 	"Pressure"

#define TAG_COUVERT     "overcast"
#define TAG_ASSEZ_NUAGEUX     "mostly cloudy"
/*Sunny
Clear
Mostly Sunny
Mostly Clear
Partly Sunny
Partly Cloudy
Mostly Cloudy
Cloudy*/

#define _1_MPH          1.609344
#define MARCHE			"allume"
#define MARCHE_FORCE	"allume (F)"
#define ETEINT			"eteint"
#define INCONNU			"inconnu"
#define THERMO_PIN 		4
#define LIGNE_CRC 		0
#define LIGNE_TEMPER 	1

#define TRUE 1
#define FALSE 0

#define RAD_OFF			0
#define RAD_ON			1
#define RAD_ON_FORCED	2

typedef enum{
	j_Lundi =0,
	j_Mardi,
	j_Mercredi,
	j_Jeudi,
	j_Vendredi,
	j_Samedi,
	j_Dimanche,
	j_jour_size} e_jour;
	
typedef enum {
	info_wind=0,
	info_visibility,
	info_sky,
	info_weather,
	info_temperature,
	info_dewpoint,
	info_humidity,
	info_pressure,
	te_info_size} te_info;
	
typedef struct {
	char date[TAILLE_MAX];
	char heure[TAILLE_MAX];
	int vent;
	int temperature;
	int humidite;
} s_noaa;