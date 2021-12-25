#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFBITSIZE 256
#define BUFFSIZE BUFFBITSIZE/8
#define iterations 20

const int K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// char* teststr = "Difs ist ein etwas längerer text um mehrere durchläufe des Algorythmus moeglich zu machen";
char * teststr = "Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?";

int rotl(int x, int n, int w) {
    return (x<<n)^(x>>w-n);
}

int rotr(int x, int n, int w) {
    return (x>>n)^(x<<w-n);
}

char * toHex(int x) {
    char out[8] = "00000000";
    int a = 0;
    int i;
    for(i = 0; i < 8; i++) {
        a = (x>>(i*4))&0b1111;
        if(a <= 9) {
            out[7-i] = '0'+a;
        } else {
            out[7-i] = 'a'+a-10;
        }
    }
    char * outbuf = malloc(9);
    for(i = 0; i < 8; i++) {
        outbuf[i] = out[i];
    }
    outbuf[i] = '\0';
    return outbuf;
}

int shr(int x, int n) {
    return (x>>n);
}

int Ch(int x, int y, int z) {
    return (x&y)^(~x&z);
}

int Maj(int x, int y, int z) {
    return (x&y)^(x&z)^(y&z);
}

int sigma256(int x, int e) {
    switch(e) {
        case 1:
            return rotr(x, 6, 32)^rotr(x, 11, 32)^rotr(x, 25, 32);
        case 0:
            return rotr(x, 2, 32)^rotr(x, 13, 32)^rotr(x, 22, 32);
        default:
            return 0;
    }
}

int standart(int x, int e) {
    switch(e) {
        case 0:
            return rotr(x, 7, 32)^rotr(x, 18, 32)^shr(x, 3);
        case 1:
            return rotr(x, 17, 32)^rotr(x, 19, 32)^shr(x, 10);
        default:
            return 0;
    }
}

int main(int argc, char **argv) {
    int num_k = 0, num_i;
    num_i = strlen(teststr);
    while((num_i+num_k)%512 != 448) num_k++;
    char* text = malloc(num_i+(num_k+64)/32);
    for(int i = 0; i < num_i; i++) {
        text[i] = teststr[i];
    }
    for(int i = 0; i < num_k/32; i++) {
        text[num_i+i] = 0;
    }
    text[num_i+(num_k/32)] = 0;
    text[num_i+(num_k/32)+1] = num_i;
    int H[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    // for(int i = 0; i < 8; i++) H[i] = 0;
    int j, k;
    int i_001;
    int T1,T2;
    int W[64];
    int a,b,c,d,e,f,g,h;
    int _a,_b,_c,_d,_e,_f,_g,_h;
    for(int i = 0; i < 64; i++) W[i] = 0;
    // printf("test\n");
    int tval = 0;
    for(int i = 0; i <= (num_i+(num_k/32)+2)/8; i++) {
        a = H[0];
        b = H[1];
        c = H[2];
        d = H[3];
        e = H[4];
        f = H[5];
        g = H[6];
        h = H[7];
        // printf("test\n");
        for(i_001 = 0; i_001 < 64; i_001++) {
            // printf("test\n");
            if(i_001 < 16) {
                W[i_001] = text[i*8+i_001];
            } else {
                W[i_001] = standart(W[i_001-2], 1)+W[i_001-7]+standart(W[i_001-15], 0)+W[i_001-16];
            }
            // _a=a,_b=b,_c=c,_d=d,_e=e,_f=f,_g=g,_h=h;
            T1=h+sigma256(e, 1)+Ch(e,f,g)+K[i_001]+W[i_001];
            T2=sigma256(a, 1)+Maj(a,b,c);
            h=g;
            g=f;
            f=e;
            e=d+T1;
            d=c;
            c=b;
            b=a;
            a=T1+T2;
            // printf("test(%i)\n", tval++);
        }
        // printf("test\n");
        H[0]+=a;
        H[1]+=b;
        H[2]+=c;
        H[3]+=d;
        H[4]+=e;
        H[5]+=f;
        H[6]+=g;
        H[7]+=h;
    }
    int num_buf;
    for(int i = 0; i < 8; i++) {
        for(int i_1 = 0; i_1 < 8; i_1++) {
            num_buf = (H[i]>>(i_1*4))&0b1111;
            if(num_buf < 10) {
                printf("%c", '0'+num_buf);
            } else {
                printf("%c", 'a'+num_buf-10);
            }
        }
    }
    printf("\n");
}