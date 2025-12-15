//Текст => Препроцессор => Текст => +(Трансляция + Линковка) (компиляция)
#include<stdio.h>
#include<stdlib.h>

// hello 1 => jfmmp
void encrypt(char* word, int shift);
void decrypt(char* word, int shift);


int main(int argc, char * argv[]) {
    printf("Hello world!\n");
    printf("argc = %d argv = %s\n", argc, argv[0]);
    

    if (argc != 3) {
        printf("Incorrect usage");
        return 1;
    }

    char*word = argv[1];
    int shift = atoi(argv[2]);

    printf("word = %s\n", word);
    printf("shift = %d\n", shift);


    //abc\0
    encrypt(word, shift);
    char * result = word;
    printf("result = %s\n", word);
    decrypt(result, shift);
    printf("dec_result = %s\n", result);
    return 0;
}

void encrypt(char* word, int shift){
    for(int i = 0; word[i] != '\0'; ++i) {
        char current_char = word[i];
        //a = 0, a + 1 = b(1)
        //a (char) => 143 (int) + => 144 (b)

        if (current_char >= 'a' && current_char <= 'z'){
            word[i] = 'a' +((current_char - 'a'+ shift)%26 +26) % 26;
        }
        if (current_char >= 'A' && current_char <= 'Z'){
            word[i] = 'A' +((current_char - 'A'+ shift)%26 +26) % 26;
        }
    }
}

void decrypt(char* word, int shift){
    // encrypt(word, -shift);
    for(int i = 0; word[i] != '\0'; ++i) {
        char current_char = word[i];

        if (current_char >= 'a' && current_char <= 'z'){
            word[i] = 'a' +((current_char - 'a'- shift)%26 + 26) % 26;
        }
        if (current_char >= 'A' && current_char <= 'Z'){
            word[i] = 'A' +((current_char - 'A'- shift)%26 + 26) % 26;
        }
    }
}