#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
static unsigned char passwd_hash[SHA256_DIGEST_LENGTH]; 
static FILE * FILE_PASSWORD;
int verify_root();
void test_fortify();

int main(int argc, char * argv[]) {

    // pour tester FORTIFY
    test_fortify();


	if (argc < 2){
		printf("Error, missing argument");
		return 1;
	}

    if (verify_root() != 0) {
        printf("Error, you must be root to run this program");
        return 1;
    }

    // Lire le hash dans le fichier FILE_PASSWORD
    FILE_PASSWORD = fopen("passwd.txt", "r");
    if (FILE_PASSWORD == NULL) {
        printf("Error, missing password file");
        return 1;
    }
    // recopier le hash dans passwd_hash en traitant les valeurs hexa avec le séparateur ','
    char ligne[1024];
    size_t nlus = fread(ligne, 1, sizeof(ligne), FILE_PASSWORD);
    printf("Nombre de caractères lus : %zu\n", nlus);

    //ligne[strcspn(ligne, "\n")] = 0; // Supprimer le saut de ligne si présent
    char *token = strtok(ligne, "\n");
    char * strToken = strtok ( token,", " );
    int i = 0;
    while ( strToken != NULL ) {
        int result = sscanf(strToken, "0x%hhx", &passwd_hash[i]);
        if (result > 0) {
            // printf("%s\n", strToken);
            // printf("%02x\n", passwd_hash[i]);
            i++;
        } 

       // On demande le token suivant.
        strToken = strtok ( NULL, ", " );
    }

    // char *strtoken = NULL;
    // int i = 0;
    // // while (token != NULL && i < (int)nlus) {
    // while (token != NULL) {
    //     int result = sscanf(token, "0x%hhx", &passwd_hash[i]);
    //     printf("%d\n", result);
    //     if (result > 0) {
    //         printf("%s\n", token);
    //         printf("%02x\n", passwd_hash[i]);
    //         i++;
    //     } 
    //     char *strtoken = strtok(token, ", ");
    //     while (strtoken != NULL) {
    //         printf("%s\n", strtoken);
    //         strtoken = strtok(NULL, ", ");
    //     }


    // }
   

	unsigned char buffer[SHA256_DIGEST_LENGTH];
	
	const size_t  sz = strnlen(argv[1], 40);
	unsigned char in[40];
	memset(in, 0, 40);
	memcpy(in, argv[1], sz);

	printf("You entered \"%s\".\n", argv[1]);
SHA256((unsigned char *) in, sz, buffer);

#ifndef NDEBUG
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		printf("%02x", buffer[i]);
	printf("\n");
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		printf("%02x", passwd_hash[i]);
#endif
	int result = 0;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		result |= buffer[i] != passwd_hash[i];
	printf("\nRESULT = %d\n", result);

#ifndef NDEBUG
	printf("\nRESULT = %d\n", result);
#endif

	return result;
}


int verify_root() {
    char foo[7]= "123456";

    printf("%s\n", foo);
    if (getuid() != 0) {
        return 1;
    }
    return 0;
}

void test_fortify() {
    char dest[10];
    strcpy(dest, "This is a long string that will overflow");
    printf("Destination: %s\n", dest);
}