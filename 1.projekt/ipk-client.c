
#include "client_server.h"

	// deklaracie funkcii

void help();
void help_concrete(int argc , char* argv[]);
void spracuj_parametre_client(int argc , char** argv);
int isDigit(int argc , char *argv[]);
bool startsWith(const char *pre , const char *str);
int hostname_to_ip(char * hostname , char* ip);

	// ---------- MAIN ---------- 

int main(int argc , char** argv){

	// spracovanie argumentov
	spracuj_parametre_client(argc,argv);
	
		// DEKLARACIA PREMENNYCH
	int client_socket;
	struct sockaddr_in client, server;	
	struct in_addr **addr_list;
	struct hostent *host_adrress;
	char ip[50];
 	


		// VYTVARANIE SOCKETU
	// --------------------------------------------------------
	client_socket = socket(AF_INET , SOCK_STREAM , 0);
	if (client_socket  < 0){
		perror("ERROR: creating socket...");
		exit(EXIT_FAILURE);
	}	

		// GETHOSTBYNAME
	// --------------------------------------------------------

	host_adrress = gethostbyname(argv[2]);
	if (host_adrress == NULL){
		perror("ERROR: gethostbyaddr");
		exit(EXIT_FAILURE);
	}

 		// TOTO PREMIENMA DNS -> IP adresu pomocou addr_listu.... a za pomoci hostent struktury
 	addr_list = (struct in_addr **) host_adrress->h_addr_list;
	 
	 int i;	

 	for(i = 0; addr_list[i] != NULL; i++) 
    {
      
        strcpy(ip , inet_ntoa(*addr_list[i]) );
    }

	// vyplnenie adresnej struktury...

	server.sin_addr.s_addr = inet_addr(ip); 				// pre local = "127.0.0.1"
	server.sin_family = AF_INET;							// pridelenie protokolu
	server.sin_port = htons(atoi(argv[4]));					//host to network short

	memset(&(client.sin_zero), 0, 8);						// vycistenie pamate )) nastavanie na 0

 	
 		// PRIPAJANIE SA NA SERVER
	
	int connect_value = connect(client_socket,(const struct sockaddr *) &server, sizeof(server));
	if(connect_value != 0){
		perror("ERROR: connect");
		exit(EXIT_FAILURE);
	}
	
	char sprava[10000];										//pole na zachytenie spravy ....
	int bytes_recv , bytes_send = 0;
	int status;


	memset(sprava , '\0' , sizeof(sprava));						// vycistenie pamate )) nastavanie na 0

	char *ptr; 												// pomocna

	if((strcmp(argv[5] , "-l") == 0) && ( argc == 6)){
		strcpy(sprava , "-l\0");   							// posielam -f 
			// POSIELANIE SOCKETU
		// -------------------------------------------------------
		bytes_send = send(client_socket, sprava, strlen(sprava)+1, 0);
		memset(sprava , '\0' , sizeof(sprava));

		while(1){
				// 	ZISKAVAM SOCKET 
			// -------------------------------------------------------
			bytes_recv = recv(client_socket , sprava , sizeof(sprava) , 0);
			if( bytes_recv < 0){
				perror("ERROR: receiving");
				break;
			}
			if(strcmp(sprava, "STOP") == 0){
				exit(1);				// koniec subora
			}
			printf("%s\n", sprava);
			memset(sprava , '\0' , sizeof(sprava));
			// POSIELANIE SOCKETU
				// -------------------------------------------------------
			bytes_send = send(client_socket , sprava ,strlen(sprava)+1 , 0);
			if(bytes_send < 0){
				perror("ERROR: sending");
				break;
			}
		}
		bytes_recv =recv(client_socket , sprava , sizeof(sprava) , 0);
		if( bytes_recv < 0){
				perror("ERROR: receiving");	
			}

		close(client_socket);
		
	}
	else if((strcmp(argv[5] , "-l") == 0) && ( argc == 7)){
		memset(sprava ,'\0', sizeof(sprava));
		strcpy(sprava , argv[6]);
			// POSIELANIE SOCKETU
		// -------------------------------------------------------
		bytes_send = send(client_socket, sprava, strlen(sprava)+1, 0);
		if(bytes_send < 0){
			perror("ERROR: sending");
		}

		while(1){
				// 	ZISKAVAM SOCKET 
			// -------------------------------------------------------
			bytes_recv = recv(client_socket , sprava , sizeof(sprava) , 0);
			if( bytes_recv < 0){
				perror("ERROR: receiving");
				break;
			}
			if(strcmp(sprava, "STOP") == 0){
				exit(EXIT_FAILURE);				// koniec subora
			}
			printf("%s\n", sprava);
			memset(sprava , '\0' , sizeof(sprava));
				// POSIELANIE SOCKETU
			// -------------------------------------------------------
			send(client_socket, sprava, strlen(sprava)+1, 0);
		}
			// 	ZISKAVAM SOCKET 
		// -------------------------------------------------------
		recv(client_socket , sprava , sizeof(sprava) , 0);
		if( bytes_recv < 0)
				perror("ERROR: receiving");
			// ZATVARAM SOCKET 
		// -------------------------------------------------------
		close(client_socket);
	
	}
	else if (strcmp(argv[5] , "-n") == 0){
		strcpy(sprava , "-n\0");   // posielam -f 
			// POSIELANIE SOCKETU
		// -------------------------------------------------------
		bytes_recv = send(client_socket, sprava, strlen(sprava), 0);	
		if (argc == 5){
			perror("You did not write login");
			exit(EXIT_FAILURE);
		}
		
			// ZISKAVAM SOCKET 
		// -------------------------------------------------------
		recv(client_socket , sprava , sizeof(sprava) , 0);
		memset(sprava , '\0' , sizeof(sprava));
		
		// ---------------------- sendujem meno...... 
		strcpy(sprava , argv[6]);	
			// POSIELANIE SOCKETU
		// -------------------------------------------------------
		send(client_socket, sprava, strlen(sprava), 0);

			// ZISKAVAM SOCKET 
		// -------------------------------------------------------
		recv(client_socket, sprava, sizeof(sprava), 0);
		if(strcmp(sprava , "")){}
		printf("%s\n",sprava);							// toto je co dostane client 
			// ZATVARAM SOCKET 
		// -------------------------------------------------------
		close(client_socket);		

	}
	else if(strcmp(argv[5] , "-f") == 0){
		strcpy(sprava , "-f\0");   // posielam -f 
			// POSIELANIE SOCKETU
		// -------------------------------------------------------
		bytes_recv = send(client_socket, sprava, strlen(sprava), 0);	
		if (argc == 5){
			perror("You did not write login");
			exit(EXIT_FAILURE);
		}
		
			// ZISKAVAM SOCKET 
		// -------------------------------------------------------
		recv(client_socket , sprava , sizeof(sprava) , 0);
		memset(sprava , '\0' , sizeof(sprava));
		
		strcpy(sprava , argv[6]);	
			// POSIELANIE SOCKETU
		// -------------------------------------------------------
		send(client_socket, sprava, strlen(sprava), 0);
		
			// ZISKAVAM SOCKET 
		// -------------------------------------------------------
		recv(client_socket, sprava, sizeof(sprava), 0);
		
		printf("%s\n",sprava);						// toto je co dostane client 
		
			// ZATVARAM SOCKET 
		// -------------------------------------------------------
		close(client_socket);	
	}
	else{
		perror("Error");	
		exit(EXIT_FAILURE);
	}


	return 0;

	
}

/**
  * @brief Funckia ktora pri zadani jedneho argumentu teda ./ipk-client ukaze napovedu uzivatelovi
  * @par argc pocet argumentov
  * @par argv pole poli 
  * @pre argc == 1
  */
void help(){

	printf("Zle zadane parametre(prepinac) pre konkrente vysvetlenie si vyberte z moznosti\n"
			"---> SPRAVNE ZADANIE ARGUMENTOV <--- ./ipk-client -h host -p port [-n|-f|-l] login\n"
			"---> MORE INFO <--- 		     ./ipk-client [-h|-p|-n|-f|-l]\n");
 }

/**
  * @brief Funckia ktora pri zadani dvoch argumentov ukaze konkretnejsiu napovedu o danych prepinacoch uzivatelovi
  * @par argc pocet argumentov
  * @par argv pole poli 
  */
void help_concrete(int argc , char* argv[]){

	if (argc == 2){
		
	 	if((strcmp(argv[1] , "-h") == 0)){
			printf("host  = (IP adresa nebo fully-qualified DNS name) identifikace serveru jakožto koncového bodu komunikace klienta\n");

		}
		else if((strcmp(argv[1] , "-p") == 0)){
			printf("port (číslo) =  cílové číslo portu\n");
		}
		else if((strcmp(argv[1] , "-n") == 0)){
			printf("-n značí, že bude vráceno plné jméno uživatele\n");
		}
		else if((strcmp(argv[1] , "-f") == 0)){
			printf("-f značí, že bude vrácena informace o domácím adresáři uživatele\n");
		}
		else if((strcmp(argv[1] , "-l") == 0)){
			printf("-l značí, že bude vrácen seznam všech uživatelů, tento bude vypsán tak,\n" 
	           	   "že každé uživatelské jméno bude na zvláštním řádku v tomto případě je  \n"
	           	   "login nepovinný. Je-li však uveden bude použit jako prefix pro výběr uži-\n"
			       "vatelů login určuje přihlašovací jméno uživatele pro výše uvedené operace.\n");
		}
		else{
			help();
		}
	}

}
/**
  * @brief Funckia ktora pri zadani dvoch argumentov ukaze konkretnejsiu napovedu o danych prepinacoch uzivatelovi
  * @par argc pocet argumentov
  * @par argv pole poli 
  */

void spracuj_parametre_client(int argc , char** argv){

	if (argc == 2){
		help_concrete( argc , argv);
		exit(EXIT_FAILURE);
	}
	else if (argc == 7 && (strcmp(argv[1] , "-h") == 0) && (strcmp(argv[3], "-p") == 0) && ((strcmp(argv[5] , "-n" ) == 0) || (strcmp(argv[5] , "-f") == 0) || (strcmp(argv[5] , "-l") == 0))){ 
			if(!(((atoi(argv[4])) > 1023)  && ((atoi(argv[4])) < 65536))){
				fprintf(stderr,"Zadal si nepovoleny port!\n");
			}
	}
	else if (argc == 6 && (strcmp(argv[1] , "-h") == 0) && (strcmp(argv[3], "-p") == 0) && ((strcmp(argv[5] , "-n" ) == 0) || (strcmp(argv[5] , "-f") == 0) || (strcmp(argv[5] , "-l") == 0))){		
		if(!(((atoi(argv[4])) > 1023)  && ((atoi(argv[4])) < 65536))){
				fprintf(stderr,"Zadal si nepovoleny port!\n");
			}
	}
	else{	// aj pre argc == 1 
		help();
		exit(EXIT_FAILURE);
	}
}

/**
  * @brief 	funckcia pomocou ktorej sa najde prefix v stringu ktory zadame
  * @par 	*pre prefix
  * @par  	*str string 
  */

bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}


