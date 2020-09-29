#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct virginia_cipher Virginia_Cipher;
typedef int (*Encrypt)(Virginia_Cipher *,const char *);
typedef int (*Decrypt)(Virginia_Cipher *,const char *);
typedef struct virginia_cipher{
    char *key;
    Encrypt encrypt;
    Decrypt decrypt;
}Virginia_Cipher;

int impl_encrypt(Virginia_Cipher *,const char *);
int impl_decrypt(Virginia_Cipher *,const char *);
int strclr(char *);

int New_Virginia_Cipher(Virginia_Cipher **self,char *key){
    if((*self = (Virginia_Cipher *)malloc(sizeof(Virginia_Cipher))) == NULL){
        printf("Fail...\n");
        return -1;
    }
    (*self)->key = key;
    (*self)->encrypt = impl_encrypt;
    (*self)->decrypt = impl_decrypt;
    return 0;
}
int impl_encrypt(Virginia_Cipher *self, const char *plaintext){
    char *ciphertext = (char *)malloc(sizeof(char) * strlen(plaintext));
    int i;
    char base;
    for(i=0;i<strlen(plaintext) - 1;i++){
        if(plaintext[i] <= 'Z' && plaintext[i] >= 'A') base = 'A';
        else if(plaintext[i] <= 'z' && plaintext[i] >= 'a') base = 'a';
        else {
            ciphertext[i] = plaintext[i];
            continue;
        }
        ciphertext[i] = (plaintext[i] - base + self->key[i % strlen(self->key)]) % 26 + base;
    }
    ciphertext[i] = '\0';
    printf("Cipher text is : %s\n",ciphertext);
    return 0;
}
int impl_decrypt(Virginia_Cipher *self, const char *ciphertext){
    char *plaintext = (char *)malloc(sizeof(char) * strlen(ciphertext));
    int i,temp = 0;
    char base;
    for(i=0;i<strlen(ciphertext) - 1;i++){
        if(ciphertext[i] <= 'Z' && ciphertext[i] >= 'A') base = 'A';
        else if(ciphertext[i] <= 'z' && ciphertext[i] >= 'a') base = 'a';
        else {
            plaintext[i] = ciphertext[i];
            continue;
        }
        for(;ciphertext[i] - base - self->key[i % strlen(self->key)] + temp < 0;temp += 26);
        plaintext[i] = (ciphertext[i] - base - self->key[i % strlen(self->key)] + temp) % 26 + base;
    }
    plaintext[i] = '\0';
    printf("Plain text is : %s\n",plaintext);
    return 0;
}
int strclr(char *str){
    for(int i = 0; i < 1024 ;i++){
        str[i] = '\0';
    }
}
int main(int argc, char *argv[]){
    Virginia_Cipher *vc;
start:
    char *text = (char *) malloc(sizeof(char) * 1024);
    fgets(text,1024, stdin);
    char *key = (char *) malloc(sizeof(char) * 1024);
    fgets(key,1024, stdin);
    New_Virginia_Cipher(&vc, key);
    int mode = 0;
    scanf("%d",&mode);
    if(mode == 0){
        vc->encrypt(vc,text);
        //impl_encrypt(vc, text);
    }else if(mode == 1){
        vc->decrypt(vc,text);
    }else{
        goto start;
    }
    system("pause");
}