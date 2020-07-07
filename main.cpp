#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <cstdio>

using namespace std;

float eng[26] = {.082,.015,.028,.043,.127,.022,.020,.061,.070,.002,.008,.040,.024,.067,.075,.019,.001,.060,.063,.091,.028,.010,.023,.001,.020,.001};
float ascii[256];

class Vig {
   public:
      string k;
   Vig(string k) {
      for (int i = 0; i < k.size(); ++i) {
         this->k += k[i];
      }
   }
   string encryption(string t) {
      string output;
      for (int i = 0, j = 0; i < t.length(); ++i) {
         char c = t[i];
         output += (c + (k[j])) % 256 - 97;
         j = (j + 1) % k.length();
      }
      return output;
   }
   string decryption(string t) {
      string output;
      for (int i = 0, j = 0; i < t.length(); ++i) {
         char c = t[i];
         output += (c - (k[j])) % 256 + 97;
         j = (j + 1) % k.length();
      }
      return output;
   }
};

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

float *IOC(string str, int n){
   float *arr = new float[n];
   for(int i = 0; i < 64; i++){
      float sum = 0.0;
      int shift = i+1;
      float bound = n-shift;
      for(int j = 0; j < bound; j++){
         if(str[j] == str[j+shift]){
            //printf("j=%d, bound= %.0f, arr[%d]: %c, %c\n",j,bound,i,str[j],str[j+shift]);
            sum += 1;
         }
      }
      arr[i] = sum / bound;
   }
   return arr;
}

string ReadFile(string fname){
    ifstream file(fname);
    string temp;
    while(getline(file,temp,'\n')){
         //loop
   }
    return temp;
}

void PrintCodes(){

   for(int i = 0; i < 256; i++){
      printf("%c",i);
   }
   cout << endl;
}

void PDF_ASCII(string txt1, string txt2){
   float arr[256];
   for(int i = 0; i < 256; i++){
      arr[i] = 0.0;
   }

   float num1 = 1.0 / txt1.length();
   for(int i = 0; i < txt1.length(); i++){
      unsigned char c = txt1[i];
      arr[int(c)] = arr[int(c)] + num1;
   }

   float num2 = 1.0 / txt2.length();
   for(int i = 0; i < txt2.length(); i++){
      unsigned char c = txt2[i];
      arr[int(c)] = arr[int(c)] + num2;
   }

   for(int i = 0; i < 256; i++){
      arr[i] = arr[i] / 2;
      ascii[i] = arr[i];
   }
}

float * PDF(string txt1){
   float * arr = new float[256];
   for(int i = 0; i < 256; i++){
      arr[i] = 0.0;
   }

   float num1 = 1.0 / txt1.length();
   for(int i = 0; i < txt1.length(); i++){
      unsigned char c = txt1[i];
      arr[int(c)] = arr[int(c)] + num1;
   }

   return arr;
}

void AttackShift(string *str, int len){

   unsigned char c;
   string plain;
   string *cpy = new string[len];

   printf("\nSuggested key: ");

   for(int i = 0; i < len; i++){
      float sad = 99999.99;
      plain = "";
      for (int k = 97; k < 123; k++){
         cpy[i] = str[i];
         for (int j = 0; j < str[i].length(); j++){
            cpy[i][j] = cpy[i][j] - k + 97; 
         }

         float *arr = PDF(cpy[i]);
         float sadtmp = 0.0;
         for(int i = 0; i < 256; i++){
            sadtmp += abs(arr[i] - ascii[i]);
         }

         if(sadtmp < sad){
            sad = sadtmp;
            c = k;
         }
      }
      printf("%c ",c);
   }  
   printf("\n");
}

void AttackVigenere(string str, int len){
   unsigned char c;
   int num = str.length() / len + (str.length() % len != 0);
   int k = 0;
   string *tmp = new string[num];
   for (int i = 0; i < len; i++){
      for (int j = 0; j < str.length(); j+=len){
         if(i > 0 && j == 0)
            j = i;
         tmp[i] += str[j];
      }
      k=0;
   }
   AttackShift(tmp,len);
}

int main() {

   string key = "roman";
   string Filename = "textdata.txt";
   string text = ReadFile(Filename); 
   for (int i = text.length()-1; i >= 0; --i) {
      if(text[i] == ' ')
         text.erase(i, 1);
   } 

   Vig v(key);
   string encrypt = v.encryption(text);
   string decrypt = v.decryption(encrypt);
   cout << "Original Message: " << text << endl;
   cout << endl;
   cout << "Encrypted Message: " << encrypt << endl;
   cout << endl;
   cout << "Decrypted Message: " << decrypt << endl;

   int mo = mode(encrypt);
   int me = median(encrypt);
   int x = mean(encrypt);
   float s = stdev(encrypt);
   float e = entropy(encrypt);
   printf("\nEncrypted stats:\nmode: %d\nmedian: %d\nmean: %d\nstdev: %.2f\nentropy: %.3f\n\n", mo, me, x, s, e);
   // string encrypt2 = v.encryption(encrypt);
   // mo = mode(encrypt2);
   // me = median(encrypt2);
   // x = mean(encrypt2);
   // s = stdev(encrypt2);
   // e = entropy(encrypt2);
   // printf("Same key double encryption stats:\nmode: %d\nmedian: %d\nmean: %d\nstdev: %.2f\nentropy: %.3f\n\n", mo, me, x, s, e);
   // Vig v2("donut");
   // string encrypt3 = v2.encryption(encrypt);
   // mo = mode(encrypt3);
   // me = median(encrypt3);
   // x = mean(encrypt3);
   // s = stdev(encrypt3);
   // e = entropy(encrypt3);
   // printf("Different key of same length double encryption stats:\nmode: %d\nmedian: %d\nmean: %d\nstdev: %.2f\nentropy: %.3f\n\n", mo, me, x, s, e);
   // Vig v3("code");
   // string encrypt4 = v3.encryption(encrypt);
   // mo = mode(encrypt4);
   // me = median(encrypt4);
   // x = mean(encrypt4);
   // s = stdev(encrypt4);
   // e = entropy(encrypt4);
   // printf("Different key of smaller length double encryption stats:\nmode: %d\nmedian: %d\nmean: %d\nstdev: %.2f\nentropy: %.3f\n\n", mo, me, x, s, e);
   // mo = mode(text);
   // me = median(text);
   // x = mean(text);
   // s = stdev(text);
   // e = entropy(text);
   // printf("Plaintext stats:\nmode: %d\nmedian: %d\nmean: %d\nstdev: %.2f\nentropy: %.3f\n\n", mo, me, x, s, e);

   int period = 0;
   float *arr = IOC(text,text.length());
   for(int i = 0; i < 64; i++){
      printf("IOC[%d] = %.3f\n",i+1,arr[i]);
      if(arr[i] >= 0.050){
         period = i + 1; 
         break;
      }
   }

   printf("Suggested key length: %d\n",period);

   string txt1 = ReadFile("Darwin.txt");
   for (int i = txt1.length()-1; i >= 0; --i) {
      if(txt1[i] == ' ')
         txt1.erase(i, 1);
   } 
   string txt2 = ReadFile("textdata.txt");
   for (int i = txt2.length()-1; i >= 0; --i) {
      if(txt2[i] == ' ')
         txt2.erase(i, 1);
   }  
   PDF_ASCII(txt1,txt2);

   unsigned char c;
   AttackVigenere(text,period);
  
   delete[] arr;
   return 0;
}