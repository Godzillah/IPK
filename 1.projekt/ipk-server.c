
#include "client_server.h"


// deklaracia premmenych

void spracuj_parametre_server(int argc , char** argv);
void help();
bool startsWith(const char *pre, const char *str);


int main(int argc , char** argv){

	spracuj_parametre_server(argc,argv);
	
	// deklarovanie premmenych s ktorymi budeme pracovat
	int server_socket , client_socket; 								// sockety 
	struct sockaddr_in server , client; 							// informacie o mojej adrese a taktiez aj adrese pripajajuceho
	int addrSize;
	char sprava[10000];

		// VYTVARANIE SOCKETU
	// --------------------------------------------------------
	server_socket = socket(AF_INET , SOCK_STREAM , 0);
	if (server_socket < 0){
		perror("Error: creating socket...");
		exit(EXIT_FAILURE);
	} 
	

	memset(&server , '\0' , sizeof(server));  						// vycistenie pamate..(resp. vynulovanie)

	server.sin_family = AF_INET;									//pridelenie protokolu
	server.sin_addr.s_addr = htonl(INADDR_ANY); 					// pridelenie IP adresy | merlin == 103.224.182.250
    server.sin_port = htons(atoi(argv[2]));  						// atoi = string -> integer 
	
		// BINDOVANIE SOCKETU 
    // --------------------------------------------------------
    int bind_value =  bind(server_socket, (struct sockaddr*) &server, sizeof (struct sockaddr));
	
	if(bind_value < 0){
		perror("Error: binding socket...\n");
        	exit(EXIT_FAILURE);
    	}
    	
	
		// NASLOUCHA
    // --------------------------------------------------------
	if ((listen(server_socket, 1)) < 0){
		perror("ERROR: listening ");
		exit(EXIT_FAILURE);
	}

	addrSize = sizeof(client);									// ziskanie si velkosti adresy
	
	memset(sprava , '\0' , sizeof(sprava));
	Here:														// navestie aby nedoslo k uviaznutiu
		// LOOP
	while(1)
	{
		memset(sprava , '\0' , sizeof(sprava));
			// POTVRDZOVANIE 
		// --------------------------------------------------------
		client_socket = accept(server_socket,
				  (struct sockaddr*) &client, (socklen_t*) &addrSize);
		if (client_socket ==  -1){
			perror("ERROR: accepting");
			exit(EXIT_FAILURE);
		}

		int bytes_recv = 0;
		int bytes_send = 0;

		memset(sprava , '\0' , sizeof(sprava));
		
			// 	ZISKAVAM SOCKET 
		// -------------------------------------------------------
		bytes_recv = recv(client_socket , sprava , sizeof(sprava) , 0);
		if( bytes_recv < 0){
			perror("ERROR: receiving");
			break;
		}

		FILE *fp;						// file
		char *ptr; 						// pomocna
		int status = 0;					
			// ARGUMENT -f
		// ----------------------------------------------------------
		if(strcmp(sprava , "-f") == 0){
				// POSIELANIE SOCKETU
			// -------------------------------------------------------
			send(client_socket , sprava , strlen(sprava)+1 , 0);
				// ZISKAVAM SOCKET 
			// -------------------------------------------------------
			recv(client_socket , sprava , sizeof(sprava) , 0);
			struct passwd *pw = getpwnam(sprava);  						// pre meno daneho uzivatela
	 		if(pw == NULL){												// ak uzivatel neexistuje vraciam ERROR
	 			perror("ERROR : wrong username");
	 			memset(sprava , '\0' , sizeof(sprava));
	 			strcpy(sprava , "\b");
	 				// 	POSIELANIE SOCKETU 
				// -------------------------------------------------------					
	 			send(client_socket , sprava , strlen(sprava)+1 , 0);	
	 			goto Here;
	 		}
			const char *homedir = pw->pw_dir;						 	// najdenie home_dir pre daneho uzivatela
		
			strcpy(sprava , homedir); 										// kopirovanie cesty 
				// POSIELANIE SOCKETU
			// -------------------------------------------------------			
			send(client_socket , sprava , strlen(sprava)+1 , 0);				// zasielanie cesty clientovi
		}
			// ARGUMENT -n
		// ----------------------------------------------------------
		else if(strcmp(sprava , "-n") == 0){
				// POSIELANIE SOCKETU
			// -------------------------------------------------------
			send(client_socket , sprava , strlen(sprava)+1 , 0);
				// ZISKAVAM SOCKET
			// -------------------------------------------------------
			recv(client_socket , sprava , sizeof(sprava) , 0);
				struct passwd *pw = getpwnam(sprava); 						 // pre meno daneho uzivatela
	 		if(pw == NULL){												// ak uzivatel neexistuje vraciam ERROR
	 			perror("ERROR : wrong username");
	 			memset(sprava , '\0' , sizeof(sprava));
	 			strcpy(sprava , "\b");
	 				// POSIELANIE SOCKETU
				// -------------------------------------------------------
	 			send(client_socket , sprava , strlen(sprava)+1 , 0);
	 			goto Here;												// navestie aby nedoslo k uviaznutiu
	 		}
			const char *user_information = pw->pw_gecos;				// najdenie si podrobnych informacii o uzivatelovi ak existuje
	
			strcpy(sprava , user_information); 							// kopirovanie cesty 
				// POSIELANIE SOCKETU
			// -------------------------------------------------------
			send(client_socket , sprava , strlen(sprava)+1 , 0);				// zasielanie podrobnych informacii o uzivateli
		}
			// ARGUMENT -l
		// ----------------------------------------------------------	
		else if(strcmp(sprava, "-l") == 0){
			fp = fopen("/etc/passwd" , "r");
			if (fp == NULL){
				perror("Error: creating file");
			}
			while(fgets(sprava , sizeof(sprava) , fp) != NULL){				// citanie po riadkoch
				ptr = strchr(sprava , ':');								// ak narazi na znak :
				if(ptr != NULL){										
					*ptr = '\0';
				}
					// POSIELANIE SOCKETU
				// -------------------------------------------------------
				bytes_send = send(client_socket, sprava, strlen(sprava)+1, 0);
				memset(sprava , '\0' , sizeof(sprava));						// cistenie pamati

			}
			if( fgets(sprava , sizeof(sprava) , fp) == NULL){
				status = fclose(fp);
				if (status == -1)
					perror("Error: closing file");	
				strcpy(sprava,"STOP");
					// POSIELANIE SOCKETU
				// -------------------------------------------------------
				bytes_send = send(client_socket, sprava, strlen(sprava)+1, 0);
				memset(sprava , '\0' , sizeof(sprava));						// cistenie pamati
			}
		}	

		// ARGUMENT -l login
		// ----------------------------------------------------------
		else {		
			fp = fopen("/etc/passwd" , "r");
			if (fp == NULL){
				perror("Error: creating file");
			}
			char prefix[25];											
			int i = 0;
			for (i = 0 ; i < strlen(prefix)+1 ; i++){
				prefix[i] = sprava[i];
			}
			while(fgets(sprava , sizeof(sprava) , fp) != NULL){				// citanie po riadkoch
				ptr = strchr(sprava , ':');								// ak narazi na znak :
				if(ptr != NULL){
					*ptr = '\0';
				}
				if(startsWith(prefix,sprava)){								// pouzita funckia prefix
						// POSIELANIE SOCKETU
					// -------------------------------------------------------
					bytes_send = send(client_socket, sprava, strlen(sprava)+1, 0);
					if(bytes_send < 0){									// kontrola posielanie packetov
						perror("ERROR: sending");
						break;
					}
					memset(sprava , '\0' , sizeof(sprava));
				}
				memset(sprava , '\0' , sizeof(sprava));
			}

			if( fgets(sprava , sizeof(sprava) , fp) == NULL){
				status = fclose(fp);
				if (status == -1)
					perror("Error: closing file");	
					strcpy(sprava,"STOP");
						// POSIELANIE SOCKETU
					// -------------------------------------------------------
					bytes_send = send(client_socket, sprava, strlen(sprava)+1, 0);
					memset(sprava , '\0' , sizeof(sprava));
			}	
			
		}	 // zatvorka od WHILE
		memset(sprava , '\0' , sizeof(sprava));					
	}	
	//return EXIT_SUCCESS;
}

/**
  * @brief Funckia ktora ma za ulohu spravne parsovat uzivatelové vstupy
  * @par argc pocet argumentov
  * @par argv pole poli  
  */

void spracuj_parametre_server(int argc , char** argv){


	// osetrenie viacerych argumentov.... argv > 3 && argc < 3 
	if(argc != 3 && argc != 2){
		printf("Zadal si nespravny pocet argumentov pre zobrazenie napovedy zadaj %s --help\n" , argv[0]);
		exit(EXIT_FAILURE);	
	
		
	}else if(argc == 2){
		if(strcmp(argv[1] , "--help") == 0){
			help(); // zavola napovedu
			exit(EXIT_FAILURE);
		}	
		else{
			help();
			exit(EXIT_FAILURE); // v pripade ak budu dva argumenty a argv[1] bude nieco ine ako --help
		}
	}
	// inak agrc == 3 len v pripade ./ipk-server -p port
	else{
		if(argc == 3 && (strcmp(argv[1] , "-p") == 0) ){
			if(!(((atoi(argv[2])) > 1023)  && ((atoi(argv[2])) < 65536))){
			perror("Zadal si nepovoleny port!\n");
			exit(EXIT_FAILURE);
			}
		}
		char* stringNumber = argv[2]; 
		if((strcmp(argv[1] , "-p") == 0) && strcmp(argv[2] ,  stringNumber) == 0){
		}
		else{
			exit(EXIT_FAILURE);
		}
		
	}
	
}

/**
  * @brief Funckia ktora pri zadani jedneho argumentu teda ./ipk-client ukaze napovedu uzivatelovi
  * @par argc pocet argumentov
  * @par argv pole poli 
  * @pre argc == 1
  */

void help(){
	
	printf(	"Pre spravne zadanie argumentov: ./ipk-server -p port\n"
	       	"./ipk-server = nazov programu\n"
	       	"-p           = prepinac na zadanie portu\n"
		"port         = celociselna hodnota  portu\n"); 
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
