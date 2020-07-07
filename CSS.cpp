#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <cstdio>

using namespace std;

int c1, c2;
int r1, r2;
int carry = 0;
int stream[999999];
char str[999999];

//Function to read file and assign to string
string ReadFile(string fname){
    ifstream file(fname);
    string temp;
    while(getline(file,temp,'\n')){
         //loop
   }
    return temp;
}

int mode(string str){
   sort(str.begin(), str.end());

   int arr[256];
   for(int i = 0; i < 256; i++){
      arr[i] = 0;
   }
   int max = 0;
   int index;

   for(int i = 0; i < str.length(); i++){
      unsigned char c = str[i];
      arr[int(c)] += 1;
   }

   for(int i = 0; i < 256; i++){
      if(arr[i] > max){
         max = arr[i];
         index = i;
      }
   }

   return index;
}

int median(string str){
   int m;
   int * nums = new int[str.length()];
   for(int i = 0; i < str.length(); i++){
      unsigned char c = str[i];
      nums[i] = int(c);
   }

   int size = str.length();
   sort(nums,nums + size);

   m = nums[size / 2];

   delete[] nums;
   return m;
}

int mean(string str){
   int sum = 0;
   for (int i = 0; i < str.length(); i++) {
      unsigned char c = str[i];
      sum += int(c);
   }
   int mean = sum / str.length();
   return mean;
}

float stdev(string str){
   int sum = 0;

   for (int i = 0; i < str.length(); ++i) {
      unsigned char c = str[i];
      sum += pow(int(c),2);
   }
   float stdev = sqrt(sum / str.length() - pow(26,2));
   return stdev;
}

float entropy(string text){
   float tmp[256];
   for(int i = 0; i < 256; i++){
      tmp[i] = 0.0;
   }
   float prob = 0.0;
   float num = 1.0 / int(text.length());

   for(int i = 0; i < text.length(); i++){
      unsigned char c = text[i];
      tmp[int(c)] = tmp[int(c)] + num;
   }

   for(int i = 0; i < 256; i++){
      if(tmp[i] != 0.0){
         float l2f = log2f(tmp[i]);
         prob = prob + (tmp[i] * log2f(tmp[i]));
      }
   }

   float e = 0.0-prob;
   return e;
}

int keystream(int n){
    int ksb;
    int count = 0;
    while (count < (n+1)){
        //8 shifts of 25 bit SR1
        for(int i = 0; i < 8; i++){
            int x = (r1 >> 14) & 1;
            x = x ^ ((r1 >> 4) & 1);
            x = x ^ ((r1 >> 3) & 1);
            x = x ^ ((r1 >> 0) & 1);
            r1 = r1 >> 1;
            r1 |= (x << 24);
        }
        //8 shifts of 17 bit SR2
        for(int i = 0; i < 8; i++){
            int x = (r2 >> 14) & 1;
            x = x ^ ((r2 >> 0) & 1);
            r2 = r2 >> 1;
            r2 |= (x << 16);
        }

        //Harvest most significant 8 bits of each SR
        c1 = (r1 >> 17);
        c2 = (r2 >> 9);

        ksb = c1 + c2 + carry;
        if(ksb > 255){
            carry = 1;
            ksb = ksb >> 1;

        }
        else{
            carry = 0;
        }
        count++;
    }

    return ksb;
}

string encryptCSS(string etext){
    unsigned char c;
    int ksb;
    string output;
    for(int i = 0; i < etext.length(); i++){
        c = etext[i];
        c = (c ^ stream[i]);
        str[i] = c;
        output += c;
    }
    return output;
}

string decryptCSS(string text){
    unsigned char c;
    int ksb;
    string output;
    for(int i = 0; i < text.length(); i++){
        c = str[i];
        c = (c ^ stream[i]);
        output += c;
    }
    return output;

}

int main(){

    string key = "12345";

    //Open file and remove spaces
    string Filename = "textdata.txt";
    string text = ReadFile(Filename); 
    for (int i = text.length()-1; i >= 0; --i) {
        if(text[i] == ' ')
            text.erase(i, 1);
        else if(text[i] >= 'A' && text[i] <= 'Z'){
            text[i] += 32;
        }
    } 

    //Initialize 25 bit SR1
    for(int i = 0; i < 3; i++){
        r1 += key [i];
    }
    //Set 4th bit to avoid 0 condition
    r1 |= (1 << 3);

    //Initialize 25 bit SR1
    for(int i = 3; i < 5; i++){
        r2 += key [i];
    }
    //Set 4th bit to avoid 0 condition
    r2 |= (1 << 3);

    for(int i = 0; i < text.length(); i++){
        stream[i] = keystream(i);
    }

    string en,de;

    cout << endl;
    cout << "Plaintext" << endl;
    cout << text << endl;
    cout << endl;
    cout << "Ciphertext" << endl;
    en = encryptCSS(text);
    cout << en << endl;
    cout << endl;
    cout << "Decrypted text" << endl;
    de = decryptCSS(text);
    cout << de << endl;

    int mo = mode(en);
    int me = median(en);
    int x = mean(en);
    float s = stdev(en);
    float e = entropy(en);
    printf("\n\nEncrypted stats:\nmode: %d\nmedian: %d\nmean: %d\nstdev: %.2f\nentropy: %.3f\n\n", mo, me, x, s, e);
    mo = mode(text);
    me = median(text);
    x = mean(text);
    s = stdev(text);
    e = entropy(text);
    printf("Plaintext stats:\nmode: %d\nmedian: %d\nmean: %d\nstdev: %.2f\nentropy: %.3f\n\n", mo, me, x, s, e);

    return 0;
}
