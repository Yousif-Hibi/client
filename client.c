
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <netdb.h>
#define _OPEN_SYS_ITOA_EXT

void errorHandling(char*argv[],int argc);
int main(int argc, char *argv[]) {
   
    char **str = (char **) malloc(argc * sizeof(char *));
    for (int i = 0; i < argc; ++i) {
        str[i] = (char *) malloc(strlen(argv[i]) * sizeof(char));
        strcpy(str[i], argv[i]);
    }

    char *urlStr, **reqStr, *pstr, *hostStr,*reqstr2;
    int port = 80, boolPost = 0,reqSize=0,reqSizeF=0,Psize=0;
    int slashcounter = 0;
    for (int i = 1; i < argc; ++i) {
        if (str[i][0] != '-' && str[i][0] != 'h') {
            errorHandling(str, argc);
        }
        for (int j = 0; j < strlen(str[i]); ++j) {
            int cbool = 0, counter = 0;
            char c = str[i][j];
            int x;
            if (c == '-') {
                cbool = 1;
                j++;
                c = str[i][j];
                int checkIfNum = 0;
                if (c == 'r') {
                    i++;
                    j = 0;
                    c = str[i][0];
                    for (int k = 0; k < strlen(str[i]); ++k) {
                        if (!isdigit(str[i][k])) { checkIfNum = -1; }
                    }
                    if (checkIfNum == 0) {
                        x = atoi(str[i]);
                        reqStr = (char **) malloc(x * sizeof(char*));
                        reqSize=x;
                        for (int k = 1; k <= x; ++k) {
                            if (str[i + k] != NULL) {
                                const char equal = '=';
                                if (strstr(str[i + k], "=")) {
                                    reqSizeF=reqSizeF+strlen(str[i+k]);
                                    reqStr[k-1]=(char*) malloc(strlen(str[i+k])*sizeof(char));
                                    strcpy(reqStr[k-1], str[i + k]);
                                } else {
                                    errorHandling(argv, argc);
                                }
                            } else {
                                errorHandling(argv, argc);
                            }
                            if (k == x || (k == 1 && x == 1)) {
                                if (str[i + k + 1] != NULL) {
                                    if (strstr(str[i + k + 1], "=")) {
                                        errorHandling(argv, argc);
                                    }
                                }
                                i = i + x;
                                break;
                            }
                        }

                    } else {
                        errorHandling(argv, argc);
                    }
                } else if (c == 'p') {
                    boolPost = 1;
                    i++;
                    j = 0;
                    c = str[i][0];//check all the digits not only the first one
                    for (int k = 0; k < strlen(str[i]); ++k) {
                        if (!isdigit(str[i][k])) { checkIfNum = -1; }
                    }

                    if (checkIfNum == 0) {
                        x = atoi(str[i]);
                        if (x==0){
                            errorHandling(argv,argc);
                        }
                        Psize=x;
                        pstr = (char *) malloc(strlen(str[i]) * sizeof(str[i]));
                        i++;
                        if (strlen(str[i]) == x) {
                            strcpy(pstr, str[i]);
                        } else {
                            errorHandling(argv, argc);
                        }
                    } else {
                        errorHandling(argv, argc);
                    }
                } else {
                    errorHandling(argv, argc);
                }
            } else if (c == 'h') {
                char *s = "http://";
                int checkIfNum = 0;
                char num[5];
                if (strlen(str[i]) < 7) {
                    errorHandling(argv, argc);
                }
                if (strncmp(s, str[i], 7) != 0) {
                    errorHandling(argv, argc);
                }

                for (int k = 7; k < strlen(str[i]); k++) {

                    if (slashcounter == 0) {
                        hostStr = (char *) malloc((strlen(str[i])) * sizeof(char));
                        for (int l = 0; l < (strlen(str[i])); l++) {
                            if(k+l== strlen(str[i])){
                                urlStr="";
                                slashcounter=2;
                                break;
                            }
                            if(str[i][k + l] == ':' || str[i][k + l] == '/') {
                                slashcounter = 1;
                                break;
                            }
                            hostStr[l] = str[i][k + l];
                        }
                    }
                    if (str[i][k] == ':') {
                        k++;
                        //////65536
                        for (int l = 0; l < 5; l++) {
                            if (str[i][k + l] == '/'||str[i][k+l+1]=='\0') {
                                break;
                            }
                            if (str[i][k + l] != '\0') {
                                if (!isdigit(str[i][k + l])) { checkIfNum = -1; }
                                num[l] = str[i][k + l];
                            }
                        }
                        if (isdigit(str[i][k + 5])) { checkIfNum = -1; }
                        if (checkIfNum == 0) {
                            int x = atoi(num);
                            if (x < 65536 && x > 0) {
                                port = x;
                            } else {
                                errorHandling(argv, argc);
                            }
                        } else {
                            errorHandling(argv, argc);
                        }
                    }
                    if (str[i][k] == '/') {
                        slashcounter == 1;
                        urlStr = (char *) malloc((strlen(str[i])) * sizeof(char));
                        for (int l = 0; l < (strlen(str[i]) - k); l++) {
                            urlStr[l] = str[i][k + l];
                        }
                        break;
                    }
                }
            }
        }
    }
    int reqstr2Size;
if(boolPost==1){
    char str[10];
    sprintf(str, "%d",Psize);
   reqstr2=(char *) malloc(sizeof(char)*(reqSizeF+reqSize+100));
    strcat(reqstr2,"POST ");
    if(slashcounter==2){
        strcat(reqstr2,"/");
    }
    strcat(reqstr2,urlStr);
    if(reqSize>0) {
        strcat(reqstr2,"?");
        for (int i = 0; i <reqSize ; ++i) {
            strcat(reqstr2,reqStr[i]);
            if(i+1!=reqSize) {
                strcat(reqstr2, "&");
            }
        }
    }
    strcat(reqstr2," HTTP/1.0 ");
    strcat(reqstr2,"\r\nHost: ");
    strcat(reqstr2,hostStr);
    strcat(reqstr2,"\r\nContent-Length: ");
    strcat(reqstr2,str);
    strcat(reqstr2,"\r\n\r\n");
    reqstr2Size= strlen(reqstr2);
    printf("HTTP request=\n%s\nLEN=%d\n",reqstr2,reqstr2Size );
} else{

    reqstr2=(char *) malloc(sizeof(char)*(reqSizeF+6+ strlen(urlStr)+reqSize+50));
    strcat(reqstr2,"GET ");
    if(slashcounter==2){
        strcat(reqstr2,"/");
    }
    strcat(reqstr2,urlStr);

    if(reqSize>0) {
          strcat(reqstr2,"?");
         for (int i = 0; i <reqSize ; ++i) {
             strcat(reqstr2,reqStr[i]);
             if(i+1!=reqSize) {
                 strcat(reqstr2, "&");
              }
          }
    }
    strcat(reqstr2," HTTP/1.0 \r\nHost: ");
    strcat(reqstr2,hostStr);
   strcat(reqstr2,"\r\n\r\n");
     reqstr2Size= strlen(reqstr2);
    printf("HTTP request=\n%s\nLEN=%d\n",reqstr2,reqstr2Size );
}
int sock;
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
     struct hostent *server;
    struct sockaddr_in rAddress;
    server = gethostbyname(hostStr);
    if(server==NULL){
        errorHandling(argv,argc);
    }
    rAddress.sin_addr.s_addr = ((struct in_addr*)(server->h_addr))->s_addr;
    rAddress.sin_family = PF_INET;
    rAddress.sin_port = htons(port);
    char response[100000];
    if (connect(sock, (struct sockaddr *) &rAddress, sizeof(rAddress)) < 0){
        exit(-1);
}
    send(sock,reqstr2, reqstr2Size,0);
 int resSize=  recv(sock,&response, 4096,0);
    printf(" %s\n",response);
    printf("\n Total received response bytes: %d\n",resSize);
    close(sock);

   free(str) ;
    for (int i = 0; i < argc; ++i) {
        free(str[i]);
    }
    free(reqstr2);
    free(urlStr);
    free(hostStr);
    free(pstr);
return 0;
}
void errorHandling(char*argv[],int argc){
    char errormsg[5000]="";
    for (int i = 1; i < argc; ++i) {
        strcat(errormsg, argv[i]);
        strcat(errormsg, " ");
    }
    printf("Usage: client %s \n", errormsg);
    exit(-1);
}








